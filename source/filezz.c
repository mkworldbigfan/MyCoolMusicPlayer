#include "app_state.h"
#include "dr_flac.h"

static bool str_contains_nocase(const char* haystack, const char* needle) {
    if (!needle || needle[0] == '\0') return true;
    if (!haystack) return false;
    size_t nlen = strlen(needle);
    for (size_t i = 0; haystack[i] != '\0'; i++) {
        if (strncasecmp(haystack + i, needle, nlen) == 0) return true;
    }
    return false;
}

void apply_file_filter(void) {
    char prevName[256] = "";
    if (selectedFile >= 0 && selectedFile < fileCount) {
        snprintf(prevName, sizeof(prevName), "%s", fileList[selectedFile].name);
    }

    fileCount = 0;
    for (int i = 0; i < allFileCount && fileCount < MAX_FILES; i++) {
        if (strcmp(allFileList[i].name, "..") == 0 ||
            searchFilter[0] == '\0' ||
            str_contains_nocase(allFileList[i].name, searchFilter)) {
            fileList[fileCount++] = allFileList[i];
        }
    }

    selectedFile = 0;
    scrollOffset = 0;
    if (prevName[0] != '\0') {
        for (int i = 0; i < fileCount; i++) {
            if (strcmp(fileList[i].name, prevName) == 0) {
                selectedFile = i;
                break;
            }
        }
    }
    if (selectedFile >= CFG_LIST_VISIBLE_ROWS) {
        scrollOffset = selectedFile - CFG_LIST_VISIBLE_ROWS + 1;
    }
    for (int s = 0; s < 4; s++) slotSongIdx[s] = -1;
    upcomingShuffleIdx = -1;
    cachedNextIdx = -2;
}

void load_directory(const char* path) {
    allFileCount = 0;
    fileCount = 0;
    selectedFile = 0;
    scrollOffset = 0;
    searchFilter[0] = '\0';
    for (int s = 0; s < 4; s++) slotSongIdx[s] = -1;

    DIR* dir = opendir(path);
    if (!dir) {
        strcpy(currentDir, "sdmc:/");
        dir = opendir("sdmc:/");
        if (!dir) return;
    }

    if (strcmp(currentDir, "sdmc:/") != 0 && strcmp(currentDir, "sdmc:") != 0) {
        strcpy(allFileList[allFileCount].name, "..");
        allFileList[allFileCount].is_dir = true;
        allFileCount++;
    }

    struct dirent* ent;
    while ((ent = readdir(dir)) != NULL && allFileCount < MAX_FILES) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;

        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", currentDir, ent->d_name);
        struct stat st;
        bool isDir = (stat(fullPath, &st) == 0 && S_ISDIR(st.st_mode));

        if (isDir || has_extension(ent->d_name, ".mp3") || has_extension(ent->d_name, ".wav") ||
            has_extension(ent->d_name, ".flac") || has_extension(ent->d_name, ".ogg") ||
            has_extension(ent->d_name, ".opus") || has_extension(ent->d_name, ".m4a") ||
            has_extension(ent->d_name, ".aac")) {
            snprintf(allFileList[allFileCount].name, sizeof(allFileList[allFileCount].name), "%s", ent->d_name);
            allFileList[allFileCount].is_dir = isDir;
            allFileCount++;
        }
    }
    closedir(dir);
    apply_file_filter();
}


