#include "app_state.h"

#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

#define DR_FLAC_BUFFER_SIZE 16384
#define DR_FLAC_IMPLEMENTATION
#include "dr_flac.h"

static mp3dec_t mp3d;
static int16_t mp3PcmResid[MINIMP3_MAX_SAMPLES_PER_FRAME];
static drwav wavState;
static drflac* flacState = NULL; //fuck this shit

void ihateflac_meta_shit(void* pUserData, drflac_metadata* pMetadata);

// this m4a thingg
static uint16_t rd_be16(const uint8_t* p) {
    return ((uint16_t)p[0] << 8) | (uint16_t)p[1];
}

static uint32_t rd_be32(const uint8_t* p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] << 8)  | (uint32_t)p[3];
}

static uint64_t rd_be64(const uint8_t* p) {
    return ((uint64_t)rd_be32(p) << 32) | (uint64_t)rd_be32(p + 4);
}

static bool read_box_header(FILE* f, uint64_t limitEnd, uint64_t* outBoxStart,
                            uint64_t* outBoxEnd, uint64_t* outPayloadStart, char outType[4]) {
    long pos = ftell(f);
    if (pos < 0 || (uint64_t)pos + 8 > limitEnd) return false;

    uint8_t hdr[8];
    if (fread(hdr, 1, 8, f) != 8) return false;

    uint64_t size = rd_be32(hdr);
    memcpy(outType, hdr + 4, 4);
    uint64_t headerLen = 8;

    if (size == 1) {
        if ((uint64_t)pos + 16 > limitEnd) return false;
        uint8_t ext[8];
        if (fread(ext, 1, 8, f) != 8) return false;
        size = rd_be64(ext);
        headerLen = 16;
    } else if (size == 0) {
        size = limitEnd - (uint64_t)pos;
    }

    if (size < headerLen) return false;
    uint64_t boxEnd = (uint64_t)pos + size;
    if (boxEnd > limitEnd) boxEnd = limitEnd;

    *outBoxStart = (uint64_t)pos;
    *outBoxEnd = boxEnd;
    *outPayloadStart = (uint64_t)pos + headerLen;
    return true;
}

static uint32_t read_mp4_descr_len(const uint8_t* p, size_t avail, size_t* consumed) {
    uint32_t len = 0;
    size_t i = 0;
    while (i < avail && i < 4) {
        uint8_t b = p[i++];
        len = (len << 7) | (b & 0x7F);
        if ((b & 0x80) == 0) break;
    }
    *consumed = i;
    return len;
}

static void parse_esds_box(const uint8_t* buf, size_t len, M4ADemuxer* d) {
    if (len < 6) return;
    size_t pos = 4;
    while (pos + 2 < len) {
        uint8_t tag = buf[pos++];
        size_t c = 0;
        uint32_t dlen = read_mp4_descr_len(buf + pos, len - pos, &c);
        pos += c;
        if (pos + dlen > len) dlen = (uint32_t)(len - pos);

        if (tag == 0x03) {
            if (dlen < 3) break;
            pos += 2;
            uint8_t flags = buf[pos++];
            if (flags & 0x80) pos += 2;
            if (flags & 0x40) {
                if (pos < len) pos += 1 + buf[pos];
            }
            if (flags & 0x20) pos += 2;
        } else if (tag == 0x04) {
            if (dlen < 13) break;
            pos += 13; 
        } else if (tag == 0x05) {
            if (dlen > 0 && dlen <= sizeof(d->asc)) {
                memcpy(d->asc, buf + pos, dlen);
                d->ascLen = dlen;
            }
            return;
        } else {
            pos += dlen;
        }
    }
}

static void parse_ilst_box(FILE* f, uint64_t ilstStart, uint64_t ilstEnd, M4ADemuxer* d) {
    fseek(f, (long)ilstStart, SEEK_SET);
    uint64_t bs, be, ps;
    char tag[4];
    while (read_box_header(f, ilstEnd, &bs, &be, &ps, tag)) {
        uint64_t ibs, ibe, ips;
        char itag[4];
        fseek(f, (long)ps, SEEK_SET);
        while (read_box_header(f, be, &ibs, &ibe, &ips, itag)) {
            if (memcmp(itag, "data", 4) == 0 && ibe > ips + 8) {
                uint8_t dhdr[8];
                fseek(f, (long)ips, SEEK_SET);
                if (fread(dhdr, 1, 8, f) == 8) {
                    uint32_t dataType = rd_be32(dhdr);
                    size_t payloadLen = (size_t)(ibe - (ips + 8));

                    if (memcmp(tag, "\xA9" "nam", 4) == 0 ||
                        memcmp(tag, "\xA9" "ART", 4) == 0 ||
                        memcmp(tag, "\xA9" "alb", 4) == 0) {
                        char* dst = (tag[1] == 'n') ? d->title :
                                    (tag[1] == 'A') ? d->artist : d->album;
                        size_t cap = 256;
                        size_t toRead = (payloadLen < cap - 1) ? payloadLen : (cap - 1);
                        if (toRead > 0 && fread(dst, 1, toRead, f) == toRead) {
                            dst[toRead] = '\0';
                        }
                    } else if (memcmp(tag, "tmpo", 4) == 0 && payloadLen >= 2) {
                        uint8_t b[2];
                        if (fread(b, 1, 2, f) == 2) {
                            uint16_t bpm = rd_be16(b);
                            if (bpm >= 40 && bpm <= 240) {
                                d->bpm = (float)bpm;
                                d->hasBpm = true;
                            }
                        }
                    } else if (memcmp(tag, "covr", 4) == 0 && !d->coverData &&
                               payloadLen > 16 && payloadLen <= 8 * 1024 * 1024 &&
                               (dataType == 13 || dataType == 14 || dataType == 0)) {
                        uint8_t* img = (uint8_t*)malloc(payloadLen);
                        if (img) {
                            if (fread(img, 1, payloadLen, f) == payloadLen) {
                                d->coverData = img;
                                d->coverSize = payloadLen;
                            } else {
                                free(img);
                            }
                        }
                    }
                }
            }
            fseek(f, (long)ibe, SEEK_SET);
        }
        fseek(f, (long)be, SEEK_SET);
    }
}

static void parse_udta_box(FILE* f, uint64_t udtaStart, uint64_t udtaEnd, M4ADemuxer* d) {
    fseek(f, (long)udtaStart, SEEK_SET);
    uint64_t bs, be, ps;
    char tag[4];
    while (read_box_header(f, udtaEnd, &bs, &be, &ps, tag)) {
        if (memcmp(tag, "meta", 4) == 0 && be > ps + 4) {
            uint64_t mbs, mbe, mps;
            char mtag[4];
            fseek(f, (long)(ps + 4), SEEK_SET); // metadata... thing......
            while (read_box_header(f, be, &mbs, &mbe, &mps, mtag)) {
                if (memcmp(mtag, "ilst", 4) == 0) {
                    parse_ilst_box(f, mps, mbe, d);
                }
                fseek(f, (long)mbe, SEEK_SET);
            }
        }
        fseek(f, (long)be, SEEK_SET);
    }
}

typedef struct {
    uint32_t firstChunk;
    uint32_t samplesPerChunk;
} M4AStscEntry;

typedef struct {
    bool isAudio;
    uint32_t timescale;
    uint64_t durationUnits;
    uint32_t channels;
    uint32_t sampleRate;
    uint8_t asc[64];
    size_t ascLen;

    M4ASttsEntry* stts;
    uint32_t sttsCount;

    uint32_t uniformSampleSize;
    uint32_t sampleCount;
    uint32_t* sampleSizes;

    M4AStscEntry* stsc;
    uint32_t stscCount;

    uint64_t* chunkOffsets;
    uint32_t chunkCount;
} M4ATrackTemp;

static void free_track_temp(M4ATrackTemp* t) {
    free(t->stts);
    free(t->sampleSizes);
    free(t->stsc);
    free(t->chunkOffsets);
    memset(t, 0, sizeof(*t));
}

static void parse_stsd(FILE* f, uint64_t ps, uint64_t be, M4ATrackTemp* t) {
    if (be < ps + 8) return;
    fseek(f, (long)(ps + 4), SEEK_SET);
    uint8_t cntBuf[4];
    if (fread(cntBuf, 1, 4, f) != 4) return;
    uint32_t entryCount = rd_be32(cntBuf);

    for (uint32_t e = 0; e < entryCount; e++) {
        uint64_t ebs, ebe, eps;
        char etag[4];
        if (!read_box_header(f, be, &ebs, &ebe, &eps, etag)) break;
        if (memcmp(etag, "mp4a", 4) == 0 && ebe >= eps + 28) {
            uint8_t sd[28];
            fseek(f, (long)eps, SEEK_SET);
            if (fread(sd, 1, 28, f) == 28) {
                t->channels = rd_be16(sd + 16);
                t->sampleRate = rd_be32(sd + 24) >> 16;
                t->isAudio = true;

                uint64_t cbs, cbe, cps;
                char ctag[4];
                while (read_box_header(f, ebe, &cbs, &cbe, &cps, ctag)) {
                    if (memcmp(ctag, "esds", 4) == 0) {
                        size_t esdsLen = (size_t)(cbe - cps);
                        if (esdsLen > 0 && esdsLen <= 4096) {
                            uint8_t* buf = (uint8_t*)malloc(esdsLen);
                            if (buf) {
                                fseek(f, (long)cps, SEEK_SET);
                                if (fread(buf, 1, esdsLen, f) == esdsLen) {
                                    M4ADemuxer tmp = {0};
                                    parse_esds_box(buf, esdsLen, &tmp);
                                    if (tmp.ascLen > 0) {
                                        memcpy(t->asc, tmp.asc, tmp.ascLen);
                                        t->ascLen = tmp.ascLen;
                                    }
                                }
                                free(buf);
                            }
                        }
                    }
                    fseek(f, (long)cbe, SEEK_SET);
                }
            }
        }
        fseek(f, (long)ebe, SEEK_SET);
    }
}

