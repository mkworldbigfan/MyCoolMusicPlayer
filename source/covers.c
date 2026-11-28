#include "app_state.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include "stb_image.h"

static inline uint32_t morton_interleave(uint32_t x, uint32_t y) {
    uint32_t i = 0;
    for (int bit = 0; bit < 3; bit++) {
        i |= ((x >> bit) & 1) << (bit * 2);
        i |= ((y >> bit) & 1) << (bit * 2 + 1);
    }
    return i;
}

void reupload_rgba8_to_tex(C3D_Tex* tex, const uint8_t* linear_rgba, int w, int h, int tex_w, int tex_h) {
    uint32_t* tiled = (uint32_t*)tex->data;
    if (!tiled) return;
    memset(tiled, 0, tex_w * tex_h * 4);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int tile_x = x / 8;
            int tile_y = y / 8;
            int tiles_per_row = tex_w / 8;
            int tile_index = tile_y * tiles_per_row + tile_x;
            int in_tile_offset = morton_interleave(x % 8, y % 8);
            int dest_idx = tile_index * 64 + in_tile_offset;

            int src_idx = (y * w + x) * 4;
            uint8_t r = linear_rgba[src_idx + 0];
            uint8_t g = linear_rgba[src_idx + 1];
            uint8_t b = linear_rgba[src_idx + 2];
            uint8_t a = linear_rgba[src_idx + 3];

            tiled[dest_idx] = ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (uint32_t)a;
        }
    }
    C3D_TexFlush(tex);
}

void upload_rgba8_to_tex(C3D_Tex* tex, const uint8_t* linear_rgba, int w, int h, int tex_w, int tex_h) {
    if (!C3D_TexInit(tex, tex_w, tex_h, GPU_RGBA8)) return;
    C3D_TexSetFilter(tex, GPU_LINEAR, GPU_LINEAR);
    C3D_TexSetWrap(tex, GPU_CLAMP_TO_EDGE, GPU_CLAMP_TO_EDGE);
    reupload_rgba8_to_tex(tex, linear_rgba, w, h, tex_w, tex_h);
}


static void make_slop_cover(uint8_t* dst, uint8_t accentR, uint8_t accentG, uint8_t accentB) {
    for (int y = 0; y < 128; y++) {
        for (int x = 0; x < 128; x++) {
            float dx = x - 64.0f;
            float dy = y - 64.0f;
            float dist = sqrtf(dx*dx + dy*dy);
            int idx = (y * 128 + x) * 4;

            if (dist > 60.0f) {
                dst[idx+0] = 26; dst[idx+1] = 28; dst[idx+2] = 34; dst[idx+3] = 255;
            } else if (dist > 56.0f) {
                dst[idx+0] = 70; dst[idx+1] = 75; dst[idx+2] = 85; dst[idx+3] = 255;
            } else if (dist < 14.0f) {
                dst[idx+0] = accentR; dst[idx+1] = accentG; dst[idx+2] = accentB; dst[idx+3] = 255;
            } else {
                int ring = (int)(dist / 6.0f);
                uint8_t c = (ring % 2 == 0) ? 18 : 32;
                dst[idx+0] = c; dst[idx+1] = c; dst[idx+2] = c + 4; dst[idx+3] = 255;
            }
        }
    }
}

static void resize_to_128(const uint8_t* src, int sw, int sh, uint8_t* dst) {
    for (int y = 0; y < 128; y++) {
        float v = (float)y / 128.0f;
        int sy = (int)(v * sh);
        if (sy >= sh) sy = sh - 1;
        for (int x = 0; x < 128; x++) {
            float u = (float)x / 128.0f;
            int sx = (int)(u * sw);
            if (sx >= sw) sx = sw - 1;

            int sidx = (sy * sw + sx) * 4;
            int didx = (y * 128 + x) * 4;
            dst[didx+0] = src[sidx+0];
            dst[didx+1] = src[sidx+1];
            dst[didx+2] = src[sidx+2];
            dst[didx+3] = src[sidx+3];
        }
    }
}

