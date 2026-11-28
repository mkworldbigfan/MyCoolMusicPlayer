//hello boy what are u doing here

#include "app_state.h"

void build_lrc_path(const char* songPath, char* lrcPath, size_t lrcPathSize) {
    if (!lrcPath || lrcPathSize == 0) return;
    lrcPath[0] = '\0';
    if (!songPath || songPath[0] == '\0') return;
    size_t maxCopy = (lrcPathSize > 5) ? (lrcPathSize - 5) : 0;
    snprintf(lrcPath, maxCopy + 1, "%s", songPath);
    char* slash = strrchr(lrcPath, '/');
    char* dot = strrchr(lrcPath, '.');
    if (dot && (!slash || dot > slash)) {
        strcpy(dot, ".lrc");
    } else {
        strcat(lrcPath, ".lrc");
    }
}

bool lrc_file_exists(const char* songPath) {
    char lrcPath[1024];
    build_lrc_path(songPath, lrcPath, sizeof(lrcPath));
    if (lrcPath[0] == '\0') return false;
    struct stat st;
    return (stat(lrcPath, &st) == 0 && !S_ISDIR(st.st_mode));
}

void load_lrc_file(const char* songPath) { //fuck you
    lrcLineCount = 0;
    currentLrcIndex = -1;
    lyricScrollPos = -1.0f;

    char lrcPath[1024];
    build_lrc_path(songPath, lrcPath, sizeof(lrcPath));

    FILE* f = fopen(lrcPath, "r");
    if (!f) return;

    char line[256];
    while (fgets(line, sizeof(line), f) && lrcLineCount < MAX_LRC_LINES) {
        char* b1 = strchr(line, '[');
        char* b2 = strchr(line, ']');
        if (b1 && b2 && b2 > b1) {
            if (strncasecmp(b1 + 1, "bpm:", 4) == 0) {
                float b = 0.0f;
                if (sscanf(b1 + 5, "%f", &b) == 1 && b >= 40.0f && b <= 240.0f) {
                    detectedBPM = b;
                    hasTrackBpmTag = true;
                }
                continue;
            }

            int min = 0; float sec = 0.0f;
            if (sscanf(b1 + 1, "%d:%f", &min, &sec) >= 2) {
                lrcLines[lrcLineCount].timestamp = (float)min * 60.0f + sec;
                char* text = b2 + 1;
                while (*text == ' ' || *text == '\t') text++;
                size_t len = strlen(text);
                while (len > 0 && (text[len-1] == '\r' || text[len-1] == '\n')) {
                    text[--len] = '\0';
                }
                char cleanText[256];
                nuke_weird_utf8_chars(text, cleanText, sizeof(cleanText));
                if (cleanText[0] == '\0' && text[0] != '\0') {
                    strcpy(cleanText, "~ ~ ~");
                }
                strncpy(lrcLines[lrcLineCount].text, cleanText, sizeof(lrcLines[lrcLineCount].text) - 1);
                lrcLines[lrcLineCount].text[sizeof(lrcLines[lrcLineCount].text) - 1] = '\0';
                lrcLineCount++;
            }
        }
    }
    fclose(f);
}

void url_encode_str(const char* src, char* dst, size_t dstSize) {
    static const char hex[] = "0123456789ABCDEF";
    size_t d = 0;
    for (size_t i = 0; src && src[i] != '\0' && d + 4 < dstSize; i++) {
        unsigned char c = (unsigned char)src[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
            c == '-' || c == '_' || c == '.' || c == '~') {
            dst[d++] = (char)c;
        } else if (c == ' ') {
            if (d + 4 < dstSize) {
                dst[d++] = '%'; dst[d++] = '2'; dst[d++] = '0';
            }
        } else {
            dst[d++] = '%';
            dst[d++] = hex[(c >> 4) & 0xF];
            dst[d++] = hex[c & 0xF];
        }
    }
    dst[d] = '\0';
}

void clean_query_title(const char* src, char* dst, size_t dstSize) {
    if (!src || !dst || dstSize == 0) return;
    const char* p = src;
    
    if (*p >= '0' && *p <= '9') {
        const char* q = p;
        while ((*q >= '0' && *q <= '9') || *q == '-') q++;
        if (*q == ' ' || *q == '.') {
            while (*q == ' ' || *q == '.' || *q == '-') q++;
            if (*q != '\0') p = q;
        }
    }
    size_t d = 0;
    while (p[0] != '\0' && d + 1 < dstSize) {
        if (p[0] == '(' && p[1] >= '0' && p[1] <= '9' && p[2] == ')') {
            p += 3;
            continue;
        }
        dst[d++] = *p++;
    }
    while (d > 0 && dst[d-1] == ' ') d--;
    dst[d] = '\0';
}