void apply_oggshit_kv(const char* comment, int len,
                                    char* outTitle, size_t titleSz,
                                    char* outArtist, size_t artistSz,
                                    char* outAlbum, size_t albumSz,
                                    float* outBpm, bool* outHasBpm) {
    if (!comment || len <= 0) return;
    char rawVal[256];
    char cleanVal[256];
    if (len > 6 && strncasecmp(comment, "TITLE=", 6) == 0 && outTitle && titleSz > 0) {
        int vlen = (len - 6 < (int)sizeof(rawVal) - 1) ? (len - 6) : ((int)sizeof(rawVal) - 1);
        memcpy(rawVal, comment + 6, vlen); rawVal[vlen] = '\0';
        nuke_weird_utf8_chars(rawVal, cleanVal, sizeof(cleanVal));
        if (cleanVal[0] != '\0') snprintf(outTitle, titleSz, "%s", cleanVal);
    } else if (len > 7 && strncasecmp(comment, "ARTIST=", 7) == 0 && outArtist && artistSz > 0) {
        int vlen = (len - 7 < (int)sizeof(rawVal) - 1) ? (len - 7) : ((int)sizeof(rawVal) - 1);
        memcpy(rawVal, comment + 7, vlen); rawVal[vlen] = '\0';
        nuke_weird_utf8_chars(rawVal, cleanVal, sizeof(cleanVal));
        if (cleanVal[0] != '\0') snprintf(outArtist, artistSz, "%s", cleanVal);
    } else if (len > 6 && strncasecmp(comment, "ALBUM=", 6) == 0 && outAlbum && albumSz > 0) {
        int vlen = (len - 6 < (int)sizeof(rawVal) - 1) ? (len - 6) : ((int)sizeof(rawVal) - 1);
        memcpy(rawVal, comment + 6, vlen); rawVal[vlen] = '\0';
        nuke_weird_utf8_chars(rawVal, cleanVal, sizeof(cleanVal));
        if (cleanVal[0] != '\0') snprintf(outAlbum, albumSz, "%s", cleanVal);
    } else if (len > 4 && strncasecmp(comment, "BPM=", 4) == 0 && outBpm) {
        int vlen = (len - 4 < (int)sizeof(rawVal) - 1) ? (len - 4) : ((int)sizeof(rawVal) - 1);
        memcpy(rawVal, comment + 4, vlen); rawVal[vlen] = '\0';
        float b = (float)atof(rawVal);
        if (b >= 40.0f && b <= 240.0f) {
            *outBpm = b;
            if (outHasBpm) *outHasBpm = true;
        }
    }
}

void parse_oggshits_array(char** comments, const int* lengths, int count,
                                        char* outTitle, size_t titleSz,
                                        char* outArtist, size_t artistSz,
                                        char* outAlbum, size_t albumSz,
                                        float* outBpm, bool* outHasBpm) {
    if (!comments || count <= 0) return;
    for (int i = 0; i < count; i++) {
        if (!comments[i]) continue;
        int clen = (lengths && lengths[i] >= 0) ? lengths[i] : (int)strlen(comments[i]);
        apply_oggshit_kv(comments[i], clen, outTitle, titleSz, outArtist, artistSz, outAlbum, albumSz, outBpm, outHasBpm);
    }
}

void ihateflac_meta_shit(void* pUserData, drflac_metadata* pMetadata) {
    if (pMetadata->type == DRFLAC_METADATA_BLOCK_TYPE_VORBIS_COMMENT) {
        drflac_vorbis_comment_iterator iter;
        drflac_init_vorbis_comment_iterator(&iter, pMetadata->data.vorbis_comment.commentCount, pMetadata->data.vorbis_comment.pComments);
        const char* comment;
        drflac_uint32 len;
        while ((comment = drflac_next_vorbis_comment(&iter, &len)) != NULL) {
            apply_oggshit_kv(comment, (int)len,
                                    currentTrackTitle, sizeof(currentTrackTitle),
                                    currentTrackArtist, sizeof(currentTrackArtist),
                                    currentTrackAlbum, sizeof(currentTrackAlbum),
                                    &detectedBPM, &hasTrackBpmTag);
        }
    }
}