static uint8_t* decode_base64_shit(const char* src, size_t srcLen, size_t* outLen) { //base64 shit
    if (!src || srcLen == 0) return NULL;
    static const int8_t b64_table[256] = {
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
        52,53,54,55,56,57,58,59,60,61,-1,-1,-1, 0,-1,-1,
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
        15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
        -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
        41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
    };
    size_t allocSize = (srcLen * 3) / 4 + 4;
    uint8_t* out = (uint8_t*)malloc(allocSize);
    if (!out) return NULL;
    size_t outPos = 0;
    uint32_t val = 0;
    int valb = -8;
    for (size_t i = 0; i < srcLen; i++) {
        unsigned char c = (unsigned char)src[i];
        if (c == '=') break;
        int8_t v = b64_table[c];
        if (v < 0) continue;
        val = (val << 6) | (uint32_t)v;
        valb += 6;
        if (valb >= 0) {
            out[outPos++] = (uint8_t)((val >> valb) & 0xFF);
            valb -= 8;
        }
    }
    *outLen = outPos;
    return out;
}

static uint8_t* steal_flac_png_somehow(const char* filePath, int* outW, int* outH, int* outChannels) { //HOW I DO TO MAKE IT DONT LOOK RED?????
    FILE* f = fopen(filePath, "rb");
    if (!f) return NULL;
    setvbuf(f, NULL, _IOFBF, 64 * 1024);
    uint8_t hdr[4];
    if (fread(hdr, 1, 4, f) != 4 || memcmp(hdr, "fLaC", 4) != 0) {
        fclose(f);
        return NULL;
    }
    bool isLast = false;
    uint8_t* result = NULL;
    while (!isLast && !feof(f)) {
        uint8_t bHdr[4];
        if (fread(bHdr, 1, 4, f) != 4) break;
        isLast = (bHdr[0] & 0x80) != 0;
        uint8_t bType = bHdr[0] & 0x7F;
        uint32_t bLen = ((uint32_t)bHdr[1] << 16) | ((uint32_t)bHdr[2] << 8) | (uint32_t)bHdr[3];

        if (bType == 6 && bLen > 32 && bLen <= 10 * 1024 * 1024) { 
            uint8_t metaHdr[8];
            if (fread(metaHdr, 1, 8, f) == 8) {
                uint32_t mimeLen = ((uint32_t)metaHdr[4] << 24) | ((uint32_t)metaHdr[5] << 16) | ((uint32_t)metaHdr[6] << 8) | (uint32_t)metaHdr[7];
                fseek(f, mimeLen, SEEK_CUR);

                uint8_t descHdr[4];
                if (fread(descHdr, 1, 4, f) == 4) {
                    uint32_t descLen = ((uint32_t)descHdr[0] << 24) | ((uint32_t)descHdr[1] << 16) | ((uint32_t)descHdr[2] << 8) | (uint32_t)descHdr[3];
                    fseek(f, descLen + 16, SEEK_CUR); 

                    uint8_t picLenHdr[4];
                    if (fread(picLenHdr, 1, 4, f) == 4) {
                        uint32_t picDataLen = ((uint32_t)picLenHdr[0] << 24) | ((uint32_t)picLenHdr[1] << 16) | ((uint32_t)picLenHdr[2] << 8) | (uint32_t)picLenHdr[3];
                        if (picDataLen > 16 && picDataLen <= 10 * 1024 * 1024) {
                            uint8_t* pPic = (uint8_t*)linearAlloc(picDataLen);
                            bool isLinear = true;
                            if (!pPic) {
                                pPic = (uint8_t*)malloc(picDataLen);
                                isLinear = false;
                            }
                            if (pPic) {
                                if (fread(pPic, 1, picDataLen, f) == picDataLen) {
                                    result = stbi_load_from_memory(pPic, (int)picDataLen, outW, outH, outChannels, 4);
                                }
                                if (isLinear) linearFree(pPic);
                                else free(pPic);
                            }
                        }
                    }
                }
            }
            break;
        } else {
            fseek(f, bLen, SEEK_CUR);
        }
    }
    fclose(f);
    return result;
}