static uint32_t hash_str_fnv1a(const char* s) {
    uint32_t h = 2166136261u;
    if (!s) return h;
    for (size_t i = 0; s[i] != '\0'; i++) {
        h ^= (unsigned char)s[i];
        h *= 16777619u;
    }
    return h;
}

bool is_nothingburger_title(const char* title) {
    if (!title || strlen(title) < 2) return true;
    return (strcasecmp(title, "Audio Track") == 0 ||
            strcasecmp(title, "Inst") == 0 ||
            strcasecmp(title, "Instrumental") == 0 ||
            strcasecmp(title, "Voices") == 0 ||
            strcasecmp(title, "Vocals") == 0);
}

char* httpc_fetch_json(const char* startUrl, uint32_t activeGen, Result* outRet, u32* outHttpStatus, size_t* outLen) {
    if (outLen) *outLen = 0;
    if (outRet) *outRet = 0;
    if (outHttpStatus) *outHttpStatus = 0;
    if (!startUrl || !hasHttpcService) return NULL;
    if (activeGen != 0 && currentTrackGeneration != activeGen) return NULL;

    char urlBuf[1024];
    snprintf(urlBuf, sizeof(urlBuf), "%s", startUrl);

    httpcContext ctx;
    u32 statusCode = 0;
    Result ret = 0;

    for (int redir = 0; redir < 3; redir++) {
        if (isAppSuspended || !lyricsThreadRun) return NULL;
        if (activeGen != 0 && currentTrackGeneration != activeGen) return NULL;
        ret = httpcOpenContext(&ctx, HTTPC_METHOD_GET, urlBuf, 1);
        if (R_FAILED(ret)) { if (outRet) *outRet = ret; return NULL; }

        if (strncmp(urlBuf, "https://", 8) == 0) { //can you please stop
            httpcSetSSLOpt(&ctx, SSLCOPT_DisableVerify);
        }
        httpcSetKeepAlive(&ctx, HTTPC_KEEPALIVE_DISABLED);
        httpcAddRequestHeaderField(&ctx, "User-Agent", "MyCoolMusicPlayer/1.0 (kirb2109@gmail.com)");
        if (strstr(urlBuf, "music.163.com")) httpcAddRequestHeaderField(&ctx, "Referer", "http://music.163.com/");

        ret = httpcBeginRequest(&ctx);
        if (R_FAILED(ret)) {
            if (outRet) *outRet = ret;
            httpcCancelConnection(&ctx);
            httpcCloseContext(&ctx);
            return NULL;
        }

        ret = httpcGetResponseStatusCodeTimeout(&ctx, &statusCode, 15000000000ULL);
        if (R_FAILED(ret)) {
            if (outRet) *outRet = ret;
            httpcCancelConnection(&ctx);
            httpcCloseContext(&ctx);
            return NULL;
        }
        if (outHttpStatus) *outHttpStatus = statusCode;

        if ((statusCode >= 301 && statusCode <= 303) || (statusCode >= 307 && statusCode <= 308)) {
            char nextUrl[1024];
            if (R_SUCCEEDED(httpcGetResponseHeader(&ctx, "Location", nextUrl, sizeof(nextUrl)))) {
                snprintf(urlBuf, sizeof(urlBuf), "%s", nextUrl);
                httpcCancelConnection(&ctx);
                httpcCloseContext(&ctx);
                continue;
            }
        }
        break;
    }

    if (statusCode != 200) {
        httpcCancelConnection(&ctx);
        httpcCloseContext(&ctx);
        return NULL;
    }

    const u32 MAX_JSON_SIZE = 256 * 1024;
    const u32 CHUNK_SIZE    = 4096; // this fucking timeout shit
    u32 cap = CHUNK_SIZE * 2;
    u32 total = 0;
    char* buf = (char*)malloc(cap);
    if (!buf) { httpcCancelConnection(&ctx); httpcCloseContext(&ctx); return NULL; }

    do {
        if (isAppSuspended || !lyricsThreadRun || (activeGen != 0 && currentTrackGeneration != activeGen)) {
            free(buf);
            httpcCancelConnection(&ctx);
            httpcCloseContext(&ctx);
            return NULL;
        }
        if (total + CHUNK_SIZE + 1 > cap) {
            if (cap >= MAX_JSON_SIZE) break;
            u32 newCap = cap + CHUNK_SIZE * 4;
            if (newCap > MAX_JSON_SIZE) newCap = MAX_JSON_SIZE;
            char* nb = (char*)realloc(buf, newCap);
            if (!nb) {
                free(buf);
                httpcCancelConnection(&ctx);
                httpcCloseContext(&ctx);
                return NULL;
            }
            buf = nb;
            cap = newCap;
        }
        u32 space = (cap - total - 1 < CHUNK_SIZE) ? (cap - total - 1) : CHUNK_SIZE;
        if (space == 0) break;
        u32 readSz = 0;
        ret = httpcDownloadData(&ctx, (u8*)(buf + total), space, &readSz);
        total += readSz;
    } while (ret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING);

    if (ret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING) {
        httpcCancelConnection(&ctx);
    }
    httpcCloseContext(&ctx);

    if (total == 0 || (ret != 0 && ret != (s32)HTTPC_RESULTCODE_DOWNLOADPENDING)) {
        if (outRet) *outRet = ret;
        free(buf);
        return NULL;
    }
    buf[total] = '\0';
    if (outLen) *outLen = total;
    return buf;
}