static void parse_stbl(FILE* f, uint64_t ps, uint64_t be, M4ATrackTemp* t) {
    fseek(f, (long)ps, SEEK_SET);
    uint64_t sbs, sbe, sps;
    char stag[4];
    while (read_box_header(f, be, &sbs, &sbe, &sps, stag)) {
        if (memcmp(stag, "stsd", 4) == 0) {
            parse_stsd(f, sps, sbe, t);
        } else if (memcmp(stag, "stts", 4) == 0 && sbe >= sps + 8) {
            uint8_t hdr[8];
            fseek(f, (long)sps, SEEK_SET);
            if (fread(hdr, 1, 8, f) == 8) {
                uint32_t cnt = rd_be32(hdr + 4);
                size_t tblBytes = (size_t)cnt * 8;
                if (cnt > 0 && sps + 8 + (uint64_t)tblBytes <= sbe) {
                    uint8_t* raw = (uint8_t*)malloc(tblBytes);
                    t->stts = (M4ASttsEntry*)malloc(cnt * sizeof(M4ASttsEntry));
                    if (raw && t->stts && fread(raw, 1, tblBytes, f) == tblBytes) {
                        t->sttsCount = cnt;
                        for (uint32_t i = 0; i < cnt; i++) {
                            t->stts[i].count = rd_be32(raw + i * 8);
                            t->stts[i].delta = rd_be32(raw + i * 8 + 4);
                        }
                    } else {
                        free(t->stts);
                        t->stts = NULL;
                    }
                    free(raw);
                }
            }
        } else if (memcmp(stag, "stsz", 4) == 0 && sbe >= sps + 12) {
            uint8_t hdr[12];
            fseek(f, (long)sps, SEEK_SET);
            if (fread(hdr, 1, 12, f) == 12) {
                t->uniformSampleSize = rd_be32(hdr + 4);
                t->sampleCount = rd_be32(hdr + 8);
                if (t->uniformSampleSize == 0 && t->sampleCount > 0 &&
                    sps + 12 + (uint64_t)t->sampleCount * 4 <= sbe) {
                    size_t tblBytes = (size_t)t->sampleCount * 4;
                    t->sampleSizes = (uint32_t*)malloc(tblBytes);
                    if (t->sampleSizes) {
                        if (fread(t->sampleSizes, 1, tblBytes, f) == tblBytes) {
                            for (uint32_t i = 0; i < t->sampleCount; i++) {
                                t->sampleSizes[i] = __builtin_bswap32(t->sampleSizes[i]);
                            }
                        } else {
                            free(t->sampleSizes);
                            t->sampleSizes = NULL;
                            t->sampleCount = 0;
                        }
                    }
                }
            }
        } else if (memcmp(stag, "stsc", 4) == 0 && sbe >= sps + 8) {
            uint8_t hdr[8];
            fseek(f, (long)sps, SEEK_SET);
            if (fread(hdr, 1, 8, f) == 8) {
                uint32_t cnt = rd_be32(hdr + 4);
                size_t tblBytes = (size_t)cnt * 12;
                if (cnt > 0 && sps + 8 + (uint64_t)tblBytes <= sbe) {
                    uint8_t* raw = (uint8_t*)malloc(tblBytes);
                    t->stsc = (M4AStscEntry*)malloc(cnt * sizeof(M4AStscEntry));
                    if (raw && t->stsc && fread(raw, 1, tblBytes, f) == tblBytes) {
                        t->stscCount = cnt;
                        for (uint32_t i = 0; i < cnt; i++) {
                            t->stsc[i].firstChunk = rd_be32(raw + i * 12);
                            t->stsc[i].samplesPerChunk = rd_be32(raw + i * 12 + 4);
                        }
                    } else {
                        free(t->stsc);
                        t->stsc = NULL;
                    }
                    free(raw);
                }
            }
        } else if ((memcmp(stag, "stco", 4) == 0 || memcmp(stag, "co64", 4) == 0) && sbe >= sps + 8) {
            bool is64 = (memcmp(stag, "co64", 4) == 0);
            uint8_t hdr[8];
            fseek(f, (long)sps, SEEK_SET);
            if (fread(hdr, 1, 8, f) == 8) {
                uint32_t cnt = rd_be32(hdr + 4);
                size_t esz = is64 ? 8 : 4;
                size_t tblBytes = (size_t)cnt * esz;
                if (cnt > 0 && sps + 8 + (uint64_t)tblBytes <= sbe) {
                    uint8_t* raw = (uint8_t*)malloc(tblBytes);
                    t->chunkOffsets = (uint64_t*)malloc(cnt * sizeof(uint64_t));
                    if (raw && t->chunkOffsets && fread(raw, 1, tblBytes, f) == tblBytes) {
                        t->chunkCount = cnt;
                        for (uint32_t i = 0; i < cnt; i++) {
                            t->chunkOffsets[i] = is64 ? rd_be64(raw + i * 8) : (uint64_t)rd_be32(raw + i * 4);
                        }
                    } else {
                        free(t->chunkOffsets);
                        t->chunkOffsets = NULL;
                    }
                    free(raw);
                }
            }
        }
        fseek(f, (long)sbe, SEEK_SET);
    }
}

static void parse_trak_box(FILE* f, uint64_t trakStart, uint64_t trakEnd, M4ATrackTemp* t) {
    fseek(f, (long)trakStart, SEEK_SET);
    uint64_t bs, be, ps;
    char tag[4];
    while (read_box_header(f, trakEnd, &bs, &be, &ps, tag)) {
        if (memcmp(tag, "mdia", 4) == 0) {
            fseek(f, (long)ps, SEEK_SET);
            uint64_t mbs, mbe, mps;
            char mtag[4];
            while (read_box_header(f, be, &mbs, &mbe, &mps, mtag)) {
                if (memcmp(mtag, "mdhd", 4) == 0 && mbe >= mps + 24) {
                    uint8_t ver[4];
                    fseek(f, (long)mps, SEEK_SET);
                    if (fread(ver, 1, 4, f) == 4) {
                        if (ver[0] == 1 && mbe >= mps + 32) {
                            uint8_t buf[28];
                            if (fread(buf, 1, 28, f) == 28) {
                                t->timescale = rd_be32(buf + 16);
                                t->durationUnits = rd_be64(buf + 20);
                            }
                        } else {
                            uint8_t buf[20];
                            if (fread(buf, 1, 20, f) == 20) {
                                t->timescale = rd_be32(buf + 8);
                                t->durationUnits = rd_be32(buf + 12);
                            }
                        }
                    }
                } else if (memcmp(mtag, "hdlr", 4) == 0 && mbe >= mps + 12) {
                    uint8_t buf[12];
                    fseek(f, (long)mps, SEEK_SET);
                    if (fread(buf, 1, 12, f) == 12) {
                        if (memcmp(buf + 8, "soun", 4) == 0) t->isAudio = true;
                    }
                } else if (memcmp(mtag, "minf", 4) == 0) {
                    fseek(f, (long)mps, SEEK_SET);
                    uint64_t ibs, ibe, ips;
                    char itag[4];
                    while (read_box_header(f, mbe, &ibs, &ibe, &ips, itag)) {
                        if (memcmp(itag, "stbl", 4) == 0) {
                            parse_stbl(f, ips, ibe, t);
                        }
                        fseek(f, (long)ibe, SEEK_SET);
                    }
                }
                fseek(f, (long)mbe, SEEK_SET);
            }
        }
        fseek(f, (long)be, SEEK_SET);
    }
}

static bool build_sample_table(M4ADemuxer* d, M4ATrackTemp* t) {
    if (!t->isAudio || t->ascLen == 0 || t->sampleCount == 0 ||
        t->chunkCount == 0 || t->stscCount == 0) {
        return false;
    }

    d->sampleOffsets = (uint32_t*)malloc(t->sampleCount * sizeof(uint32_t));
    d->sampleSizes = (uint32_t*)malloc(t->sampleCount * sizeof(uint32_t));
    if (!d->sampleOffsets || !d->sampleSizes) {
        free(d->sampleOffsets);
        free(d->sampleSizes);
        d->sampleOffsets = NULL;
        d->sampleSizes = NULL;
        return false;
    }

    uint32_t sIdx = 0;
    uint32_t stscIdx = 0;
    for (uint32_t chunk = 0; chunk < t->chunkCount && sIdx < t->sampleCount; chunk++) {
        uint32_t chunkNum = chunk + 1; // i hate this
        while (stscIdx + 1 < t->stscCount && t->stsc[stscIdx + 1].firstChunk <= chunkNum) {
            stscIdx++;
        }
        uint32_t spc = t->stsc[stscIdx].samplesPerChunk;
        uint64_t off = t->chunkOffsets[chunk];
        for (uint32_t k = 0; k < spc && sIdx < t->sampleCount; k++) {
            uint32_t sz = (t->uniformSampleSize > 0) ? t->uniformSampleSize : t->sampleSizes[sIdx];
            d->sampleOffsets[sIdx] = (uint32_t)off;
            d->sampleSizes[sIdx] = sz;
            off += sz;
            sIdx++;
        }
    }

    d->sampleCount = sIdx;
    d->currentSample = 0;
    d->timescale = (t->timescale > 0) ? t->timescale : 44100;
    d->durationUnits = t->durationUnits;
    d->durationSec = (d->timescale > 0) ? (float)((double)t->durationUnits / (double)d->timescale) : 0.0f;
    d->channels = (t->channels > 0) ? t->channels : 2;
    d->sampleRate = (t->sampleRate > 0) ? t->sampleRate : d->timescale;
    memcpy(d->asc, t->asc, t->ascLen);
    d->ascLen = t->ascLen;

    d->stts = t->stts;
    d->sttsCount = t->sttsCount;
    t->stts = NULL; // TRANSfer... hehe... yk... like... transfems... oh my god shut the fuck up
    return (d->sampleCount > 0);
}

bool m4a_open(M4ADemuxer* d, const char* path) {
    if (!d || !path) return false;
    memset(d, 0, sizeof(*d));

    FILE* f = fopen(path, "rb");
    if (!f) return false;
    setvbuf(f, NULL, _IOFBF, 64 * 1024);
    fseek(f, 0, SEEK_END);
    long fsz = ftell(f);
    if (fsz <= 16) { fclose(f); return false; }
    uint64_t fileEnd = (uint64_t)fsz;
    fseek(f, 0, SEEK_SET);

    bool foundTrack = false;
    uint64_t bs, be, ps;
    char tag[4];
    while (read_box_header(f, fileEnd, &bs, &be, &ps, tag)) {
        if (memcmp(tag, "moov", 4) == 0) {
            uint64_t mbs, mbe, mps;
            char mtag[4];
            fseek(f, (long)ps, SEEK_SET);
            while (read_box_header(f, be, &mbs, &mbe, &mps, mtag)) {
                if (memcmp(mtag, "udta", 4) == 0) {
                    parse_udta_box(f, mps, mbe, d);
                } else if (memcmp(mtag, "trak", 4) == 0 && !foundTrack) {
                    M4ATrackTemp tt;
                    memset(&tt, 0, sizeof(tt));
                    parse_trak_box(f, mps, mbe, &tt);
                    if (build_sample_table(d, &tt)) {
                        foundTrack = true;
                    }
                    free_track_temp(&tt);
                }
                fseek(f, (long)mbe, SEEK_SET);
            }
        }
        fseek(f, (long)be, SEEK_SET);
    }

    if (!foundTrack) {
        fclose(f);
        m4a_close(d);
        return false;
    }

    d->file = f;
    return true;
}

