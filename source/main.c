#include "app_state.h"

u32 __stacksize__ = 256 * 1024;

C3D_Tex customGustinaTex[10];
Tex3DS_SubTexture customGustinaSubtex[10]; //mod shit ok
bool hasCustomGustina[10] = {false};


ndspWaveBuf waveBuf[NUM_WAVE_BUFS];
int16_t* audioBuffer[NUM_WAVE_BUFS];

#define PAUSE_BGM_CHANNEL   1
#define PAUSE_NUM_BUFS      4
#define PAUSE_BUF_FRAMES    4096
ndspWaveBuf pauseBgmWaveBuf[PAUSE_NUM_BUFS];
int16_t* pauseBgmStreamBuf[PAUSE_NUM_BUFS] = {NULL};
int16_t* pauseBgmBuffer = NULL;
u32 pauseBgmTotalFrames = 0;
u16 pauseBgmChannels = 2;
u32 pauseBgmCursor = 0;
bool hasPauseBgm = false;
volatile bool pauseBgmPlaying = false;
float pauseBgmVol = 0.0f;
float pauseBgmLastSentVol = -1.0f;
u32 waveBufSamples[NUM_WAVE_BUFS] = {0};
FILE* currentAudioFile = NULL;
AudioFormat currentFormat = FORMAT_NONE;
volatile uint64_t samplesCompleted = 0;
volatile uint64_t samplesFed = 0;
volatile bool audioStreamEOF = false;
volatile bool trackFinished = false;

#define MAX_PENDING_BEATS 64
#define OSF_LEN 768
float pendingBeatTimes[MAX_PENDING_BEATS];
volatile int beatQueueHead = 0;
volatile int beatQueueTail = 0;
float beatLpBass = 0.0f;
float beatLpMid = 0.0f;
float beatPrevBass = 0.0f;
float beatPrevMid = 0.0f;
float beatAvgEnergy = 0.015f;
float lastDetectedBeatTime = -1.0f;
float nextVisualBeatTime = 0.0f;
bool visualBeatLocked = false;
bool beatSideToggle = false;
bool hasTrackBpmTag = false;
float osfBuf[OSF_LEN];
int osfWriteIdx = 0;
int osfCount = 0;
int osfFramesSinceCalc = 0;
float bpmLastCand = 120.0f;
int bpmCandVotes = 0;


Thread audioThread = NULL;
volatile bool audioThreadRun = true;
LightLock audioLock;
volatile bool isAppSuspended = false;
aptHookCookie appAptHookCookie;

uint8_t* mp3ReadBuffer = NULL;
size_t mp3BufferLen = 0;
size_t mp3BufferPos = 0;
int mp3PcmPos = 0;
int mp3PcmCount = 0;
int mp3PcmChannels = 2;
long mp3DataStartOffset = 0;
long mp3DataSize = 0;

bool isWavOpen = false;

FILE* flacFile = NULL;
OggVorbis_File oggFile;

M4ADemuxer m4aDemuxer;
bool isM4aOpen = false;
NeAACDecHandle aacDec = NULL;
uint8_t aacPacketBuf[16384];
int16_t aacPcmResid[4096 * 2];
int aacPcmPos = 0;
int aacPcmCount = 0;
int aacPcmChannels = 2;
bool isOggOpen = false;
OggOpusFile* opusFile = NULL;

volatile bool isPlaying = false;
volatile bool isPaused = false;
char currentTrackPath[1024] = "";
char currentTrackName[256] = "Select a Song!";
char currentTrackTitle[256] = "";
char currentTrackArtist[256] = "";
char currentTrackAlbum[256] = "";
int currentSampleRate = 44100;
int currentChannels = 2;
float volumeGain = 1.0f;
float volumeLimEnv = 0.0f;
bool headphonesConnected = false;
int headphonePollCounter = 15;
bool bottomWaveThingyMode = false;
int deferredPrevCoverIdx = -1;
int deferredNextCoverIdx = -1;
bool coverCacheDirReady = false; 
volatile float songTime = 0.0f;
float songDuration = 0.0f;
float detectedBPM = 120.0f;
int lastBeat = -1;

float fullSongWaveform[NUM_WAVE_SAMPLES];
int16_t waveScanChunk[512 * 2];

LrcLine lrcLines[MAX_LRC_LINES];
int lrcLineCount = 0;
int currentLrcIndex = -1;
float lyricScrollPos = -1.0f;
bool lyricsEnabled = true;

Thread lyricsThread = NULL;
volatile bool lyricsThreadRun = true;
LightLock lyricsLock;
LyricsFetchJob pendingLyricsJob;
volatile bool hasPendingLyricsJob = false;
volatile bool downloadedLrcReady = false;
volatile uint32_t currentTrackGeneration = 0;
bool hasAcService = false;
bool hasHttpcService = false;
bool isWifiConnected = false;
char lyricsDebugStatus[24] = "";


char   lastfm_session_key[128]  = "";
char   lastfm_username[64]      = "";
bool   lastfm_has_session       = false;
bool   lastfm_scrobbled_track   = false;
float  lastfm_played_seconds    = 0.0f;
u32    lastfm_track_start_ts    = 0;
LightLock lastfmLock;

uint8_t lfm_qrcode[qrcodegen_BUFFER_LEN_FOR_VERSION(10)];
bool    lfm_qr_ready = false;
char    lfm_auth_token[128] = "";
float   lfm_poll_timer = 0.0f;
float   lfm_done_timer = 0.0f;
bool    g_lastfm_login_active   = false;
bool    g_lastfm_login_done     = false;
bool    g_lastfm_login_cancelled= false;
char    g_lastfm_login_status[64] = "";


FileEntry allFileList[MAX_FILES];
int allFileCount = 0;
FileEntry fileList[MAX_FILES];
int fileCount = 0;
int selectedFile = 0;
int scrollOffset = 0;
char currentDir[1024] = "sdmc:/music";
char searchFilter[64] = "";
float uiElapsedTime = 0.0f;

C3D_RenderTarget* topTarget = NULL;
C3D_RenderTarget* botTarget = NULL;
C2D_SpriteSheet spriteSheet;

C2D_SpriteSheet fontSheet1;
C2D_SpriteSheet fontSheet2;
C3D_Tex* fontTex1 = NULL;
C3D_Tex* fontTex2 = NULL;
Tex3DS_SubTexture fontGlyphs[96];
C2D_TextBuf sysTextBuf = NULL;
C2D_TextBuf sysMeasureBuf = NULL;