static bool extract_json_lrc_field(const char* json, const char* keyWithQuotes, char* outLrc, size_t outCap) {
    if (!json || !keyWithQuotes || !outLrc || outCap < 16) return false;
    size_t klen = strlen(keyWithQuotes);
    const char* p = json;
    while ((p = strstr(p, keyWithQuotes)) != NULL) {
        p += klen;
        while (*p == ' ' || *p == '\t' || *p == ':') p++;
        if (strncmp(p, "null", 4) == 0) continue;
        if (*p != '"') continue;
        p++;

        size_t o = 0;
        while (*p != '\0' && *p != '"' && o + 4 < outCap) {
            if (*p == '\\' && p[1] != '\0') {
                p++;
                if (*p == 'n') { outLrc[o++] = '\n'; p++; }
                else if (*p == 'r') { p++; }
                else if (*p == 't') { outLrc[o++] = ' '; p++; }
                else if (*p == '"' || *p == '\\' || *p == '/') { outLrc[o++] = *p++; }
                else if (*p == 'u' && p[1] && p[2] && p[3] && p[4]) {
                    unsigned int cp = 0;
                    if (sscanf(p + 1, "%4x", &cp) == 1) {
                        if (cp < 0x80) {
                            outLrc[o++] = (char)cp;
                        } else if (cp < 0x800) {
                            outLrc[o++] = (char)(0xC0 | (cp >> 6));
                            outLrc[o++] = (char)(0x80 | (cp & 0x3F));
                        } else {
                            outLrc[o++] = (char)(0xE0 | (cp >> 12));
                            outLrc[o++] = (char)(0x80 | ((cp >> 6) & 0x3F));
                            outLrc[o++] = (char)(0x80 | (cp & 0x3F));
                        }
                    }
                    p += 5;
                } else {
                    outLrc[o++] = *p++;
                }
            } else {
                outLrc[o++] = *p++;
            }
        }
        outLrc[o] = '\0';
        if (o > 8 && strchr(outLrc, '[') && strchr(outLrc, ':')) {
            return true;
        }
    }
    return false;
}