void m4a_close(M4ADemuxer* d) {
    if (!d) return;
    if (d->file) {
        fclose(d->file);
        d->file = NULL;
    }
    free(d->sampleOffsets);
    free(d->sampleSizes);
    free(d->stts);
    free(d->coverData);
    memset(d, 0, sizeof(*d));
}

uint32_t m4a_read_sample(M4ADemuxer* d, uint8_t* dst, size_t maxBytes) {
    if (!d || !d->file || !dst || d->currentSample >= d->sampleCount) return 0;
    uint32_t sz = d->sampleSizes[d->currentSample];
    uint32_t off = d->sampleOffsets[d->currentSample];
    d->currentSample++;
    if (sz == 0 || sz > maxBytes) return 0;
    fseek(d->file, (long)off, SEEK_SET);
    if (fread(dst, 1, sz, d->file) != sz) return 0;
    return sz;
}

float m4a_seek(M4ADemuxer* d, float seconds) {
    if (!d || d->sampleCount == 0 || d->timescale == 0) return 0.0f;
    if (seconds <= 0.0f) {
        d->currentSample = 0;
        return 0.0f;
    }

    uint64_t targetUnits = (uint64_t)((double)seconds * (double)d->timescale);
    uint64_t accumUnits = 0;
    uint32_t sampleIdx = 0;

    if (d->sttsCount > 0) {
        for (uint32_t i = 0; i < d->sttsCount; i++) {
            uint32_t cnt = d->stts[i].count;
            uint32_t delta = d->stts[i].delta;
            if (delta == 0) {
                sampleIdx += cnt;
                continue;
            }
            uint64_t span = (uint64_t)cnt * (uint64_t)delta;
            if (accumUnits + span > targetUnits) {
                uint32_t step = (uint32_t)((targetUnits - accumUnits) / delta);
                sampleIdx += step;
                accumUnits += (uint64_t)step * (uint64_t)delta;
                break;
            }
            accumUnits += span;
            sampleIdx += cnt;
        }
    } else {
        uint64_t approx = (uint64_t)((double)seconds * (double)d->sampleRate / 1024.0);
        sampleIdx = (approx < d->sampleCount) ? (uint32_t)approx : (d->sampleCount - 1);
        accumUnits = targetUnits;
    }

    if (sampleIdx >= d->sampleCount) sampleIdx = d->sampleCount - 1;
    d->currentSample = sampleIdx;
    return (float)((double)accumUnits / (double)d->timescale);
}

bool m4a_read_metadata(const char* path,
                       char* title, size_t titleSz,
                       char* artist, size_t artistSz,
                       char* album, size_t albumSz,
                       float* outBpm, bool* outHasBpm,
                       uint8_t** outCover, size_t* outCoverSize) {
    if (!path) return false;
    FILE* f = fopen(path, "rb");
    if (!f) return false;
    setvbuf(f, NULL, _IOFBF, 64 * 1024);
    fseek(f, 0, SEEK_END);
    long fsz = ftell(f);
    if (fsz <= 16) { fclose(f); return false; }
    uint64_t fileEnd = (uint64_t)fsz;
    fseek(f, 0, SEEK_SET);

    M4ADemuxer tmp;
    memset(&tmp, 0, sizeof(tmp));

    uint64_t bs, be, ps;
    char tag[4];
    bool foundMoov = false;
    while (read_box_header(f, fileEnd, &bs, &be, &ps, tag)) {
        if (memcmp(tag, "moov", 4) == 0) {
            foundMoov = true;
            uint64_t mbs, mbe, mps;
            char mtag[4];
            fseek(f, (long)ps, SEEK_SET);
            while (read_box_header(f, be, &mbs, &mbe, &mps, mtag)) {
                if (memcmp(mtag, "udta", 4) == 0) {
                    parse_udta_box(f, mps, mbe, &tmp);
                }
                fseek(f, (long)mbe, SEEK_SET);
            }
            break;
        }
        fseek(f, (long)be, SEEK_SET);
    }
    fclose(f);

    if (title && titleSz > 0 && tmp.title[0] != '\0') snprintf(title, titleSz, "%s", tmp.title);
    if (artist && artistSz > 0 && tmp.artist[0] != '\0') snprintf(artist, artistSz, "%s", tmp.artist);
    if (album && albumSz > 0 && tmp.album[0] != '\0') snprintf(album, albumSz, "%s", tmp.album);
    if (outBpm && tmp.hasBpm) *outBpm = tmp.bpm;
    if (outHasBpm && tmp.hasBpm) *outHasBpm = true;
    if (outCover && outCoverSize) {
        *outCover = tmp.coverData;
        *outCoverSize = tmp.coverSize;
        tmp.coverData = NULL;
    }
    free(tmp.coverData);
    return foundMoov;
}

static inline int16_t apply_gain_and_limit(int16_t sample, float gain) {
    (void)gain;
    return sample;
}

void ear_destroyer_limiter(int16_t* dest, int frames, float gain) {
    if (!dest || frames <= 0) return;
    if (gain == 1.0f) {
        volumeLimEnv = 0.0f;
        return;
    }
    if (gain < 1.0f) {
        volumeLimEnv = 0.0f;
        int32_t gfp = (int32_t)(gain * 32768.0f);
        int total = frames * 2;
        for (int i = 0; i < total; i++) {
            dest[i] = (int16_t)(((int32_t)dest[i] * gfp) >> 15);
        }
        return;
    }

    float env = volumeLimEnv;
    for (int i = 0; i < frames; i++) {
        float l = (float)dest[i * 2] * (1.0f / 32768.0f);
        float r = (float)dest[i * 2 + 1] * (1.0f / 32768.0f);
        float peak = fabsf(l);
        if (fabsf(r) > peak) peak = fabsf(r);

        if (peak > env) {
            env = env * 0.25f + peak * 0.75f;
        } else {
            env = env * 0.9993f + peak * 0.0007f;
        }

        float effGain = gain;
        float wanted = env * gain;
        if (wanted > 0.80f && env > 1e-5f) {
            float over = wanted - 0.80f;
            float targetPeak = 0.80f + over / (1.0f + over * 9.0f);
            effGain = targetPeak / env;
            if (effGain > gain) effGain = gain;
        }

        float ol = l * effGain;
        float or = r * effGain;

        if (fabsf(ol) > 0.90f) {
            float s = (ol > 0.0f) ? 1.0f : -1.0f;
            float a = fabsf(ol) - 0.90f;
            ol = s * (0.90f + a / (1.0f + a * 10.0f));
        }
        if (fabsf(or) > 0.90f) {
            float s = (or > 0.0f) ? 1.0f : -1.0f;
            float a = fabsf(or) - 0.90f;
            or = s * (0.90f + a / (1.0f + a * 10.0f));
        }

        int32_t il = (int32_t)(ol * 32767.0f);
        int32_t ir = (int32_t)(or * 32767.0f);
        if (il > 32767) il = 32767; else if (il < -32768) il = -32768;
        if (ir > 32767) ir = 32767; else if (ir < -32768) ir = -32768;
        dest[i * 2]     = (int16_t)il;
        dest[i * 2 + 1] = (int16_t)ir;
    }
    volumeLimEnv = env;
}


static void fake_some_waveform_bars(void) {
    #define NUM_WAVE_PINS 16
    float pinPeaks[NUM_WAVE_PINS];
    unsigned int seed = (unsigned int)(songDuration * 1000.0f) ^ 0x811C9DC5u;
    for (int i = 0; currentTrackPath[i] != '\0'; i++) {
        seed ^= (unsigned char)currentTrackPath[i];
        seed *= 16777619u;
    }
    if (seed == 0) seed = 12345;
    
    for (int p = 0; p < NUM_WAVE_PINS; p++) {
        float normPos = (float)p / (float)(NUM_WAVE_PINS - 1);
        float dyn = 0.35f + 0.45f * sinf(normPos * 3.14159265f);
        if (p == 0 || p == NUM_WAVE_PINS - 1) dyn *= 0.4f;

        seed = seed * 1103515245 + 12345;
        float var = ((float)((seed >> 16) & 0x7F) / 127.0f - 0.5f) * 0.28f;
        float val = dyn + var;
        if (val < 0.15f) val = 0.15f;
        if (val > 0.95f) val = 0.95f;
        pinPeaks[p] = val;
    }

    if (currentFormat == FORMAT_WAV && isWavOpen && wavState.totalPCMFrameCount > 0) {
        drwav_uint64 total = wavState.totalPCMFrameCount;
        for (int p = 0; p < 8; p++) {
            drwav_uint64 target = (drwav_uint64)((float)p / 8.0f * (float)total);
            drwav_seek_to_pcm_frame(&wavState, target);
            drwav_uint64 read = drwav_read_pcm_frames_s16(&wavState, 64, waveScanChunk);
            int32_t peak = 0;
            for (drwav_uint64 s = 0; s < read * wavState.channels; s++) {
                int32_t a = abs(waveScanChunk[s]);
                if (a > peak) peak = a;
            }
            float norm = (float)peak / 32768.0f;
            if (norm > 0.12f) pinPeaks[p * 2] = norm;
        }
        drwav_seek_to_pcm_frame(&wavState, 0);
    }

    for (int i = 0; i < NUM_WAVE_SAMPLES; i++) {
        float t = (float)i / (float)(NUM_WAVE_SAMPLES - 1) * (float)(NUM_WAVE_PINS - 1);
        int p0 = (int)t;
        int p1 = (p0 + 1 < NUM_WAVE_PINS) ? p0 + 1 : p0;
        float frac = t - (float)p0;
        float smoothFrac = frac * frac * (3.0f - 2.0f * frac);
        float baseEnv = pinPeaks[p0] * (1.0f - smoothFrac) + pinPeaks[p1] * smoothFrac;

        float fine = sinf((float)i * 2.37f) * 0.20f + sinf((float)i * 0.73f) * 0.18f + sinf((float)i * 4.91f) * 0.12f;
        float jitter = ((float)((i * 37 + 13) % 23) / 23.0f - 0.5f) * 0.30f;
        float val = baseEnv + fine * baseEnv + jitter * baseEnv;
        if (val < 0.10f) val = 0.10f;
        if (val > 1.0f) val = 1.0f;
        fullSongWaveform[i] = val;
    }
}