static uint8_t* extract_mp3_cover(const char* filePath, int* outW, int* outH, int* outChannels) {
    FILE* f = fopen(filePath, "rb");
    if (!f) return NULL;
    uint8_t id3Hdr[10];
    if (fread(id3Hdr, 1, 10, f) != 10 || memcmp(id3Hdr, "ID3", 3) != 0) {
        fclose(f);
        return NULL;
    }
    uint8_t id3Ver = id3Hdr[3];
    uint32_t tagSize = ((uint32_t)(id3Hdr[6] & 0x7F) << 21) |
                       ((uint32_t)(id3Hdr[7] & 0x7F) << 14) |
                       ((uint32_t)(id3Hdr[8] & 0x7F) << 7)  |
                       (uint32_t)(id3Hdr[9] & 0x7F);
    if (tagSize < 32 || tagSize > 12 * 1024 * 1024) {
        fclose(f);
        return NULL;
    }

    uint8_t* tagBuf = (uint8_t*)linearAlloc(tagSize);
    bool isLinear = true;
    if (!tagBuf) {
        tagBuf = (uint8_t*)malloc(tagSize);
        isLinear = false;
    }
    if (!tagBuf) {
        fclose(f);
        return NULL;
    }
    if (fread(tagBuf, 1, tagSize, f) != tagSize) {
        if (isLinear) linearFree(tagBuf); else free(tagBuf);
        fclose(f);
        return NULL;
    }
    fclose(f);

    uint8_t* result = NULL;
    size_t pos = 0;
    while (pos + 10 < tagSize) {
        if (tagBuf[pos] == 0) break;

        uint32_t fSize = 0;
        if (id3Ver == 4) {
            fSize = ((uint32_t)(tagBuf[pos+4] & 0x7F) << 21) |
                    ((uint32_t)(tagBuf[pos+5] & 0x7F) << 14) |
                    ((uint32_t)(tagBuf[pos+6] & 0x7F) << 7)  |
                    (uint32_t)(tagBuf[pos+7] & 0x7F);
        } else {
            fSize = ((uint32_t)tagBuf[pos+4] << 24) |
                    ((uint32_t)tagBuf[pos+5] << 16) |
                    ((uint32_t)tagBuf[pos+6] << 8)  |
                    (uint32_t)tagBuf[pos+7];
        }

        pos += 10;
        if (pos + fSize > tagSize) fSize = tagSize - pos;

        if (memcmp(tagBuf + pos - 10, "APIC", 4) == 0 && fSize > 16) {
            uint8_t* frameData = tagBuf + pos;
            for (size_t k = 0; k + 4 < fSize; k++) {
                if ((frameData[k] == 0xFF && frameData[k+1] == 0xD8 && frameData[k+2] == 0xFF) ||
                    (frameData[k] == 0x89 && frameData[k+1] == 0x50 && frameData[k+2] == 0x4E && frameData[k+3] == 0x47)) {
                    result = stbi_load_from_memory(frameData + k, (int)(fSize - k), outW, outH, outChannels, 4);
                    break;
                }
            }
            if (result) break;
        }
        pos += fSize;
    }

    if (!result) {
        for (size_t k = 0; k + 1024 < tagSize; k++) {
            if ((tagBuf[k] == 0xFF && tagBuf[k+1] == 0xD8 && tagBuf[k+2] == 0xFF) ||
                (tagBuf[k] == 0x89 && tagBuf[k+1] == 0x50 && tagBuf[k+2] == 0x4E && tagBuf[k+3] == 0x47)) {
                result = stbi_load_from_memory(tagBuf + k, (int)(tagSize - k), outW, outH, outChannels, 4);
                if (result) break;
            }
        }
    }

    if (isLinear) linearFree(tagBuf); else free(tagBuf);
    return result;
}

static uint8_t* decode_single_vorbis_picture_comment(const char* c, size_t clen, int* outW, int* outH, int* outChannels) {
    if (!c || clen < 10) return NULL;
    uint8_t* result = NULL;
    if (clen > 23 && strncasecmp(c, "METADATA_BLOCK_PICTURE=", 23) == 0) {
        size_t decLen = 0;
        uint8_t* dec = decode_base64_shit(c + 23, clen - 23, &decLen);
        if (dec) {
            if (decLen > 32) {
                uint32_t mimeLen = ((uint32_t)dec[4] << 24) | ((uint32_t)dec[5] << 16) | ((uint32_t)dec[6] << 8) | (uint32_t)dec[7];
                uint32_t descPos = 8 + mimeLen;
                if (mimeLen < decLen && descPos + 4 <= decLen) {
                    uint32_t descLen = ((uint32_t)dec[descPos] << 24) | ((uint32_t)dec[descPos+1] << 16) | ((uint32_t)dec[descPos+2] << 8) | (uint32_t)dec[descPos+3];
                    uint32_t picDataPos = descPos + 4 + descLen + 16;
                    if (descLen < decLen && picDataPos + 4 <= decLen) {
                        uint32_t picDataLen = ((uint32_t)dec[picDataPos] << 24) | ((uint32_t)dec[picDataPos+1] << 16) | ((uint32_t)dec[picDataPos+2] << 8) | (uint32_t)dec[picDataPos+3];
                        if (picDataLen > 16 && picDataPos + 4 + picDataLen <= decLen) {
                            result = stbi_load_from_memory(dec + picDataPos + 4, (int)picDataLen, outW, outH, outChannels, 4);
                        }
                    }
                }
            }
            free(dec);
        }
    } else if (clen > 9 && strncasecmp(c, "COVERART=", 9) == 0) {
        size_t decLen = 0;
        uint8_t* dec = decode_base64_shit(c + 9, clen - 9, &decLen);
        if (dec) {
            if (decLen > 16) {
                result = stbi_load_from_memory(dec, (int)decLen, outW, outH, outChannels, 4);
            }
            free(dec);
        }
    }
    return result;
}