static int extract_netease_song_ids(const char* json, long* outIds, int maxIds) {
    if (!json || !outIds || maxIds <= 0) return 0;
    const char* p = strstr(json, "\"songs\"");
    if (!p) return 0;
    p = strchr(p, '[');
    if (!p) return 0;
    p++;

    int count = 0;
    int depth = 0;
    bool inStr = false;

    while (*p != '\0' && count < maxIds) {
        if (inStr) {
            if (*p == '\\' && p[1] != '\0') { p += 2; continue; }
            if (*p == '"') { inStr = false; p++; continue; }
            p++;
            continue;
        }

        if (*p == '"') {
            if (depth == 1 && strncmp(p, "\"id\"", 4) == 0) {
                const char* q = p + 4;
                while (*q == ' ' || *q == '\t') q++;
                if (*q == ':') {
                    q++;
                    while (*q == ' ' || *q == '\t') q++;
                    long idVal = atol(q);
                    if (idVal > 0) outIds[count++] = idVal;
                }
                p += 4;
                continue;
            }
            inStr = true;
            p++;
            continue;
        }

        if (*p == '{') depth++;
        else if (*p == '}') {
            if (depth > 0) depth--;
        } else if (*p == ']' && depth == 0) {
            break;
        }
        p++;
    }
    return count;
}
static int extract_netease_songs(const char* json, long* outIds,
                                  char* outArtists, char* outTitles,
                                  int maxIds, int cap) {
    if (!json || !outIds || maxIds <= 0) return 0;
    const char* p = strstr(json, "\"songs\"");
    if (!p) return 0;
    p = strchr(p, '[');
    if (!p) return 0;
    p++;

    int count = 0;
    int depth = 0;
    bool inStr = false;

    while (*p != '\0' && count < maxIds) {
        if (inStr) {
            if (*p == '\\' && p[1] != '\0') { p += 2; continue; }
            if (*p == '"') { inStr = false; p++; continue; }
            p++;
            continue;
        }

        if (*p == '"') {
            if (depth == 1 && strncmp(p, "\"id\"", 4) == 0) {
                const char* q = p + 4;
                while (*q == ' ' || *q == '\t') q++;
                if (*q == ':') {
                    q++;
                    while (*q == ' ' || *q == '\t') q++;
                    long idVal = atol(q);
                    if (idVal > 0) {
                        outIds[count] = idVal;
                        if (outArtists) outArtists[count * cap] = '\0';
                        if (outTitles)  outTitles[count * cap]  = '\0';
                        count++;
                    }
                }
                p += 4; continue;
            }
            if (outTitles && count > 0 && depth == 1 && strncmp(p, "\"name\"", 6) == 0) {
                const char* n = p + 6;
                while (*n == ' ' || *n == '\t' || *n == ':') n++;
                if (*n == '"') {
                    n++;
                    char* dst = outTitles + (count - 1) * cap;
                    int ni = 0;
                    while (*n && *n != '"' && ni < cap - 1) {
                        if (*n == '\\' && n[1]) n++;
                        dst[ni++] = *n++;
                    }
                    dst[ni] = '\0';
                }
                p += 6; continue;
            }
            if (outArtists && count > 0 && depth == 1 && strncmp(p, "\"artists\"", 9) == 0) {
                const char* a = strstr(p + 9, "\"name\"");
                const char* endBracket = strchr(p + 9, ']');
                if (a && (!endBracket || a < endBracket)) {
                    a += 6;
                    while (*a == ' ' || *a == '\t' || *a == ':') a++;
                    if (*a == '"') {
                        a++;
                        char* dst = outArtists + (count - 1) * cap;
                        int ai = 0;
                        while (*a && *a != '"' && ai < cap - 1) {
                            if (*a == '\\' && a[1]) a++;
                            dst[ai++] = *a++;
                        }
                        dst[ai] = '\0';
                    }
                }
                p += 9; continue;
            }
            inStr = true; p++; continue;
        }

        if (*p == '{') depth++;
        else if (*p == '}') { if (depth > 0) depth--; }
        else if (*p == ']' && depth == 0) break;
        p++;
    }
    return count;
}

static bool str_fuzzy_match(const char* haystack, const char* needle) {
    if (!needle || needle[0] == '\0') return true;
    if (!haystack || haystack[0] == '\0') return true;
    size_t nlen = strlen(needle);
    for (size_t i = 0; haystack[i]; i++) {
        if (strncasecmp(haystack + i, needle, nlen) == 0) return true;
    }
    return false;
}

static bool candidate_matches_artist(const char* artistField, const char* titleField,
                                      const char* expectedArtist) {
    if (!expectedArtist || expectedArtist[0] == '\0') return true;
    bool artistKnown = (artistField && artistField[0] != '\0');
    bool titleKnown  = (titleField  && titleField[0]  != '\0');
    if (!artistKnown && !titleKnown) return true;
    if (artistKnown && str_fuzzy_match(artistField, expectedArtist)) return true;
    if (titleKnown  && str_fuzzy_match(titleField,  expectedArtist)) return true;
    return false;
}


static void set_lyrics_debug_status(const char* msg) {
    LightLock_Lock(&lyricsLock);
    snprintf(lyricsDebugStatus, sizeof(lyricsDebugStatus), "%s", msg ? msg : "");
    LightLock_Unlock(&lyricsLock);
}