C3D_Tex coverTextures[4];
C3D_Tex coverBlurTextures[4];
C3D_Tex coverPeekNext2Tex;
C3D_Tex coverPeekPrev2Tex;
bool hasPeekNext2Tex = false;
bool hasPeekPrev2Tex = false;
int peekNext2SongIdx = -1;
int peekPrev2SongIdx = -1;
int deferredNext2CoverIdx = -1;
int deferredPrev2CoverIdx = -1;
Tex3DS_SubTexture cover64Subtex;
Tex3DS_SubTexture coverBlurSubtex;
bool hasCoverTex[4] = {false, false, false, false};
int slotSongIdx[4] = {-1, -1, -1, -1};
uint8_t coverAvgR[4] = {255, 255, 255, 255};
uint8_t coverAvgG[4] = {255, 255, 255, 255};
uint8_t coverAvgB[4] = {255, 255, 255, 255};
int slotCurrent = 0;
int slotPrev = 1;
int slotNext = 2;
int slotOld = 3;

#define coverTexCurrent (coverTextures[slotCurrent])
#define coverTexPrev    (coverTextures[slotPrev])
#define coverTexNext    (coverTextures[slotNext])
#define coverTexOld     (coverTextures[slotOld])
#define hasCoverCurrent (hasCoverTex[slotCurrent])
#define hasCoverPrev    (hasCoverTex[slotPrev])
#define hasCoverNext    (hasCoverTex[slotNext])
#define hasCoverOld     (hasCoverTex[slotOld])
Tex3DS_SubTexture coverSubtex;

uint8_t coverResizeBuf[128 * 128 * 4];


TransitionDir coverTransDir = TRANS_NONE;
float coverTransProgress = 1.0f;
int playingFileIdx = -1;
int coverScrollQueue[8] = {0};
int coverScrollCount = 0;
int coverScrollPos = 0;
TransitionDir coverScrollDir = TRANS_NONE; 
float coverBgPanX = 0.0f;
float coverBgPanY = 0.0f;
float noLyricsFocus = 0.0f;
float longBreakFocus = 0.0f;
float idleDimFocus = 0.0f;
float idleTimer = 0.0f;
float repeatFade = 0.0f;
float speakerVolumeGain = 1.0f;
float headphoneVolumeGain = 1.0f;
bool prevHeadphonesConnected = false;
static bool appStateDirty = false;
static float appStateSaveTimer = 0.0f;

void mark_app_state_dirty(void) {
    appStateDirty = true;
}

void save_app_state(void) {
    mkdir("sdmc:/3ds", 0777);
    mkdir("sdmc:/3ds/MCMP", 0777);
    FILE* f = fopen("sdmc:/3ds/MCMP/state.cfg", "w");
    if (!f) return;
    const char* songName = "";
    if (playingFileIdx >= 0 && playingFileIdx < fileCount && !fileList[playingFileIdx].is_dir) {
        songName = fileList[playingFileIdx].name;
    } else if (selectedFile >= 0 && selectedFile < fileCount && !fileList[selectedFile].is_dir) {
        songName = fileList[selectedFile].name;
    }
    fprintf(f, "dir=%s\n", currentDir);
    fprintf(f, "song=%s\n", songName);
    fprintf(f, "spk_vol=%.2f\n", speakerVolumeGain);
    fprintf(f, "hp_vol=%.2f\n", headphoneVolumeGain);
    fprintf(f, "repeat=%d\n", repeatEnabled ? 1 : 0);
    fprintf(f, "shuffle=%d\n", shuffleEnabled ? 1 : 0);
    fprintf(f, "lyrics=%d\n", lyricsEnabled ? 1 : 0);
    fclose(f);
    appStateDirty = false;
}

static void load_app_state(char* outLastSong, size_t outSize) {
    if (outLastSong && outSize > 0) outLastSong[0] = '\0';
    FILE* f = fopen("sdmc:/3ds/MCMP/state.cfg", "r");
    if (!f) return;
    char line[1280];
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) line[--len] = '\0';
        if (strncmp(line, "dir=", 4) == 0 && line[4] != '\0') {
            DIR* d = opendir(line + 4);
            if (d) {
                closedir(d);
                snprintf(currentDir, sizeof(currentDir), "%s", line + 4);
            }
        } else if (strncmp(line, "song=", 5) == 0 && outLastSong && outSize > 0) {
            snprintf(outLastSong, outSize, "%s", line + 5);
        } else if (strncmp(line, "spk_vol=", 8) == 0) {
            float v = strtof(line + 8, NULL);
            if (v >= 0.10f && v <= 1.0f) speakerVolumeGain = v;
        } else if (strncmp(line, "hp_vol=", 7) == 0) {
            float v = strtof(line + 7, NULL);
            if (v >= 0.10f && v <= 5.0f) headphoneVolumeGain = v;
        } else if (strncmp(line, "repeat=", 7) == 0) {
            repeatEnabled = (atoi(line + 7) != 0);
        } else if (strncmp(line, "shuffle=", 8) == 0) {
            shuffleEnabled = (atoi(line + 8) != 0);
        } else if (strncmp(line, "lyrics=", 7) == 0) {
            lyricsEnabled = (atoi(line + 7) != 0);
        }
    }
    fclose(f);
}
float beatPulseVal = 0.0f;

GustinaState gustinaState = GUSTINA_NOTHING;
int gustinaFrame = 0;
float gustinaFrameTimer = 0.0f;

int global10FpsFrame = 0;
float global10FpsTimer = 0.0f;

float bgScrollX = 0.0f;
float bgScrollY = 0.0f;

int bottomScreenMode = 0;
int wheelTouchSprite = sprites_control1_idx;
bool repeatEnabled = false;
bool shuffleEnabled = false;
int upcomingShuffleIdx = -1;
int cachedNextIdx = -2;
bool cachedNextRepeat = false;
char cachedNextDir[1024] = "";
uint32_t cachedNextGen = 0;
char cachedNextDisplay[256] = "";



static void app_apt_hook(APT_HookType hook, void* param) {
    (void)param;
    switch (hook) {
        case APTHOOK_ONSUSPEND:
        case APTHOOK_ONSLEEP:
            isAppSuspended = true;
            LightLock_Lock(&audioLock);
            LightLock_Unlock(&audioLock);
            break;
        case APTHOOK_ONRESTORE:
        case APTHOOK_ONWAKEUP:
            isAppSuspended = false;
            break;
        case APTHOOK_ONEXIT:
            isAppSuspended = true;
            audioThreadRun = false;
            lyricsThreadRun = false;
            break;
        default:
            break;
    }
}

static void toggle_3ds_screens_pls(bool on) {
    if (R_SUCCEEDED(gspLcdInit())) {
        if (on) GSPLCD_PowerOnBacklight(GSPLCD_SCREEN_BOTH);
        else    GSPLCD_PowerOffBacklight(GSPLCD_SCREEN_BOTH);
        gspLcdExit();
    }
}


