#include "app_state.h"

static int get_qr_cell_size(void) {
    if (!lfm_qr_ready) return 4;
    int qrSize = qrcodegen_getSize(lfm_qrcode);
    int cell = 132 / qrSize;
    if (cell < 2) cell = 2;
    return cell;
}

void draw_qr_code_centered(float cx, float topY) {
    if (!lfm_qr_ready) return;
    int qrSize = qrcodegen_getSize(lfm_qrcode);
    float sz = (float)get_qr_cell_size();
    float qrW = (float)qrSize * sz;
    float px = cx - qrW * 0.5f;
    float py = topY;

    C2D_DrawRectSolid(px - 6.0f, py - 6.0f, CFG_DEPTH_TEXTS - 0.01f,
                      qrW + 12.0f, qrW + 12.0f, C2D_Color32(255, 255, 255, 255));

    for (int row = 0; row < qrSize; row++) {
        for (int col = 0; col < qrSize; col++) {
            if (qrcodegen_getModule(lfm_qrcode, col, row)) {
                C2D_DrawRectSolid(px + col * sz, py + row * sz, CFG_DEPTH_TEXTS,
                                  sz, sz, C2D_Color32(0, 0, 0, 255));
            }
        }
    }
}


void lastfm_load_cfg(void) {
    LightLock_Lock(&lastfmLock);
    FILE *f = fopen(LASTFM_CFG_PATH, "r");
    if (f) {
        fscanf(f, "key=%127s\nuser=%63s", lastfm_session_key, lastfm_username);
        fclose(f);
        lastfm_has_session = (lastfm_session_key[0] != '\0');
    }
    LightLock_Unlock(&lastfmLock);
}

static void lastfm_save_cfg(const char *key, const char *user) {
    //does this exist?
    mkdir("sdmc:/3ds", 0777);
    mkdir("sdmc:/3ds/MusicPlayer", 0777);
    LightLock_Lock(&lastfmLock);
    FILE *f = fopen(LASTFM_CFG_PATH, "w");
    if (f) {
        fprintf(f, "key=%s\nuser=%s\n", key, user);
        fclose(f);
    }
    snprintf(lastfm_session_key, sizeof(lastfm_session_key), "%s", key);
    snprintf(lastfm_username,    sizeof(lastfm_username),    "%s", user);
    lastfm_has_session = true;
    LightLock_Unlock(&lastfmLock);
}

static void lastfm_enqueue_scrobble(const char *json_line) {
    LightLock_Lock(&lastfmLock);
    FILE *f = fopen(LASTFM_QUEUE_PATH, "a");
    if (f) { fprintf(f, "%s\n", json_line); fclose(f); }
    LightLock_Unlock(&lastfmLock);
}

static u32 lastfm_post_json(const char *url, const char *json,
                             char *outBuf, size_t outSize) {
    if (!hasHttpcService) return 0;
    httpcContext ctx;
    Result rc = httpcOpenContext(&ctx, HTTPC_METHOD_POST, url, 1);
    if (R_FAILED(rc)) return 0;
    httpcSetSSLOpt(&ctx, SSLCOPT_DisableVerify);
    httpcSetKeepAlive(&ctx, HTTPC_KEEPALIVE_DISABLED);
    httpcAddRequestHeaderField(&ctx, "Content-Type", "application/json");
    httpcAddRequestHeaderField(&ctx, "User-Agent",   "3DSMusicPlayer/1.0");
    httpcAddPostDataRaw(&ctx, (const u32*)json, (u32)strlen(json));
    rc = httpcBeginRequest(&ctx);
    if (R_FAILED(rc)) { httpcCloseContext(&ctx); return 0; }
    u32 status = 0;
    httpcGetResponseStatusCodeTimeout(&ctx, &status, 10000000000ULL);
    if (outBuf && outSize > 1) {
        u32 got = 0;
        httpcDownloadData(&ctx, (u8*)outBuf, (u32)(outSize-1), &got);
        outBuf[got] = '\0';
    }
    httpcCloseContext(&ctx);
    return status;
}