static char* fetch_with_retry(const char* url, uint32_t gen,
                              Result* outErr, u32* outHttp, size_t* outLen,
                              int maxTries) {
    char* r = NULL;
    for (int t = 0; t < maxTries; t++) {
        if (isAppSuspended || !lyricsThreadRun) return NULL;
        if (gen != 0 && currentTrackGeneration != gen) return NULL;
        if (t > 0) svcSleepThread((s64)(600 + t * 400) * 1000 * 1000LL);
        r = httpc_fetch_json(url, gen, outErr, outHttp, outLen);
        if (r) return r;
        if (*outErr == 0 && *outHttp != 0 && *outHttp != 200) break;
    }
    return NULL;
}

static bool fetch_lrc_for_ids(long* ids, char* artists, char* titles, int count,
                               uint32_t gen, const char* expectedArtist, int cap,
                               char* lrcOut, size_t lrcCap,
                               Result* outErr, u32* outHttp) {
    char url[1024];
    size_t jlen = 0;
    bool useFilter = (expectedArtist && expectedArtist[0] != '\0');

    for (int c = 0; c < count; c++) {
        if (gen != 0 && currentTrackGeneration != gen) return false;
        if (useFilter) {
            const char* art   = artists ? (artists + c * cap) : NULL;
            const char* title = titles  ? (titles  + c * cap) : NULL;
            if (!candidate_matches_artist(art, title, expectedArtist)) continue;
        }
        svcSleepThread(100 * 1000 * 1000LL);
        snprintf(url, sizeof(url), "http://music.163.com/api/song/lyric?id=%ld&lv=1", ids[c]);
        char* lJson = fetch_with_retry(url, gen, outErr, outHttp, &jlen, 3);
        if (lJson) {
            const char* lrcSec = strstr(lJson, "\"lrc\"");
            bool ok = extract_json_lrc_field(lrcSec ? lrcSec : lJson, "\"lyric\"", lrcOut, lrcCap);
            free(lJson);
            if (ok) return true;
        }
    }
    return false;
}