static void step_loading_splash(float* animTime, int frames) {
    for (int f = 0; f < frames; f++) {
        *animTime += 1.0f / 60.0f;
        global10FpsTimer += 1.0f / 60.0f;
        if (global10FpsTimer >= 0.10f) {
            global10FpsTimer = 0.0f;
            global10FpsFrame = (global10FpsFrame == 0) ? 1 : 0;
        }
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(topTarget, C2D_Color32(0, 0, 0, 255));
        C2D_TargetClear(botTarget, C2D_Color32(0, 0, 0, 255));
        C2D_SceneBegin(topTarget);
        draw_bouncing_loading_text(12.0f, 206.0f, 0.45f, *animTime, 255);
        C2D_SceneBegin(botTarget);
        C3D_FrameEnd(0);
    }
}

int main(int argc, char** argv) {
    osSetSpeedupEnable(true);
    aptSetSleepAllowed(false);

    bool hasPtmu = R_SUCCEEDED(ptmuInit());
    bool isShellClosed = false;
    int shellPollCounter = 0;
    int wifiPollCounter = 60;

    romfsInit();
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    topTarget = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    botTarget = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    LightLock_Init(&audioLock);
    LightLock_Init(&lyricsLock);
    LightLock_Init(&lastfmLock);
    aptHook(&appAptHookCookie, app_apt_hook, NULL);
    hasAcService = R_SUCCEEDED(acInit());
    hasHttpcService = R_SUCCEEDED(httpcInit(0));
    lastfm_load_cfg();

    spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    init_custom_font();

    float loadingAnimTime = 0.0f;
    step_loading_splash(&loadingAnimTime, 2);

    load_gustina_mod_slop();
    step_loading_splash(&loadingAnimTime, 2);

    if (R_FAILED(ndspInit())) {
        while (aptMainLoop()) {
            hidScanInput();
            if (hidKeysDown() & KEY_START) break;
            C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
            C2D_TargetClear(topTarget, C2D_Color32(20, 20, 28, 255));
            C2D_TargetClear(botTarget, C2D_Color32(20, 20, 28, 255));
            C2D_SceneBegin(topTarget);
            draw_custom_font_centered(200.0f, 84.0f, 0.45f, C2D_Color32(255, 110, 110, 255), "ndspInit() failed!");
            draw_custom_font_centered(200.0f, 112.0f, 0.38f, C2D_Color32(255, 255, 255, 255), "Missing sdmc:/3ds/dspfirm.cdc");
            draw_custom_font_centered(200.0f, 142.0f, 0.36f, C2D_Color32(180, 180, 195, 255), "Press START to exit");
            C2D_SceneBegin(botTarget);
            C3D_FrameEnd(0);
        }
        return 1;
    }

    ndspSetOutputMode(NDSP_OUTPUT_STEREO);
    init_pause_bgm();
    step_loading_splash(&loadingAnimTime, 2);

    for (int i = 0; i < NUM_WAVE_BUFS; i++) {
        audioBuffer[i] = (int16_t*)linearAlloc(STREAM_BUF_SIZE);
        memset(&waveBuf[i], 0, sizeof(ndspWaveBuf));
        waveBuf[i].data_vaddr = audioBuffer[i];
        waveBuf[i].status = NDSP_WBUF_DONE;
    }
    mp3ReadBuffer = (uint8_t*)malloc(FILE_READ_CHUNK);

    audioThreadRun = true;
    audioThread = threadCreate(audio_thread_entry, NULL, 48 * 1024, 0x2E, -2, false);

    lyricsThreadRun = true;
    lyricsThread = threadCreate(fnf_lyrics_slave, NULL, 16 * 1024, 0x3A, -1, false);

    char lastSongName[512] = "";
    load_app_state(lastSongName, sizeof(lastSongName));
    repeatFade = repeatEnabled ? 1.0f : 0.0f;
    {
        bool hp = false;
        if (R_SUCCEEDED(DSP_GetHeadphoneStatus(&hp))) {
            headphonesConnected = hp;
        }
        prevHeadphonesConnected = headphonesConnected;
        volumeGain = headphonesConnected ? headphoneVolumeGain : speakerVolumeGain;
    }

    load_directory(currentDir);
    if (lastSongName[0] != '\0') {
        for (int i = 0; i < fileCount; i++) {
            if (!fileList[i].is_dir && strcmp(fileList[i].name, lastSongName) == 0) {
                selectedFile = i;
                if (selectedFile >= CFG_LIST_VISIBLE_ROWS) {
                    scrollOffset = selectedFile - (CFG_LIST_VISIBLE_ROWS / 2);
                    int maxScroll = fileCount - CFG_LIST_VISIBLE_ROWS;
                    if (scrollOffset > maxScroll) scrollOffset = maxScroll;
                    if (scrollOffset < 0) scrollOffset = 0;
                }
                break;
            }
        }
    }
    step_loading_splash(&loadingAnimTime, 2);
    refresh_the_3_covers();
    step_loading_splash(&loadingAnimTime, 18);

    float startupFade = 1.0f;
    
    int volHoldDir = 0;
    float volHoldTime = 0.0f;
    float volRepeatTimer = 0.0f;
    int trackHoldDir = 0;
    float trackHoldTime = 0.0f;
    float trackSeekTimer = 0.0f;
    bool trackDidSeek = false;

    int listHoldDir = 0;
    float listHoldTime = 0.0f;
    float listRepeatTimer = 0.0f;

    u64 lastTick = osGetTime();

    while (aptMainLoop()) {
        
        if (++headphonePollCounter >= 15) {
            headphonePollCounter = 0;
            bool hp = false;
            if (R_SUCCEEDED(DSP_GetHeadphoneStatus(&hp))) {
                if (hp != prevHeadphonesConnected) {
                    if (prevHeadphonesConnected) headphoneVolumeGain = volumeGain;
                    else speakerVolumeGain = volumeGain;
                    headphonesConnected = hp;
                    prevHeadphonesConnected = hp;
                    volumeGain = headphonesConnected ? headphoneVolumeGain : speakerVolumeGain;
                    appStateDirty = true;
                } else {
                    headphonesConnected = hp;
                }
            }
            if (!headphonesConnected && volumeGain > 1.0f) {
                volumeGain = 1.0f;
                speakerVolumeGain = 1.0f;
            }
        }

        if (hasPtmu && (++shellPollCounter >= 10 || isShellClosed)) {
            shellPollCounter = 0;
            u8 shellOpen = 1;
            if (R_SUCCEEDED(PTMU_GetShellState(&shellOpen))) {
                if (shellOpen == 0 && !isShellClosed) {
                    isShellClosed = true;
                    toggle_3ds_screens_pls(false);
                } else if (shellOpen == 1 && isShellClosed) {
                    isShellClosed = false;
                    toggle_3ds_screens_pls(true);
                    lastTick = osGetTime();
                }
            }
        }

        if (trackFinished) {
            trackFinished = false;
            if (repeatEnabled) {
                play_track_by_index(selectedFile, TRANS_NONE);
            } else if (shuffleEnabled) {
                int rIdx = (upcomingShuffleIdx >= 0 && upcomingShuffleIdx < fileCount && !fileList[upcomingShuffleIdx].is_dir)
                           ? upcomingShuffleIdx : find_random_song_index(selectedFile);
                upcomingShuffleIdx = -1;
                if (rIdx >= 0) play_track_by_index(rIdx, TRANS_NEXT);
            } else {
                int nIdx = find_next_song_index(selectedFile, 1);
                if (nIdx >= 0) play_track_by_index(nIdx, TRANS_NEXT);
            }
            cachedNextIdx = -2;
            lastTick = osGetTime();
        }

        if (isShellClosed) {
            update_audio_stream_fallback();
            svcSleepThread(25 * 1000 * 1000LL);
            continue;
        }

        if (hasAcService && ++wifiPollCounter >= 60) {
            wifiPollCounter = 0;
            u32 wifiStatus = 0;
            bool prevWifi = isWifiConnected;
            isWifiConnected = (R_SUCCEEDED(ACU_GetStatus(&wifiStatus)) && wifiStatus == 3);
            if (isWifiConnected && !prevWifi && lastfm_has_session) {
                lastfm_dump_offline_shit();
            }
        }

        hidScanInput();
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();
        touchPosition touch;
        hidTouchRead(&touch);

        if (kDown & KEY_START) break;

        u64 currentTick = osGetTime();
        float dt = (float)(currentTick - lastTick) / 1000.0f;
        lastTick = currentTick;
        if (dt > 0.05f) dt = 0.05f;
        uiElapsedTime += dt;
        update_pause_bgm(dt);

        if (kDown != 0 || kHeld != 0 || touch.px != 0 || touch.py != 0 || !isPlaying || g_lastfm_login_active || bottomScreenMode != 0) {
            idleTimer = 0.0f;
        } else {
            idleTimer += dt;
        }

        bool reloadOnlineLrc = false;
        LightLock_Lock(&lyricsLock);
        if (downloadedLrcReady) {
            downloadedLrcReady = false;
            reloadOnlineLrc = true;
        }
        LightLock_Unlock(&lyricsLock);
        if (reloadOnlineLrc && isPlaying) {
            load_lrc_file(currentTrackPath);
            currentLrcIndex = -1;
            lyricScrollPos = -1.0f;
        }

        bgScrollX += CFG_BOT_BG_SCROLL_SPD_X * dt;
        bgScrollY += CFG_BOT_BG_SCROLL_SPD_Y * dt;

        coverBgPanX = sinf(songTime * CFG_BG_PAN_SPEED_X) * CFG_BG_PAN_X_AMP;
        coverBgPanY = cosf(songTime * CFG_BG_PAN_SPEED_Y) * CFG_BG_PAN_Y_AMP;

        if (coverTransDir != TRANS_NONE) {
            float spd = (coverScrollCount > 0 && coverScrollPos + 1 < coverScrollCount)
                ? (CFG_TRANS_SPEED * 3.8f)
                : CFG_TRANS_SPEED;
            coverTransProgress += dt * spd;
            if (coverTransProgress >= 1.0f) {
                if (coverScrollPos + 1 < coverScrollCount) {
                    coverScrollPos++;
                    bool isFly = (coverScrollPos + 1 < coverScrollCount);
                    advance_cover_slot_step(coverScrollQueue[coverScrollPos], coverScrollDir, isFly);
                } else {
                    coverScrollCount = 0;
                    coverScrollPos = 0;
                    coverTransProgress = 1.0f;
                    coverTransDir = TRANS_NONE;
                }
            }
        }
        global10FpsTimer += dt;
        if (global10FpsTimer >= 0.10f) {
            global10FpsTimer = 0.0f;
            global10FpsFrame = (global10FpsFrame == 0) ? 1 : 0;
        }
        if (isPlaying && !isPaused) {
            songTime += dt;
            if (currentSampleRate > 0) {
                float audioTime = (float)((double)samplesCompleted / (double)currentSampleRate);
                float diff = songTime - audioTime;
                if (diff > 0.15f || diff < -0.05f) {
                    songTime = audioTime;
                }
            }
            if (songDuration > 0.0f && songTime > songDuration) {
                songTime = songDuration;
            }
        }
        if (isPlaying && !isPaused && !isAppSuspended && lastfm_has_session) {
            lastfm_played_seconds += dt;
            if (!lastfm_scrobbled_track &&
                lastfm_played_seconds >= 30.0f &&
                (songDuration <= 0.0f ||
                 lastfm_played_seconds >= songDuration * 0.5f ||
                 lastfm_played_seconds >= 240.0f)) {
                snitch_song_to_lastfm();
                lastfm_scrobbled_track = true;
            }
        }
        if (isPlaying && !isPaused) {
            float beatPeriod = (detectedBPM > 40.0f) ? (60.0f / detectedBPM) : 0.5f;
            while (beatQueueHead != beatQueueTail) {
                float bt = pendingBeatTimes[beatQueueHead];
                if (bt > songTime) break;
                beatQueueHead = (beatQueueHead + 1) % MAX_PENDING_BEATS;

                if (!visualBeatLocked) {
                    nextVisualBeatTime = bt;
                    visualBeatLocked = true;
                } else {
                    float err = bt - nextVisualBeatTime;
                    while (err > beatPeriod * 0.5f) err -= beatPeriod;
                    while (err < -beatPeriod * 0.5f) err += beatPeriod;
                    if (fabsf(err) < beatPeriod * 0.30f) {
                        nextVisualBeatTime += err * 0.25f;
                    }
                }
            }

            if (!visualBeatLocked && songTime >= 0.35f) {
                nextVisualBeatTime = songTime;
                visualBeatLocked = true;
            }
            if (visualBeatLocked && songTime >= nextVisualBeatTime) {
                if ((songTime - nextVisualBeatTime) > beatPeriod * 2.0f) {
                    nextVisualBeatTime = songTime + beatPeriod;
                } else {
                    nextVisualBeatTime += beatPeriod;
                }
                beatSideToggle = !beatSideToggle;
                gustinaState = beatSideToggle ? GUSTINA_BOP_RIGHT : GUSTINA_BOP_LEFT;
                gustinaFrame = 0;
                gustinaFrameTimer = 0.0f;
                beatPulseVal = 1.0f;
            }
        }
        if (beatPulseVal > 0.0f) {
            beatPulseVal -= dt * 3.2f;
            if (beatPulseVal < 0.0f) beatPulseVal = 0.0f;
        }
        {
            bool targetEmptyLyrics = (isPlaying && lyricsEnabled && lrcLineCount > 0 &&
                                      currentLrcIndex >= 0 && lrcLines[currentLrcIndex].text[0] == '\0');
            bool targetLongBreak = false;
            if (targetEmptyLyrics) {
                float startEmptyTime = lrcLines[currentLrcIndex].timestamp;
                for (int k = currentLrcIndex - 1; k >= 0; k--) {
                    if (lrcLines[k].text[0] == '\0') {
                        startEmptyTime = lrcLines[k].timestamp;
                    } else {
                        break;
                    }
                }
                float nextTextTime = (songDuration > 0.0f) ? songDuration : (startEmptyTime + 20.0f);
                for (int k = currentLrcIndex + 1; k < lrcLineCount; k++) {
                    if (lrcLines[k].text[0] != '\0') {
                        nextTextTime = lrcLines[k].timestamp;
                        break;
                    }
                }
                if ((nextTextTime - startEmptyTime) > 15.0f) {
                    targetLongBreak = true;
                }
            }
            bool targetIdleDim = (idleTimer >= 30.0f && isPlaying && !g_lastfm_login_active && bottomScreenMode == 0 && !targetLongBreak);
            bool targetNoLyrics = (targetEmptyLyrics || targetIdleDim);
            float fTarget = targetNoLyrics ? 1.0f : 0.0f;
            if (noLyricsFocus < fTarget) {
                noLyricsFocus += dt * 2.4f;
                if (noLyricsFocus > fTarget) noLyricsFocus = fTarget;
            } else if (noLyricsFocus > fTarget) {
                noLyricsFocus -= dt * 2.8f;
                if (noLyricsFocus < fTarget) noLyricsFocus = fTarget;
            }
            float lTarget = targetLongBreak ? 1.0f : 0.0f;
            if (longBreakFocus < lTarget) {
                longBreakFocus += dt * 0.9f;
                if (longBreakFocus > lTarget) longBreakFocus = lTarget;
            } else if (longBreakFocus > lTarget) {
                longBreakFocus -= dt * 1.4f;
                if (longBreakFocus < lTarget) longBreakFocus = lTarget;
            }
            float iTarget = targetIdleDim ? 1.0f : 0.0f;
            if (idleDimFocus < iTarget) {
                idleDimFocus += dt * 1.4f;
                if (idleDimFocus > iTarget) idleDimFocus = iTarget;
            } else if (idleDimFocus > iTarget) {
                idleDimFocus -= dt * 2.8f;
                if (idleDimFocus < iTarget) idleDimFocus = iTarget;
            }
            float rTarget = repeatEnabled ? 1.0f : 0.0f;
            if (repeatFade < rTarget) {
                repeatFade += dt * 4.5f;
                if (repeatFade > rTarget) repeatFade = rTarget;
            } else if (repeatFade > rTarget) {
                repeatFade -= dt * 4.5f;
                if (repeatFade < rTarget) repeatFade = rTarget;
            }
        }
        if (gustinaState == GUSTINA_BOP_LEFT || gustinaState == GUSTINA_BOP_RIGHT) { //shit
            gustinaFrameTimer += dt;
            if (gustinaFrameTimer >= CFG_GUSTINA_FRAME_TIME) {
                gustinaFrameTimer = 0.0f;
                gustinaFrame++;
                if (gustinaFrame > 2) {
                    gustinaState = GUSTINA_IDLE;
                    gustinaFrame = 0;
                }
            }
        }
        if (lrcLineCount > 0 && isPlaying) {
            int oldLrc = currentLrcIndex;
            for (int i = 0; i < lrcLineCount; i++) {
                if (songTime >= lrcLines[i].timestamp) {
                    currentLrcIndex = i;
                } else {
                    break;
                }
            }
            if (currentLrcIndex != oldLrc && currentLrcIndex >= 0) {
                if (lyricScrollPos < 0.0f || oldLrc < 0 || abs(currentLrcIndex - oldLrc) > 2) {
                    lyricScrollPos = (float)currentLrcIndex;
                }
            }
        }
        if (currentLrcIndex >= 0 && lyricScrollPos >= 0.0f) {
            float target = (float)currentLrcIndex;
            float diff = target - lyricScrollPos;
            if (fabsf(diff) < 0.002f) {
                lyricScrollPos = target;
            } else {
                float factor = 1.0f - expf(-dt * 9.0f);
                lyricScrollPos += diff * factor;
            }
        }
        if (kDown & KEY_X) {
            lyricsEnabled = !lyricsEnabled;
        }
        if (kDown & KEY_Y) {
            bottomScreenMode = (bottomScreenMode == 0) ? 1 : 0;
            volHoldDir = 0;
            trackHoldDir = 0;
            trackDidSeek = false;
            listHoldDir = 0;
            listHoldTime = 0.0f;
            listRepeatTimer = 0.0f;
        }
        wheelTouchSprite = sprites_control1_idx; 
        if (bottomScreenMode == 0) {
            int curVolDir = 0;
            int curTrackDir = 0;

            if (kDown & KEY_R) bottomWaveThingyMode = !bottomWaveThingyMode;

            if (kHeld & KEY_A) {
                wheelTouchSprite = sprites_controlPause_idx;
            }
            if (kDown & KEY_A) {
                if (isPlaying) {
                    isPaused = !isPaused;
                    ndspChnSetPaused(AUDIO_CHANNEL, isPaused);
                } else if (fileCount > 0) {
                    int startIdx = fileList[selectedFile].is_dir ? find_next_song_index(selectedFile, 1) : selectedFile;
                    if (startIdx >= 0) {
                        play_track_by_index(startIdx, TRANS_NONE);
                        lastTick = osGetTime();
                    }
                }
            }

            if (kHeld & KEY_DUP) {
                wheelTouchSprite = sprites_controlVolumeUp_idx;
                curVolDir = 1;
            } else if (kHeld & KEY_DDOWN) {
                wheelTouchSprite = sprites_controlVolumeDown_idx;
                curVolDir = -1;
            }
            if (kHeld & KEY_DLEFT) {
                wheelTouchSprite = sprites_controlBack_idx;
                curTrackDir = -1;
            } else if (kHeld & KEY_DRIGHT) {
                wheelTouchSprite = sprites_controlNextSong_idx;
                curTrackDir = 1;
            }

            if (kHeld & KEY_TOUCH) {
                if (bottomWaveThingyMode && touch.py >= 198 && touch.py <= 240 && touch.px >= 8 && touch.px <= 312) {
                    if (isPlaying && songDuration > 0.0f) {
                        float pct = (float)(touch.px - 14) / 292.0f;
                        if (pct < 0.0f) pct = 0.0f;
                        if (pct > 1.0f) pct = 1.0f;
                        float targetSec = pct * songDuration;
                        if ((kDown & KEY_TOUCH) || fabsf(targetSec - songTime) > 0.45f) {
                            seek_current_track(targetSec);
                            lastTick = osGetTime();
                        }
                    }
                } else {
                float wx = CFG_WHEEL_CENTER_X; 
                float wy = CFG_WHEEL_CENTER_Y;
                float dx = (float)touch.px - wx;
                float dy = (float)touch.py - wy;
                float dist = sqrtf(dx*dx + dy*dy);

                if (dist <= CFG_WHEEL_INNER_RADIUS) {
                    
                    wheelTouchSprite = sprites_controlPause_idx;
                    if (kDown & KEY_TOUCH) {
                        if (isPlaying) {
                            isPaused = !isPaused;
                            ndspChnSetPaused(AUDIO_CHANNEL, isPaused);
                        } else if (fileCount > 0) {
                            int startIdx = fileList[selectedFile].is_dir ? find_next_song_index(selectedFile, 1) : selectedFile;
                            if (startIdx >= 0) {
                                play_track_by_index(startIdx, TRANS_NONE);
                                lastTick = osGetTime();
                            }
                        }
                    }
                } else if (dist <= CFG_WHEEL_OUTER_RADIUS) {
                    float angle = atan2f(dy, dx) * (180.0f / 3.14159265f);
                    if (angle < 0) angle += 360.0f;

                    if (angle >= 45.0f && angle < 135.0f) {
                        
                        wheelTouchSprite = sprites_controlVolumeDown_idx;
                        curVolDir = -1;
                    } else if (angle >= 135.0f && angle < 225.0f) {
                        
                        wheelTouchSprite = sprites_controlBack_idx;
                        curTrackDir = -1;
                    } else if (angle >= 225.0f && angle < 315.0f) {
                        
                        wheelTouchSprite = sprites_controlVolumeUp_idx;
                        curVolDir = 1;
                    } else {
                        
                        wheelTouchSprite = sprites_controlNextSong_idx;
                        curTrackDir = 1;
                    }
                }
                }
            }
            if (curVolDir != 0) {
                float maxVolGain = headphonesConnected ? 5.0f : 1.0f;
                if (volHoldDir != curVolDir) {
                    volHoldDir = curVolDir;
                    volHoldTime = 0.0f;
                    volRepeatTimer = 0.0f;
                    volumeGain += (float)curVolDir * 0.10f;
                    if (volumeGain < 0.10f) volumeGain = 0.10f;
                    if (volumeGain > maxVolGain) volumeGain = maxVolGain;
                    if (headphonesConnected) headphoneVolumeGain = volumeGain; else speakerVolumeGain = volumeGain;
                    appStateDirty = true;
                } else {
                    volHoldTime += dt;
                    if (volHoldTime >= 0.28f) {
                        volRepeatTimer += dt;
                        while (volRepeatTimer >= 0.05f) {
                            volRepeatTimer -= 0.05f;
                            volumeGain += (float)curVolDir * 0.05f;
                            if (volumeGain < 0.10f) volumeGain = 0.10f;
                            if (volumeGain > maxVolGain) volumeGain = maxVolGain;
                            if (headphonesConnected) headphoneVolumeGain = volumeGain; else speakerVolumeGain = volumeGain;
                            appStateDirty = true;
                        }
                    }
                }
            } else {
                volHoldDir = 0;
                volHoldTime = 0.0f;
                volRepeatTimer = 0.0f;
            }
            if (curTrackDir != 0) {
                if (trackHoldDir != curTrackDir) {
                    trackHoldDir = curTrackDir;
                    trackHoldTime = 0.0f;
                    trackSeekTimer = 0.0f;
                    trackDidSeek = false;
                } else {
                    trackHoldTime += dt;
                    if (trackHoldTime >= 0.35f && isPlaying) {
                        if (!trackDidSeek) {
                            trackDidSeek = true;
                            trackSeekTimer = 0.12f; 
                        } else {
                            trackSeekTimer += dt;
                        }
                        if (trackSeekTimer >= 0.12f) {
                            trackSeekTimer = 0.0f;
                            seek_current_track(songTime + ((float)curTrackDir * 2.0f));
                            lastTick = osGetTime();
                        }
                    }
                }
            } else if (trackHoldDir != 0) {
                if (!trackDidSeek) {
                    if (trackHoldDir < 0) {
                        int prevSong = find_next_song_index(selectedFile, -1);
                        if (prevSong >= 0) {
                            play_track_by_index(prevSong, TRANS_PREV);
                            lastTick = osGetTime();
                        }
                    } else {
                        int nextSong = -1;
                        if (shuffleEnabled) {
                            nextSong = (upcomingShuffleIdx >= 0 && upcomingShuffleIdx < fileCount && !fileList[upcomingShuffleIdx].is_dir)
                                       ? upcomingShuffleIdx : find_random_song_index(selectedFile);
                            upcomingShuffleIdx = -1;
                        } else {
                            nextSong = find_next_song_index(selectedFile, 1);
                        }
                        if (nextSong >= 0) {
                            play_track_by_index(nextSong, TRANS_NEXT);
                            cachedNextIdx = -2;
                            lastTick = osGetTime();
                        }
                    }
                }
                trackHoldDir = 0;
                trackHoldTime = 0.0f;
                trackSeekTimer = 0.0f;
                trackDidSeek = false;
            }
            if (kDown & KEY_TOUCH) {
                float dxRep = (float)touch.px - CFG_REPEAT_ICON_X;
                float dyRep = (float)touch.py - CFG_REPEAT_ICON_Y;
                float dxShf = (float)touch.px - CFG_SHUFFLE_ICON_X;
                float dyShf = (float)touch.py - CFG_SHUFFLE_ICON_Y;

                if (sqrtf(dxRep * dxRep + dyRep * dyRep) <= CFG_MODE_BTN_HIT_RADIUS) {
                    repeatEnabled = !repeatEnabled;
                    cachedNextIdx = -2;
                } else if (sqrtf(dxShf * dxShf + dyShf * dyShf) <= CFG_MODE_BTN_HIT_RADIUS) {
                    shuffleEnabled = !shuffleEnabled;
                    upcomingShuffleIdx = shuffleEnabled ? find_random_song_index(selectedFile) : -1;
                    cachedNextIdx = -2;
                } else if (touch.py < 24 && touch.px < 88) {
                    bottomScreenMode = 1;
                } else if (touch.py < 24 && touch.px >= 88 && touch.px < 230) {
                    lyricsEnabled = !lyricsEnabled;
                } else if (touch.py < 24 && touch.px >= 230) {
                    bottomWaveThingyMode = !bottomWaveThingyMode;
                } else if (!bottomWaveThingyMode && touch.px >= 248 && touch.py >= 195) {
                    if (g_lastfm_login_active) {
                        g_lastfm_login_active = false;
                        g_lastfm_login_cancelled = true;
                    } else {
                        lastfm_open_login();
                    }
                }
            }
        } else {
            if (kDown & KEY_SELECT) {
                open_3ds_keyboard_thingy(&lastTick);
            }
            int curListDir = 0;
            if (kHeld & KEY_DDOWN) curListDir = 1;
            else if (kHeld & KEY_DUP) curListDir = -1;

            if (curListDir != 0) {
                bool moveStep = false;
                if (listHoldDir != curListDir) {
                    listHoldDir = curListDir;
                    listHoldTime = 0.0f;
                    listRepeatTimer = 0.0f;
                    moveStep = true;
                } else {
                    listHoldTime += dt;
                    if (listHoldTime >= 0.30f) {
                        listRepeatTimer += dt;
                        if (listRepeatTimer >= 0.075f) {
                            listRepeatTimer = 0.0f;
                            moveStep = true;
                        }
                    }
                }
                if (moveStep && fileCount > 0) {
                    selectedFile += curListDir;
                    if (selectedFile < 0) selectedFile = 0;
                    if (selectedFile >= fileCount) selectedFile = fileCount - 1;
                    if (selectedFile < scrollOffset) scrollOffset = selectedFile;
                    if (selectedFile >= scrollOffset + CFG_LIST_VISIBLE_ROWS) scrollOffset = selectedFile - CFG_LIST_VISIBLE_ROWS + 1;
                }
            } else {
                listHoldDir = 0;
                listHoldTime = 0.0f;
                listRepeatTimer = 0.0f;
            }
            if ((kDown & (KEY_DLEFT | KEY_L)) && fileCount > 0) {
                selectedFile -= CFG_LIST_VISIBLE_ROWS;
                if (selectedFile < 0) selectedFile = 0;
                if (selectedFile < scrollOffset) scrollOffset = selectedFile;
            }
            if ((kDown & (KEY_DRIGHT | KEY_R)) && fileCount > 0) {
                selectedFile += CFG_LIST_VISIBLE_ROWS;
                if (selectedFile >= fileCount) selectedFile = fileCount - 1;
                if (selectedFile >= scrollOffset + CFG_LIST_VISIBLE_ROWS) scrollOffset = selectedFile - CFG_LIST_VISIBLE_ROWS + 1;
            }

            bool triggerOpen = (kDown & KEY_A) != 0;
            if (kDown & KEY_TOUCH) {
                if (touch.px >= 224 && touch.py <= 30) {
                    open_3ds_keyboard_thingy(&lastTick);
                } else if (touch.px <= 96 && touch.py >= 212) {
                    bottomScreenMode = 0;
                } else if (touch.px >= (int)CFG_LIST_BOX_X && touch.px <= (int)(CFG_LIST_BOX_X + CFG_LIST_BOX_WIDTH)) {
                    float topY = CFG_LIST_ROW_START_Y - 8.0f;
                    float botY = topY + (float)CFG_LIST_VISIBLE_ROWS * CFG_LIST_ROW_HEIGHT;
                    if ((float)touch.py >= topY && (float)touch.py < botY) {
                        int clickedRow = (int)(((float)touch.py - topY) / CFG_LIST_ROW_HEIGHT);
                        int targetIdx = scrollOffset + clickedRow;
                        if (targetIdx >= 0 && targetIdx < fileCount) {
                            if (selectedFile == targetIdx) {
                                triggerOpen = true;
                            } else {
                                selectedFile = targetIdx;
                            }
                        }
                    }
                }
            }
            if ((kHeld & KEY_TOUCH) && fileCount > CFG_LIST_VISIBLE_ROWS) {
                if (touch.px >= 210 && touch.px <= 232 && touch.py >= 34 && touch.py <= 208) {
                    float barTop = CFG_LIST_ROW_START_Y - 6.0f;
                    float barH = (float)CFG_LIST_VISIBLE_ROWS * CFG_LIST_ROW_HEIGHT - 4.0f;
                    float frac = ((float)touch.py - barTop) / barH;
                    if (frac < 0.0f) frac = 0.0f;
                    if (frac > 1.0f) frac = 1.0f;
                    int maxScroll = fileCount - CFG_LIST_VISIBLE_ROWS;
                    scrollOffset = (int)(frac * (float)maxScroll + 0.5f);
                    if (scrollOffset < 0) scrollOffset = 0;
                    if (scrollOffset > maxScroll) scrollOffset = maxScroll;
                    if (selectedFile < scrollOffset) selectedFile = scrollOffset;
                    if (selectedFile >= scrollOffset + CFG_LIST_VISIBLE_ROWS) selectedFile = scrollOffset + CFG_LIST_VISIBLE_ROWS - 1;
                }
            }
            if (triggerOpen && fileCount > 0) {
                if (fileList[selectedFile].is_dir) {
                    if (strcmp(fileList[selectedFile].name, "..") == 0) {
                        char* s = strrchr(currentDir, '/');
                        if (s && s != currentDir + 5) *s = '\0';
                        else strcpy(currentDir, "sdmc:/");
                    } else {
                        char temp[1024];
                        if (strcmp(currentDir, "sdmc:/") == 0) snprintf(temp, sizeof(temp), "sdmc:/%s", fileList[selectedFile].name);
                        else snprintf(temp, sizeof(temp), "%s/%s", currentDir, fileList[selectedFile].name);
                        strncpy(currentDir, temp, sizeof(currentDir)-1);
                        currentDir[sizeof(currentDir)-1] = '\0';
                    }
                    load_directory(currentDir);
                    playingFileIdx = -1;
                    coverScrollCount = 0;
                    refresh_the_3_covers();
                } else {
                    TransitionDir pickDir = (playingFileIdx >= 0 && selectedFile < playingFileIdx) ? TRANS_PREV : TRANS_NEXT;
                    play_track_by_index(selectedFile, pickDir);
                    lastTick = osGetTime();
                    bottomScreenMode = 0;
                }
            }
            if (kDown & KEY_B) {
                bottomScreenMode = 0;
            }
        }
        update_audio_stream_fallback();
        if (sysTextBuf) C2D_TextBufClear(sysTextBuf);
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        if (g_lastfm_login_active) {
            if (kDown & KEY_B) {
                g_lastfm_login_active    = false;
                g_lastfm_login_cancelled = true;
            } else if (g_lastfm_login_done) {
                lfm_done_timer += dt;
                if (lfm_done_timer >= 1.5f) {
                    g_lastfm_login_active = false;
                }
            } else if (lfm_qr_ready) {
                lfm_poll_timer += dt;
                if (lfm_poll_timer >= 2.5f) {
                    lfm_poll_timer = 0.0f;
                    int p = lastfm_poll_qr_auth();
                    if (p < 0) {
                        snprintf(g_lastfm_login_status, sizeof(g_lastfm_login_status),
                                 "Error de autorizacion");
                    }
                }
            }
        }

        render_top_screen();
        render_bottom_screen();
        if (startupFade > 0.0f) {
            loadingAnimTime += dt;
            u8 fadeAlpha = (u8)(startupFade * 255.0f);
            C2D_SceneBegin(topTarget);
            C2D_DrawRectSolid(0.0f, 0.0f, 0.98f, 400.0f, 240.0f, C2D_Color32(0, 0, 0, fadeAlpha));
            draw_bouncing_loading_text(12.0f, 206.0f, 0.45f, loadingAnimTime, fadeAlpha);
            C2D_SceneBegin(botTarget);
            C2D_DrawRectSolid(0.0f, 0.0f, 0.98f, 320.0f, 240.0f, C2D_Color32(0, 0, 0, fadeAlpha));
            startupFade -= dt * 2.8f;
            if (startupFade < 0.0f) startupFade = 0.0f;
        }
        C3D_FrameEnd(0);

        if (coverTransDir == TRANS_NONE && coverScrollCount == 0) {
            static int deferredCooldown = 0;
            if (deferredCooldown > 0) {
                deferredCooldown--;
            } else if (deferredNextCoverIdx >= 0 && deferredNextCoverIdx < fileCount) {
                int idx = deferredNextCoverIdx;
                deferredNextCoverIdx = -1;
                if (!fileList[idx].is_dir && (slotSongIdx[slotNext] != idx || !hasCoverNext)) {
                    static char fpNext[1024];
                    snprintf(fpNext, sizeof(fpNext), "%s/%s", currentDir, fileList[idx].name);
                    load_song_cover_tex(fpNext, &coverTexNext, &hasCoverNext, 255, 60, 180);
                    slotSongIdx[slotNext] = idx;
                    lastTick = osGetTime();
                    deferredCooldown = 4;
                }
            } else if (deferredPrevCoverIdx >= 0 && deferredPrevCoverIdx < fileCount) {
                int idx = deferredPrevCoverIdx;
                deferredPrevCoverIdx = -1;
                if (!fileList[idx].is_dir && (slotSongIdx[slotPrev] != idx || !hasCoverPrev)) {
                    static char fpPrev[1024];
                    snprintf(fpPrev, sizeof(fpPrev), "%s/%s", currentDir, fileList[idx].name);
                    load_song_cover_tex(fpPrev, &coverTexPrev, &hasCoverPrev, 255, 120, 0);
                    slotSongIdx[slotPrev] = idx;
                    lastTick = osGetTime();
                    deferredCooldown = 4;
                }
            } else if (deferredNext2CoverIdx >= 0 && deferredNext2CoverIdx < fileCount) {
                int idx = deferredNext2CoverIdx;
                deferredNext2CoverIdx = -1;
                if (!fileList[idx].is_dir && (peekNext2SongIdx != idx || !hasPeekNext2Tex)) {
                    static char fpN2[1024];
                    snprintf(fpN2, sizeof(fpN2), "%s/%s", currentDir, fileList[idx].name);
                    load_song_cover_lowres(fpN2, &coverPeekNext2Tex, &hasPeekNext2Tex, 255, 60, 180);
                    peekNext2SongIdx = idx;
                    lastTick = osGetTime();
                    deferredCooldown = 4;
                }
            } else if (deferredPrev2CoverIdx >= 0 && deferredPrev2CoverIdx < fileCount) {
                int idx = deferredPrev2CoverIdx;
                deferredPrev2CoverIdx = -1;
                if (!fileList[idx].is_dir && (peekPrev2SongIdx != idx || !hasPeekPrev2Tex)) {
                    static char fpP2[1024];
                    snprintf(fpP2, sizeof(fpP2), "%s/%s", currentDir, fileList[idx].name);
                    load_song_cover_lowres(fpP2, &coverPeekPrev2Tex, &hasPeekPrev2Tex, 255, 120, 0);
                    peekPrev2SongIdx = idx;
                    lastTick = osGetTime();
                    deferredCooldown = 4;
                }
            }
        }
        if (appStateDirty) {
            appStateSaveTimer += dt;
            if (appStateSaveTimer >= 2.0f) {
                appStateSaveTimer = 0.0f;
                save_app_state();
            }
        }
    }

    save_app_state();
    isAppSuspended = true;
    lyricsThreadRun = false;
    bool lyricsJoined = true;
    if (lyricsThread) {
        if (R_SUCCEEDED(threadJoin(lyricsThread, 3000000000ULL))) {
            threadFree(lyricsThread);
        } else {
            lyricsJoined = false;
        }
        lyricsThread = NULL;
    }

    audioThreadRun = false;
    if (audioThread) {
        if (R_SUCCEEDED(threadJoin(audioThread, 2000000000ULL))) {
            threadFree(audioThread);
        }
        audioThread = NULL;
    }

    stop_playback();

    for (int i = 0; i < NUM_WAVE_BUFS; i++) {
        if (audioBuffer[i]) linearFree(audioBuffer[i]);
    }
    if (hasPauseBgm) {
        ndspChnWaveBufClear(PAUSE_BGM_CHANNEL);
        hasPauseBgm = false;
    }
    for (int i = 0; i < PAUSE_NUM_BUFS; i++) {
        if (pauseBgmStreamBuf[i]) {
            linearFree(pauseBgmStreamBuf[i]);
            pauseBgmStreamBuf[i] = NULL;
        }
    }
    if (pauseBgmBuffer) {
        free(pauseBgmBuffer);
        pauseBgmBuffer = NULL;
    }
    if (mp3ReadBuffer) free(mp3ReadBuffer);

    for (int s = 0; s < 4; s++) {
        if (hasCoverTex[s]) {
            C3D_TexDelete(&coverTextures[s]);
            hasCoverTex[s] = false;
        }
    }
    for (int i = 0; i < 10; i++) {
        if (hasCustomGustina[i]) {
            C3D_TexDelete(&customGustinaTex[i]);
            hasCustomGustina[i] = false;
        }
    }

    if (fontSheet1) C2D_SpriteSheetFree(fontSheet1);
    if (fontSheet2) C2D_SpriteSheetFree(fontSheet2);
    if (sysTextBuf) C2D_TextBufDelete(sysTextBuf);
    if (sysMeasureBuf) C2D_TextBufDelete(sysMeasureBuf);
    C2D_SpriteSheetFree(spriteSheet);

    C2D_Fini();
    C3D_Fini();
    ndspExit();
    if (hasHttpcService && lyricsJoined) httpcExit();
    if (hasAcService) acExit();
    aptUnhook(&appAptHookCookie);
    if (isShellClosed) toggle_3ds_screens_pls(true);
    if (hasPtmu) ptmuExit();
    aptSetSleepAllowed(true);
    gfxExit();
    romfsExit();

    return 0;
}