typedef void (*OggCommentVisitor)(const char* comment, int len, void* userData);

bool ogg_scan(const char* filePath, size_t maxPacketBytes, OggCommentVisitor visitor, void* userData) {
    if (!filePath || !visitor) return false;
    FILE* f = fopen(filePath, "rb");
    if (!f) return false;
    setvbuf(f, NULL, _IOFBF, 32 * 1024);

    size_t pktCap = 8192;
    size_t pktLen = 0;
    uint8_t* pkt = (uint8_t*)malloc(pktCap);
    if (!pkt) { fclose(f); return false; }

    int packetIndex = 0;
    bool packetDone = false;
    bool stopScan = false;
    uint8_t skipBuf[255];

    for (int page = 0; page < 256 && !packetDone && !stopScan; page++) {
        uint8_t hdr[27];
        if (fread(hdr, 1, 27, f) != 27) break;
        if (memcmp(hdr, "OggS", 4) != 0) break;

        uint8_t nSegs = hdr[26];
        uint8_t segTable[255];
        if (nSegs > 0 && fread(segTable, 1, nSegs, f) != nSegs) break;

        for (int s = 0; s < (int)nSegs; s++) {
            uint8_t segLen = segTable[s];
            if (packetIndex == 0) {
                if (segLen > 0 && fread(skipBuf, 1, segLen, f) != segLen) { stopScan = true; break; }
                if (segLen < 255) packetIndex = 1;
            } else if (packetIndex == 1) {
                if (segLen > 0) {
                    if (pktLen + segLen > maxPacketBytes) { stopScan = true; break; }
                    if (pktLen + segLen > pktCap) {
                        size_t newCap = pktCap * 2;
                        while (newCap < pktLen + segLen) newCap *= 2;
                        if (newCap > maxPacketBytes) newCap = maxPacketBytes;
                        uint8_t* grown = (uint8_t*)realloc(pkt, newCap);
                        if (!grown) { stopScan = true; break; }
                        pkt = grown;
                        pktCap = newCap;
                    }
                    if (fread(pkt + pktLen, 1, segLen, f) != segLen) { stopScan = true; break; }
                    pktLen += segLen;
                }
                if (segLen < 255) {
                    packetDone = true;
                    break;
                }
            }
        }
    }
    fclose(f);

    bool parsed = false;
    size_t pos = 0;
    if (pktLen >= 7 && memcmp(pkt, "\x03vorbis", 7) == 0) pos = 7;
    else if (pktLen >= 8 && memcmp(pkt, "OpusTags", 8) == 0) pos = 8;

    if (pos > 0 && pos + 4 <= pktLen) {
        uint32_t vendorLen = (uint32_t)pkt[pos] | ((uint32_t)pkt[pos+1] << 8) | ((uint32_t)pkt[pos+2] << 16) | ((uint32_t)pkt[pos+3] << 24);
        pos += 4;
        if (vendorLen <= pktLen - pos) {
            pos += vendorLen;
            if (pos + 4 <= pktLen) {
                uint32_t cnt = (uint32_t)pkt[pos] | ((uint32_t)pkt[pos+1] << 8) | ((uint32_t)pkt[pos+2] << 16) | ((uint32_t)pkt[pos+3] << 24);
                pos += 4;
                parsed = true;
                for (uint32_t i = 0; i < cnt && pos + 4 <= pktLen; i++) {
                    uint32_t clen = (uint32_t)pkt[pos] | ((uint32_t)pkt[pos+1] << 8) | ((uint32_t)pkt[pos+2] << 16) | ((uint32_t)pkt[pos+3] << 24);
                    pos += 4;
                    if (clen > pktLen - pos) break;
                    visitor((const char*)(pkt + pos), (int)clen, userData);
                    pos += clen;
                }
            }
        }
    }

    free(pkt);
    return parsed;
}