static u32 lastfm_get_json(const char *url, char *outBuf, size_t outSize) {
    if (!hasHttpcService) return 0;
    httpcContext ctx;
    Result rc = httpcOpenContext(&ctx, HTTPC_METHOD_GET, url, 1);
    if (R_FAILED(rc)) return 0;
    httpcSetSSLOpt(&ctx, SSLCOPT_DisableVerify);
    httpcSetKeepAlive(&ctx, HTTPC_KEEPALIVE_DISABLED);
    httpcAddRequestHeaderField(&ctx, "User-Agent", "3DSMusicPlayer/1.0");
    rc = httpcBeginRequest(&ctx);
    if (R_FAILED(rc)) { httpcCloseContext(&ctx); return 0; }
    u32 status = 0;
    httpcGetResponseStatusCodeTimeout(&ctx, &status, 10000000000ULL);
    if (outBuf && outSize > 1) {
        u32 got = 0;
        httpcDownloadData(&ctx, (u8*)outBuf, (u32)(outSize - 1), &got);
        outBuf[got] = '\0';
    }
    httpcCloseContext(&ctx);
    return status;
}

static bool lastfm_extract_json_str(const char *json, const char *key,
                                    char *out, size_t outSz) {
    char pat[64];
    snprintf(pat, sizeof(pat), "\"%s\"", key);
    const char *p = strstr(json, pat);
    if (!p) return false;
    p += strlen(pat);
    p = strchr(p, '"');
    if (!p) return false;
    p++;
    size_t i = 0;
    while (*p && *p != '"' && i + 1 < outSz) {
        out[i++] = *p++;
    }
    out[i] = '\0';
    return (i > 0);
}

static bool lastfm_start_qr_auth(void) {
    lfm_qr_ready = false;
    lfm_auth_token[0] = '\0';

    char url[256];
    snprintf(url, sizeof(url), "%s/lastfm/start", LASTFM_WORKER_BASE);

    char resp[512] = {0};
    u32 st = lastfm_get_json(url, resp, sizeof(resp));
    if (st != 200 || resp[0] == '\0') return false;

    char authUrl[256] = {0};
    if (!lastfm_extract_json_str(resp, "token", lfm_auth_token, sizeof(lfm_auth_token)))
        return false;
    if (!lastfm_extract_json_str(resp, "authUrl", authUrl, sizeof(authUrl)))
        return false;

    uint8_t tempBuf[qrcodegen_BUFFER_LEN_FOR_VERSION(10)];
    if (!qrcodegen_encodeText(authUrl, tempBuf, lfm_qrcode, qrcodegen_Ecc_LOW,
                              qrcodegen_VERSION_MIN, 10, qrcodegen_Mask_AUTO, true)) {
        return false;
    }
    lfm_qr_ready = true;
    return true;
}

void tell_lastfm_im_listening(void);

int lastfm_poll_qr_auth(void) {
    if (!lfm_auth_token[0]) return -1;

    char url[384];
    snprintf(url, sizeof(url), "%s/lastfm/poll?token=%s",
             LASTFM_WORKER_BASE, lfm_auth_token);

    char resp[512] = {0};
    u32 st = lastfm_get_json(url, resp, sizeof(resp));
    if (st == 202) return 0;
    if (st != 200 || resp[0] == '\0') return (st == 0) ? 0 : -1;

    char newKey[128] = {0}, newUser[64] = {0};
    if (lastfm_extract_json_str(resp, "session_key", newKey, sizeof(newKey)) &&
        lastfm_extract_json_str(resp, "username", newUser, sizeof(newUser))) {
        lastfm_save_cfg(newKey, newUser);
        snprintf(g_lastfm_login_status, sizeof(g_lastfm_login_status),
                 "OK! %s", newUser);
        g_lastfm_login_done = true;
        lfm_done_timer = 0.0f;
        if (isPlaying && !isPaused && currentTrackTitle[0]) {
            lastfm_track_start_ts = (u32)time(NULL);
            tell_lastfm_im_listening();
        }
        return 1;
    }
    return 0;
}