static void decode_id3_text(const uint8_t* data, uint32_t len, char* dst, size_t dstSize) {
    if (!data || len == 0 || !dst || dstSize == 0) {
        if (dst && dstSize > 0) dst[0] = '\0';
        return;
    }
    uint8_t enc = data[0];
    const uint8_t* p = data + 1;
    uint32_t rem = len - 1;
    char utf8Buf[512];
    size_t u = 0;

    if (enc == 0) { 
        for (uint32_t i = 0; i < rem && u + 3 < sizeof(utf8Buf); i++) {
            uint8_t c = p[i];
            if (c == 0) break;
            if (c < 0x80) {
                utf8Buf[u++] = (char)c;
            } else {
                utf8Buf[u++] = (char)(0xC0 | (c >> 6));
                utf8Buf[u++] = (char)(0x80 | (c & 0x3F));
            }
        }
    } else if (enc == 1 || enc == 2) { 
        bool isLE = false;
        uint32_t i = 0;
        if (rem >= 2) {
            if (p[0] == 0xFF && p[1] == 0xFE) { isLE = true; i = 2; }
            else if (p[0] == 0xFE && p[1] == 0xFF) { isLE = false; i = 2; }
        }
        while (i + 1 < rem && u + 4 < sizeof(utf8Buf)) {
            uint16_t cp = isLE ? ((uint16_t)p[i] | ((uint16_t)p[i+1] << 8))
                               : (((uint16_t)p[i] << 8) | (uint16_t)p[i+1]);
            i += 2;
            if (cp == 0) break;
            if (cp >= 0xD800 && cp <= 0xDFFF) continue;
            if (cp < 0x80) {
                utf8Buf[u++] = (char)cp;
            } else if (cp < 0x800) {
                utf8Buf[u++] = (char)(0xC0 | (cp >> 6));
                utf8Buf[u++] = (char)(0x80 | (cp & 0x3F));
            } else {
                utf8Buf[u++] = (char)(0xE0 | (cp >> 12));
                utf8Buf[u++] = (char)(0x80 | ((cp >> 6) & 0x3F));
                utf8Buf[u++] = (char)(0x80 | (cp & 0x3F));
            }
        }
    } else { 
        for (uint32_t i = 0; i < rem && u + 1 < sizeof(utf8Buf); i++) {
            if (p[i] == 0) break;
            utf8Buf[u++] = (char)p[i];
        }
    }
    utf8Buf[u] = '\0';
    nuke_weird_utf8_chars(utf8Buf, dst, dstSize);
}