typedef struct {
    int* outW;
    int* outH;
    int* outChannels;
    uint8_t* result;
} OggCoverVisitorCtx;

static void on_ogg_cover_comment(const char* comment, int len, void* userData) {
    OggCoverVisitorCtx* ctx = (OggCoverVisitorCtx*)userData;
    if (!ctx || ctx->result) return;
    ctx->result = decode_single_vorbis_picture_comment(comment, (size_t)len, ctx->outW, ctx->outH, ctx->outChannels);
}

static uint8_t* extract_ogg_opus_cover(const char* filePath, int* outW, int* outH, int* outChannels) {
    if (!filePath) return NULL;
    OggCoverVisitorCtx ctx = { outW, outH, outChannels, NULL };
    ogg_scan(filePath, 2 * 1024 * 1024, on_ogg_cover_comment, &ctx);
    return ctx.result;
}

static uint8_t* extract_m4a_cover(const char* filePath, int* outW, int* outH, int* outChannels) {
    if (!filePath) return NULL;
    if (has_extension(filePath, ".aac")) {
        return extract_mp3_cover(filePath, outW, outH, outChannels);
    }
    uint8_t* coverData = NULL;
    size_t coverSize = 0;
    if (!m4a_read_metadata(filePath, NULL, 0, NULL, 0, NULL, 0, NULL, NULL, &coverData, &coverSize)) {
        return NULL;
    }
    if (!coverData || coverSize == 0) {
        free(coverData);
        return NULL;
    }
    uint8_t* result = stbi_load_from_memory(coverData, (int)coverSize, outW, outH, outChannels, 4);
    free(coverData);
    return result;
}

static void build_cover_cache_path(const char* filePath, char* outPath, size_t outSz) {
    uint64_t h = 14695981039346656037ULL;
    for (const unsigned char* p = (const unsigned char*)filePath; *p; p++) {
        h ^= *p;
        h *= 1099511628211ULL;
    }
    snprintf(outPath, outSz, "sdmc:/3ds/MCMP/cache/%016llx.cov", (unsigned long long)h);
}

static void update_slot_avg_color(C3D_Tex* tex, const uint8_t* rgba128) {
    int slot = -1;
    for (int s = 0; s < 4; s++) {
        if (&coverTextures[s] == tex) { slot = s; break; }
    }
    if (slot < 0) return;

    uint64_t sr = 0, sg = 0, sb = 0, cnt = 0;
    for (int i = 0; i < 128 * 128; i += 4) {
        uint8_t r = rgba128[i * 4];
        uint8_t g = rgba128[i * 4 + 1];
        uint8_t b = rgba128[i * 4 + 2];
        if ((int)r + (int)g + (int)b > 70) {
            int maxc = r > g ? (r > b ? r : b) : (g > b ? g : b);
            int minc = r < g ? (r < b ? r : b) : (g < b ? g : b);
            int w = 1 + (maxc - minc) / 32;
            sr += (uint64_t)r * w;
            sg += (uint64_t)g * w;
            sb += (uint64_t)b * w;
            cnt += w;
        }
    }
    if (cnt > 0) {
        coverAvgR[slot] = (uint8_t)(sr / cnt);
        coverAvgG[slot] = (uint8_t)(sg / cnt);
        coverAvgB[slot] = (uint8_t)(sb / cnt);
    } else {
        coverAvgR[slot] = 255;
        coverAvgG[slot] = 255;
        coverAvgB[slot] = 255;
    }

    static uint8_t blurA[64 * 64 * 4];
    static uint8_t blurB[64 * 64 * 4];
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            int ar = 0, ag = 0, ab = 0;
            for (int dy = 0; dy < 2; dy++) {
                for (int dx = 0; dx < 2; dx++) {
                    int si = ((y * 2 + dy) * 128 + (x * 2 + dx)) * 4;
                    ar += rgba128[si];
                    ag += rgba128[si + 1];
                    ab += rgba128[si + 2];
                }
            }
            int di = (y * 64 + x) * 4;
            blurA[di]     = (uint8_t)(ar >> 2);
            blurA[di + 1] = (uint8_t)(ag >> 2);
            blurA[di + 2] = (uint8_t)(ab >> 2);
            blurA[di + 3] = 255;
        }
    }
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            int ar = 0, ag = 0, ab = 0;
            for (int dy = -1; dy <= 1; dy++) {
                int ny = y + dy;
                if (ny < 0) ny = 0; else if (ny > 63) ny = 63;
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = x + dx;
                    if (nx < 0) nx = 0; else if (nx > 63) nx = 63;
                    int si = (ny * 64 + nx) * 4;
                    ar += blurA[si];
                    ag += blurA[si + 1];
                    ab += blurA[si + 2];
                }
            }
            int di = (y * 64 + x) * 4;
            blurB[di]     = (uint8_t)(ar / 9);
            blurB[di + 1] = (uint8_t)(ag / 9);
            blurB[di + 2] = (uint8_t)(ab / 9);
            blurB[di + 3] = 255;
        }
    }
    if (coverBlurTextures[slot].data) {
        reupload_rgba8_to_tex(&coverBlurTextures[slot], blurB, 64, 64, 64, 64);
    } else {
        upload_rgba8_to_tex(&coverBlurTextures[slot], blurB, 64, 64, 64, 64);
    }
}