static LightLock lastfmAsyncLock;
static bool lastfmAsyncLockInit = false;
static char pendingNowPlayingUrl[1536] = "";
static char pendingScrobbleUrl[1536] = "";

static void ensure_lastfm_lock(void) {
    if (!lastfmAsyncLockInit) {
        LightLock_Init(&lastfmAsyncLock);
        lastfmAsyncLockInit = true;
    }
}

void lastfm_process_async(void) {
    if (!lastfmAsyncLockInit || !hasHttpcService) return;
    char npUrl[1536] = "";
    char scUrl[1536] = "";

    LightLock_Lock(&lastfmAsyncLock);
    if (pendingNowPlayingUrl[0] != '\0') {
        snprintf(npUrl, sizeof(npUrl), "%s", pendingNowPlayingUrl);
        pendingNowPlayingUrl[0] = '\0';
    }
    if (pendingScrobbleUrl[0] != '\0') {
        snprintf(scUrl, sizeof(scUrl), "%s", pendingScrobbleUrl);
        pendingScrobbleUrl[0] = '\0';
    }
    LightLock_Unlock(&lastfmAsyncLock);

    if (npUrl[0] != '\0') {
        char resp[256] = {0};
        lastfm_get_json(npUrl, resp, sizeof(resp));
    }
    if (scUrl[0] != '\0') {
        char resp[256] = {0};
        u32 st = lastfm_get_json(scUrl, resp, sizeof(resp));
        if (st != 200) {
            lastfm_enqueue_scrobble(scUrl);
        }
    }
}

void tell_lastfm_im_listening(void) {
    if (!lastfm_has_session || !hasHttpcService) return;
    if (currentTrackTitle[0] == '\0') return;

    char encSk[256] = {0}, encArtist[384] = {0}, encTrack[384] = {0}, encAlbum[384] = {0};
    url_encode_str(lastfm_session_key, encSk, sizeof(encSk));
    url_encode_str(currentTrackArtist[0] ? currentTrackArtist : "Unknown Artist", encArtist, sizeof(encArtist));
    url_encode_str(currentTrackTitle, encTrack, sizeof(encTrack));
    url_encode_str(currentTrackAlbum[0] ? currentTrackAlbum : "", encAlbum, sizeof(encAlbum));

    ensure_lastfm_lock();
    LightLock_Lock(&lastfmAsyncLock);
    snprintf(pendingNowPlayingUrl, sizeof(pendingNowPlayingUrl),
             "%s/lastfm/nowplaying?sk=%s&artist=%s&track=%s&album=%s&duration=%d",
             LASTFM_WORKER_BASE, encSk, encArtist, encTrack, encAlbum, (int)songDuration);
    LightLock_Unlock(&lastfmAsyncLock);
}

void snitch_song_to_lastfm(void) {
    if (!lastfm_has_session) return;
    if (currentTrackTitle[0] == '\0') return;

    char encSk[256] = {0}, encArtist[384] = {0}, encTrack[384] = {0}, encAlbum[384] = {0};
    url_encode_str(lastfm_session_key, encSk, sizeof(encSk));
    url_encode_str(currentTrackArtist[0] ? currentTrackArtist : "Unknown Artist", encArtist, sizeof(encArtist));
    url_encode_str(currentTrackTitle, encTrack, sizeof(encTrack));
    url_encode_str(currentTrackAlbum[0] ? currentTrackAlbum : "", encAlbum, sizeof(encAlbum));

    char url[1536];
    snprintf(url, sizeof(url),
             "%s/lastfm/scrobble?sk=%s&artist=%s&track=%s&album=%s&timestamp=%lu&duration=%d",
             LASTFM_WORKER_BASE, encSk, encArtist, encTrack, encAlbum,
             (unsigned long)lastfm_track_start_ts, (int)songDuration);

    if (!hasHttpcService) {
        lastfm_enqueue_scrobble(url);
        return;
    }
    ensure_lastfm_lock();
    LightLock_Lock(&lastfmAsyncLock);
    snprintf(pendingScrobbleUrl, sizeof(pendingScrobbleUrl), "%s", url);
    LightLock_Unlock(&lastfmAsyncLock);
}