void stop_playback(void) {
    LightLock_Lock(&audioLock);

    ndspChnReset(AUDIO_CHANNEL);
    for (int i = 0; i < NUM_WAVE_BUFS; i++) {
        waveBuf[i].status = NDSP_WBUF_DONE;
        waveBufSamples[i] = 0;
    }
    isPlaying = false;
    isPaused = false;
    samplesCompleted = 0;
    samplesFed = 0;
    reset_beat_tracking(true);
    audioStreamEOF = false;
    trackFinished = false;
    songTime = 0.0f;
    songDuration = 0.0f;
    gustinaState = GUSTINA_NOTHING;

    if (currentAudioFile) { fclose(currentAudioFile); currentAudioFile = NULL; }
    if (isWavOpen) { drwav_uninit(&wavState); isWavOpen = false; }
    if (flacState) { drflac_close(flacState); flacState = NULL; }
    if (flacFile) { fclose(flacFile); flacFile = NULL; }
    if (isOggOpen) { ov_clear(&oggFile); isOggOpen = false; }
    if (opusFile) { op_free(opusFile); opusFile = NULL; }
    if (isM4aOpen) { m4a_close(&m4aDemuxer); isM4aOpen = false; }
    if (aacDec) { NeAACDecClose(aacDec); aacDec = NULL; }

    currentFormat = FORMAT_NONE;
    mp3BufferLen = 0;
    mp3BufferPos = 0;
    mp3PcmPos = 0;
    mp3PcmCount = 0;
    aacPcmPos = 0;
    aacPcmCount = 0;

    LightLock_Unlock(&audioLock);
}

static size_t ihateflac_read(void* pUserData, void* pBufferOut, size_t bytesToRead) {
    return fread(pBufferOut, 1, bytesToRead, (FILE*)pUserData);
}

static drflac_bool32 ihateflac_seek(void* pUserData, int offset, drflac_seek_origin origin) {
    int whence = (origin == DRFLAC_SEEK_SET) ? SEEK_SET : ((origin == DRFLAC_SEEK_CUR) ? SEEK_CUR : SEEK_END);
    return (fseek((FILE*)pUserData, (long)offset, whence) == 0) ? DRFLAC_TRUE : DRFLAC_FALSE;
}

static drflac_bool32 ihateflac_tell(void* pUserData, drflac_int64* pCursor) {
    long pos = ftell((FILE*)pUserData);
    if (pos < 0) return DRFLAC_FALSE;
    *pCursor = (drflac_int64)pos;
    return DRFLAC_TRUE;
}


typedef struct {
    FILE* fp;
    long fileSize;
    long cursor;
    long cacheOffset;
    size_t cacheLen;
    uint8_t cacheBuf[32 * 1024];
} OggFastStream;

static OggFastStream oggFastStream;

static size_t ogg_cb_read(void* ptr, size_t size, size_t nmemb, void* datasource) {
    OggFastStream* s = (OggFastStream*)datasource;
    if (!s || !s->fp || !ptr || size == 0 || nmemb == 0) return 0;
    size_t totalWanted = size * nmemb;
    if (s->cursor >= s->fileSize) return 0;

    size_t availInFile = (size_t)(s->fileSize - s->cursor);
    if (totalWanted > availInFile) totalWanted = availInFile;

    uint8_t* dst = (uint8_t*)ptr;
    size_t copied = 0;

    while (copied < totalWanted) {
        if (s->cursor >= s->cacheOffset &&
            s->cursor < s->cacheOffset + (long)s->cacheLen) {
            size_t inOff = (size_t)(s->cursor - s->cacheOffset);
            size_t inAvail = s->cacheLen - inOff;
            size_t chunk = totalWanted - copied;
            if (chunk > inAvail) chunk = inAvail;
            memcpy(dst + copied, s->cacheBuf + inOff, chunk);
            s->cursor += (long)chunk;
            copied += chunk;
        } else {
            if (fseek(s->fp, s->cursor, SEEK_SET) != 0) break;
            s->cacheOffset = s->cursor;
            s->cacheLen = fread(s->cacheBuf, 1, sizeof(s->cacheBuf), s->fp);
            if (s->cacheLen == 0) break;
        }
    }

    return copied / size;
}

static int ogg_cb_seek(void* datasource, ogg_int64_t offset, int whence) {
    OggFastStream* s = (OggFastStream*)datasource;
    if (!s || !s->fp) return -1;
    long newPos = 0;
    if (whence == SEEK_SET) newPos = (long)offset;
    else if (whence == SEEK_CUR) newPos = s->cursor + (long)offset;
    else if (whence == SEEK_END) newPos = s->fileSize + (long)offset;
    else return -1;

    if (newPos < 0 || newPos > s->fileSize) return -1;
    s->cursor = newPos;
    return 0;
}

static int ogg_cb_close(void* datasource) {
    OggFastStream* s = (OggFastStream*)datasource;
    if (!s) return 0;
    if (s->fp) {
        fclose(s->fp);
        s->fp = NULL;
    }
    s->cacheLen = 0;
    s->cacheOffset = 0;
    s->cursor = 0;
    return 0;
}

static long ogg_cb_tell(void* datasource) {
    OggFastStream* s = (OggFastStream*)datasource;
    return s ? s->cursor : -1;
}

static const ov_callbacks oggFastCallbacks = {
    ogg_cb_read,
    ogg_cb_seek,
    ogg_cb_close,
    ogg_cb_tell
};

static int fill_audio_buffer(int16_t* dest, int samplesNeeded);