void load_song_cover_tex(const char* filePath, C3D_Tex* tex, bool* hasTex, uint8_t accentR, uint8_t accentG, uint8_t accentB) {
    if (*hasTex) {
        C3D_TexDelete(tex);
        *hasTex = false;
    }

    char cachePath[128] = "";
    if (filePath && filePath[0] != '\0') {
        build_cover_cache_path(filePath, cachePath, sizeof(cachePath));
        FILE* cf = fopen(cachePath, "rb");
        if (cf) {
            size_t need = 128 * 128 * 4;
            if (fread(coverResizeBuf, 1, need, cf) == need) {
                fclose(cf);
                update_slot_avg_color(tex, coverResizeBuf);
                upload_rgba8_to_tex(tex, coverResizeBuf, 128, 128, 128, 128);
                *hasTex = true;
                return;
            }
            fclose(cf);
        }
    }

    uint8_t* decodedRgba = NULL;
    int imgW = 0, imgH = 0, channels = 0;

    if (filePath) {
        if (has_extension(filePath, ".flac")) {
            decodedRgba = steal_flac_png_somehow(filePath, &imgW, &imgH, &channels);
        } else if (has_extension(filePath, ".mp3")) {
            decodedRgba = extract_mp3_cover(filePath, &imgW, &imgH, &channels);
        } else if (has_extension(filePath, ".ogg") || has_extension(filePath, ".opus")) {
            decodedRgba = extract_ogg_opus_cover(filePath, &imgW, &imgH, &channels);
        } else if (has_extension(filePath, ".m4a") || has_extension(filePath, ".aac") || has_extension(filePath, ".mp4")) {
            decodedRgba = extract_m4a_cover(filePath, &imgW, &imgH, &channels);
        }
    }

    if (!decodedRgba && filePath) {
        static char songImgPath[1024];
        strncpy(songImgPath, filePath, sizeof(songImgPath) - 8);
        char* extDot = strrchr(songImgPath, '.');
        if (extDot) *extDot = '\0';
        const char* songExts[] = { ".jpg", ".png", ".jpeg", ".JPG", ".PNG" };
        for (size_t e = 0; e < sizeof(songExts)/sizeof(songExts[0]); e++) {
            static char testSongPath[1024];
            snprintf(testSongPath, sizeof(testSongPath), "%s%s", songImgPath, songExts[e]);
            FILE* f = fopen(testSongPath, "rb");
            if (f) {
                fclose(f);
                decodedRgba = stbi_load(testSongPath, &imgW, &imgH, &channels, 4);
                if (decodedRgba) break;
            }
        }
    }

    if (decodedRgba && imgW > 0 && imgH > 0) {
        resize_to_128(decodedRgba, imgW, imgH, coverResizeBuf);
        update_slot_avg_color(tex, coverResizeBuf);
        upload_rgba8_to_tex(tex, coverResizeBuf, 128, 128, 128, 128);
        *hasTex = true;
        stbi_image_free(decodedRgba);

        if (cachePath[0] != '\0') {
            if (!coverCacheDirReady) {
                mkdir("sdmc:/3ds", 0777);
                mkdir("sdmc:/3ds/MCMP", 0777);
                mkdir("sdmc:/3ds/MCMP/cache", 0777);
                coverCacheDirReady = true;
            }
            FILE* cf = fopen(cachePath, "wb");
            if (cf) {
                fwrite(coverResizeBuf, 1, 128 * 128 * 4, cf);
                fclose(cf);
            }
        }
    } else {
        make_slop_cover(coverResizeBuf, accentR, accentG, accentB);
        update_slot_avg_color(tex, coverResizeBuf);
        upload_rgba8_to_tex(tex, coverResizeBuf, 128, 128, 128, 128);
        *hasTex = true;
    }
}