void parse_mp3_id3v2_meta_into(FILE* f,
                                      char* outTitle, size_t titleSz,
                                      char* outArtist, size_t artistSz,
                                      char* outAlbum, size_t albumSz,
                                      float* outBpm, bool* outHasBpm,
                                      long* outAudioOffset, float* outTlenSec) {
    long offset = 0;
    float tlenSec = 0.0f;
    fseek(f, 0, SEEK_SET);

    for (int tagPass = 0; tagPass < 4; tagPass++) {
        uint8_t id3Hdr[10];
        if (fread(id3Hdr, 1, 10, f) != 10 || memcmp(id3Hdr, "ID3", 3) != 0) {
            break;
        }
        uint8_t ver = id3Hdr[3];
        uint8_t flags = id3Hdr[5];
        uint32_t tagBodySize = ((uint32_t)(id3Hdr[6] & 0x7F) << 21) |
                               ((uint32_t)(id3Hdr[7] & 0x7F) << 14) |
                               ((uint32_t)(id3Hdr[8] & 0x7F) << 7)  |
                               (uint32_t)(id3Hdr[9] & 0x7F);
        long tagEnd = offset + 10 + (long)tagBodySize;

        if (flags & 0x40) { 
            uint8_t extHdr[4];
            if (fread(extHdr, 1, 4, f) == 4) {
                uint32_t extSz = (ver == 4)
                    ? (((uint32_t)(extHdr[0]&0x7F)<<21)|((uint32_t)(extHdr[1]&0x7F)<<14)|((uint32_t)(extHdr[2]&0x7F)<<7)|(uint32_t)(extHdr[3]&0x7F))
                    : (((uint32_t)extHdr[0]<<24)|((uint32_t)extHdr[1]<<16)|((uint32_t)extHdr[2]<<8)|(uint32_t)extHdr[3]);
                long skip = (ver == 4) ? ((long)extSz - 4) : (long)extSz;
                if (skip > 0 && ftell(f) + skip < tagEnd) fseek(f, skip, SEEK_CUR);
            }
        }

        while (ftell(f) < tagEnd) {
            long curPos = ftell(f);
            if (ver == 2) {
                if (curPos + 6 > tagEnd) break;
                uint8_t fh[6];
                if (fread(fh, 1, 6, f) != 6 || fh[0] == 0) break;
                uint32_t fSize = ((uint32_t)fh[3] << 16) | ((uint32_t)fh[4] << 8) | (uint32_t)fh[5];
                if (fSize == 0 || curPos + 6 + (long)fSize > tagEnd) break;

                bool isMeta = (memcmp(fh, "TT2", 3) == 0 || memcmp(fh, "TP1", 3) == 0 ||
                               memcmp(fh, "TAL", 3) == 0 || memcmp(fh, "TLE", 3) == 0 ||
                               memcmp(fh, "TBP", 3) == 0);
                if (isMeta) {
                    uint8_t buf[512];
                    uint32_t rLen = (fSize < sizeof(buf)) ? fSize : (uint32_t)sizeof(buf);
                    if (fread(buf, 1, rLen, f) == rLen) {
                        char cleanVal[256];
                        decode_id3_text(buf, rLen, cleanVal, sizeof(cleanVal));
                        if (cleanVal[0] != '\0') {
                            if (memcmp(fh, "TT2", 3) == 0 && outTitle && titleSz > 0) snprintf(outTitle, titleSz, "%s", cleanVal);
                            else if (memcmp(fh, "TP1", 3) == 0 && outArtist && artistSz > 0) snprintf(outArtist, artistSz, "%s", cleanVal);
                            else if (memcmp(fh, "TAL", 3) == 0 && outAlbum && albumSz > 0) snprintf(outAlbum, albumSz, "%s", cleanVal);
                            else if (memcmp(fh, "TLE", 3) == 0) {
                                long ms = atol(cleanVal);
                                if (ms > 500) tlenSec = (float)ms / 1000.0f;
                            } else if (memcmp(fh, "TBP", 3) == 0 && outBpm) {
                                float b = (float)atof(cleanVal);
                                if (b >= 40.0f && b <= 240.0f) { *outBpm = b; if (outHasBpm) *outHasBpm = true; }
                            }
                        }
                    }
                    if (fSize > rLen) fseek(f, (long)(fSize - rLen), SEEK_CUR);
                } else {
                    fseek(f, (long)fSize, SEEK_CUR);
                }
            } else {
                if (curPos + 10 > tagEnd) break;
                uint8_t fh[10];
                if (fread(fh, 1, 10, f) != 10 || fh[0] == 0) break;
                uint32_t fSize = (ver == 4)
                    ? (((uint32_t)(fh[4]&0x7F)<<21)|((uint32_t)(fh[5]&0x7F)<<14)|((uint32_t)(fh[6]&0x7F)<<7)|(uint32_t)(fh[7]&0x7F))
                    : (((uint32_t)fh[4]<<24)|((uint32_t)fh[5]<<16)|((uint32_t)fh[6]<<8)|(uint32_t)fh[7]);
                if (fSize == 0 || curPos + 10 + (long)fSize > tagEnd) break;

                bool isMeta = (memcmp(fh, "TIT2", 4) == 0 || memcmp(fh, "TPE1", 4) == 0 ||
                               memcmp(fh, "TALB", 4) == 0 || memcmp(fh, "TLEN", 4) == 0 ||
                               memcmp(fh, "TBPM", 4) == 0);
                if (isMeta) {
                    uint8_t buf[512];
                    uint32_t rLen = (fSize < sizeof(buf)) ? fSize : (uint32_t)sizeof(buf);
                    if (fread(buf, 1, rLen, f) == rLen) {
                        char cleanVal[256];
                        decode_id3_text(buf, rLen, cleanVal, sizeof(cleanVal));
                        if (cleanVal[0] != '\0') {
                            if (memcmp(fh, "TIT2", 4) == 0 && outTitle && titleSz > 0) snprintf(outTitle, titleSz, "%s", cleanVal);
                            else if (memcmp(fh, "TPE1", 4) == 0 && outArtist && artistSz > 0) snprintf(outArtist, artistSz, "%s", cleanVal);
                            else if (memcmp(fh, "TALB", 4) == 0 && outAlbum && albumSz > 0) snprintf(outAlbum, albumSz, "%s", cleanVal);
                            else if (memcmp(fh, "TLEN", 4) == 0) {
                                long ms = atol(cleanVal);
                                if (ms > 500) tlenSec = (float)ms / 1000.0f;
                            } else if (memcmp(fh, "TBPM", 4) == 0 && outBpm) {
                                float b = (float)atof(cleanVal);
                                if (b >= 40.0f && b <= 240.0f) { *outBpm = b; if (outHasBpm) *outHasBpm = true; }
                            }
                        }
                    }
                    if (fSize > rLen) fseek(f, (long)(fSize - rLen), SEEK_CUR);
                } else {
                    fseek(f, (long)fSize, SEEK_CUR);
                }
            }
        }

        offset = tagEnd + ((flags & 0x10) ? 10 : 0);
        fseek(f, offset, SEEK_SET);
    }

    if (outAudioOffset) *outAudioOffset = offset;
    if (outTlenSec) *outTlenSec = tlenSec;
}