void fnf_lyrics_slave(void* arg) {
    char* lrcOut = (char*)malloc(64 * 1024);
    uint32_t failedCache[64] = {0};
    int failedWritePos = 0;

    while (lyricsThreadRun) {
        if (isAppSuspended) {
            svcSleepThread(200 * 1000 * 1000LL);
            continue;
        }
        lastfm_process_async();
        bool haveJob = false;
        bool deferJob = false;
        LyricsFetchJob job;

        LightLock_Lock(&lyricsLock);
        if (hasPendingLyricsJob) {
            if ((osGetTime() - pendingLyricsJob.enqueueTimeMs) < 1800ULL) {
                deferJob = true;
            } else {
                job = pendingLyricsJob;
                hasPendingLyricsJob = false;
                haveJob = true;
            }
        }
        LightLock_Unlock(&lyricsLock);

        if (!haveJob) {
            svcSleepThread(deferJob ? (200 * 1000 * 1000LL) : (250 * 1000 * 1000LL));
            continue;
        }

        if (!lrcOut || !hasAcService || !hasHttpcService || !lyricsEnabled) {
            set_lyrics_debug_status(!hasHttpcService ? "NO HTTPC" : "");
            continue;
        }
        if (job.generation != currentTrackGeneration) continue;
        if (lrc_file_exists(job.songPath)) { set_lyrics_debug_status("LOCAL"); continue; }

        uint32_t pathHash = hash_str_fnv1a(job.songPath);
        bool alreadyFailed = false;
        for (int fc = 0; fc < 64; fc++) {
            if (failedCache[fc] == pathHash && pathHash != 0) { alreadyFailed = true; break; }
        }
        if (alreadyFailed) continue;

        char cleanTitle[256];
        clean_query_title(job.title, cleanTitle, sizeof(cleanTitle));
        if (is_nothingburger_title(cleanTitle)) { set_lyrics_debug_status("INST"); continue; }

        u32 wifiStatus = 0;
        Result acuRet = ACU_GetStatus(&wifiStatus);
        if (R_SUCCEEDED(acuRet) && wifiStatus != 3) {
            set_lyrics_debug_status("NO WIFI");
            continue;
        }

        set_lyrics_debug_status("...");

        char queryTitle[256];
        snprintf(queryTitle, sizeof(queryTitle), "%s", cleanTitle);
        for (size_t qi = 0; queryTitle[qi] != '\0'; qi++) {
            if (queryTitle[qi] == '-' && (qi == 0 || queryTitle[qi-1] == ' ') && queryTitle[qi+1] == ' ') {
                queryTitle[qi] = ' ';
            }
        }

        char encTitle[384], encArtist[384], encAlbum[384];
        url_encode_str(queryTitle, encTitle, sizeof(encTitle));

        bool hasArtist = (job.artist[0] != '\0' && strcmp(job.artist, "3DS Music Library") != 0);
        bool hasAlbum  = (job.album[0]  != '\0' && strcmp(job.album, "Audio Collection") != 0);
        if (hasArtist) url_encode_str(job.artist, encArtist, sizeof(encArtist));
        if (hasAlbum)  url_encode_str(job.album, encAlbum, sizeof(encAlbum));

        bool found = false;
        char url[1024];
        Result lastNetErr = 0;
        u32 lastHttpCode = 0;
        size_t jlen = 0;

        if (job.generation == currentTrackGeneration) {
            if (hasArtist && hasAlbum) {
                snprintf(url, sizeof(url),
                    "http://mycoollyricsapi.kirb2109.workers.dev/?artist=%s&track=%s&album=%s",
                    encArtist, encTitle, encAlbum);
            } else if (hasArtist) {
                snprintf(url, sizeof(url),
                    "http://mycoollyricsapi.kirb2109.workers.dev/?artist=%s&track=%s",
                    encArtist, encTitle);
            } else {
                snprintf(url, sizeof(url),
                    "http://mycoollyricsapi.kirb2109.workers.dev/?track=%s", //js securingg
                    encTitle);
            }

            // i hate 3ds wifi
            char* lJson = fetch_with_retry(url, job.generation, &lastNetErr, &lastHttpCode, &jlen, 3);
            if (lJson) {
                bool ok = extract_json_lrc_field(lJson, "\"syncedLyrics\"", lrcOut, 64 * 1024);
                if (!ok) ok = extract_json_lrc_field(lJson, "\"plainLyrics\"", lrcOut, 64 * 1024);
                free(lJson);
                found = ok;
            }
        }

        #define ARTIST_CAP 128
        if (!found && job.generation == currentTrackGeneration) {
            svcSleepThread(200 * 1000 * 1000LL);
            if (hasArtist) {
                snprintf(url, sizeof(url), "http://music.163.com/api/search/get?s=%s%%20%s&type=1&limit=5", encTitle, encArtist);
            } else {
                snprintf(url, sizeof(url), "http://music.163.com/api/search/get?s=%s&type=1&limit=5", encTitle);
            }
            char* neJson = fetch_with_retry(url, job.generation, &lastNetErr, &lastHttpCode, &jlen, 3);
            if (neJson) {
                long neIds[5] = {0};
                char neArtists[5 * ARTIST_CAP];
                char neTitles[5 * ARTIST_CAP];
                memset(neArtists, 0, sizeof(neArtists));
                memset(neTitles,  0, sizeof(neTitles));
                int neCnt = extract_netease_songs(neJson, neIds, neArtists, neTitles, 5, ARTIST_CAP);
                free(neJson);
                found = fetch_lrc_for_ids(neIds, neArtists, neTitles, neCnt, job.generation,
                                          hasArtist ? job.artist : NULL, ARTIST_CAP,
                                          lrcOut, 64 * 1024, &lastNetErr, &lastHttpCode);
            }
        }

        if (found && !lrc_file_exists(job.songPath)) {
            char lrcPath[1024];
            build_lrc_path(job.songPath, lrcPath, sizeof(lrcPath));

            FILE* f = fopen(lrcPath, "w");
            if (f) {
                fwrite(lrcOut, 1, strlen(lrcOut), f);
                fclose(f);
                set_lyrics_debug_status("OK");
            } else {
                set_lyrics_debug_status("SD ERR");
            }

            LightLock_Lock(&lyricsLock);
            if (job.generation == currentTrackGeneration) {
                downloadedLrcReady = true;
            }
            LightLock_Unlock(&lyricsLock);
        } else if (!found && job.generation == currentTrackGeneration) {
            if (lastNetErr != 0) {
                char msg[24];
                snprintf(msg, sizeof(msg), "ERR %08lX", (unsigned long)lastNetErr);
                set_lyrics_debug_status(msg);
            } else if (lastHttpCode != 0 && lastHttpCode != 200) {
                char msg[24];
                snprintf(msg, sizeof(msg), "HTTP %lu", (unsigned long)lastHttpCode);
                set_lyrics_debug_status(msg);
            } else {
                set_lyrics_debug_status("NO LRC");
            }
            failedCache[failedWritePos] = pathHash;
            failedWritePos = (failedWritePos + 1) % 64;
        }
    }
    if (lrcOut) free(lrcOut);
    threadExit(0);
}