void refresh_the_3_covers(void) {
    if (fileCount == 0) return;

    coverSubtex.width = 128;
    coverSubtex.height = 128;
    coverSubtex.left = 0.0f;
    coverSubtex.top = 1.0f;
    coverSubtex.right = 1.0f;
    coverSubtex.bottom = 0.0f;

    coverBlurSubtex.width = 64;
    coverBlurSubtex.height = 64;
    coverBlurSubtex.left = 0.0f;
    coverBlurSubtex.top = 1.0f;
    coverBlurSubtex.right = 1.0f;
    coverBlurSubtex.bottom = 0.0f;

    cover64Subtex = coverBlurSubtex;

    int prevIdx = find_next_song_index(selectedFile, -1);
    int nextIdx = find_next_song_index(selectedFile, 1);
    if (prevIdx < 0) prevIdx = selectedFile;
    if (nextIdx < 0) nextIdx = selectedFile;

    if (slotSongIdx[slotCurrent] != selectedFile || !hasCoverCurrent) {
        static char fpCurrent[1024];
        snprintf(fpCurrent, sizeof(fpCurrent), "%s/%s", currentDir, fileList[selectedFile].name);
        load_song_cover_tex(fpCurrent, &coverTexCurrent, &hasCoverCurrent, 0, 220, 255);
        slotSongIdx[slotCurrent] = selectedFile;
    }

    if (slotSongIdx[slotPrev] != prevIdx || !hasCoverPrev) {
        deferredPrevCoverIdx = prevIdx;
    } else {
        deferredPrevCoverIdx = -1;
    }

    if (slotSongIdx[slotNext] != nextIdx || !hasCoverNext) {
        deferredNextCoverIdx = nextIdx;
    } else {
        deferredNextCoverIdx = -1;
    }

    int prev2Idx = find_next_song_index(prevIdx, -1);
    int next2Idx = find_next_song_index(nextIdx, 1);
    deferredPrev2CoverIdx = (prev2Idx >= 0 && (peekPrev2SongIdx != prev2Idx || !hasPeekPrev2Tex)) ? prev2Idx : -1;
    deferredNext2CoverIdx = (next2Idx >= 0 && (peekNext2SongIdx != next2Idx || !hasPeekNext2Tex)) ? next2Idx : -1;
}

void load_song_cover_lowres(const char* filePath, C3D_Tex* tex, bool* hasTex, uint8_t accentR, uint8_t accentG, uint8_t accentB) {
    static uint8_t buf64[64 * 64 * 4];
    bool loaded128 = false;

    if (filePath && filePath[0] != '\0') {
        char cachePath[128] = "";
        build_cover_cache_path(filePath, cachePath, sizeof(cachePath));
        FILE* cf = fopen(cachePath, "rb");
        if (cf) {
            size_t need = 128 * 128 * 4;
            if (fread(coverResizeBuf, 1, need, cf) == need) {
                loaded128 = true;
            }
            fclose(cf);
        }
    }
    if (!loaded128) {
        make_slop_cover(coverResizeBuf, accentR, accentG, accentB);
    }
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            int si = ((y * 2) * 128 + (x * 2)) * 4;
            int di = (y * 64 + x) * 4;
            buf64[di]     = coverResizeBuf[si];
            buf64[di + 1] = coverResizeBuf[si + 1];
            buf64[di + 2] = coverResizeBuf[si + 2];
            buf64[di + 3] = 255;
        }
    }
    if (*hasTex && tex->data) {
        reupload_rgba8_to_tex(tex, buf64, 64, 64, 64, 64);
    } else {
        upload_rgba8_to_tex(tex, buf64, 64, 64, 64, 64);
        *hasTex = true;
    }
}