void parse_mp3_id3v2_meta(FILE* f, long* outAudioOffset, float* outTlenSec) {
    parse_mp3_id3v2_meta_into(f,
                              currentTrackTitle, sizeof(currentTrackTitle),
                              currentTrackArtist, sizeof(currentTrackArtist),
                              currentTrackAlbum, sizeof(currentTrackAlbum),
                              &detectedBPM, &hasTrackBpmTag,
                              outAudioOffset, outTlenSec);
}

typedef struct {
    char* outTitle;
    size_t titleSz;
    char* outArtist;
    size_t artistSz;
} OggSummaryCtx;

static void on_ogg_summary_comment(const char* comment, int len, void* userData) {
    OggSummaryCtx* ctx = (OggSummaryCtx*)userData;
    if (!ctx) return;
    apply_oggshit_kv(comment, len, ctx->outTitle, ctx->titleSz, ctx->outArtist, ctx->artistSz, NULL, 0, NULL, NULL);
}

static void read_track_summary_tags(const char* filePath, const char* fileName,
                                    char* outTitle, size_t titleSz,
                                    char* outArtist, size_t artistSz) {
    char noExt[256];
    strip_extension_copy(fileName, noExt, sizeof(noExt));
    nuke_weird_utf8_chars(noExt, outTitle, titleSz);
    if (outTitle[0] == '\0') snprintf(outTitle, titleSz, "Audio Track");
    outArtist[0] = '\0';
    if (!filePath || !fileName) return;

    if (has_extension(fileName, ".mp3")) {
        FILE* f = fopen(filePath, "rb");
        if (f) {
            parse_mp3_id3v2_meta_into(f, outTitle, titleSz, outArtist, artistSz, NULL, 0, NULL, NULL, NULL, NULL);
            fclose(f);
        }
    } else if (has_extension(fileName, ".flac")) {
        FILE* f = fopen(filePath, "rb");
        if (f) {
            uint8_t hdr[4];
            if (fread(hdr, 1, 4, f) == 4 && memcmp(hdr, "fLaC", 4) == 0) {
                bool isLast = false;
                while (!isLast && !feof(f)) {
                    uint8_t bHdr[4];
                    if (fread(bHdr, 1, 4, f) != 4) break;
                    isLast = (bHdr[0] & 0x80) != 0;
                    uint8_t bType = bHdr[0] & 0x7F;
                    uint32_t bLen = ((uint32_t)bHdr[1] << 16) | ((uint32_t)bHdr[2] << 8) | (uint32_t)bHdr[3];
                    if (bType == 4 && bLen >= 8 && bLen <= 64 * 1024) {
                        uint8_t* vcBuf = (uint8_t*)malloc(bLen);
                        if (vcBuf) {
                            if (fread(vcBuf, 1, bLen, f) == bLen) {
                                uint32_t vLen = ((uint32_t)vcBuf[0]) | ((uint32_t)vcBuf[1] << 8) | ((uint32_t)vcBuf[2] << 16) | ((uint32_t)vcBuf[3] << 24);
                                uint32_t pos = 4 + vLen;
                                if (pos + 4 <= bLen) {
                                    uint32_t cnt = ((uint32_t)vcBuf[pos]) | ((uint32_t)vcBuf[pos+1] << 8) | ((uint32_t)vcBuf[pos+2] << 16) | ((uint32_t)vcBuf[pos+3] << 24);
                                    pos += 4;
                                    for (uint32_t c = 0; c < cnt && pos + 4 <= bLen; c++) {
                                        uint32_t slen = ((uint32_t)vcBuf[pos]) | ((uint32_t)vcBuf[pos+1] << 8) | ((uint32_t)vcBuf[pos+2] << 16) | ((uint32_t)vcBuf[pos+3] << 24);
                                        pos += 4;
                                        if (slen > bLen - pos) break;
                                        apply_oggshit_kv((const char*)(vcBuf + pos), (int)slen, outTitle, titleSz, outArtist, artistSz, NULL, 0, NULL, NULL);
                                        pos += slen;
                                    }
                                }
                            }
                            free(vcBuf);
                        }
                        break;
                    } else {
                        fseek(f, (long)bLen, SEEK_CUR);
                    }
                }
            }
            fclose(f);
        }
    } else if (has_extension(fileName, ".ogg") || has_extension(fileName, ".opus")) {
        OggSummaryCtx ctx = { outTitle, titleSz, outArtist, artistSz };
        ogg_scan(filePath, 1024 * 1024, on_ogg_summary_comment, &ctx);
    } else if (has_extension(fileName, ".m4a") || has_extension(fileName, ".mp4")) {
        m4a_read_metadata(filePath, outTitle, titleSz, outArtist, artistSz, NULL, 0, NULL, NULL, NULL, NULL);
    } else if (has_extension(fileName, ".aac")) {
        FILE* f = fopen(filePath, "rb");
        if (f) {
            parse_mp3_id3v2_meta_into(f, outTitle, titleSz, outArtist, artistSz, NULL, 0, NULL, NULL, NULL, NULL);
            fclose(f);
        }
    }
}