void start_playback(const char* filePath, const char* fileName, TransitionDir transDirection) {
    
    stop_playback();

    LightLock_Lock(&audioLock);

    strncpy(currentTrackPath, filePath, sizeof(currentTrackPath) - 1);
    strncpy(currentTrackName, fileName, sizeof(currentTrackName) - 1);
    currentTrackName[sizeof(currentTrackName) - 1] = '\0';

    static char cleanTitle[256];
    nuke_weird_utf8_chars(fileName, cleanTitle, sizeof(cleanTitle));
    strncpy(currentTrackTitle, cleanTitle, sizeof(currentTrackTitle) - 1);
    char* extDot = strrchr(currentTrackTitle, '.');
    if (extDot) *extDot = '\0';
    if (currentTrackTitle[0] == '\0') strcpy(currentTrackTitle, "Audio Track");

    strcpy(currentTrackArtist, "3DS Music Library");
    strcpy(currentTrackAlbum, "Audio Collection");

    detectedBPM = 120.0f;
    load_lrc_file(filePath);

    if (has_extension(fileName, ".mp3")) {
        currentAudioFile = fopen(filePath, "rb");
        if (!currentAudioFile) { LightLock_Unlock(&audioLock); return; }
        setvbuf(currentAudioFile, NULL, _IOFBF, 64 * 1024);

        
        long audioStartOffset = 0;
        float id3TlenSec = 0.0f;
        parse_mp3_id3v2_meta(currentAudioFile, &audioStartOffset, &id3TlenSec);
        fseek(currentAudioFile, audioStartOffset, SEEK_SET);

        
        uint8_t zeroSkip[512];
        while (true) {
            long posNow = ftell(currentAudioFile);
            size_t r = fread(zeroSkip, 1, sizeof(zeroSkip), currentAudioFile);
            if (r == 0) break;
            size_t nz = 0;
            while (nz < r && zeroSkip[nz] == 0) nz++;
            if (nz < r) {
                audioStartOffset = posNow + (long)nz;
                fseek(currentAudioFile, audioStartOffset, SEEK_SET);
                break;
            }
            audioStartOffset = posNow + (long)r;
        }

        mp3dec_init(&mp3d);
        mp3BufferPos = 0;
        mp3BufferLen = fread(mp3ReadBuffer, 1, FILE_READ_CHUNK, currentAudioFile);

        mp3dec_frame_info_t info;
        memset(&info, 0, sizeof(info));
        int16_t tempPcm[MINIMP3_MAX_SAMPLES_PER_FRAME];
        int samples = 0;
        int firstFrameOffset = 0;

        
        for (int tries = 0; tries < 64; tries++) {
            if (mp3BufferLen - mp3BufferPos < 4096 && !feof(currentAudioFile)) {
                size_t rem = mp3BufferLen - mp3BufferPos;
                if (rem > 0) memmove(mp3ReadBuffer, mp3ReadBuffer + mp3BufferPos, rem);
                size_t r = fread(mp3ReadBuffer + rem, 1, FILE_READ_CHUNK - rem, currentAudioFile);
                mp3BufferLen = rem + r;
                mp3BufferPos = 0;
                audioStartOffset = ftell(currentAudioFile) - (long)mp3BufferLen;
            }
            if (mp3BufferPos >= mp3BufferLen) break;

            samples = mp3dec_decode_frame(&mp3d, mp3ReadBuffer + mp3BufferPos, mp3BufferLen - mp3BufferPos, tempPcm, &info);
            if (samples > 0 && info.hz > 0) {
                firstFrameOffset = (int)mp3BufferPos + info.frame_offset;
                break;
            }
            if (info.frame_bytes > 0 && mp3BufferPos + info.frame_bytes <= mp3BufferLen) {
                mp3BufferPos += info.frame_bytes;
            } else {
                break;
            }
        }

        if (samples <= 0 || info.hz == 0) {
            fclose(currentAudioFile); currentAudioFile = NULL;
            LightLock_Unlock(&audioLock);
            return;
        }

        currentSampleRate = info.hz;
        currentChannels = info.channels;
        currentFormat = FORMAT_MP3;

        
        float vbrDuration = 0.0f;
        int scanStart = firstFrameOffset;
        int scanEnd = scanStart + 2048;
        if (scanEnd > (int)mp3BufferLen) scanEnd = (int)mp3BufferLen;

        
        int spf = 1152;
        if (scanStart + 4 <= (int)mp3BufferLen) {
            uint8_t b1 = mp3ReadBuffer[scanStart + 1];
            int verId = (b1 >> 3) & 3;
            int layer = (b1 >> 1) & 3;
            if (layer == 3) spf = 384;
            else if (layer == 1 && verId != 3) spf = 576;
        }

        for (int k = scanStart; k + 18 <= scanEnd; k++) {
            if ((memcmp(mp3ReadBuffer + k, "Xing", 4) == 0 || memcmp(mp3ReadBuffer + k, "Info", 4) == 0)) {
                uint32_t xflags = ((uint32_t)mp3ReadBuffer[k+4] << 24) |
                                  ((uint32_t)mp3ReadBuffer[k+5] << 16) |
                                  ((uint32_t)mp3ReadBuffer[k+6] << 8)  |
                                  (uint32_t)mp3ReadBuffer[k+7];
                if (xflags & 0x01) {
                    uint32_t totalFrames = ((uint32_t)mp3ReadBuffer[k+8]  << 24) |
                                           ((uint32_t)mp3ReadBuffer[k+9]  << 16) |
                                           ((uint32_t)mp3ReadBuffer[k+10] << 8)  |
                                           (uint32_t)mp3ReadBuffer[k+11];
                    if (totalFrames > 0) {
                        vbrDuration = (float)((double)totalFrames * (double)spf / (double)info.hz);
                    }
                }
                break;
            }
            if (memcmp(mp3ReadBuffer + k, "VBRI", 4) == 0) {
                uint32_t totalFrames = ((uint32_t)mp3ReadBuffer[k+14] << 24) |
                                       ((uint32_t)mp3ReadBuffer[k+15] << 16) |
                                       ((uint32_t)mp3ReadBuffer[k+16] << 8)  |
                                       (uint32_t)mp3ReadBuffer[k+17];
                if (totalFrames > 0) {
                    vbrDuration = (float)((double)totalFrames * (double)spf / (double)info.hz);
                }
                break;
            }
        }

        fseek(currentAudioFile, 0, SEEK_END);
        long totalFileSz = ftell(currentAudioFile);
        mp3DataStartOffset = audioStartOffset + firstFrameOffset;
        mp3DataSize = totalFileSz - mp3DataStartOffset;
        if (mp3DataSize < 0) mp3DataSize = totalFileSz;
        if (vbrDuration > 0.5f) {
            songDuration = vbrDuration;
        } else if (id3TlenSec > 0.5f) {
            songDuration = id3TlenSec;
        } else {
            long brSum = 0;
            int brCnt = 0;
            size_t probePos = mp3BufferPos + info.frame_bytes;
            mp3dec_t probeDec;
            mp3dec_init(&probeDec);
            for (int fIdx = 0; fIdx < 24 && probePos < mp3BufferLen; fIdx++) {
                mp3dec_frame_info_t pInfo;
                int ps = mp3dec_decode_frame(&probeDec, mp3ReadBuffer + probePos, mp3BufferLen - probePos, NULL, &pInfo);
                if (pInfo.frame_bytes <= 0) break;
                if (ps > 0 && pInfo.bitrate_kbps > 0) {
                    brSum += pInfo.bitrate_kbps;
                    brCnt++;
                }
                probePos += pInfo.frame_bytes;
            }
            int estBitrate = (brCnt > 0) ? (int)(brSum / brCnt) : info.bitrate_kbps;
            songDuration = (estBitrate > 0) ? (float)((double)mp3DataSize * 8.0 / ((double)estBitrate * 1000.0)) : 180.0f;
        }

        if (audioStartOffset + (long)mp3BufferLen > mp3DataStartOffset && firstFrameOffset < (int)mp3BufferLen) {
            size_t keep = mp3BufferLen - (size_t)firstFrameOffset;
            memmove(mp3ReadBuffer, mp3ReadBuffer + firstFrameOffset, keep);
            mp3BufferLen = keep;
            fseek(currentAudioFile, audioStartOffset + (long)(firstFrameOffset + keep), SEEK_SET);
        } else {
            fseek(currentAudioFile, mp3DataStartOffset, SEEK_SET);
            mp3BufferLen = fread(mp3ReadBuffer, 1, FILE_READ_CHUNK, currentAudioFile);
        }
        mp3BufferPos = 0;
        mp3PcmPos = 0;
        mp3PcmCount = 0;
        mp3dec_init(&mp3d);
    } else if (has_extension(fileName, ".wav")) {
        if (!drwav_init_file(&wavState, filePath, NULL)) { LightLock_Unlock(&audioLock); return; }
        isWavOpen = true;
        currentSampleRate = wavState.sampleRate;
        currentChannels = wavState.channels;
        songDuration = (float)wavState.totalPCMFrameCount / (float)currentSampleRate;
        currentFormat = FORMAT_WAV;
    } else if (has_extension(fileName, ".flac")) { //im killing myself
        flacFile = fopen(filePath, "rb");
        if (!flacFile) { LightLock_Unlock(&audioLock); return; }
        setvbuf(flacFile, NULL, _IOFBF, 64 * 1024);
        flacState = drflac_open_with_metadata(ihateflac_read, ihateflac_seek, ihateflac_tell, ihateflac_meta_shit, flacFile, NULL);
        if (!flacState) {
            fclose(flacFile); flacFile = NULL;
            LightLock_Unlock(&audioLock);
            return;
        }
        currentSampleRate = flacState->sampleRate;
        currentChannels = flacState->channels;
        songDuration = (float)flacState->totalPCMFrameCount / (float)currentSampleRate;
        currentFormat = FORMAT_FLAC;

    } else if (has_extension(fileName, ".ogg")) {
        FILE* f = fopen(filePath, "rb");
        if (!f) { LightLock_Unlock(&audioLock); return; }
        setvbuf(f, NULL, _IONBF, 0); 
        fseek(f, 0, SEEK_END);
        long fSize = ftell(f);
        fseek(f, 0, SEEK_SET);
        if (fSize <= 0) {
            fclose(f);
            LightLock_Unlock(&audioLock);
            return;
        }
        oggFastStream.fp = f;
        oggFastStream.fileSize = fSize;
        oggFastStream.cursor = 0;
        oggFastStream.cacheOffset = 0;
        oggFastStream.cacheLen = 0;
        if (ov_open_callbacks(&oggFastStream, &oggFile, NULL, 0, oggFastCallbacks) < 0) {
            ogg_cb_close(&oggFastStream);
            LightLock_Unlock(&audioLock);
            return;
        }
        isOggOpen = true;
        
        vorbis_info* vi = ov_info(&oggFile, -1);
        currentSampleRate = (vi && vi->rate > 0) ? (int)vi->rate : 44100;
        currentChannels = (vi && vi->channels > 0) ? (int)vi->channels : 2;
        ogg_int64_t pcmTot = ov_pcm_total(&oggFile, -1);
        if (pcmTot > 0 && currentSampleRate > 0) {
            songDuration = (float)((double)pcmTot / (double)currentSampleRate);
        } else {
            ogg_int64_t msTot = ov_time_total(&oggFile, -1);
            songDuration = (msTot > 0) ? (float)((double)msTot / 1000.0) : 180.0f;
        }
        vorbis_comment* vc = ov_comment(&oggFile, -1);
        if (vc) {
            parse_oggshits_array(vc->user_comments, vc->comment_lengths, vc->comments,
                                        currentTrackTitle, sizeof(currentTrackTitle),
                                        currentTrackArtist, sizeof(currentTrackArtist),
                                        currentTrackAlbum, sizeof(currentTrackAlbum),
                                        &detectedBPM, &hasTrackBpmTag);
        }
        currentFormat = FORMAT_OGG;

    } else if (has_extension(fileName, ".opus")) {
        int error = 0;
        opusFile = op_open_file(filePath, &error);
        if (!opusFile || error != 0) { LightLock_Unlock(&audioLock); return; }
        currentSampleRate = 48000;
        currentChannels = 2;
        ogg_int64_t pcmTot = op_pcm_total(opusFile, -1);
        songDuration = (pcmTot > 0) ? (float)pcmTot / 48000.0f : 180.0f;
        const OpusTags* tags = op_tags(opusFile, -1);
        if (tags) {
            parse_oggshits_array(tags->user_comments, tags->comment_lengths, tags->comments,
                                        currentTrackTitle, sizeof(currentTrackTitle),
                                        currentTrackArtist, sizeof(currentTrackArtist),
                                        currentTrackAlbum, sizeof(currentTrackAlbum),
                                        &detectedBPM, &hasTrackBpmTag);
        }
        currentFormat = FORMAT_OPUS;
    } else if (has_extension(fileName, ".m4a") || has_extension(fileName, ".mp4")) {
        if (!m4a_open(&m4aDemuxer, filePath)) { LightLock_Unlock(&audioLock); return; }
        isM4aOpen = true;
        aacDec = NeAACDecOpen();
        if (!aacDec) { m4a_close(&m4aDemuxer); isM4aOpen = false; LightLock_Unlock(&audioLock); return; }
        NeAACDecConfigurationPtr cfg = NeAACDecGetCurrentConfiguration(aacDec);
        cfg->outputFormat = FAAD_FMT_16BIT;
        NeAACDecSetConfiguration(aacDec, cfg);

        unsigned long sr = 0;
        unsigned char ch = 0;
        if (NeAACDecInit2(aacDec, m4aDemuxer.asc, (unsigned long)m4aDemuxer.ascLen, &sr, &ch) < 0 || sr == 0) {
            NeAACDecClose(aacDec); aacDec = NULL;
            m4a_close(&m4aDemuxer); isM4aOpen = false;
            LightLock_Unlock(&audioLock);
            return;
        }

        currentSampleRate = (int)sr;
        currentChannels = (ch > 0) ? (int)ch : 2;
        songDuration = (m4aDemuxer.durationSec > 0.0f) ? m4aDemuxer.durationSec : 180.0f;
        if (m4aDemuxer.title[0] != '\0') snprintf(currentTrackTitle, sizeof(currentTrackTitle), "%s", m4aDemuxer.title);
        if (m4aDemuxer.artist[0] != '\0') snprintf(currentTrackArtist, sizeof(currentTrackArtist), "%s", m4aDemuxer.artist);
        if (m4aDemuxer.album[0] != '\0') snprintf(currentTrackAlbum, sizeof(currentTrackAlbum), "%s", m4aDemuxer.album);
        if (m4aDemuxer.hasBpm) {
            detectedBPM = m4aDemuxer.bpm;
            hasTrackBpmTag = true;
        }
        aacPcmPos = 0;
        aacPcmCount = 0;
        currentFormat = FORMAT_M4A;
    } else if (has_extension(fileName, ".aac")) {
        currentAudioFile = fopen(filePath, "rb");
        if (!currentAudioFile) { LightLock_Unlock(&audioLock); return; }
        setvbuf(currentAudioFile, NULL, _IOFBF, 64 * 1024);

        fseek(currentAudioFile, 0, SEEK_END);
        long totalFileSize = ftell(currentAudioFile);
        fseek(currentAudioFile, 0, SEEK_SET);

        long audioStartOffset = 0;
        float id3TlenSec = 0.0f;
        parse_mp3_id3v2_meta(currentAudioFile, &audioStartOffset, &id3TlenSec);
        mp3DataStartOffset = audioStartOffset;
        mp3DataSize = (totalFileSize > mp3DataStartOffset) ? (totalFileSize - mp3DataStartOffset) : 0;

        fseek(currentAudioFile, mp3DataStartOffset, SEEK_SET);
        mp3BufferLen = fread(mp3ReadBuffer, 1, FILE_READ_CHUNK, currentAudioFile);
        mp3BufferPos = 0;

        aacDec = NeAACDecOpen();
        if (!aacDec || mp3BufferLen == 0) {
            if (aacDec) { NeAACDecClose(aacDec); aacDec = NULL; }
            fclose(currentAudioFile); currentAudioFile = NULL;
            LightLock_Unlock(&audioLock);
            return;
        }
        NeAACDecConfigurationPtr cfg = NeAACDecGetCurrentConfiguration(aacDec);
        cfg->outputFormat = FAAD_FMT_16BIT;
        NeAACDecSetConfiguration(aacDec, cfg);

        unsigned long sr = 0;
        unsigned char ch = 0;
        long initBytes = NeAACDecInit(aacDec, mp3ReadBuffer, (unsigned long)mp3BufferLen, &sr, &ch);
        if (initBytes < 0 || sr == 0) {
            NeAACDecClose(aacDec); aacDec = NULL;
            fclose(currentAudioFile); currentAudioFile = NULL;
            LightLock_Unlock(&audioLock);
            return;
        }
        mp3BufferPos = (size_t)initBytes;
        currentSampleRate = (int)sr;
        currentChannels = (ch > 0) ? (int)ch : 2;
        songDuration = (mp3DataSize > 0) ? (float)mp3DataSize / 16000.0f : 180.0f;
        aacPcmPos = 0;
        aacPcmCount = 0;
        currentFormat = FORMAT_AAC;
    } else {
        LightLock_Unlock(&audioLock);
        return;
    }

    fake_some_waveform_bars();
    LightLock_Unlock(&audioLock);

    int prevPlaying = playingFileIdx;
    playingFileIdx = selectedFile;
    coverScrollCount = 0;
    coverScrollPos = 0;

    if (prevPlaying == selectedFile && hasCoverCurrent && slotSongIdx[slotCurrent] == selectedFile) {
        coverTransDir = TRANS_NONE;
        coverTransProgress = 1.0f;
    } else if (transDirection != TRANS_NONE && prevPlaying >= 0 && prevPlaying != selectedFile && fileCount > 1 &&
               selectedFile != find_next_song_index(prevPlaying, 1) &&
               selectedFile != find_next_song_index(prevPlaying, -1)) {
        int step = (selectedFile > prevPlaying) ? 1 : -1;
        TransitionDir dir = (step > 0) ? TRANS_NEXT : TRANS_PREV;
        int tempList[MAX_FILES];
        int tempCount = 0;
        for (int i = prevPlaying + step; (step > 0) ? (i <= selectedFile) : (i >= selectedFile); i += step) {
            if (!fileList[i].is_dir) {
                tempList[tempCount++] = i;
            }
        }
        if (tempCount > 1) {
            int maxSteps = 6;
            if (tempCount <= maxSteps) {
                for (int k = 0; k < tempCount; k++) coverScrollQueue[k] = tempList[k];
                coverScrollCount = tempCount;
            } else {
                for (int k = 0; k < maxSteps - 1; k++) {
                    int idx = (k * (tempCount - 1)) / (maxSteps - 1);
                    coverScrollQueue[k] = tempList[idx];
                }
                coverScrollQueue[maxSteps - 1] = selectedFile;
                coverScrollCount = maxSteps;
            }
            coverScrollDir = dir;
            coverScrollPos = 0;
            advance_cover_slot_step(coverScrollQueue[0], coverScrollDir, true);
        } else {
            advance_cover_slot_step(selectedFile, dir, false);
        }
    } else if (transDirection == TRANS_NEXT || transDirection == TRANS_PREV) {
        advance_cover_slot_step(selectedFile, transDirection, false);
    } else {
        refresh_the_3_covers();
        coverTransDir = TRANS_NONE;
        coverTransProgress = 1.0f;
    }
    mark_app_state_dirty();

    LightLock_Lock(&audioLock);

    ndspChnReset(AUDIO_CHANNEL);
    ndspChnSetInterp(AUDIO_CHANNEL, NDSP_INTERP_LINEAR);
    ndspChnSetRate(AUDIO_CHANNEL, (float)currentSampleRate);
    ndspChnSetFormat(AUDIO_CHANNEL, NDSP_FORMAT_STEREO_PCM16);
    float mix[12];
    memset(mix, 0, sizeof(mix));
    mix[0] = 1.0f; mix[1] = 1.0f;
    ndspChnSetMix(AUDIO_CHANNEL, mix);

    samplesCompleted = 0;
    samplesFed = 0;
    reset_beat_tracking(false);
    audioStreamEOF = false;
    trackFinished = false;
    songTime = 0.0f;
    isPlaying = true;
    isPaused = false;
    gustinaState = GUSTINA_IDLE;
    lastBeat = -1;

    for (int pre = 0; pre < 2; pre++) {
        if (waveBuf[pre].status == NDSP_WBUF_DONE) {
            int s = fill_audio_buffer(audioBuffer[pre], SAMPLES_PER_BUF);
            if (s > 0) {
                DSP_FlushDataCache(audioBuffer[pre], s * 2 * sizeof(int16_t));
                waveBuf[pre].nsamples = s;
                waveBufSamples[pre] = (u32)s;
                ndspChnWaveBufAdd(AUDIO_CHANNEL, &waveBuf[pre]);
            }
        }
    }

    LightLock_Unlock(&audioLock);

    bool hasLocalLrc = (lrcLineCount > 0) || lrc_file_exists(currentTrackPath);
    LightLock_Lock(&lyricsLock);
    currentTrackGeneration++;
    downloadedLrcReady = false;
    lyricsDebugStatus[0] = '\0';
    if (!hasLocalLrc) {
        memset(&pendingLyricsJob, 0, sizeof(pendingLyricsJob));
        strncpy(pendingLyricsJob.songPath, currentTrackPath, sizeof(pendingLyricsJob.songPath) - 1);
        strncpy(pendingLyricsJob.title, currentTrackTitle, sizeof(pendingLyricsJob.title) - 1);
        strncpy(pendingLyricsJob.artist, currentTrackArtist, sizeof(pendingLyricsJob.artist) - 1);
        strncpy(pendingLyricsJob.album, currentTrackAlbum, sizeof(pendingLyricsJob.album) - 1);
        pendingLyricsJob.durationSec = (int)(songDuration + 0.5f);
        pendingLyricsJob.generation = currentTrackGeneration;
        pendingLyricsJob.enqueueTimeMs = osGetTime();
        hasPendingLyricsJob = true;
    } else {
        hasPendingLyricsJob = false;
    }
    LightLock_Unlock(&lyricsLock);

    lastfm_scrobbled_track  = false;
    lastfm_played_seconds   = 0.0f;
    lastfm_track_start_ts   = (u32)time(NULL);
    tell_lastfm_im_listening();
}