static void load_song_cover_fast(const char* filePath, C3D_Tex* tex, bool* hasTex) {
    char cachePath[128] = "";
    if (filePath && filePath[0] != '\0') {
        build_cover_cache_path(filePath, cachePath, sizeof(cachePath));
        FILE* cf = fopen(cachePath, "rb");
        if (cf) {
            size_t need = 128 * 128 * 4;
            if (fread(coverResizeBuf, 1, need, cf) == need) {
                fclose(cf);
                if (*hasTex && tex->data) {
                    reupload_rgba8_to_tex(tex, coverResizeBuf, 128, 128, 128, 128);
                } else {
                    upload_rgba8_to_tex(tex, coverResizeBuf, 128, 128, 128, 128);
                    *hasTex = true;
                }
                return;
            }
            fclose(cf);
        }
    }
    make_slop_cover(coverResizeBuf, 0, 220, 255);
    if (*hasTex && tex->data) {
        reupload_rgba8_to_tex(tex, coverResizeBuf, 128, 128, 128, 128);
    } else {
        upload_rgba8_to_tex(tex, coverResizeBuf, 128, 128, 128, 128);
        *hasTex = true;
    }
}

void advance_cover_slot_step(int targetSongIdx, TransitionDir dir, bool fastFlyBy) {
    if (targetSongIdx < 0 || targetSongIdx >= fileCount) return;

    if (dir == TRANS_NEXT) {
        int recycledSlot = slotPrev;
        slotOld = slotCurrent;
        slotPrev = slotCurrent;
        slotCurrent = slotNext;
        slotNext = recycledSlot;
    } else if (dir == TRANS_PREV) {
        int recycledSlot = slotNext;
        slotOld = slotCurrent;
        slotNext = slotCurrent;
        slotCurrent = slotPrev;
        slotPrev = recycledSlot;
    }

    if (fastFlyBy) {
        deferredNextCoverIdx = -1;
        deferredPrevCoverIdx = -1;
        deferredNext2CoverIdx = -1;
        deferredPrev2CoverIdx = -1;
        if (slotSongIdx[slotCurrent] != targetSongIdx || !hasCoverCurrent) {
            static char fpCurr[1024];
            snprintf(fpCurr, sizeof(fpCurr), "%s/%s", currentDir, fileList[targetSongIdx].name);
            load_song_cover_fast(fpCurr, &coverTexCurrent, &hasCoverCurrent);
            slotSongIdx[slotCurrent] = targetSongIdx;
        }
    } else {
        int prevIdx = find_next_song_index(targetSongIdx, -1);
        int nextIdx = find_next_song_index(targetSongIdx, 1);
        if (prevIdx < 0) prevIdx = targetSongIdx;
        if (nextIdx < 0) nextIdx = targetSongIdx;
        deferredNextCoverIdx = nextIdx;
        deferredPrevCoverIdx = prevIdx;

        int prev2Idx = find_next_song_index(prevIdx, -1);
        int next2Idx = find_next_song_index(nextIdx, 1);
        deferredPrev2CoverIdx = (prev2Idx >= 0) ? prev2Idx : -1;
        deferredNext2CoverIdx = (next2Idx >= 0) ? next2Idx : -1;

        if (slotSongIdx[slotCurrent] != targetSongIdx || !hasCoverCurrent) {
            static char fpCurr[1024];
            snprintf(fpCurr, sizeof(fpCurr), "%s/%s", currentDir, fileList[targetSongIdx].name);
            load_song_cover_tex(fpCurr, &coverTexCurrent, &hasCoverCurrent, 0, 220, 255);
            slotSongIdx[slotCurrent] = targetSongIdx;
        } else if (coverBlurTextures[slotCurrent].data == NULL) {
            static char fpCurr[1024];
            snprintf(fpCurr, sizeof(fpCurr), "%s/%s", currentDir, fileList[targetSongIdx].name);
            load_song_cover_tex(fpCurr, &coverTexCurrent, &hasCoverCurrent, 0, 220, 255);
        }
    }
    coverTransDir = dir;
    coverTransProgress = (dir == TRANS_NONE) ? 1.0f : 0.0f;
}