void lastfm_dump_offline_shit(void) {
    if (!hasHttpcService || !lastfm_has_session) return;
    LightLock_Lock(&lastfmLock);
    FILE *f = fopen(LASTFM_QUEUE_PATH, "r");
    if (!f) { LightLock_Unlock(&lastfmLock); return; }

    char line[1536];
    char remaining[8192] = {0};
    size_t remLen = 0;
    bool anyFailed = false;

    while (fgets(line, sizeof(line), f) && !anyFailed) {
        line[strcspn(line, "\r\n")] = '\0';
        if (strncmp(line, "http", 4) != 0) continue;
        LightLock_Unlock(&lastfmLock);
        char resp[256] = {0};
        u32 st = lastfm_get_json(line, resp, sizeof(resp));
        LightLock_Lock(&lastfmLock);
        if (st != 200) {
            size_t l = strlen(line);
            if (remLen + l + 2 < sizeof(remaining)) {
                memcpy(remaining+remLen, line, l);
                remLen += l;
                remaining[remLen++] = '\n';
            }
            anyFailed = true;
        }
    }
    if (anyFailed) {
        while (fgets(line, sizeof(line), f)) {
            line[strcspn(line, "\r\n")] = '\0';
            if (!line[0]) continue;
            size_t l = strlen(line);
            if (remLen + l + 2 < sizeof(remaining)) {
                memcpy(remaining+remLen, line, l);
                remLen += l;
                remaining[remLen++] = '\n';
            }
        }
    }
    fclose(f);

    FILE *fw = fopen(LASTFM_QUEUE_PATH, "w");
    if (fw) {
        if (remLen > 0) fwrite(remaining, 1, remLen, fw);
        fclose(fw);
    }
    LightLock_Unlock(&lastfmLock);
}

void render_lastfm_login_screen(void) {
    C2D_DrawRectSolid(0.0f, 0.0f, CFG_DEPTH_BG_DIMMER + 0.01f,
                      400.0f, 240.0f, C2D_Color32(8, 9, 12, 165));

    if (lfm_qr_ready) {
        draw_qr_code_centered(200.0f, 18.0f);
    } else {
        draw_custom_font_centered(200.0f, 84.0f, 0.42f,
                                  C2D_Color32(255, 255, 255, 255),
                                  "loading Qr code,,,");
    }

    draw_custom_font_centered(200.0f, 166.0f, 0.40f,
                              C2D_Color32(255, 255, 255, 255),
                              "Continue from ur phone!");
    draw_custom_font_centered(200.0f, 188.0f, 0.32f,
                              C2D_Color32(190, 200, 220, 255),
                              "B: Back");

    if (g_lastfm_login_status[0]) {
        u32 stCol = (strncmp(g_lastfm_login_status, "OK", 2) == 0)
                    ? C2D_Color32(80, 255, 140, 255)
                    : C2D_Color32(255, 120, 120, 255);
        draw_custom_font_centered(200.0f, 212.0f, 0.36f, stCol,
                                  g_lastfm_login_status);
    }
}

void lastfm_open_login(void) {
    g_lastfm_login_active    = true;
    g_lastfm_login_done      = false;
    g_lastfm_login_cancelled = false;
    g_lastfm_login_status[0] = '\0';
    lfm_poll_timer           = 0.0f;
    lfm_done_timer           = 0.0f;
    if (!isWifiConnected || !hasHttpcService) {
        lfm_qr_ready = false;
        snprintf(g_lastfm_login_status, sizeof(g_lastfm_login_status),
                 "Sin conexion Wi-Fi");
        return;
    }
    if (!lastfm_start_qr_auth()) {
        snprintf(g_lastfm_login_status, sizeof(g_lastfm_login_status),
                 "Error al generar QR");
    }
}

static void lastfm_json_escape(const char *src, char *dst, size_t dstSz) {
    size_t d = 0;
    for (size_t i = 0; src[i] && d+2 < dstSz; i++) {
        char c = src[i];
        if (c == '"' || c == '\\') dst[d++] = '\\';
        dst[d++] = c;
    }
    dst[d] = '\0';
}