static int fill_audio_buffer(int16_t* dest, int samplesNeeded) {
    int samplesWritten = 0;
    if (currentFormat == FORMAT_MP3 && currentAudioFile) {
        
        while (samplesWritten < samplesNeeded && mp3PcmPos < mp3PcmCount) {
            if (mp3PcmChannels == 2) {
                dest[samplesWritten*2]   = apply_gain_and_limit(mp3PcmResid[mp3PcmPos*2], volumeGain);
                dest[samplesWritten*2+1] = apply_gain_and_limit(mp3PcmResid[mp3PcmPos*2+1], volumeGain);
            } else {
                int16_t m = apply_gain_and_limit(mp3PcmResid[mp3PcmPos], volumeGain);
                dest[samplesWritten*2] = m; dest[samplesWritten*2+1] = m;
            }
            mp3PcmPos++;
            samplesWritten++;
        }

        while (samplesWritten < samplesNeeded && currentAudioFile) {
            if (mp3BufferLen - mp3BufferPos < 4096 && !feof(currentAudioFile)) {
                size_t rem = mp3BufferLen - mp3BufferPos;
                if (rem > 0) memmove(mp3ReadBuffer, mp3ReadBuffer + mp3BufferPos, rem);
                size_t read = fread(mp3ReadBuffer + rem, 1, FILE_READ_CHUNK - rem, currentAudioFile);
                mp3BufferLen = rem + read;
                mp3BufferPos = 0;
            }
            if (mp3BufferLen <= mp3BufferPos) break;

            mp3dec_frame_info_t info;
            int s = mp3dec_decode_frame(&mp3d, mp3ReadBuffer + mp3BufferPos, mp3BufferLen - mp3BufferPos, mp3PcmResid, &info);
            mp3BufferPos += info.frame_bytes;

            if (s > 0) {
                int i = 0;
                while (i < s && samplesWritten < samplesNeeded) {
                    if (info.channels == 2) {
                        dest[samplesWritten*2]   = apply_gain_and_limit(mp3PcmResid[i*2], volumeGain);
                        dest[samplesWritten*2+1] = apply_gain_and_limit(mp3PcmResid[i*2+1], volumeGain);
                    } else {
                        int16_t m = apply_gain_and_limit(mp3PcmResid[i], volumeGain);
                        dest[samplesWritten*2] = m; dest[samplesWritten*2+1] = m;
                    }
                    samplesWritten++;
                    i++;
                }
                if (i < s) {
                    mp3PcmPos = i;
                    mp3PcmCount = s;
                    mp3PcmChannels = info.channels;
                } else {
                    mp3PcmPos = 0;
                    mp3PcmCount = 0;
                }
            } else if (info.frame_bytes == 0) break;
        }
    } else if (currentFormat == FORMAT_WAV && isWavOpen) {
        int16_t temp[512 * 2];
        while (samplesWritten < samplesNeeded) {
            int toRead = samplesNeeded - samplesWritten;
            if (toRead > 512) toRead = 512;
            drwav_uint64 read = drwav_read_pcm_frames_s16(&wavState, toRead, temp);
            if (read == 0) break;
            for (drwav_uint64 i = 0; i < read; i++) {
                if (wavState.channels == 2) {
                    dest[samplesWritten*2]   = apply_gain_and_limit(temp[i*2], volumeGain);
                    dest[samplesWritten*2+1] = apply_gain_and_limit(temp[i*2+1], volumeGain);
                } else {
                    int16_t m = apply_gain_and_limit(temp[i], volumeGain);
                    dest[samplesWritten*2] = m; dest[samplesWritten*2+1] = m;
                }
                samplesWritten++;
            }
        }
    } else if (currentFormat == FORMAT_FLAC && flacState) { //I HGATE FLAC OMFGGGG
        if (flacState->channels == 2) {
            drflac_uint64 read = drflac_read_pcm_frames_s16(flacState, samplesNeeded - samplesWritten, dest + (samplesWritten * 2));
            samplesWritten += (int)read;
        } else {
            int16_t temp[512 * 2];
            while (samplesWritten < samplesNeeded) {
                int toRead = samplesNeeded - samplesWritten;
                if (toRead > 512) toRead = 512;
                drflac_uint64 read = drflac_read_pcm_frames_s16(flacState, toRead, temp);
                if (read == 0) break;
                for (drflac_uint64 i = 0; i < read; i++) {
                    dest[samplesWritten*2]   = temp[i];
                    dest[samplesWritten*2+1] = temp[i];
                    samplesWritten++;
                }
            }
        }
    } else if (currentFormat == FORMAT_OGG && isOggOpen) {
        char temp[2048];
        int sec = 0;
        while (samplesWritten < samplesNeeded) {
            int maxBytes = (samplesNeeded - samplesWritten) * currentChannels * (int)sizeof(int16_t);
            if (maxBytes > (int)sizeof(temp)) maxBytes = (int)sizeof(temp);
            if (maxBytes <= 0) break;
            long bytes = ov_read(&oggFile, temp, maxBytes, &sec);
            if (bytes <= 0) break;
            int frames = bytes / (currentChannels * sizeof(int16_t));
            int16_t* pcm = (int16_t*)temp;
            for (int i = 0; i < frames; i++) {
                if (samplesWritten >= samplesNeeded) break;
                if (currentChannels == 2) {
                    dest[samplesWritten*2]   = apply_gain_and_limit(pcm[i*2], volumeGain);
                    dest[samplesWritten*2+1] = apply_gain_and_limit(pcm[i*2+1], volumeGain);
                } else {
                    int16_t m = apply_gain_and_limit(pcm[i], volumeGain);
                    dest[samplesWritten*2] = m; dest[samplesWritten*2+1] = m;
                }
                samplesWritten++;
            }
        }
    } else if (currentFormat == FORMAT_OPUS && opusFile) {
        int16_t temp[512 * 2];
        while (samplesWritten < samplesNeeded) {
            int toRead = samplesNeeded - samplesWritten;
            if (toRead > 512) toRead = 512;
            int read = op_read_stereo(opusFile, temp, toRead * 2);
            if (read <= 0) break;
            for (int i = 0; i < read; i++) {
                dest[samplesWritten*2]   = apply_gain_and_limit(temp[i*2], volumeGain);
                dest[samplesWritten*2+1] = apply_gain_and_limit(temp[i*2+1], volumeGain);
                samplesWritten++;
            }
        }
    } else if ((currentFormat == FORMAT_M4A && isM4aOpen && aacDec) ||
               (currentFormat == FORMAT_AAC && currentAudioFile && aacDec)) {
        while (samplesWritten < samplesNeeded && aacPcmPos < aacPcmCount) {
            if (aacPcmChannels >= 2) {
                dest[samplesWritten*2]   = apply_gain_and_limit(aacPcmResid[aacPcmPos*aacPcmChannels], volumeGain);
                dest[samplesWritten*2+1] = apply_gain_and_limit(aacPcmResid[aacPcmPos*aacPcmChannels+1], volumeGain);
            } else {
                int16_t s = apply_gain_and_limit(aacPcmResid[aacPcmPos], volumeGain);
                dest[samplesWritten*2]   = s;
                dest[samplesWritten*2+1] = s;
            }
            aacPcmPos++;
            samplesWritten++;
        }

        while (samplesWritten < samplesNeeded) {
            NeAACDecFrameInfo fi;
            memset(&fi, 0, sizeof(fi));
            int16_t* decoded = NULL;

            if (currentFormat == FORMAT_M4A) {
                uint32_t pktSz = m4a_read_sample(&m4aDemuxer, aacPacketBuf, sizeof(aacPacketBuf));
                if (pktSz == 0) break;
                decoded = (int16_t*)NeAACDecDecode(aacDec, &fi, aacPacketBuf, (unsigned long)pktSz);
            } else {
                if (mp3BufferLen - mp3BufferPos < 4096) {
                    size_t rem = mp3BufferLen - mp3BufferPos;
                    if (rem > 0 && mp3BufferPos > 0) memmove(mp3ReadBuffer, mp3ReadBuffer + mp3BufferPos, rem);
                    size_t r = fread(mp3ReadBuffer + rem, 1, FILE_READ_CHUNK - rem, currentAudioFile);
                    mp3BufferLen = rem + r;
                    mp3BufferPos = 0;
                }
                if (mp3BufferLen <= mp3BufferPos) break;
                decoded = (int16_t*)NeAACDecDecode(aacDec, &fi, mp3ReadBuffer + mp3BufferPos,
                                                   (unsigned long)(mp3BufferLen - mp3BufferPos));
                if (fi.bytesconsumed > 0) {
                    mp3BufferPos += (size_t)fi.bytesconsumed;
                    if (mp3BufferPos > mp3BufferLen) mp3BufferPos = mp3BufferLen;
                } else if (fi.error != 0) {
                    mp3BufferPos++;
                    continue;
                } else {
                    break;
                }
            }

            if (fi.error != 0 || !decoded || fi.samples == 0 || fi.channels == 0) {
                if (currentFormat == FORMAT_M4A) continue;
                else continue;
            }

            int frames = (int)(fi.samples / fi.channels);
            if (frames > 4096) frames = 4096;
            int ch = (int)fi.channels;
            if (ch > 2) ch = 2;
            aacPcmChannels = ch;
            for (int f = 0; f < frames; f++) {
                if (ch == 2) {
                    aacPcmResid[f * 2]     = decoded[f * fi.channels];
                    aacPcmResid[f * 2 + 1] = decoded[f * fi.channels + 1];
                } else {
                    aacPcmResid[f] = decoded[f * fi.channels];
                }
            }
            aacPcmPos = 0;
            aacPcmCount = frames;

            while (samplesWritten < samplesNeeded && aacPcmPos < aacPcmCount) {
                if (aacPcmChannels >= 2) {
                    dest[samplesWritten*2]   = apply_gain_and_limit(aacPcmResid[aacPcmPos*2], volumeGain);
                    dest[samplesWritten*2+1] = apply_gain_and_limit(aacPcmResid[aacPcmPos*2+1], volumeGain);
                } else {
                    int16_t s = apply_gain_and_limit(aacPcmResid[aacPcmPos], volumeGain);
                    dest[samplesWritten*2]   = s;
                    dest[samplesWritten*2+1] = s;
                }
                aacPcmPos++;
                samplesWritten++;
            }
        }
    }

    if (samplesWritten > 0) {
        do_the_bpm_thingy(dest, samplesWritten);
        ear_destroyer_limiter(dest, samplesWritten, volumeGain);
    }

    if (samplesWritten < samplesNeeded) {
        memset(dest + (samplesWritten * 2), 0, (samplesNeeded - samplesWritten) * 2 * sizeof(int16_t));
    }

    return samplesWritten;
}