const char* get_next_song_display(int targetIdx, bool repeating) {
    if (targetIdx < 0 || targetIdx >= fileCount || fileList[targetIdx].is_dir) {
        return ": ---";
    }
    if (cachedNextIdx == targetIdx &&
        cachedNextRepeat == repeating &&
        cachedNextGen == currentTrackGeneration &&
        strcmp(cachedNextDir, currentDir) == 0) {
        return cachedNextDisplay;
    }

    char title[256] = "";
    char artist[256] = "";

    if (repeating && isPlaying) {
        snprintf(title, sizeof(title), "%s", currentTrackTitle);
        if (title[0] == '\0') strip_extension_copy(fileList[targetIdx].name, title, sizeof(title));
        if (strcmp(currentTrackArtist, "3DS Music Library") != 0) {
            snprintf(artist, sizeof(artist), "%s", currentTrackArtist);
        }
    } else {
        char fp[1024];
        if (strcmp(currentDir, "sdmc:/") == 0) snprintf(fp, sizeof(fp), "sdmc:/%s", fileList[targetIdx].name);
        else snprintf(fp, sizeof(fp), "%s/%s", currentDir, fileList[targetIdx].name);
        read_track_summary_tags(fp, fileList[targetIdx].name, title, sizeof(title), artist, sizeof(artist));
    }

    if (artist[0] != '\0') {
        snprintf(cachedNextDisplay, sizeof(cachedNextDisplay), ": %s / %s", title, artist);
    } else {
        snprintf(cachedNextDisplay, sizeof(cachedNextDisplay), ": %s", title);
    }

    cachedNextIdx = targetIdx;
    cachedNextRepeat = repeating;
    cachedNextGen = currentTrackGeneration;
    snprintf(cachedNextDir, sizeof(cachedNextDir), "%s", currentDir);
    return cachedNextDisplay;
}


int find_next_song_index(int fromIdx, int step) {
    if (fileCount <= 0) return -1;
    for (int k = 1; k <= fileCount; k++) {
        int idx = (fromIdx + step * k) % fileCount;
        if (idx < 0) idx += fileCount;
        if (!fileList[idx].is_dir) return idx;
    }
    return -1;
}

int find_random_song_index(int currentIdx) {
    int songIndices[MAX_FILES];
    int count = 0;
    for (int i = 0; i < fileCount; i++) {
        if (!fileList[i].is_dir) songIndices[count++] = i;
    }
    if (count == 0) return -1;
    if (count == 1) return songIndices[0];
    srand((unsigned int)(osGetTime() ^ ((u64)(songTime * 1000.0f) << 8) ^ (u64)rand()));
    int pick = songIndices[rand() % count];
    if (pick == currentIdx) {
        pick = songIndices[(rand() % (count - 1) + 1) % count];
        if (pick == currentIdx) {
            for (int i = 0; i < count; i++) {
                if (songIndices[i] != currentIdx) { pick = songIndices[i]; break; }
            }
        }
    }
    return pick;
}

void play_track_by_index(int idx, TransitionDir dir) {
    if (idx < 0 || idx >= fileCount || fileList[idx].is_dir) return;
    selectedFile = idx;
    if (selectedFile < scrollOffset) scrollOffset = selectedFile;
    if (selectedFile >= scrollOffset + CFG_LIST_VISIBLE_ROWS) scrollOffset = selectedFile - (CFG_LIST_VISIBLE_ROWS - 1);
    if (scrollOffset < 0) scrollOffset = 0;

    char fp[1024];
    snprintf(fp, sizeof(fp), "%s/%s", currentDir, fileList[selectedFile].name);
    start_playback(fp, fileList[selectedFile].name, dir);
}