static void step_audio_buffers(void) {
    for (int i = 0; i < NUM_WAVE_BUFS; i++) {
        if (waveBuf[i].status == NDSP_WBUF_DONE) {
            if (waveBufSamples[i] > 0) {
                samplesCompleted += waveBufSamples[i];
                waveBufSamples[i] = 0;
            }
            if (!audioStreamEOF) {
                int samples = fill_audio_buffer(audioBuffer[i], SAMPLES_PER_BUF);
                if (samples > 0) {
                    DSP_FlushDataCache(audioBuffer[i], samples * 2 * sizeof(int16_t));
                    waveBuf[i].nsamples = samples;
                    waveBufSamples[i] = (u32)samples;
                    ndspChnWaveBufAdd(AUDIO_CHANNEL, &waveBuf[i]);
                } else {
                    audioStreamEOF = true;
                }
            }
        }
    }

    if (audioStreamEOF && isPlaying) {
        bool anyBusy = false;
        for (int i = 0; i < NUM_WAVE_BUFS; i++) {
            if (waveBuf[i].status != NDSP_WBUF_DONE) {
                anyBusy = true;
                break;
            }
        }
        if (!anyBusy) {
            isPlaying = false;
            trackFinished = true;
        }
    }
}

static void step_pause_bgm_buffers(void) {
    if (!hasPauseBgm || !pauseBgmBuffer || pauseBgmTotalFrames == 0) return;
    for (int i = 0; i < PAUSE_NUM_BUFS; i++) {
        if (pauseBgmWaveBuf[i].status == NDSP_WBUF_DONE) {
            int16_t* dst = pauseBgmStreamBuf[i];
            u32 remaining = PAUSE_BUF_FRAMES;
            u32 dstOff = 0;
            while (remaining > 0) {
                u32 avail = pauseBgmTotalFrames - pauseBgmCursor;
                u32 chunk = (remaining < avail) ? remaining : avail;
                memcpy(dst + (dstOff * pauseBgmChannels),
                       pauseBgmBuffer + (pauseBgmCursor * pauseBgmChannels),
                       chunk * pauseBgmChannels * sizeof(int16_t));
                pauseBgmCursor += chunk;
                if (pauseBgmCursor >= pauseBgmTotalFrames) pauseBgmCursor = 0;
                dstOff += chunk;
                remaining -= chunk;
            }
            DSP_FlushDataCache(dst, PAUSE_BUF_FRAMES * pauseBgmChannels * sizeof(int16_t));
            pauseBgmWaveBuf[i].nsamples = PAUSE_BUF_FRAMES;
            ndspChnWaveBufAdd(PAUSE_BGM_CHANNEL, &pauseBgmWaveBuf[i]);
        }
    }
}

void audio_thread_entry(void* arg) {
    while (audioThreadRun) {
        if (isAppSuspended) {
            svcSleepThread(25 * 1000 * 1000LL);
            continue;
        }
        if (isPlaying && !isPaused) {
            LightLock_Lock(&audioLock);
            if (!isAppSuspended && isPlaying && !isPaused) {
                step_audio_buffers();
            }
            LightLock_Unlock(&audioLock);
        } else if (hasPauseBgm && pauseBgmPlaying) {
            step_pause_bgm_buffers();
        }
        svcSleepThread(5 * 1000 * 1000LL); 
    }
    threadExit(0);
}

void update_audio_stream_fallback(void) {
    if (audioThread != NULL) return;
    if (!isPlaying || isPaused) return;
    step_audio_buffers();
}


void seek_current_track(float targetSec) {
    if (!isPlaying || currentFormat == FORMAT_NONE || currentSampleRate <= 0) return;

    if (targetSec < 0.0f) targetSec = 0.0f;
    if (songDuration > 0.5f && targetSec > songDuration - 0.3f) {
        targetSec = songDuration - 0.3f;
    }

    LightLock_Lock(&audioLock);

    ndspChnReset(AUDIO_CHANNEL);
    ndspChnSetInterp(AUDIO_CHANNEL, NDSP_INTERP_LINEAR);
    ndspChnSetRate(AUDIO_CHANNEL, (float)currentSampleRate);
    ndspChnSetFormat(AUDIO_CHANNEL, NDSP_FORMAT_STEREO_PCM16);

    float mix[12];
    memset(mix, 0, sizeof(mix));
    mix[0] = 1.0f; mix[1] = 1.0f;
    ndspChnSetMix(AUDIO_CHANNEL, mix);

    for (int i = 0; i < NUM_WAVE_BUFS; i++) {
        waveBuf[i].status = NDSP_WBUF_DONE;
        waveBufSamples[i] = 0;
    }
    audioStreamEOF = false;
    trackFinished = false;

    uint64_t targetFrame = (uint64_t)((double)targetSec * (double)currentSampleRate);

    if (currentFormat == FORMAT_WAV && isWavOpen) {
        if (wavState.totalPCMFrameCount > 0 && targetFrame >= wavState.totalPCMFrameCount) {
            targetFrame = wavState.totalPCMFrameCount - 1;
        }
        drwav_seek_to_pcm_frame(&wavState, targetFrame);
    } else if (currentFormat == FORMAT_FLAC && flacState) {
        if (flacState->totalPCMFrameCount > 0 && targetFrame >= flacState->totalPCMFrameCount) {
            targetFrame = flacState->totalPCMFrameCount - 1;
        }
        drflac_seek_to_pcm_frame(flacState, targetFrame);
    } else if (currentFormat == FORMAT_OGG && isOggOpen) {
        ov_pcm_seek(&oggFile, (ogg_int64_t)targetFrame);
    } else if (currentFormat == FORMAT_OPUS && opusFile) {
        op_pcm_seek(opusFile, (ogg_int64_t)targetFrame);
    } else if (currentFormat == FORMAT_M4A && isM4aOpen && aacDec) {
        float actualSec = m4a_seek(&m4aDemuxer, targetSec);
        targetSec = actualSec;
        targetFrame = (uint64_t)((double)targetSec * (double)currentSampleRate);
        NeAACDecPostSeekReset(aacDec, 1);
        aacPcmPos = 0;
        aacPcmCount = 0;
    } else if (currentFormat == FORMAT_AAC && currentAudioFile && aacDec && songDuration > 0.0f && mp3DataSize > 0) {
        long byteOff = mp3DataStartOffset + (long)(((double)targetSec / (double)songDuration) * (double)mp3DataSize);
        if (byteOff < mp3DataStartOffset) byteOff = mp3DataStartOffset;
        fseek(currentAudioFile, byteOff, SEEK_SET);
        mp3BufferLen = fread(mp3ReadBuffer, 1, FILE_READ_CHUNK, currentAudioFile);
        mp3BufferPos = 0;
        while (mp3BufferPos + 2 < mp3BufferLen) {
            if (mp3ReadBuffer[mp3BufferPos] == 0xFF && (mp3ReadBuffer[mp3BufferPos + 1] & 0xF0) == 0xF0) break;
            mp3BufferPos++;
        }
        NeAACDecPostSeekReset(aacDec, 1);
        aacPcmPos = 0;
        aacPcmCount = 0;
    } else if (currentFormat == FORMAT_MP3 && currentAudioFile && songDuration > 0.0f && mp3DataSize > 0) {
        long byteOff = mp3DataStartOffset + (long)(((double)targetSec / (double)songDuration) * (double)mp3DataSize);
        if (byteOff < mp3DataStartOffset) byteOff = mp3DataStartOffset;
        fseek(currentAudioFile, byteOff, SEEK_SET);
        mp3BufferLen = fread(mp3ReadBuffer, 1, FILE_READ_CHUNK, currentAudioFile);
        mp3BufferPos = 0;
        mp3PcmPos = 0;
        mp3PcmCount = 0;
        mp3dec_init(&mp3d);
    }

    samplesCompleted = targetFrame;
    samplesFed = targetFrame;
    beatQueueHead = 0;
    beatQueueTail = 0;
    lastDetectedBeatTime = targetSec;
    nextVisualBeatTime = targetSec;
    visualBeatLocked = false;
    osfWriteIdx = 0;
    osfCount = 0;
    osfFramesSinceCalc = 0;
    beatPrevBass = 0.0f;
    beatPrevMid = 0.0f;
    songTime = targetSec;
    currentLrcIndex = -1;
    lyricScrollPos = -1.0f;
    if (detectedBPM > 0.0f) {
        lastBeat = (int)(songTime / (60.0f / detectedBPM));
    }

    for (int pre = 0; pre < 2; pre++) {
        if (waveBuf[pre].status == NDSP_WBUF_DONE) {
            int s = fill_audio_buffer(audioBuffer[pre], SAMPLES_PER_BUF);
            if (s > 0) {
                DSP_FlushDataCache(audioBuffer[pre], s * 2 * sizeof(int16_t));
                waveBuf[pre].nsamples = s;
                waveBufSamples[pre] = (u32)s;
                ndspChnWaveBufAdd(AUDIO_CHANNEL, &waveBuf[pre]);
            }
        }
    }
    if (isPaused) {
        ndspChnSetPaused(AUDIO_CHANNEL, true);
    }
    LightLock_Unlock(&audioLock);
}


void init_pause_bgm(void) {
    drwav wav;
    if (!drwav_init_file(&wav, "romfs:/pause_bg.wav", NULL)) return;
    if (wav.totalPCMFrameCount == 0 || wav.channels == 0) {
        drwav_uninit(&wav);
        return;
    }
    size_t totalSamples = (size_t)wav.totalPCMFrameCount * (size_t)wav.channels;
    size_t dataBytes = totalSamples * sizeof(int16_t);
    pauseBgmBuffer = (int16_t*)malloc(dataBytes);
    if (!pauseBgmBuffer) {
        drwav_uninit(&wav);
        return;
    }
    drwav_uint64 framesRead = drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, pauseBgmBuffer);
    u32 rate = wav.sampleRate;
    u16 ch   = wav.channels;
    drwav_uninit(&wav);

    if (framesRead == 0) {
        free(pauseBgmBuffer);
        pauseBgmBuffer = NULL;
        return;
    }

    size_t bufBytes = PAUSE_BUF_FRAMES * ch * sizeof(int16_t);
    for (int i = 0; i < PAUSE_NUM_BUFS; i++) {
        pauseBgmStreamBuf[i] = (int16_t*)linearAlloc(bufBytes);
        if (!pauseBgmStreamBuf[i]) {
            for (int j = 0; j < i; j++) {
                linearFree(pauseBgmStreamBuf[j]);
                pauseBgmStreamBuf[j] = NULL;
            }
            free(pauseBgmBuffer);
            pauseBgmBuffer = NULL;
            return;
        }
        memset(&pauseBgmWaveBuf[i], 0, sizeof(ndspWaveBuf));
        pauseBgmWaveBuf[i].data_vaddr = pauseBgmStreamBuf[i];
        pauseBgmWaveBuf[i].status     = NDSP_WBUF_DONE;
    }

    pauseBgmTotalFrames = (u32)framesRead;
    pauseBgmChannels    = ch;
    pauseBgmCursor      = 0;

    ndspChnReset(PAUSE_BGM_CHANNEL);
    ndspChnSetInterp(PAUSE_BGM_CHANNEL, NDSP_INTERP_LINEAR);
    ndspChnSetRate(PAUSE_BGM_CHANNEL, (float)rate);
    ndspChnSetFormat(PAUSE_BGM_CHANNEL, (ch == 2) ? NDSP_FORMAT_STEREO_PCM16 : NDSP_FORMAT_MONO_PCM16);

    float mix[12] = {0};
    ndspChnSetMix(PAUSE_BGM_CHANNEL, mix);
    ndspChnSetPaused(PAUSE_BGM_CHANNEL, true);

    hasPauseBgm = true;
    pauseBgmPlaying = false;
    pauseBgmVol = 0.0f;
    pauseBgmLastSentVol = -1.0f;
}

void update_pause_bgm(float dt) {
    if (!hasPauseBgm) return;
    bool shouldPlay = (!isAppSuspended) && (!isPlaying || isPaused);

    if (shouldPlay) {
        if (!pauseBgmPlaying) {
            pauseBgmVol = 0.0f;
            pauseBgmLastSentVol = 0.0f;
            float mix[12] = {0};
            ndspChnSetMix(PAUSE_BGM_CHANNEL, mix);
            ndspChnSetPaused(PAUSE_BGM_CHANNEL, false);
            pauseBgmPlaying = true;
        }
        float targetVol = 0.25f * ((volumeGain < 1.0f) ? volumeGain : 1.0f); //those who remembers
        float fadeRate = (targetVol > 0.001f) ? (targetVol / 30.0f) : 0.025f;
        if (pauseBgmVol < targetVol) {
            pauseBgmVol += fadeRate * dt;
            if (pauseBgmVol > targetVol) pauseBgmVol = targetVol;
        } else if (pauseBgmVol > targetVol) {
            pauseBgmVol = targetVol;
        }
        if (fabsf(pauseBgmVol - pauseBgmLastSentVol) >= 0.005f) {
            pauseBgmLastSentVol = pauseBgmVol;
            float mix[12] = {0};
            mix[0] = pauseBgmVol;
            mix[1] = pauseBgmVol;
            ndspChnSetMix(PAUSE_BGM_CHANNEL, mix);
        }
    } else {
        if (pauseBgmPlaying) {
            pauseBgmPlaying = false;
            pauseBgmVol = 0.0f;
            pauseBgmLastSentVol = -1.0f;
            float mix[12] = {0};
            ndspChnSetMix(PAUSE_BGM_CHANNEL, mix);
            ndspChnSetPaused(PAUSE_BGM_CHANNEL, true);
        }
    }
}

