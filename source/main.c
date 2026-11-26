#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include "stb_image.h"

#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

#define DR_FLAC_BUFFER_SIZE 16384
#define DR_FLAC_IMPLEMENTATION
#include "dr_flac.h"

#include <tremor/ivorbisfile.h> //someone is rlly watching this?
#include <opus/opusfile.h>

#include "sprites.h"
#include "qrcodegen.h"


u32 __stacksize__ = 256 * 1024;

#define CFG_QUAD_X0             22.0f   
#define CFG_QUAD_Y0             26.0f   
#define CFG_QUAD_X1             142.0f  
#define CFG_QUAD_Y1             64.0f   
#define CFG_QUAD_X2             142.0f  
#define CFG_QUAD_Y2             172.0f  
#define CFG_QUAD_X3             22.0f   
#define CFG_QUAD_Y3             206.0f  

#define CFG_PERSPECTIVE_SLICES  24      // fun
#define CFG_SUBPIXEL_OVERLAP    0.95f   
#define CFG_COVER_SHADOW_OFF_X  4.0f    
#define CFG_COVER_SHADOW_OFF_Y  4.0f    
#define CFG_COVER_SHADOW_ALPHA  160     

#define CFG_PEEK_TOP_X          82.0f   
#define CFG_PEEK_TOP_Y          -14.0f  
#define CFG_PEEK_BOT_X          82.0f   
#define CFG_PEEK_BOT_Y          222.0f  
#define CFG_PEEK_COVER_SCALE    0.70f   
#define CFG_PEEK_COVER_ALPHA    180     
#define CFG_COVER_HALF_SIZE     44.0f   

#define CFG_TRANS_SPEED         2.8f    

#define CFG_DEPTH_BG_BLUR       0.05f   
#define CFG_DEPTH_BG_DIMMER     0.08f   
#define CFG_DEPTH_PEEK_COVERS   0.25f   
#define CFG_DEPTH_TRANS_OLD     0.35f   
#define CFG_DEPTH_COVER_SHADOW  0.40f   
#define CFG_DEPTH_COVER_MAIN    0.45f   
#define CFG_DEPTH_WAVEFORM      0.65f   
#define CFG_DEPTH_PLAYHEAD      0.70f   
#define CFG_DEPTH_ICONS         0.75f   
#define CFG_DEPTH_TEXTS         0.85f   

#define CFG_BG_PAN_X_AMP        15.0f   
#define CFG_BG_PAN_Y_AMP        8.0f    
#define CFG_BG_PAN_SPEED_X      0.25f   
#define CFG_BG_PAN_SPEED_Y      0.18f   

#define CFG_LYRICS_CENTER_X     275.0f  
#define CFG_LYRICS_PREV_Y       48.0f   
#define CFG_LYRICS_ACTIVE_Y     88.0f   
#define CFG_LYRICS_NEXT_Y       128.0f  

#define CFG_LYRICS_ACTIVE_SCALE 0.52f   
#define CFG_LYRICS_SUB_SCALE    0.38f   
#define CFG_LYRICS_MAX_WIDTH    225.0f  
#define CFG_LYRICS_SLIDE_DIST   14.0f   
#define CFG_LYRICS_TWEEN_SPEED  4.0f    

#define CFG_CLOCK_X             316.0f  
#define CFG_CLOCK_Y             8.0f    
#define CFG_CLOCK_SCALE         0.48f   
#define CFG_WIFI_ICON_X         298.0f  
#define CFG_WIFI_ICON_Y         16.0f   
#define CFG_WIFI_ICON_SCALE     0.36f   

#define CFG_TIME_COUNTER_X      276.0f  
#define CFG_TIME_COUNTER_Y      186.0f  
#define CFG_TIME_COUNTER_SCALE  0.54f   

#define CFG_META_ICONS_X        172.0f  
#define CFG_META_ICON_SONG_Y    52.0f   
#define CFG_META_ICON_ARTIST_Y  90.0f   
#define CFG_META_ICON_ALBUM_Y   125.0f  
#define CFG_META_ICONS_SCALE    0.75f   

#define CFG_META_TEXT_X         190.0f  
#define CFG_META_TEXT_WIDTH     198.0f  
#define CFG_META_TITLE_Y        40.0f   
#define CFG_META_TITLE_SCALE    0.70f   
#define CFG_META_ARTIST_Y       76.0f   
#define CFG_META_ARTIST_SCALE   0.50f   
#define CFG_META_ALBUM_Y        112.0f  
#define CFG_META_ALBUM_SCALE    0.50f   

#define CFG_WAVE_CENTER_Y       230.0f  
#define CFG_WAVE_START_X        10.0f   
#define CFG_WAVE_MAX_HEIGHT     24.0f   
#define CFG_WAVE_BAR_STEP       4       
#define CFG_WAVE_BAR_WIDTH      3.0f    
#define CFG_PLAYHEAD_WIDTH      2.5f    

#define CFG_BOT_BG_SCROLL_SPD_X 25.0f   
#define CFG_BOT_BG_SCROLL_SPD_Y 25.0f   

#define CFG_GUSTINA_X           245.0f  
#define CFG_GUSTINA_Y           118.0f  
#define CFG_GUSTINA_SCALE_X     1.3f    
#define CFG_GUSTINA_SCALE_Y     1.3f    
#define CFG_GUSTINA_FRAME_TIME  0.08f   

#define CFG_WHEEL_CENTER_X      82.0f   
#define CFG_WHEEL_CENTER_Y      95.0f   
#define CFG_WHEEL_SCALE         1.1f    
#define CFG_WHEEL_INNER_RADIUS  26.0f   
#define CFG_WHEEL_OUTER_RADIUS  68.0f   

#define CFG_VOL_ICON_X          25.0f   
#define CFG_VOL_ICON_Y          188.0f  
#define CFG_VOL_ICON_SCALE      0.60f   
#define CFG_VOL_TEXT_X          42.0f   
#define CFG_VOL_TEXT_Y          178.0f  
#define CFG_VOL_TEXT_SCALE      0.52f   

#define CFG_NEXT_ICON_X         25.0f   
#define CFG_NEXT_ICON_Y         222.0f  
#define CFG_NEXT_ICON_SCALE     0.60f   
#define CFG_NEXT_TEXT_X         40.0f   
#define CFG_NEXT_TEXT_Y         212.0f  
#define CFG_NEXT_TEXT_WIDTH     130.0f  
#define CFG_NEXT_TEXT_SCALE     0.55f   

//funkn with hitler :joy
#define CFG_REPEAT_ICON_X       148.0f  
#define CFG_REPEAT_ICON_Y       42.0f   
#define CFG_REPEAT_ICON_SCALE   0.42f   

#define CFG_SHUFFLE_ICON_X      148.0f  
#define CFG_SHUFFLE_ICON_Y      150.0f  
#define CFG_SHUFFLE_ICON_SCALE  0.42f   
#define CFG_MODE_BTN_HIT_RADIUS 24.0f   

#define CFG_LIST_FOLDER_ICON_X  20.0f   
#define CFG_LIST_FOLDER_ICON_Y  18.0f   
#define CFG_LIST_DIR_TEXT_X     35.0f   
#define CFG_LIST_DIR_TEXT_Y     10.0f   
#define CFG_LIST_VISIBLE_ROWS   6       
#define CFG_LIST_ROW_START_Y    44.0f   
#define CFG_LIST_ROW_HEIGHT     28.0f   
#define CFG_LIST_TEXT_SCALE     0.35f   
#define CFG_LIST_BOX_X          8.0f    
#define CFG_LIST_BOX_WIDTH      206.0f  
#define CFG_LIST_BOX_HEIGHT     24.0f   
#define CFG_LIST_CURSOR_X       16.0f   
#define CFG_LIST_CURSOR_SCALE   0.48f   
#define CFG_LIST_SCROLL_X       218.0f  

#define AUDIO_CHANNEL       0
#define NUM_WAVE_BUFS       6
#define SAMPLES_PER_BUF     4096
#define STREAM_BUF_SIZE     (SAMPLES_PER_BUF * 2 * sizeof(int16_t))
#define FILE_READ_CHUNK     (32 * 1024)
#define NUM_WAVE_SAMPLES    380

#define MAX_CUSTOM_FRAMES 10
C3D_Tex customGustinaTex[10];
Tex3DS_SubTexture customGustinaSubtex[10]; //mod shit ok
bool hasCustomGustina[10] = {false};

typedef enum {
    FORMAT_NONE,
    FORMAT_MP3,
    FORMAT_WAV,
    FORMAT_FLAC, //i wanna kill you
    FORMAT_OGG,
    FORMAT_OPUS
} AudioFormat;

typedef enum {
    GUSTINA_NOTHING,
    GUSTINA_BOP_LEFT,
    GUSTINA_BOP_RIGHT,
    GUSTINA_IDLE
} GustinaState;

typedef struct {
    char name[256];
    bool is_dir;
} FileEntry;

typedef struct {
    float timestamp;
    char text[256];
} LrcLine;

#define MAX_FILES 256
#define MAX_LRC_LINES 512

static ndspWaveBuf waveBuf[NUM_WAVE_BUFS];
static int16_t* audioBuffer[NUM_WAVE_BUFS];

#define PAUSE_BGM_CHANNEL   1
#define PAUSE_NUM_BUFS      4
#define PAUSE_BUF_FRAMES    4096
static ndspWaveBuf pauseBgmWaveBuf[PAUSE_NUM_BUFS];
static int16_t* pauseBgmStreamBuf[PAUSE_NUM_BUFS] = {NULL};
static int16_t* pauseBgmBuffer = NULL;
static u32 pauseBgmTotalFrames = 0;
static u16 pauseBgmChannels = 2;
static u32 pauseBgmCursor = 0;
static bool hasPauseBgm = false;
static volatile bool pauseBgmPlaying = false;
static float pauseBgmVol = 0.0f;
static float pauseBgmLastSentVol = -1.0f;
static u32 waveBufSamples[NUM_WAVE_BUFS] = {0};
static FILE* currentAudioFile = NULL;
static AudioFormat currentFormat = FORMAT_NONE;
static volatile uint64_t samplesCompleted = 0;
static volatile uint64_t samplesFed = 0;
static volatile bool audioStreamEOF = false;
static volatile bool trackFinished = false;

#define MAX_PENDING_BEATS 64
static float pendingBeatTimes[MAX_PENDING_BEATS];
static volatile int beatQueueHead = 0;
static volatile int beatQueueTail = 0;
static float beatLpState = 0.0f;
static float beatAvgEnergy = 0.04f;
static float lastDetectedBeatTime = -1.0f;
static float lastVisualBeatTime = 0.0f;
static bool beatSideToggle = false;
static bool hasTrackBpmTag = false;

static Thread audioThread = NULL;
static volatile bool audioThreadRun = true;
static LightLock audioLock;
static volatile bool isAppSuspended = false;
static aptHookCookie appAptHookCookie;

static mp3dec_t mp3d;
static uint8_t* mp3ReadBuffer = NULL;
static size_t mp3BufferLen = 0;
static size_t mp3BufferPos = 0;
static int16_t mp3PcmResid[MINIMP3_MAX_SAMPLES_PER_FRAME];
static int mp3PcmPos = 0;
static int mp3PcmCount = 0;
static int mp3PcmChannels = 2;
static long mp3DataStartOffset = 0;
static long mp3DataSize = 0;

static drwav wavState;
static bool isWavOpen = false;

static drflac* flacState = NULL; //fuck this shit
static FILE* flacFile = NULL;
static OggVorbis_File oggFile;
static bool isOggOpen = false;
static OggOpusFile* opusFile = NULL;

static volatile bool isPlaying = false;
static volatile bool isPaused = false;
static char currentTrackPath[1024] = "";
static char currentTrackName[256] = "Select a Song!";
static char currentTrackTitle[256] = "";
static char currentTrackArtist[256] = "";
static char currentTrackAlbum[256] = "";
static int currentSampleRate = 44100;
static int currentChannels = 2;
static float volumeGain = 1.0f; 
static volatile float songTime = 0.0f;
static float songDuration = 0.0f;
static float detectedBPM = 120.0f;
static int lastBeat = -1;

static float fullSongWaveform[NUM_WAVE_SAMPLES];
static int16_t waveScanChunk[512 * 2];

static LrcLine lrcLines[MAX_LRC_LINES];
static int lrcLineCount = 0;
static int currentLrcIndex = -1;
static float lyricAnimTimer = 1.0f;
static bool lyricsEnabled = true;

typedef struct {
    char songPath[1024];
    char title[256];
    char artist[256];
    char album[256];
    int durationSec;
    uint32_t generation;
    u64 enqueueTimeMs;
} LyricsFetchJob;

static Thread lyricsThread = NULL;
static volatile bool lyricsThreadRun = true;
static LightLock lyricsLock;
static LyricsFetchJob pendingLyricsJob;
static volatile bool hasPendingLyricsJob = false;
static volatile bool downloadedLrcReady = false;
static volatile uint32_t currentTrackGeneration = 0;
static bool hasAcService = false;
static bool hasHttpcService = false;
static bool isWifiConnected = false;
static char lyricsDebugStatus[24] = "";

#define LASTFM_CFG_PATH     "sdmc:/3ds/MusicPlayer/lastfm.cfg"
#define LASTFM_QUEUE_PATH   "sdmc:/3ds/MusicPlayer/lfm_queue.txt"
#define LASTFM_WORKER_BASE  "http://mycoollyricsapi.kirb2109.workers.dev"

#define CFG_LASTFM_BTN_X        282.0f
#define CFG_LASTFM_BTN_Y        216.0f
#define CFG_LASTFM_BTN_SCALE    0.58f

static char   lastfm_session_key[128]  = "";
static char   lastfm_username[64]      = "";
static bool   lastfm_has_session       = false;
static bool   lastfm_scrobbled_track   = false;
static float  lastfm_played_seconds    = 0.0f;
static u32    lastfm_track_start_ts    = 0;
static LightLock lastfmLock;

static uint8_t lfm_qrcode[qrcodegen_BUFFER_LEN_FOR_VERSION(10)];
static bool    lfm_qr_ready = false;
static char    lfm_auth_token[128] = "";
static float   lfm_poll_timer = 0.0f;
static float   lfm_done_timer = 0.0f;
static bool    g_lastfm_login_active   = false;
static bool    g_lastfm_login_done     = false;
static bool    g_lastfm_login_cancelled= false;
static char    g_lastfm_login_status[64] = "";

static int get_qr_cell_size(void) {
    if (!lfm_qr_ready) return 4;
    int qrSize = qrcodegen_getSize(lfm_qrcode);
    int cell = 132 / qrSize;
    if (cell < 2) cell = 2;
    return cell;
}

static void draw_qr_code_centered(float cx, float topY) {
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

static FileEntry allFileList[MAX_FILES];
static int allFileCount = 0;
static FileEntry fileList[MAX_FILES];
static int fileCount = 0;
static int selectedFile = 0;
static int scrollOffset = 0;
static char currentDir[1024] = "sdmc:/music";
static char searchFilter[64] = "";
static float uiElapsedTime = 0.0f;

static C3D_RenderTarget* topTarget = NULL;
static C3D_RenderTarget* botTarget = NULL;
static C2D_SpriteSheet spriteSheet;

static C2D_SpriteSheet fontSheet1;
static C2D_SpriteSheet fontSheet2;
static C3D_Tex* fontTex1 = NULL;
static C3D_Tex* fontTex2 = NULL;
static Tex3DS_SubTexture fontGlyphs[96];
static C2D_TextBuf sysTextBuf = NULL;
static C2D_TextBuf sysMeasureBuf = NULL;

static C3D_Tex coverTextures[4];
static bool hasCoverTex[4] = {false, false, false, false};
static int slotSongIdx[4] = {-1, -1, -1, -1};
static int slotCurrent = 0;
static int slotPrev = 1;
static int slotNext = 2;
static int slotOld = 3;

#define coverTexCurrent (coverTextures[slotCurrent])
#define coverTexPrev    (coverTextures[slotPrev])
#define coverTexNext    (coverTextures[slotNext])
#define coverTexOld     (coverTextures[slotOld])
#define hasCoverCurrent (hasCoverTex[slotCurrent])
#define hasCoverPrev    (hasCoverTex[slotPrev])
#define hasCoverNext    (hasCoverTex[slotNext])
#define hasCoverOld     (hasCoverTex[slotOld])
static Tex3DS_SubTexture coverSubtex;

static uint8_t coverResizeBuf[128 * 128 * 4];

typedef enum {
    TRANS_NONE, //i love transfems
    TRANS_NEXT,
    TRANS_PREV
} TransitionDir;

static TransitionDir coverTransDir = TRANS_NONE;
static float coverTransProgress = 1.0f; 
static float coverBgPanX = 0.0f;
static float coverBgPanY = 0.0f;

static GustinaState gustinaState = GUSTINA_NOTHING;
static int gustinaFrame = 0;
static float gustinaFrameTimer = 0.0f;

static int global10FpsFrame = 0;
static float global10FpsTimer = 0.0f;

static float bgScrollX = 0.0f;
static float bgScrollY = 0.0f;

static int bottomScreenMode = 0;
static int wheelTouchSprite = sprites_control1_idx;
static bool repeatEnabled = false;
static bool shuffleEnabled = false;
static int upcomingShuffleIdx = -1;
static int cachedNextIdx = -2;
static bool cachedNextRepeat = false;
static char cachedNextDir[1024] = "";
static uint32_t cachedNextGen = 0;
static char cachedNextDisplay[256] = "";

static inline uint32_t morton_interleave(uint32_t x, uint32_t y) {
    uint32_t i = 0;
    for (int bit = 0; bit < 3; bit++) {
        i |= ((x >> bit) & 1) << (bit * 2);
        i |= ((y >> bit) & 1) << (bit * 2 + 1);
    }
    return i;
}

static void upload_rgba8_to_tex(C3D_Tex* tex, const uint8_t* linear_rgba, int w, int h, int tex_w, int tex_h) {
    if (!C3D_TexInit(tex, tex_w, tex_h, GPU_RGBA8)) return;
    C3D_TexSetFilter(tex, GPU_LINEAR, GPU_LINEAR);
    C3D_TexSetWrap(tex, GPU_CLAMP_TO_EDGE, GPU_CLAMP_TO_EDGE);

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

static inline int16_t apply_gain_and_limit(int16_t sample, float gain) {
    if (gain == 1.0f) return sample;
    int32_t val = (int32_t)((float)sample * gain);
    if (val > 32767) return 32767;
    if (val < -32768) return -32768;
    return (int16_t)val;
}

static bool has_extension(const char* fn, const char* ext) {
    size_t l1 = strlen(fn), l2 = strlen(ext);
    if (l1 < l2) return false;
    return (strcasecmp(fn + l1 - l2, ext) == 0);
}

static int utf8_next_char(const char* s, uint32_t* outCp) {
    unsigned char c0 = (unsigned char)s[0];
    if (c0 == 0) {
        if (outCp) *outCp = 0;
        return 0;
    }
    if (c0 < 0x80) {
        if (outCp) *outCp = c0;
        return 1;
    }
    if ((c0 & 0xE0) == 0xC0 && (s[1] & 0xC0) == 0x80) {
        if (outCp) *outCp = ((uint32_t)(c0 & 0x1F) << 6) | (uint32_t)(s[1] & 0x3F);
        return 2;
    }
    if ((c0 & 0xF0) == 0xE0 && (s[1] & 0xC0) == 0x80 && (s[2] & 0xC0) == 0x80) {
        if (outCp) *outCp = ((uint32_t)(c0 & 0x0F) << 12) | ((uint32_t)(s[1] & 0x3F) << 6) | (uint32_t)(s[2] & 0x3F);
        return 3;
    }
    if ((c0 & 0xF8) == 0xF0 && (s[1] & 0xC0) == 0x80 && (s[2] & 0xC0) == 0x80 && (s[3] & 0xC0) == 0x80) {
        if (outCp) *outCp = ((uint32_t)(c0 & 0x07) << 18) | ((uint32_t)(s[1] & 0x3F) << 12) | ((uint32_t)(s[2] & 0x3F) << 6) | (uint32_t)(s[3] & 0x3F);
        return 4;
    }
    if (outCp) *outCp = 0xFFFD;
    return 1;
}

static inline bool is_custom_font_cp(uint32_t cp) {
    return (cp >= 32 && cp <= 126);
}

static void sanitize_utf8_to_ascii(const char* src, char* dst, size_t dstSize) {
    if (!src || !dst || dstSize == 0) return;
    size_t s = 0, d = 0;

    
    if ((unsigned char)src[0] == 0xEF && (unsigned char)src[1] == 0xBB && (unsigned char)src[2] == 0xBF) {
        s = 3;
    }

    while (src[s] != '\0' && d + 1 < dstSize) {
        uint32_t cp = 0;
        int len = utf8_next_char(src + s, &cp);
        if (len <= 0) break;

        if (cp == '\t') {
            dst[d++] = ' ';
        } else if (cp >= 32 && cp != 0xFFFD) {
            if (d + (size_t)len < dstSize) {
                for (int k = 0; k < len; k++) dst[d++] = src[s + k];
            } else {
                break;
            }
        }
        s += (size_t)len;
    }
    dst[d] = '\0';
    while (d > 0 && (dst[d-1] == ' ' || dst[d-1] == '\t' || dst[d-1] == '\r' || dst[d-1] == '\n')) {
        dst[--d] = '\0';
    }
}

static void strip_extension_copy(const char* src, char* dst, size_t dstSize) {
    if (!dst || dstSize == 0) return;
    if (!src) { dst[0] = '\0'; return; }
    snprintf(dst, dstSize, "%s", src);
    if (strcmp(dst, "..") == 0) return;
    char* dot = strrchr(dst, '.');
    if (dot && dot > dst) *dot = '\0';
}

static void utf8_truncate_chars(const char* src, char* dst, size_t dstSize, int maxChars) {
    if (!src || !dst || dstSize == 0) return;
    size_t s = 0, d = 0;
    int chars = 0;
    while (src[s] != '\0' && chars < maxChars) {
        uint32_t cp = 0;
        int len = utf8_next_char(src + s, &cp);
        if (len <= 0 || d + (size_t)len >= dstSize) break;
        for (int k = 0; k < len; k++) dst[d++] = src[s + k];
        s += (size_t)len;
        chars++;
    }
    dst[d] = '\0';
}

static void init_custom_font(void) { //reminds me to maro...
    fontSheet1 = C2D_SpriteSheetLoad("romfs:/gfx/font1.t3x");
    fontSheet2 = C2D_SpriteSheetLoad("romfs:/gfx/font2.t3x");

    if (fontSheet1) {
        C2D_Image img1 = C2D_SpriteSheetGetImage(fontSheet1, 0);
        fontTex1 = img1.tex;
    }
    if (fontSheet2) {
        C2D_Image img2 = C2D_SpriteSheetGetImage(fontSheet2, 0);
        fontTex2 = img2.tex;
    }
    if (!fontTex1 && fontTex2) fontTex1 = fontTex2;
    if (!fontTex2 && fontTex1) fontTex2 = fontTex1;

    
    sysTextBuf = C2D_TextBufNew(1024);
    sysMeasureBuf = C2D_TextBufNew(256);

    
    
    for (int i = 0; i < 96; i++) {
        int col = i % 16;
        int row = i / 16;
        fontGlyphs[i].width = 32;
        fontGlyphs[i].height = 64;
        fontGlyphs[i].left = (float)(col * 32) / 512.0f;
        fontGlyphs[i].right = (float)((col + 1) * 32) / 512.0f;
        fontGlyphs[i].top = 1.0f - (float)(row * 64) / 512.0f;
        fontGlyphs[i].bottom = 1.0f - (float)((row + 1) * 64) / 512.0f;
    }
}

static float measure_sys_run(const char* seg, float scale) {
    if (!sysMeasureBuf || !seg || seg[0] == '\0') return 0.0f;
    C2D_TextBufClear(sysMeasureBuf);
    C2D_Text t;
    if (!C2D_TextParse(&t, sysMeasureBuf, seg)) return 0.0f;
    return t.width * (scale * 1.45f);
}

static float draw_sys_run(const char* seg, float x, float y, float scale, u32 color) {
    if (!sysTextBuf || !seg || seg[0] == '\0') return 0.0f;
    C2D_Text t;
    if (!C2D_TextParse(&t, sysTextBuf, seg)) return 0.0f;
    C2D_TextOptimize(&t);
    float sysScale = scale * 1.45f;
    float sysY = y + (10.0f * scale);
    C2D_DrawText(&t, C2D_WithColor, x, sysY, CFG_DEPTH_TEXTS, sysScale, sysScale, color);
    return t.width * sysScale;
}

static float measure_custom_font(float scale, const char* str) {
    if (!str) return 0.0f;
    float w = 0.0f;
    float step = 20.0f * scale;
    char runBuf[256];
    size_t runLen = 0;

    for (size_t i = 0; str[i] != '\0'; ) {
        uint32_t cp = 0;
        int len = utf8_next_char(str + i, &cp);
        if (len <= 0) break;

        if (is_custom_font_cp(cp)) {
            if (runLen > 0) {
                runBuf[runLen] = '\0';
                w += measure_sys_run(runBuf, scale);
                runLen = 0;
            }
            if (cp == ' ') w += 14.0f * scale;
            else w += step;
        } else {
            if (runLen + (size_t)len < sizeof(runBuf)) {
                for (int k = 0; k < len; k++) runBuf[runLen++] = str[i + k];
            } else {
                runBuf[runLen] = '\0';
                w += measure_sys_run(runBuf, scale);
                runLen = 0;
                for (int k = 0; k < len; k++) runBuf[runLen++] = str[i + k];
            }
        }
        i += (size_t)len;
    }
    if (runLen > 0) {
        runBuf[runLen] = '\0';
        w += measure_sys_run(runBuf, scale);
    }
    return w;
}

static void draw_custom_font(float x, float y, float scale, u32 color, const char* str) { //NO PLEASE, NO MORE DRAWING PLEASEEEE RNFRNFSDH
    if (!fontTex1 || !fontTex2 || !str) return;

    C3D_Tex* curTex = (global10FpsFrame == 0) ? fontTex1 : fontTex2;
    float curX = x;
    float step = 20.0f * scale;

    C2D_ImageTint tint;
    C2D_PlainImageTint(&tint, color, 1.0f);

    char runBuf[256];
    size_t runLen = 0;

    for (size_t i = 0; str[i] != '\0'; ) {
        uint32_t cp = 0;
        int len = utf8_next_char(str + i, &cp);
        if (len <= 0) break;

        if (is_custom_font_cp(cp)) {
            if (runLen > 0) {
                runBuf[runLen] = '\0';
                curX += draw_sys_run(runBuf, curX, y, scale, color);
                runLen = 0;
            }
            if (cp == ' ') {
                curX += 14.0f * scale;
            } else {
                int idx = (int)cp - 32;
                C2D_Image charImg = { curTex, &fontGlyphs[idx] };
                C2D_DrawImageAt(charImg, curX, y, CFG_DEPTH_TEXTS, &tint, scale, scale);
                curX += step;
            }
        } else {
            if (runLen + (size_t)len < sizeof(runBuf)) {
                for (int k = 0; k < len; k++) runBuf[runLen++] = str[i + k];
            } else {
                runBuf[runLen] = '\0';
                curX += draw_sys_run(runBuf, curX, y, scale, color);
                runLen = 0;
                for (int k = 0; k < len; k++) runBuf[runLen++] = str[i + k];
            }
        }
        i += (size_t)len;
    }
    if (runLen > 0) {
        runBuf[runLen] = '\0';
        draw_sys_run(runBuf, curX, y, scale, color);
    }
}

static void draw_custom_font_centered(float cx, float y, float scale, u32 color, const char* str) {
    float w = measure_custom_font(scale, str);
    draw_custom_font(cx - (w * 0.5f), y, scale, color, str);
}

static void draw_marquee_font(float x, float y, float maxW, float scale, u32 color, const char* str, float time) {
    if (!fontTex1 || !fontTex2 || !str) return;

    float textW = measure_custom_font(scale, str);
    C3D_Tex* curTex = (global10FpsFrame == 0) ? fontTex1 : fontTex2;
    float step = 20.0f * scale;

    if (textW <= maxW) {
        draw_custom_font(x, y, scale, color, str);
        return;
    }

    float excess = textW - maxW;
    float scrollSpeed = 28.0f; 
    float travelTime = excess / scrollSpeed;
    if (travelTime < 0.6f) travelTime = 0.6f;
    float pauseTime = 1.4f;
    float period = 2.0f * (pauseTime + travelTime);
    float t = fmodf(time, period);
    if (t < 0.0f) t += period;

    float offset = 0.0f;
    if (t < pauseTime) {
        offset = 0.0f;
    } else if (t < pauseTime + travelTime) {
        float p = (t - pauseTime) / travelTime;
        float smooth = p * p * (3.0f - 2.0f * p);
        offset = smooth * excess;
    } else if (t < 2.0f * pauseTime + travelTime) {
        offset = excess;
    } else {
        float p = (t - (2.0f * pauseTime + travelTime)) / travelTime;
        float smooth = p * p * (3.0f - 2.0f * p);
        offset = (1.0f - smooth) * excess;
    }

    float startX = x - offset;
    float fadeEdge = 16.0f;
    float remRight = excess - offset;
    float leftStrength  = (offset < fadeEdge)   ? (offset / fadeEdge)   : 1.0f;
    float rightStrength = (remRight < fadeEdge) ? (remRight / fadeEdge) : 1.0f;

    for (size_t i = 0; str[i] != '\0'; ) {
        uint32_t cp = 0;
        int len = utf8_next_char(str + i, &cp);
        if (len <= 0) break;

        float charX = startX;
        if (cp == ' ') {
            startX += 14.0f * scale;
            i += (size_t)len;
            continue;
        }

        char gBuf[8];
        float gW = step;
        bool isCustom = is_custom_font_cp(cp);
        if (!isCustom) {
            for (int k = 0; k < len && k < 7; k++) gBuf[k] = str[i + k];
            gBuf[len < 7 ? len : 7] = '\0';
            gW = measure_sys_run(gBuf, scale);
            if (gW <= 0.5f) gW = 14.0f * scale;
        }

        startX += gW;
        i += (size_t)len;

        float charCenter = charX + gW * 0.5f;
        if (charX + 2.0f < x || charX + gW > x + maxW + 2.0f) continue;

        float rawL = (charCenter - x) / fadeEdge;
        if (rawL < 0.0f) rawL = 0.0f; else if (rawL > 1.0f) rawL = 1.0f;
        float alphaL = 1.0f - leftStrength * (1.0f - rawL);

        float rawR = (x + maxW - charCenter) / fadeEdge;
        if (rawR < 0.0f) rawR = 0.0f; else if (rawR > 1.0f) rawR = 1.0f;
        float alphaR = 1.0f - rightStrength * (1.0f - rawR);

        float alpha = alphaL * alphaR;
        if (alpha <= 0.02f) continue;

        u32 a = (u32)(((color >> 24) & 0xFF) * alpha);
        u32 charColor = (color & 0x00FFFFFF) | (a << 24);

        if (isCustom) {
            int idx = (int)cp - 32;
            C2D_ImageTint tint;
            C2D_PlainImageTint(&tint, charColor, 1.0f);
            C2D_Image charImg = { curTex, &fontGlyphs[idx] };
            C2D_DrawImageAt(charImg, charX, y, CFG_DEPTH_TEXTS, &tint, scale, scale);
        } else {
            draw_sys_run(gBuf, charX, y, scale, charColor);
        }
    }
}

static void draw_wrapped_lyrics(float cx, float cy, float maxW, float scale, u32 color, const char* text) {
    if (!text || text[0] == '\0') return;

    float textW = measure_custom_font(scale, text);
    if (textW <= maxW) {
        draw_custom_font_centered(cx, cy, scale, color, text);
        return;
    }

    int len = (int)strlen(text);
    int mid = len / 2;
    int splitIdx = -1;
    int bestDist = 9999;

    for (int i = 0; i < len; i++) {
        if (text[i] == ' ') {
            int dist = abs(i - mid);
            if (dist < bestDist) {
                bestDist = dist;
                splitIdx = i;
            }
        }
    }
    
    bool splitOnSpace = (splitIdx > 0);
    if (!splitOnSpace) {
        bestDist = 9999;
        for (int i = 1; i < len; i++) {
            if (((unsigned char)text[i] & 0xC0) != 0x80) {
                int dist = abs(i - mid);
                if (dist < bestDist) {
                    bestDist = dist;
                    splitIdx = i;
                }
            }
        }
    }

    if (splitIdx > 0) {
        char line1[256];
        char line2[256];
        snprintf(line1, sizeof(line1), "%.*s", splitIdx, text);
        const char* p2 = text + (splitOnSpace ? splitIdx + 1 : splitIdx);
        while (*p2 == ' ') p2++;
        snprintf(line2, sizeof(line2), "%s", p2);

        float lineSpacing = 16.0f * scale * 2.4f;
        draw_custom_font_centered(cx, cy - lineSpacing * 0.45f, scale, color, line1);
        draw_custom_font_centered(cx, cy + lineSpacing * 0.55f, scale, color, line2);
    } else {
        float fitScale = scale * (maxW / textW);
        if (fitScale < 0.28f) fitScale = 0.28f;
        draw_custom_font_centered(cx, cy, fitScale, color, text);
    }
}

static void make_procedural_cover(uint8_t* dst, uint8_t accentR, uint8_t accentG, uint8_t accentB) {
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

static uint8_t* base64_decode(const char* src, size_t srcLen, size_t* outLen) { //base64 shit
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

static uint8_t* extract_flac_cover(const char* filePath, int* outW, int* outH, int* outChannels) { //HOW I DO TO MAKE IT DONT LOOK RED?????
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
        uint8_t* dec = base64_decode(c + 23, clen - 23, &decLen);
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
        uint8_t* dec = base64_decode(c + 9, clen - 9, &decLen);
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

static bool ogg_scan(const char* filePath, size_t maxPacketBytes, OggCommentVisitor visitor, void* userData) {
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
    FILE* f = fopen(filePath, "rb");
    if (!f) return NULL;
    size_t scanSize = 1024 * 1024;
    uint8_t* buf = (uint8_t*)malloc(scanSize);
    if (!buf) { fclose(f); return NULL; }
    size_t bytesRead = fread(buf, 1, scanSize, f);
    fclose(f);
    if (bytesRead < 64) { free(buf); return NULL; }

    uint8_t* result = NULL;
    for (size_t i = 0; i + 16 < bytesRead; i++) {
        if (memcmp(buf + i, "covr", 4) == 0) {
            for (size_t j = i + 4; j + 16 < bytesRead && j < i + 128; j++) {
                if (memcmp(buf + j + 4, "data", 4) == 0) {
                    uint32_t dataSize = ((uint32_t)buf[j] << 24) | ((uint32_t)buf[j+1] << 16) | ((uint32_t)buf[j+2] << 8) | (uint32_t)buf[j+3];
                    if (dataSize > 16 && j + dataSize <= bytesRead) {
                        result = stbi_load_from_memory(buf + j + 16, (int)(dataSize - 16), outW, outH, outChannels, 4);
                    }
                    break;
                }
            }
            if (result) break;
        }
    }
    free(buf);
    return result;
}

static void load_song_cover_tex(const char* filePath, C3D_Tex* tex, bool* hasTex, uint8_t accentR, uint8_t accentG, uint8_t accentB) {
    if (*hasTex) {
        C3D_TexDelete(tex);
        *hasTex = false;
    }

    uint8_t* decodedRgba = NULL;
    int imgW = 0, imgH = 0, channels = 0;

    
    if (filePath) { //unoptimized bullshit :joy
        if (has_extension(filePath, ".flac")) {
            decodedRgba = extract_flac_cover(filePath, &imgW, &imgH, &channels);
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
        upload_rgba8_to_tex(tex, coverResizeBuf, 128, 128, 128, 128);
        *hasTex = true;
        stbi_image_free(decodedRgba);
    } else {
        make_procedural_cover(coverResizeBuf, accentR, accentG, accentB);
        upload_rgba8_to_tex(tex, coverResizeBuf, 128, 128, 128, 128);
        *hasTex = true;
    }
}

static void update_all_carousel_covers(void) {
    if (fileCount == 0) return;

    coverSubtex.width = 128;
    coverSubtex.height = 128;
    coverSubtex.left = 0.0f;
    coverSubtex.top = 1.0f;
    coverSubtex.right = 1.0f;
    coverSubtex.bottom = 0.0f;

    int prevIdx = (selectedFile > 0) ? (selectedFile - 1) : (fileCount - 1);
    int nextIdx = (selectedFile + 1 < fileCount) ? (selectedFile + 1) : 0;

    if (slotSongIdx[slotCurrent] != selectedFile || !hasCoverCurrent) {
        static char fpCurrent[1024];
        snprintf(fpCurrent, sizeof(fpCurrent), "%s/%s", currentDir, fileList[selectedFile].name);
        load_song_cover_tex(fpCurrent, &coverTexCurrent, &hasCoverCurrent, 0, 220, 255);
        slotSongIdx[slotCurrent] = selectedFile;
    }

    if (slotSongIdx[slotPrev] != prevIdx || !hasCoverPrev) {
        static char fpPrev[1024];
        snprintf(fpPrev, sizeof(fpPrev), "%s/%s", currentDir, fileList[prevIdx].name);
        load_song_cover_tex(fpPrev, &coverTexPrev, &hasCoverPrev, 255, 120, 0);
        slotSongIdx[slotPrev] = prevIdx;
    }

    if (slotSongIdx[slotNext] != nextIdx || !hasCoverNext) {
        static char fpNext[1024];
        snprintf(fpNext, sizeof(fpNext), "%s/%s", currentDir, fileList[nextIdx].name);
        load_song_cover_tex(fpNext, &coverTexNext, &hasCoverNext, 255, 60, 180);
        slotSongIdx[slotNext] = nextIdx;
    }
}

static void build_lrc_path(const char* songPath, char* lrcPath, size_t lrcPathSize) {
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

static bool lrc_file_exists(const char* songPath) {
    char lrcPath[1024];
    build_lrc_path(songPath, lrcPath, sizeof(lrcPath));
    if (lrcPath[0] == '\0') return false;
    struct stat st;
    return (stat(lrcPath, &st) == 0 && !S_ISDIR(st.st_mode));
}

static void load_lrc_file(const char* songPath) { //fuck you
    lrcLineCount = 0;
    currentLrcIndex = -1;

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
                sanitize_utf8_to_ascii(text, cleanText, sizeof(cleanText));
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

static void url_encode_str(const char* src, char* dst, size_t dstSize) {
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

static void clean_query_title(const char* src, char* dst, size_t dstSize) {
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

static bool is_trivial_no_lyrics_title(const char* title) {
    if (!title || strlen(title) < 2) return true;
    return (strcasecmp(title, "Audio Track") == 0 ||
            strcasecmp(title, "Inst") == 0 ||
            strcasecmp(title, "Instrumental") == 0 ||
            strcasecmp(title, "Voices") == 0 ||
            strcasecmp(title, "Vocals") == 0);
}

static char* httpc_fetch_json(const char* startUrl, uint32_t activeGen, Result* outRet, u32* outHttpStatus, size_t* outLen) {
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

static void lastfm_load_cfg(void) {
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

static void lastfm_send_nowplaying(void);

static int lastfm_poll_qr_auth(void) {
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
            lastfm_send_nowplaying();
        }
        return 1;
    }
    return 0;
}

static void lastfm_send_nowplaying(void) {
    if (!lastfm_has_session || !hasHttpcService) return;
    if (currentTrackTitle[0] == '\0') return;

    char encSk[256] = {0}, encArtist[384] = {0}, encTrack[384] = {0}, encAlbum[384] = {0};
    url_encode_str(lastfm_session_key, encSk, sizeof(encSk));
    url_encode_str(currentTrackArtist[0] ? currentTrackArtist : "Unknown Artist", encArtist, sizeof(encArtist));
    url_encode_str(currentTrackTitle, encTrack, sizeof(encTrack));
    url_encode_str(currentTrackAlbum[0] ? currentTrackAlbum : "", encAlbum, sizeof(encAlbum));

    char url[1536];
    snprintf(url, sizeof(url),
             "%s/lastfm/nowplaying?sk=%s&artist=%s&track=%s&album=%s&duration=%d",
             LASTFM_WORKER_BASE, encSk, encArtist, encTrack, encAlbum, (int)songDuration);

    char resp[256] = {0};
    lastfm_get_json(url, resp, sizeof(resp));
}

static void lastfm_send_scrobble(void) {
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
    char resp[256] = {0};
    u32 st = lastfm_get_json(url, resp, sizeof(resp));
    if (st != 200) {
        lastfm_enqueue_scrobble(url);
    }
}

static void lastfm_flush_queue(void) {
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

static void render_lastfm_login_screen(void) {
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

static void lastfm_open_login(void) {
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

static void lyrics_worker_entry(void* arg) {
    char* lrcOut = (char*)malloc(64 * 1024);
    uint32_t failedCache[64] = {0};
    int failedWritePos = 0;

    while (lyricsThreadRun) {
        if (isAppSuspended) {
            svcSleepThread(200 * 1000 * 1000LL);
            continue;
        }
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
        if (is_trivial_no_lyrics_title(cleanTitle)) { set_lyrics_debug_status("INST"); continue; }

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
                    "http://mycoollyricsapi.kirb2109.workers.dev/?track=%s",
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

static bool str_contains_nocase(const char* haystack, const char* needle) {
    if (!needle || needle[0] == '\0') return true;
    if (!haystack) return false;
    size_t nlen = strlen(needle);
    for (size_t i = 0; haystack[i] != '\0'; i++) {
        if (strncasecmp(haystack + i, needle, nlen) == 0) return true;
    }
    return false;
}

static void apply_file_filter(void) {
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

static void load_directory(const char* path) {
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
            has_extension(ent->d_name, ".opus")) {
            snprintf(allFileList[allFileCount].name, sizeof(allFileList[allFileCount].name), "%s", ent->d_name);
            allFileList[allFileCount].is_dir = isDir;
            allFileCount++;
        }
    }
    closedir(dir);
    apply_file_filter();
}

static void generate_song_waveform(void) {
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

static void stop_playback(void) {
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
    beatQueueHead = 0;
    beatQueueTail = 0;
    lastDetectedBeatTime = -1.0f;
    lastVisualBeatTime = 0.0f;
    beatLpState = 0.0f;
    beatAvgEnergy = 0.04f;
    hasTrackBpmTag = false;
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

    currentFormat = FORMAT_NONE;
    mp3BufferLen = 0;
    mp3BufferPos = 0;
    mp3PcmPos = 0;
    mp3PcmCount = 0;

    LightLock_Unlock(&audioLock);
}

static size_t flac_on_read(void* pUserData, void* pBufferOut, size_t bytesToRead) {
    return fread(pBufferOut, 1, bytesToRead, (FILE*)pUserData);
}

static drflac_bool32 flac_on_seek(void* pUserData, int offset, drflac_seek_origin origin) {
    int whence = (origin == DRFLAC_SEEK_SET) ? SEEK_SET : ((origin == DRFLAC_SEEK_CUR) ? SEEK_CUR : SEEK_END);
    return (fseek((FILE*)pUserData, (long)offset, whence) == 0) ? DRFLAC_TRUE : DRFLAC_FALSE;
}

static drflac_bool32 flac_on_tell(void* pUserData, drflac_int64* pCursor) {
    long pos = ftell((FILE*)pUserData);
    if (pos < 0) return DRFLAC_FALSE;
    *pCursor = (drflac_int64)pos;
    return DRFLAC_TRUE;
}

static void apply_oggshit_kv(const char* comment, int len,
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
        sanitize_utf8_to_ascii(rawVal, cleanVal, sizeof(cleanVal));
        if (cleanVal[0] != '\0') snprintf(outTitle, titleSz, "%s", cleanVal);
    } else if (len > 7 && strncasecmp(comment, "ARTIST=", 7) == 0 && outArtist && artistSz > 0) {
        int vlen = (len - 7 < (int)sizeof(rawVal) - 1) ? (len - 7) : ((int)sizeof(rawVal) - 1);
        memcpy(rawVal, comment + 7, vlen); rawVal[vlen] = '\0';
        sanitize_utf8_to_ascii(rawVal, cleanVal, sizeof(cleanVal));
        if (cleanVal[0] != '\0') snprintf(outArtist, artistSz, "%s", cleanVal);
    } else if (len > 6 && strncasecmp(comment, "ALBUM=", 6) == 0 && outAlbum && albumSz > 0) {
        int vlen = (len - 6 < (int)sizeof(rawVal) - 1) ? (len - 6) : ((int)sizeof(rawVal) - 1);
        memcpy(rawVal, comment + 6, vlen); rawVal[vlen] = '\0';
        sanitize_utf8_to_ascii(rawVal, cleanVal, sizeof(cleanVal));
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

static void parse_oggshits_array(char** comments, const int* lengths, int count,
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

static void flac_meta_callback(void* pUserData, drflac_metadata* pMetadata) {
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
    sanitize_utf8_to_ascii(utf8Buf, dst, dstSize);
}

static void parse_mp3_id3v2_meta_into(FILE* f,
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

static void parse_mp3_id3v2_meta(FILE* f, long* outAudioOffset, float* outTlenSec) {
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
    sanitize_utf8_to_ascii(noExt, outTitle, titleSz);
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
    }
}

static const char* get_next_song_display(int targetIdx, bool repeating) {
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

static void start_playback(const char* filePath, const char* fileName, TransitionDir transDirection) {
    
    stop_playback();

    LightLock_Lock(&audioLock);

    strncpy(currentTrackPath, filePath, sizeof(currentTrackPath) - 1);
    strncpy(currentTrackName, fileName, sizeof(currentTrackName) - 1);
    currentTrackName[sizeof(currentTrackName) - 1] = '\0';

    static char cleanTitle[256];
    sanitize_utf8_to_ascii(fileName, cleanTitle, sizeof(cleanTitle));
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

        fseek(currentAudioFile, mp3DataStartOffset, SEEK_SET);
        mp3BufferLen = fread(mp3ReadBuffer, 1, FILE_READ_CHUNK, currentAudioFile);
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
        flacState = drflac_open_with_metadata(flac_on_read, flac_on_seek, flac_on_tell, flac_meta_callback, flacFile, NULL);
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
    } else {
        LightLock_Unlock(&audioLock);
        return;
    }

    generate_song_waveform();
    LightLock_Unlock(&audioLock);

    int prevIdx = (selectedFile > 0) ? (selectedFile - 1) : (fileCount - 1);
    int nextIdx = (selectedFile + 1 < fileCount) ? (selectedFile + 1) : 0;

    if (transDirection == TRANS_NEXT) {
        int recycledSlot = slotPrev; 
        slotOld = slotCurrent;       
        slotPrev = slotCurrent;      
        slotCurrent = slotNext;      
        slotNext = recycledSlot;     

        if (slotSongIdx[slotCurrent] != selectedFile || !hasCoverCurrent) {
            static char fpCurr[1024];
            snprintf(fpCurr, sizeof(fpCurr), "%s/%s", currentDir, fileList[selectedFile].name);
            load_song_cover_tex(fpCurr, &coverTexCurrent, &hasCoverCurrent, 0, 220, 255);
            slotSongIdx[slotCurrent] = selectedFile;
        }

        static char fpNext[1024];
        snprintf(fpNext, sizeof(fpNext), "%s/%s", currentDir, fileList[nextIdx].name);
        load_song_cover_tex(fpNext, &coverTexNext, &hasCoverNext, 255, 60, 180);
        slotSongIdx[slotNext] = nextIdx;

    } else if (transDirection == TRANS_PREV) {
        int recycledSlot = slotNext; 
        slotOld = slotCurrent;       
        slotNext = slotCurrent;      
        slotCurrent = slotPrev;      
        slotPrev = recycledSlot;     

        if (slotSongIdx[slotCurrent] != selectedFile || !hasCoverCurrent) {
            static char fpCurr[1024];
            snprintf(fpCurr, sizeof(fpCurr), "%s/%s", currentDir, fileList[selectedFile].name);
            load_song_cover_tex(fpCurr, &coverTexCurrent, &hasCoverCurrent, 0, 220, 255);
            slotSongIdx[slotCurrent] = selectedFile;
        }

        static char fpPrev[1024];
        snprintf(fpPrev, sizeof(fpPrev), "%s/%s", currentDir, fileList[prevIdx].name);
        load_song_cover_tex(fpPrev, &coverTexPrev, &hasCoverPrev, 255, 120, 0);
        slotSongIdx[slotPrev] = prevIdx;

    } else {
        update_all_carousel_covers();
    }
    coverTransDir = transDirection;
    coverTransProgress = (transDirection == TRANS_NONE) ? 1.0f : 0.0f;

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
    beatQueueHead = 0;
    beatQueueTail = 0;
    beatLpState = 0.0f;
    beatAvgEnergy = 0.04f;
    lastDetectedBeatTime = -1.0f;
    lastVisualBeatTime = 0.0f;
    beatSideToggle = false;
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
    lastfm_send_nowplaying();
}

static void analyze_audio_beat(const int16_t* pcm, int frames) {
    if (!pcm || frames <= 0 || currentSampleRate <= 0) return;

    const int subWin = 512;
    int offset = 0;
    float minGap = (hasTrackBpmTag && detectedBPM > 40.0f) ? (60.0f / detectedBPM) * 0.62f : 0.25f;
    if (minGap < 0.22f) minGap = 0.22f;
    if (minGap > 0.50f) minGap = 0.50f;

    while (offset < frames) {
        int count = frames - offset;
        if (count > subWin) count = subWin;

        float sumBass = 0.0f;
        float sumFull = 0.0f;
        for (int i = 0; i < count; i++) {
            int idx = (offset + i) * 2;
            float mono = ((float)pcm[idx] + (float)pcm[idx + 1]) * (0.5f / 32768.0f);
            beatLpState += 0.14f * (mono - beatLpState);
            sumBass += fabsf(beatLpState);
            sumFull += fabsf(mono);
        }

        float energy = (sumBass * 0.72f + sumFull * 0.28f) / (float)count;
        float frameTime = (float)((double)(samplesFed + (uint64_t)offset) / (double)currentSampleRate);

        if (energy > beatAvgEnergy * 1.40f && energy > 0.032f &&
            (lastDetectedBeatTime < 0.0f || (frameTime - lastDetectedBeatTime) >= minGap)) {
            lastDetectedBeatTime = frameTime;
            int nextTail = (beatQueueTail + 1) % MAX_PENDING_BEATS;
            if (nextTail != beatQueueHead) {
                pendingBeatTimes[beatQueueTail] = frameTime;
                beatQueueTail = nextTail;
            }
        }

        beatAvgEnergy = beatAvgEnergy * 0.88f + energy * 0.12f;
        if (beatAvgEnergy < 0.015f) beatAvgEnergy = 0.015f;
        offset += count;
    }
    samplesFed += (uint64_t)frames;
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
        int gain_fp = (int)(volumeGain * 256.0f);
        if (gain_fp == 256 && flacState->channels == 2) {
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
                    if (flacState->channels == 2) {
                        int32_t l = ((int32_t)temp[i*2] * gain_fp) >> 8;
                        int32_t r = ((int32_t)temp[i*2+1] * gain_fp) >> 8;
                        if (l > 32767) l = 32767; else if (l < -32768) l = -32768;
                        if (r > 32767) r = 32767; else if (r < -32768) r = -32768;
                        dest[samplesWritten*2]   = (int16_t)l;
                        dest[samplesWritten*2+1] = (int16_t)r;
                    } else {
                        int32_t m = ((int32_t)temp[i] * gain_fp) >> 8;
                        if (m > 32767) m = 32767; else if (m < -32768) m = -32768;
                        dest[samplesWritten*2]   = (int16_t)m;
                        dest[samplesWritten*2+1] = (int16_t)m;
                    }
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
    }

    if (samplesWritten > 0) {
        analyze_audio_beat(dest, samplesWritten);
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

static void audio_thread_entry(void* arg) {
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

static void update_audio_stream_fallback(void) {
    if (audioThread != NULL) return;
    if (!isPlaying || isPaused) return;
    step_audio_buffers();
}

static int find_next_song_index(int fromIdx, int step) {
    if (fileCount <= 0) return -1;
    for (int k = 1; k <= fileCount; k++) {
        int idx = (fromIdx + step * k) % fileCount;
        if (idx < 0) idx += fileCount;
        if (!fileList[idx].is_dir) return idx;
    }
    return -1;
}

static int find_random_song_index(int currentIdx) {
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

static void play_track_by_index(int idx, TransitionDir dir) {
    if (idx < 0 || idx >= fileCount || fileList[idx].is_dir) return;
    selectedFile = idx;
    if (selectedFile < scrollOffset) scrollOffset = selectedFile;
    if (selectedFile >= scrollOffset + CFG_LIST_VISIBLE_ROWS) scrollOffset = selectedFile - (CFG_LIST_VISIBLE_ROWS - 1);
    if (scrollOffset < 0) scrollOffset = 0;

    char fp[1024];
    snprintf(fp, sizeof(fp), "%s/%s", currentDir, fileList[selectedFile].name);
    start_playback(fp, fileList[selectedFile].name, dir);
}

static void seek_current_track(float targetSec) {
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
    lastVisualBeatTime = targetSec;
    songTime = targetSec;
    currentLrcIndex = -1;
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

static void draw_sprite_centered_tinted(int spriteIdx, float cx, float cy, float scaleX, float scaleY, const C2D_ImageTint* tint) {
    C2D_Image img = C2D_SpriteSheetGetImage(spriteSheet, spriteIdx);
    float w = (float)img.subtex->width * scaleX;
    float h = (float)img.subtex->height * scaleY;
    C2D_DrawImageAt(img, cx - (w * 0.5f), cy - (h * 0.5f), 0.70f, tint, scaleX, scaleY);
}

static void draw_sprite_centered(int spriteIdx, float cx, float cy, float scaleX, float scaleY) {
    draw_sprite_centered_tinted(spriteIdx, cx, cy, scaleX, scaleY, NULL);
}

static void draw_perspective_cover(C3D_Tex* tex, float cx, float cy, float scale, float shearY, float rotAngle, float depth, u32 tintColor) {
    if (!tex) return;
    C2D_Image cImg = { tex, &coverSubtex };
    C2D_ImageTint tint;
    if (tintColor != 0xFFFFFFFF) {
        u8 alpha = (u8)((tintColor >> 24) & 0xFF);
        
        C2D_PlainImageTint(&tint, C2D_Color32(0, 0, 0, alpha), 0.42f);
    }
    C3D_Mtx savedView;
    C2D_ViewSave(&savedView);

    C2D_ViewTranslate(cx, cy);
    if (rotAngle != 0.0f) C2D_ViewRotate(rotAngle);
    if (shearY != 0.0f)   C2D_ViewShear(0.0f, shearY);
    C2D_ViewScale(scale, scale);

    C2D_DrawImageAt(cImg, -64.0f, -64.0f, depth, (tintColor == 0xFFFFFFFF) ? NULL : &tint, 1.0f, 1.0f);

    C2D_ViewRestore(&savedView);
}

static void draw_perspective_quad_cover(
    C3D_Tex* tex,
    float x0, float y0,  
    float x1, float y1,  
    float x2, float y2,  
    float x3, float y3,  
    float depth, u32 tintColor)
{
    if (!tex) return;

    float h_left = y3 - y0;
    float h_right = y2 - y1;
    if (h_left <= 0.0f || h_right <= 0.0f) return;

    if (depth >= CFG_DEPTH_COVER_MAIN - 0.05f) {
        u32 backColor = C2D_Color32(10, 11, 14, 255);
        C2D_DrawTriangle(x0, y0, backColor, x1, y1, backColor, x2, y2, backColor, depth - 0.001f);
        C2D_DrawTriangle(x0, y0, backColor, x2, y2, backColor, x3, y3, backColor, depth - 0.001f);
    }

    float q = h_left / h_right;
    const int NUM_STRIPS = CFG_PERSPECTIVE_SLICES;

    C2D_ImageTint tint;
    bool hasTint = (tintColor != 0xFFFFFFFF);
    if (hasTint) {
        
        if (depth >= CFG_DEPTH_COVER_SHADOW - 0.01f && depth <= CFG_DEPTH_COVER_SHADOW + 0.01f) {
            C2D_PlainImageTint(&tint, tintColor, 1.0f);
        } else {
            float alpha = (float)((tintColor >> 24) & 0xFF) / 255.0f;
            C2D_AlphaImageTint(&tint, alpha);
        }
    }

    Tex3DS_SubTexture sliceSubtex;
    sliceSubtex.top = 1.0f;
    sliceSubtex.bottom = 0.0f;
    sliceSubtex.width = 128;
    sliceSubtex.height = 128;

    for (int i = 0; i < NUM_STRIPS; i++) {
        float s0 = (float)i / (float)NUM_STRIPS;
        float s1 = (float)(i + 1) / (float)NUM_STRIPS;

        float u0 = s0 / (s0 + (1.0f - s0) * q);
        float u1 = s1 / (s1 + (1.0f - s1) * q);

        float xt0 = x0 + s0 * (x1 - x0);
        float yt0 = y0 + s0 * (y1 - y0);
        float xt1 = x0 + s1 * (x1 - x0);
        float yt1 = y0 + s1 * (y1 - y0);

        float xb0 = x3 + s0 * (x2 - x3);
        float yb0 = y3 + s0 * (y2 - y3);
        float xb1 = x3 + s1 * (x2 - x3);
        float yb1 = y3 + s1 * (y2 - y3);

        float sx0 = (xt0 + xb0) * 0.5f;
        float sx1 = (xt1 + xb1) * 0.5f;
        float sy_top = (yt0 < yt1) ? yt0 : yt1;
        float sy_bot = (yb0 > yb1) ? yb0 : yb1;

        float sw = (sx1 - sx0) + CFG_SUBPIXEL_OVERLAP;
        float sh = (sy_bot - sy_top) + 0.6f;

        sliceSubtex.left = u0;
        sliceSubtex.right = u1;

        C2D_Image sliceImg = { tex, &sliceSubtex };
        C2D_DrawParams params = {
            .pos = { .x = sx0, .y = sy_top, .w = sw, .h = sh },
            .center = { 0.0f, 0.0f },
            .depth = depth,
            .angle = 0.0f
        };
        C2D_DrawImage(sliceImg, &params, hasTint ? &tint : NULL);
    }
}
static void render_top_screen(void) {
    C2D_TargetClear(topTarget, C2D_Color32(10, 11, 14, 255));
    C2D_SceneBegin(topTarget);

    if (g_lastfm_login_active) {
        if (hasCoverCurrent) {
            C2D_Image cImg = { &coverTexCurrent, &coverSubtex };
            C2D_ImageTint bgTint;
            C2D_PlainImageTint(&bgTint, C2D_Color32(0, 0, 0, 255), 0.30f);
            C2D_DrawParams bgParams = {
                .pos    = { .x = -20.0f + coverBgPanX, .y = -20.0f + coverBgPanY,
                            .w = 440.0f, .h = 280.0f },
                .center = { 0.0f, 0.0f },
                .depth  = CFG_DEPTH_BG_BLUR,
                .angle  = 0.0f
            };
            C2D_DrawImage(cImg, &bgParams, &bgTint);
        }
        render_lastfm_login_screen();
        return;
    }

    if (hasCoverCurrent) {
        C2D_Image cImg = { &coverTexCurrent, &coverSubtex };
        C2D_ImageTint bgTint;
        
        C2D_PlainImageTint(&bgTint, C2D_Color32(0, 0, 0, 255), 0.30f);

        C2D_DrawParams bgParams = {
            .pos = {
                .x = -20.0f + coverBgPanX,
                .y = -20.0f + coverBgPanY,
                .w = 440.0f,
                .h = 280.0f
            },
            .center = { 0.0f, 0.0f },
            .depth = CFG_DEPTH_BG_BLUR,
            .angle = 0.0f
        };
        C2D_DrawImage(cImg, &bgParams, &bgTint);
        C2D_DrawRectSolid(0.0f, 0.0f, CFG_DEPTH_BG_DIMMER, 400.0f, 240.0f, C2D_Color32(8, 9, 12, 165));
    }

    time_t rawtime;
    struct tm* ti;
    time(&rawtime);
    ti = localtime(&rawtime);
    char timeStr[32];
    int hour12 = ti->tm_hour % 12;
    if (hour12 == 0) hour12 = 12;
    snprintf(timeStr, sizeof(timeStr), "%d:%02d %s", hour12, ti->tm_min, (ti->tm_hour >= 12) ? "PM" : "AM");
    draw_custom_font(CFG_CLOCK_X, CFG_CLOCK_Y, CFG_CLOCK_SCALE, C2D_Color32(255, 255, 255, 255), timeStr);

    int wifiIcon = isWifiConnected
        ? ((global10FpsFrame == 0) ? sprites_wifi1_idx : sprites_wifi2_idx)
        : ((global10FpsFrame == 0) ? sprites_nowifi1_idx : sprites_nowifi2_idx);
    draw_sprite_centered(wifiIcon, CFG_WIFI_ICON_X, CFG_WIFI_ICON_Y, CFG_WIFI_ICON_SCALE, CFG_WIFI_ICON_SCALE);

    const float Q_X0 = CFG_QUAD_X0, Q_Y0 = CFG_QUAD_Y0;
    const float Q_X1 = CFG_QUAD_X1, Q_Y1 = CFG_QUAD_Y1;
    const float Q_X2 = CFG_QUAD_X2, Q_Y2 = CFG_QUAD_Y2;
    const float Q_X3 = CFG_QUAD_X3, Q_Y3 = CFG_QUAD_Y3;

    if (coverTransDir == TRANS_NEXT) {
        float t = coverTransProgress;
        float ease = t * t * (3.0f - 2.0f * t);

        if (hasCoverNext) {
            draw_perspective_cover(&coverTexNext, CFG_PEEK_BOT_X, CFG_PEEK_BOT_Y, CFG_PEEK_COVER_SCALE * ease, 0.0f, 0.0f, CFG_DEPTH_PEEK_COVERS, C2D_Color32(130, 135, 145, (u32)(CFG_PEEK_COVER_ALPHA * ease)));
        }

        if (hasCoverOld) {
            float ox0 = Q_X0 * (1.0f - ease) + (CFG_PEEK_TOP_X - CFG_COVER_HALF_SIZE) * ease;
            float oy0 = Q_Y0 * (1.0f - ease) + (CFG_PEEK_TOP_Y - CFG_COVER_HALF_SIZE) * ease;
            float ox1 = Q_X1 * (1.0f - ease) + (CFG_PEEK_TOP_X + CFG_COVER_HALF_SIZE) * ease;
            float oy1 = Q_Y1 * (1.0f - ease) + (CFG_PEEK_TOP_Y - CFG_COVER_HALF_SIZE) * ease;
            float ox2 = Q_X2 * (1.0f - ease) + (CFG_PEEK_TOP_X + CFG_COVER_HALF_SIZE) * ease;
            float oy2 = Q_Y2 * (1.0f - ease) + (CFG_PEEK_TOP_Y + CFG_COVER_HALF_SIZE) * ease;
            float ox3 = Q_X3 * (1.0f - ease) + (CFG_PEEK_TOP_X - CFG_COVER_HALF_SIZE) * ease;
            float oy3 = Q_Y3 * (1.0f - ease) + (CFG_PEEK_TOP_Y + CFG_COVER_HALF_SIZE) * ease;
            u32 oldAlpha = (u32)(255.0f * (1.0f - ease * 0.35f));

            draw_perspective_quad_cover(&coverTexOld,
                ox0, oy0, ox1, oy1, ox2, oy2, ox3, oy3,
                CFG_DEPTH_TRANS_OLD, C2D_Color32(255, 255, 255, oldAlpha));
        }

        if (hasCoverCurrent) {
            float nx0 = (CFG_PEEK_BOT_X - CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_X0 * ease;
            float ny0 = (CFG_PEEK_BOT_Y - CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_Y0 * ease;
            float nx1 = (CFG_PEEK_BOT_X + CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_X1 * ease;
            float ny1 = (CFG_PEEK_BOT_Y - CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_Y1 * ease;
            float nx2 = (CFG_PEEK_BOT_X + CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_X2 * ease;
            float ny2 = (CFG_PEEK_BOT_Y + CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_Y2 * ease;
            float nx3 = (CFG_PEEK_BOT_X - CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_X3 * ease;
            float ny3 = (CFG_PEEK_BOT_Y + CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_Y3 * ease;

            
            draw_perspective_quad_cover(&coverTexCurrent,
                nx0 + CFG_COVER_SHADOW_OFF_X, ny0 + CFG_COVER_SHADOW_OFF_Y,
                nx1 + CFG_COVER_SHADOW_OFF_X, ny1 + CFG_COVER_SHADOW_OFF_Y,
                nx2 + CFG_COVER_SHADOW_OFF_X, ny2 + CFG_COVER_SHADOW_OFF_Y,
                nx3 + CFG_COVER_SHADOW_OFF_X, ny3 + CFG_COVER_SHADOW_OFF_Y,
                CFG_DEPTH_COVER_SHADOW, C2D_Color32(0, 0, 0, (u32)(CFG_COVER_SHADOW_ALPHA * ease)));

            
            draw_perspective_quad_cover(&coverTexCurrent,
                nx0, ny0, nx1, ny1, nx2, ny2, nx3, ny3,
                CFG_DEPTH_COVER_MAIN, 0xFFFFFFFF);
        }
    } else if (coverTransDir == TRANS_PREV) {
        float t = coverTransProgress;
        float ease = t * t * (3.0f - 2.0f * t);

        if (hasCoverPrev) {
            draw_perspective_cover(&coverTexPrev, CFG_PEEK_TOP_X, CFG_PEEK_TOP_Y, CFG_PEEK_COVER_SCALE * ease, 0.0f, 0.0f, CFG_DEPTH_PEEK_COVERS, C2D_Color32(130, 135, 145, (u32)(CFG_PEEK_COVER_ALPHA * ease)));
        }
        if (hasCoverOld) {
            float ox0 = Q_X0 * (1.0f - ease) + (CFG_PEEK_BOT_X - CFG_COVER_HALF_SIZE) * ease;
            float oy0 = Q_Y0 * (1.0f - ease) + (CFG_PEEK_BOT_Y - CFG_COVER_HALF_SIZE) * ease;
            float ox1 = Q_X1 * (1.0f - ease) + (CFG_PEEK_BOT_X + CFG_COVER_HALF_SIZE) * ease;
            float oy1 = Q_Y1 * (1.0f - ease) + (CFG_PEEK_BOT_Y - CFG_COVER_HALF_SIZE) * ease;
            float ox2 = Q_X2 * (1.0f - ease) + (CFG_PEEK_BOT_X + CFG_COVER_HALF_SIZE) * ease;
            float oy2 = Q_Y2 * (1.0f - ease) + (CFG_PEEK_BOT_Y + CFG_COVER_HALF_SIZE) * ease;
            float ox3 = Q_X3 * (1.0f - ease) + (CFG_PEEK_BOT_X - CFG_COVER_HALF_SIZE) * ease;
            float oy3 = Q_Y3 * (1.0f - ease) + (CFG_PEEK_BOT_Y + CFG_COVER_HALF_SIZE) * ease;
            u32 oldAlpha = (u32)(255.0f * (1.0f - ease * 0.35f));

            draw_perspective_quad_cover(&coverTexOld,
                ox0, oy0, ox1, oy1, ox2, oy2, ox3, oy3,
                CFG_DEPTH_TRANS_OLD, C2D_Color32(255, 255, 255, oldAlpha));
        }
        if (hasCoverCurrent) {
            float nx0 = (CFG_PEEK_TOP_X - CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_X0 * ease;
            float ny0 = (CFG_PEEK_TOP_Y - CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_Y0 * ease;
            float nx1 = (CFG_PEEK_TOP_X + CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_X1 * ease;
            float ny1 = (CFG_PEEK_TOP_Y - CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_Y1 * ease;
            float nx2 = (CFG_PEEK_TOP_X + CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_X2 * ease;
            float ny2 = (CFG_PEEK_TOP_Y + CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_Y2 * ease;
            float nx3 = (CFG_PEEK_TOP_X - CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_X3 * ease;
            float ny3 = (CFG_PEEK_TOP_Y + CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_Y3 * ease;

            draw_perspective_quad_cover(&coverTexCurrent,
                nx0 + CFG_COVER_SHADOW_OFF_X, ny0 + CFG_COVER_SHADOW_OFF_Y,
                nx1 + CFG_COVER_SHADOW_OFF_X, ny1 + CFG_COVER_SHADOW_OFF_Y,
                nx2 + CFG_COVER_SHADOW_OFF_X, ny2 + CFG_COVER_SHADOW_OFF_Y,
                nx3 + CFG_COVER_SHADOW_OFF_X, ny3 + CFG_COVER_SHADOW_OFF_Y,
                CFG_DEPTH_COVER_SHADOW, C2D_Color32(0, 0, 0, (u32)(CFG_COVER_SHADOW_ALPHA * ease)));

            draw_perspective_quad_cover(&coverTexCurrent,
                nx0, ny0, nx1, ny1, nx2, ny2, nx3, ny3,
                CFG_DEPTH_COVER_MAIN, 0xFFFFFFFF);
        }

    } else {
        if (hasCoverPrev) {
            draw_perspective_cover(&coverTexPrev, CFG_PEEK_TOP_X, CFG_PEEK_TOP_Y, CFG_PEEK_COVER_SCALE, 0.0f, 0.0f, CFG_DEPTH_PEEK_COVERS, C2D_Color32(130, 135, 145, CFG_PEEK_COVER_ALPHA));
        }
        
        if (hasCoverNext) {
            draw_perspective_cover(&coverTexNext, CFG_PEEK_BOT_X, CFG_PEEK_BOT_Y, CFG_PEEK_COVER_SCALE, 0.0f, 0.0f, CFG_DEPTH_PEEK_COVERS, C2D_Color32(130, 135, 145, CFG_PEEK_COVER_ALPHA));
        }
        
        if (hasCoverCurrent) {
            draw_perspective_quad_cover(&coverTexCurrent,
                Q_X0 + CFG_COVER_SHADOW_OFF_X, Q_Y0 + CFG_COVER_SHADOW_OFF_Y,
                Q_X1 + CFG_COVER_SHADOW_OFF_X, Q_Y1 + CFG_COVER_SHADOW_OFF_Y,
                Q_X2 + CFG_COVER_SHADOW_OFF_X, Q_Y2 + CFG_COVER_SHADOW_OFF_Y,
                Q_X3 + CFG_COVER_SHADOW_OFF_X, Q_Y3 + CFG_COVER_SHADOW_OFF_Y,
                CFG_DEPTH_COVER_SHADOW, C2D_Color32(0, 0, 0, CFG_COVER_SHADOW_ALPHA));

            
            draw_perspective_quad_cover(&coverTexCurrent,
                Q_X0, Q_Y0,
                Q_X1, Q_Y1,
                Q_X2, Q_Y2,
                Q_X3, Q_Y3,
                CFG_DEPTH_COVER_MAIN, 0xFFFFFFFF);
        }
    }
    
    if (lyricsEnabled && lrcLineCount > 0 && currentLrcIndex >= 0) {
        float lEase = lyricAnimTimer * lyricAnimTimer * (3.0f - 2.0f * lyricAnimTimer);
        float yOff = (1.0f - lEase) * CFG_LYRICS_SLIDE_DIST;

        if (currentLrcIndex > 0 && lrcLines[currentLrcIndex - 1].text[0] != '\0') {
            u32 prevAlpha = (u32)(140.0f * (1.0f - lEase * 0.6f));
            draw_wrapped_lyrics(CFG_LYRICS_CENTER_X, CFG_LYRICS_PREV_Y + yOff, CFG_LYRICS_MAX_WIDTH, CFG_LYRICS_SUB_SCALE, C2D_Color32(255, 255, 255, prevAlpha), lrcLines[currentLrcIndex - 1].text);
        }
        if (lrcLines[currentLrcIndex].text[0] != '\0') {
            u32 activeAlpha = (u32)(60.0f + 195.0f * lEase);
            draw_wrapped_lyrics(CFG_LYRICS_CENTER_X, CFG_LYRICS_ACTIVE_Y + yOff, CFG_LYRICS_MAX_WIDTH, CFG_LYRICS_ACTIVE_SCALE, C2D_Color32(255, 255, 255, activeAlpha), lrcLines[currentLrcIndex].text);
        } else {
            draw_custom_font_centered(CFG_LYRICS_CENTER_X, CFG_LYRICS_ACTIVE_Y + yOff, CFG_LYRICS_SUB_SCALE, C2D_Color32(255, 255, 255, 180), "~ ~ ~");
        }

        if (currentLrcIndex < lrcLineCount - 1 && lrcLines[currentLrcIndex + 1].text[0] != '\0') {
            u32 nextAlpha = (u32)(140.0f * lEase);
            draw_wrapped_lyrics(CFG_LYRICS_CENTER_X, CFG_LYRICS_NEXT_Y + yOff, CFG_LYRICS_MAX_WIDTH, CFG_LYRICS_SUB_SCALE, C2D_Color32(255, 255, 255, nextAlpha), lrcLines[currentLrcIndex + 1].text);
        }
    } else {
        
        int songIcon   = (global10FpsFrame == 0) ? sprites_song1_idx : sprites_song2_idx;
        int artistIcon = (global10FpsFrame == 0) ? sprites_artist1_idx : sprites_artist2_idx;
        int albumIcon  = (global10FpsFrame == 0) ? sprites_album1_idx : sprites_album2_idx;

        
        draw_sprite_centered(songIcon, CFG_META_ICONS_X, CFG_META_ICON_SONG_Y, CFG_META_ICONS_SCALE, CFG_META_ICONS_SCALE);
        draw_marquee_font(CFG_META_TEXT_X, CFG_META_TITLE_Y, CFG_META_TEXT_WIDTH, CFG_META_TITLE_SCALE, C2D_Color32(255, 255, 255, 255), currentTrackTitle, uiElapsedTime);

        
        draw_sprite_centered(artistIcon, CFG_META_ICONS_X, CFG_META_ICON_ARTIST_Y, CFG_META_ICONS_SCALE, CFG_META_ICONS_SCALE);
        draw_marquee_font(CFG_META_TEXT_X, CFG_META_ARTIST_Y, CFG_META_TEXT_WIDTH, CFG_META_ARTIST_SCALE, C2D_Color32(255, 255, 255, 255), currentTrackArtist, uiElapsedTime);

        
        draw_sprite_centered(albumIcon, CFG_META_ICONS_X, CFG_META_ICON_ALBUM_Y, CFG_META_ICONS_SCALE, CFG_META_ICONS_SCALE);
        draw_marquee_font(CFG_META_TEXT_X, CFG_META_ALBUM_Y, CFG_META_TEXT_WIDTH, CFG_META_ALBUM_SCALE, C2D_Color32(255, 255, 255, 255), currentTrackAlbum, uiElapsedTime);
    }

    int minE = (int)songTime / 60;
    int secE = (int)songTime % 60;
    int minT = (int)songDuration / 60;
    int secT = (int)songDuration % 60;
    char timeStrFull[32];
    snprintf(timeStrFull, sizeof(timeStrFull), "%d:%02d/%d:%02d", minE, secE, minT, secT);
    draw_custom_font(CFG_TIME_COUNTER_X, CFG_TIME_COUNTER_Y, CFG_TIME_COUNTER_SCALE, C2D_Color32(255, 255, 255, 255), timeStrFull);

    float waveCenterY = CFG_WAVE_CENTER_Y;
    float progress = (songDuration > 0.0f) ? (songTime / songDuration) : 0.0f;
    if (progress > 1.0f) progress = 1.0f;
    int activeCols = (int)(progress * (float)NUM_WAVE_SAMPLES);

    for (int i = 0; i < NUM_WAVE_SAMPLES; i += CFG_WAVE_BAR_STEP) {
        float colX = CFG_WAVE_START_X + (float)i;
        float h = fullSongWaveform[i] * CFG_WAVE_MAX_HEIGHT;
        if (h < 1.5f) h = 1.5f;

        u32 col = (i <= activeCols) ? C2D_Color32(255, 255, 255, 255) : C2D_Color32(110, 115, 130, 140);
        C2D_DrawRectSolid(colX, waveCenterY - h, CFG_DEPTH_WAVEFORM, CFG_WAVE_BAR_WIDTH, h * 2.0f, col);
    }

    if (activeCols >= 0 && activeCols < NUM_WAVE_SAMPLES) {
        float playheadX = CFG_WAVE_START_X + (float)activeCols;
        float phH = fullSongWaveform[activeCols] * CFG_WAVE_MAX_HEIGHT + 3.0f;
        C2D_DrawRectSolid(playheadX - 0.5f, waveCenterY - phH, CFG_DEPTH_PLAYHEAD, CFG_PLAYHEAD_WIDTH, phH * 2.0f, C2D_Color32(255, 255, 255, 255));
    }
}

static void render_bottom_screen(void) {
    C2D_TargetClear(botTarget, C2D_Color32(185, 150, 115, 255));
    C2D_SceneBegin(botTarget);

    C2D_Image imgBg = C2D_SpriteSheetGetImage(spriteSheet, sprites_bg_idx);
    int startX = ((int)bgScrollX % 100) - 100;
    int startY = ((int)bgScrollY % 100) - 100;

    for (int x = startX; x < 320; x += 100) {
        for (int y = startY; y < 240; y += 100) {
            C2D_DrawImageAt(imgBg, (float)x, (float)y, 0.1f, NULL, 1.0f, 1.0f);
        }
    }

    int gustinaSpriteIdx = sprites_gustinaNothing1_idx;

    if (!isPlaying || isPaused) {
        gustinaSpriteIdx = (global10FpsFrame == 0) ? sprites_gustinaNothing1_idx : sprites_gustinaNothing2_idx;
    } else {
        if (gustinaState == GUSTINA_BOP_LEFT) {
            if (gustinaFrame == 0) gustinaSpriteIdx = sprites_gustinaBopLeft1_idx;
            else if (gustinaFrame == 1) gustinaSpriteIdx = sprites_gustinaBopLeft2_idx;
            else gustinaSpriteIdx = sprites_gustinaBopLeft3_idx;
        } else if (gustinaState == GUSTINA_BOP_RIGHT) {
            if (gustinaFrame == 0) gustinaSpriteIdx = sprites_gustinaBopRight1_idx;
            else if (gustinaFrame == 1) gustinaSpriteIdx = sprites_gustinaBopRight2_idx;
            else gustinaSpriteIdx = sprites_gustinaBopRight3_idx;
        } else {
            gustinaSpriteIdx = (global10FpsFrame == 0) ? sprites_gustinaIdle1_idx : sprites_gustinaIdle2_idx;
        }
    }

    int customIdx = -1;
    if (gustinaSpriteIdx == sprites_gustinaNothing1_idx) customIdx = 0;
    else if (gustinaSpriteIdx == sprites_gustinaNothing2_idx) customIdx = 1;
    else if (gustinaSpriteIdx == sprites_gustinaIdle1_idx) customIdx = 2;
    else if (gustinaSpriteIdx == sprites_gustinaIdle2_idx) customIdx = 3;
    else if (gustinaSpriteIdx == sprites_gustinaBopLeft1_idx) customIdx = 4;
    else if (gustinaSpriteIdx == sprites_gustinaBopLeft2_idx) customIdx = 5;
    else if (gustinaSpriteIdx == sprites_gustinaBopLeft3_idx) customIdx = 6;
    else if (gustinaSpriteIdx == sprites_gustinaBopRight1_idx) customIdx = 7;
    else if (gustinaSpriteIdx == sprites_gustinaBopRight2_idx) customIdx = 8;
    else if (gustinaSpriteIdx == sprites_gustinaBopRight3_idx) customIdx = 9;

    float gustinaX = (bottomScreenMode == 1) ? (CFG_GUSTINA_X + 18.0f) : CFG_GUSTINA_X;

    if (customIdx >= 0 && hasCustomGustina[customIdx]) {
        C2D_Image customImg = { &customGustinaTex[customIdx], &customGustinaSubtex[customIdx] };
        float sh = (customGustinaSubtex[customIdx].height > 0) ? (float)customGustinaSubtex[customIdx].height : 198.0f;
        float sw = (customGustinaSubtex[customIdx].width  > 0) ? (float)customGustinaSubtex[customIdx].width  : 160.0f;
        float drawScale = (198.0f * CFG_GUSTINA_SCALE_Y) / sh;
        float w = sw * drawScale;
        float h = sh * drawScale;
        C2D_DrawImageAt(customImg, gustinaX - (w * 0.5f), CFG_GUSTINA_Y - (h * 0.5f), 0.70f, NULL, drawScale, drawScale);
    } else {
        draw_sprite_centered(gustinaSpriteIdx, gustinaX, CFG_GUSTINA_Y, CFG_GUSTINA_SCALE_X, CFG_GUSTINA_SCALE_Y);
    }

    if (bottomScreenMode == 0) {
        draw_sprite_centered(wheelTouchSprite, CFG_WHEEL_CENTER_X, CFG_WHEEL_CENTER_Y, CFG_WHEEL_SCALE, CFG_WHEEL_SCALE);
        int repIcon = (global10FpsFrame == 0) ? sprites_repeat1_idx : sprites_repeat2_idx;
        C2D_ImageTint repTint;
        if (repeatEnabled) C2D_PlainImageTint(&repTint, C2D_Color32(120, 255, 170, 255), 0.35f);
        else               C2D_AlphaImageTint(&repTint, 0.35f);
        draw_sprite_centered_tinted(repIcon, CFG_REPEAT_ICON_X, CFG_REPEAT_ICON_Y, CFG_REPEAT_ICON_SCALE, CFG_REPEAT_ICON_SCALE, &repTint);

        int shufIcon = (global10FpsFrame == 0) ? sprites_suffle1_idx : sprites_suffle2_idx;
        C2D_ImageTint shufTint;
        if (shuffleEnabled) C2D_PlainImageTint(&shufTint, C2D_Color32(120, 255, 170, 255), 0.35f);
        else                C2D_AlphaImageTint(&shufTint, 0.35f);
        draw_sprite_centered_tinted(shufIcon, CFG_SHUFFLE_ICON_X, CFG_SHUFFLE_ICON_Y, CFG_SHUFFLE_ICON_SCALE, CFG_SHUFFLE_ICON_SCALE, &shufTint);

        int volIcon = (global10FpsFrame == 0) ? sprites_volume1_idx : sprites_volume2_idx;
        draw_sprite_centered(volIcon, CFG_VOL_ICON_X, CFG_VOL_ICON_Y, CFG_VOL_ICON_SCALE, CFG_VOL_ICON_SCALE);

        char volTxt[32];
        int pct = (int)(volumeGain * 100.0f);
        snprintf(volTxt, sizeof(volTxt), "%d%%%s", pct, pct > 100 ? "" : "");
        u32 vCol = (pct > 100) ? C2D_Color32(240, 45, 45, 255) : C2D_Color32(255, 255, 255, 255);
        draw_custom_font(CFG_VOL_TEXT_X, CFG_VOL_TEXT_Y, CFG_VOL_TEXT_SCALE, vCol, volTxt);

        int nextIcon = (global10FpsFrame == 0) ? sprites_nextSong1_idx : sprites_nextSong2_idx;
        draw_sprite_centered(nextIcon, CFG_NEXT_ICON_X, CFG_NEXT_ICON_Y, CFG_NEXT_ICON_SCALE, CFG_NEXT_ICON_SCALE);

        int nextIdx = find_next_song_index(selectedFile, 1);
        if (repeatEnabled && fileCount > 0 && !fileList[selectedFile].is_dir) {
            draw_marquee_font(CFG_NEXT_TEXT_X, CFG_NEXT_TEXT_Y, CFG_NEXT_TEXT_WIDTH, CFG_NEXT_TEXT_SCALE, C2D_Color32(255, 255, 255, 255), get_next_song_display(selectedFile, true), uiElapsedTime);
        } else if (shuffleEnabled) {
            if (upcomingShuffleIdx < 0 || upcomingShuffleIdx >= fileCount || fileList[upcomingShuffleIdx].is_dir ||
                (fileCount > 1 && upcomingShuffleIdx == selectedFile)) {
                upcomingShuffleIdx = find_random_song_index(selectedFile);
            }
            if (upcomingShuffleIdx >= 0) {
                draw_marquee_font(CFG_NEXT_TEXT_X, CFG_NEXT_TEXT_Y, CFG_NEXT_TEXT_WIDTH, CFG_NEXT_TEXT_SCALE, C2D_Color32(255, 255, 255, 255), get_next_song_display(upcomingShuffleIdx, false), uiElapsedTime);
            }
        } else if (nextIdx >= 0) {
            draw_marquee_font(CFG_NEXT_TEXT_X, CFG_NEXT_TEXT_Y, CFG_NEXT_TEXT_WIDTH, CFG_NEXT_TEXT_SCALE, C2D_Color32(255, 255, 255, 255), get_next_song_display(nextIdx, false), uiElapsedTime);
        }

        draw_custom_font(10.0f, 5.0f, 0.40f, C2D_Color32(255, 255, 255, 255), "Y: Files");
        u32 lrcToggleCol = lyricsEnabled ? C2D_Color32(130, 255, 190, 255) : C2D_Color32(180, 180, 180, 190);
        char lrcLabel[48];
        if (lyricsEnabled && lyricsDebugStatus[0] != '\0') {
            snprintf(lrcLabel, sizeof(lrcLabel), "X: LRC [ON] (%s)", lyricsDebugStatus);
        } else {
            snprintf(lrcLabel, sizeof(lrcLabel), lyricsEnabled ? "X: LRC [ON]" : "X: LRC [OFF]");
        }
        draw_custom_font(106.0f, 5.0f, 0.40f, lrcToggleCol, lrcLabel);

        int lfmIcon = (global10FpsFrame == 0) ? sprites_lastfm1_idx : sprites_lastfm2_idx;
        if (lastfm_has_session) {
            C2D_ImageTint lfmTint;
            C2D_PlainImageTint(&lfmTint, C2D_Color32(90, 255, 130, 255), 0.70f);
            draw_sprite_centered_tinted(lfmIcon, CFG_LASTFM_BTN_X, CFG_LASTFM_BTN_Y,
                                        CFG_LASTFM_BTN_SCALE, CFG_LASTFM_BTN_SCALE, &lfmTint);
        } else {
            draw_sprite_centered(lfmIcon, CFG_LASTFM_BTN_X, CFG_LASTFM_BTN_Y,
                                 CFG_LASTFM_BTN_SCALE, CFG_LASTFM_BTN_SCALE);
        }

    } else {
        int folderIcon = (global10FpsFrame == 0) ? sprites_folder1_idx : sprites_folder2_idx;
        draw_sprite_centered(folderIcon, CFG_LIST_FOLDER_ICON_X, CFG_LIST_FOLDER_ICON_Y, 0.55f, 0.55f);

        char dirShort[48];
        utf8_truncate_chars(currentDir, dirShort, sizeof(dirShort), 18);
        draw_custom_font(CFG_LIST_DIR_TEXT_X, CFG_LIST_DIR_TEXT_Y, 0.34f, C2D_Color32(255, 255, 255, 255), dirShort);

        u32 searchBoxCol = (searchFilter[0] != '\0') ? C2D_Color32(60, 175, 145, 210) : C2D_Color32(35, 35, 45, 175);
        C2D_DrawRectSolid(226.0f, 4.0f, 0.45f, 88.0f, 20.0f, searchBoxCol);
        draw_custom_font(232.0f, 220.0f, 0.40f, C2D_Color32(255, 255, 255, 255), "SEL: Search");

        if (searchFilter[0] != '\0') {
            char fPill[48];
            snprintf(fPill, sizeof(fPill), "Search filter: %s", searchFilter);
            char fTrunc[40];
            utf8_truncate_chars(fPill, fTrunc, sizeof(fTrunc), 24);
            draw_custom_font(30.0f, 28.0f, 0.42f, C2D_Color32(140, 255, 210, 255), fTrunc);
        }

        int cursorIcon = (global10FpsFrame == 0) ? sprites_cursor1_idx : sprites_cursor2_idx;

        if (fileCount == 0) {
            draw_custom_font(24.0f, 80.0f, 0.38f, C2D_Color32(255, 220, 160, 255), "nothingburger :(");
        }
        for (int i = 0; i < CFG_LIST_VISIBLE_ROWS; i++) {
            int idx = scrollOffset + i;
            float rowY = CFG_LIST_ROW_START_Y + (i * CFG_LIST_ROW_HEIGHT);
            if (idx < fileCount) {
                char rowPath[1024];
                if (strcmp(currentDir, "sdmc:/") == 0) snprintf(rowPath, sizeof(rowPath), "sdmc:/%s", fileList[idx].name);
                else snprintf(rowPath, sizeof(rowPath), "%s/%s", currentDir, fileList[idx].name);
                bool isActiveTrack = (isPlaying && !fileList[idx].is_dir && strcmp(currentTrackPath, rowPath) == 0);

                if (isActiveTrack && idx != selectedFile) {
                    C2D_DrawRectSolid(CFG_LIST_BOX_X, rowY - 8.0f, 0.35f, CFG_LIST_BOX_WIDTH, CFG_LIST_BOX_HEIGHT, C2D_Color32(60, 180, 140, 95));
                }
                if (idx == selectedFile) {
                    u32 selCol = isActiveTrack ? C2D_Color32(175, 240, 210, 200) : C2D_Color32(245, 230, 210, 185);
                    C2D_DrawRectSolid(CFG_LIST_BOX_X, rowY - 8.0f, 0.4f, CFG_LIST_BOX_WIDTH, CFG_LIST_BOX_HEIGHT, selCol);
                    draw_sprite_centered(cursorIcon, CFG_LIST_CURSOR_X, rowY + 6.0f, CFG_LIST_CURSOR_SCALE, CFG_LIST_CURSOR_SCALE);
                }

                u32 col = fileList[idx].is_dir ? C2D_Color32(255, 220, 100, 255) :
                          (isActiveTrack ? C2D_Color32(130, 255, 190, 255) : C2D_Color32(255, 255, 255, 255));
                char dispName[256];
                if (fileList[idx].is_dir) snprintf(dispName, sizeof(dispName), "%s", fileList[idx].name);
                else strip_extension_copy(fileList[idx].name, dispName, sizeof(dispName));

                if (idx == selectedFile) {
                    draw_marquee_font(30.0f, rowY - 1.0f, CFG_LIST_BOX_WIDTH - 28.0f, CFG_LIST_TEXT_SCALE, col, dispName, uiElapsedTime);
                } else {
                    char lineName[64];
                    utf8_truncate_chars(dispName, lineName, sizeof(lineName), 20);
                    draw_custom_font(30.0f, rowY, CFG_LIST_TEXT_SCALE, col, lineName);
                }
            }
        }
        if (fileCount > CFG_LIST_VISIBLE_ROWS) {
            float barTop = CFG_LIST_ROW_START_Y - 6.0f;
            float barH = (float)CFG_LIST_VISIBLE_ROWS * CFG_LIST_ROW_HEIGHT - 4.0f;
            C2D_DrawRectSolid(CFG_LIST_SCROLL_X, barTop, 0.45f, 4.0f, barH, C2D_Color32(0, 0, 0, 110));

            float thumbH = barH * ((float)CFG_LIST_VISIBLE_ROWS / (float)fileCount);
            if (thumbH < 16.0f) thumbH = 16.0f;
            int maxScroll = fileCount - CFG_LIST_VISIBLE_ROWS;
            float ratio = (maxScroll > 0) ? ((float)scrollOffset / (float)maxScroll) : 0.0f;
            float thumbY = barTop + ratio * (barH - thumbH);
            C2D_DrawRectSolid(CFG_LIST_SCROLL_X, thumbY, 0.5f, 4.0f, thumbH, C2D_Color32(255, 255, 255, 220));
        }

        draw_custom_font(10.0f, 220.0f, 0.45f, C2D_Color32(255, 255, 255, 255), "Y: Back");
        char countStr[32];
        snprintf(countStr, sizeof(countStr), "%d/%d L/R", (fileCount > 0 ? selectedFile + 1 : 0), fileCount);
        draw_custom_font(105.0f, 220.0f, 0.45f, C2D_Color32(220, 220, 220, 220), countStr);
    }
}
static void open_search_keyboard(u64* lastTickPtr) {
    SwkbdState swkbd;
    char inputBuf[64];
    strncpy(inputBuf, searchFilter, sizeof(inputBuf) - 1);
    inputBuf[sizeof(inputBuf) - 1] = '\0';

    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, 60);
    swkbdSetInitialText(&swkbd, inputBuf);
    swkbdSetHintText(&swkbd, "Search...");
    swkbdSetButton(&swkbd, SWKBD_BUTTON_LEFT, "Clear", false);
    swkbdSetButton(&swkbd, SWKBD_BUTTON_RIGHT, "Search", true);
    swkbdSetFeatures(&swkbd, SWKBD_DARKEN_TOP_SCREEN | SWKBD_ALLOW_HOME);

    SwkbdButton btn = swkbdInputText(&swkbd, inputBuf, sizeof(inputBuf));
    if (btn == SWKBD_BUTTON_RIGHT) {
        strncpy(searchFilter, inputBuf, sizeof(searchFilter) - 1);
        searchFilter[sizeof(searchFilter) - 1] = '\0';
        apply_file_filter();
        update_all_carousel_covers();
    } else if (btn == SWKBD_BUTTON_LEFT) {
        searchFilter[0] = '\0';
        apply_file_filter();
        update_all_carousel_covers();
    }
    if (lastTickPtr) *lastTickPtr = osGetTime();
}

static void load_gustina_mods(void) {
    const char* filenames[10] = {
        "gustinaNothing1.png", "gustinaNothing2.png",
        "gustinaIdle1.png", "gustinaIdle2.png",
        "gustinaBopLeft1.png", "gustinaBopLeft2.png", "gustinaBopLeft3.png",
        "gustinaBopRight1.png", "gustinaBopRight2.png", "gustinaBopRight3.png"
    };

    for (int i = 0; i < 10; i++) {
        char path[256];
        snprintf(path, sizeof(path), "sdmc:/3ds/MCMP/mods/%s", filenames[i]);
        
        FILE* f = fopen(path, "rb");
        if (f) {
            fclose(f);
            int imgW = 0, imgH = 0, channels = 0;
            uint8_t* pixels = stbi_load(path, &imgW, &imgH, &channels, 4);
            if (pixels && imgW > 0 && imgH > 0) {
                uint8_t* resizedBuf = (uint8_t*)malloc(256 * 256 * 4);
                if (resizedBuf) {
                    memset(resizedBuf, 0, 256 * 256 * 4);
                    int copyW = (imgW < 256) ? imgW : 256;
                    int copyH = (imgH < 256) ? imgH : 256;
                    for (int y = 0; y < copyH; y++) {
                        memcpy(resizedBuf + (y * 256 * 4), pixels + (y * imgW * 4), copyW * 4);
                    }
                    upload_rgba8_to_tex(&customGustinaTex[i], resizedBuf, 256, 256, 256, 256);
                    customGustinaSubtex[i].width  = (u16)copyW;
                    customGustinaSubtex[i].height = (u16)copyH;
                    customGustinaSubtex[i].left   = 0.0f;
                    customGustinaSubtex[i].top    = 1.0f;
                    customGustinaSubtex[i].right  = (float)copyW / 256.0f;
                    customGustinaSubtex[i].bottom = 1.0f - ((float)copyH / 256.0f);
                    hasCustomGustina[i] = true;
                    free(resizedBuf);
                }
                stbi_image_free(pixels);
            }
        }
    }
}


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

static void set_screens_backlight(bool on) {
    if (R_SUCCEEDED(gspLcdInit())) {
        if (on) GSPLCD_PowerOnBacklight(GSPLCD_SCREEN_BOTH);
        else    GSPLCD_PowerOffBacklight(GSPLCD_SCREEN_BOTH);
        gspLcdExit();
    }
}

static void init_pause_bgm(void) {
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

static void update_pause_bgm(float dt) {
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
        float targetVol = 0.10f * ((volumeGain < 1.0f) ? volumeGain : 1.0f);
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
    load_gustina_mods();

    if (R_FAILED(ndspInit())) {
        while (aptMainLoop()) {
            hidScanInput();
            if (hidKeysDown() & KEY_START) break;
            gspWaitForVBlank();
        }
        return 1;
    }

    ndspSetOutputMode(NDSP_OUTPUT_STEREO);
    init_pause_bgm();

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
    lyricsThread = threadCreate(lyrics_worker_entry, NULL, 16 * 1024, 0x3A, -1, false);

    load_directory(currentDir);
    update_all_carousel_covers();
    
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
        
        if (hasPtmu && (++shellPollCounter >= 10 || isShellClosed)) {
            shellPollCounter = 0;
            u8 shellOpen = 1;
            if (R_SUCCEEDED(PTMU_GetShellState(&shellOpen))) {
                if (shellOpen == 0 && !isShellClosed) {
                    isShellClosed = true;
                    set_screens_backlight(false);
                } else if (shellOpen == 1 && isShellClosed) {
                    isShellClosed = false;
                    set_screens_backlight(true);
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
                lastfm_flush_queue();
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
            lyricAnimTimer = 1.0f;
        }

        bgScrollX += CFG_BOT_BG_SCROLL_SPD_X * dt;
        bgScrollY += CFG_BOT_BG_SCROLL_SPD_Y * dt;

        coverBgPanX = sinf(songTime * CFG_BG_PAN_SPEED_X) * CFG_BG_PAN_X_AMP;
        coverBgPanY = cosf(songTime * CFG_BG_PAN_SPEED_Y) * CFG_BG_PAN_Y_AMP;

        if (coverTransDir != TRANS_NONE) {
            coverTransProgress += dt * CFG_TRANS_SPEED;
            if (coverTransProgress >= 1.0f) {
                coverTransProgress = 1.0f;
                coverTransDir = TRANS_NONE;
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
                lastfm_send_scrobble();
                lastfm_scrobbled_track = true;
            }
        }
        if (isPlaying && !isPaused) {
            float beatPeriod = (detectedBPM > 40.0f) ? (60.0f / detectedBPM) : 0.5f;
            float minVisGap = hasTrackBpmTag ? (beatPeriod * 0.55f) : 0.22f;
            if (minVisGap < 0.20f) minVisGap = 0.20f;

            while (beatQueueHead != beatQueueTail) {
                float bt = pendingBeatTimes[beatQueueHead];
                if (bt > songTime) break;
                beatQueueHead = (beatQueueHead + 1) % MAX_PENDING_BEATS;

                if (lastVisualBeatTime <= 0.001f || (bt - lastVisualBeatTime) >= minVisGap) {
                    if (!hasTrackBpmTag && lastVisualBeatTime > 0.001f) {
                        float interval = bt - lastVisualBeatTime;
                        if (interval >= 0.26f && interval <= 1.25f) {
                            float instBpm = 60.0f / interval;
                            while (instBpm < 72.0f) instBpm *= 2.0f;
                            while (instBpm > 168.0f) instBpm *= 0.5f;
                            detectedBPM = detectedBPM * 0.85f + instBpm * 0.15f;
                        }
                    }
                    beatSideToggle = !beatSideToggle;
                    gustinaState = beatSideToggle ? GUSTINA_BOP_RIGHT : GUSTINA_BOP_LEFT;
                    gustinaFrame = 0;
                    gustinaFrameTimer = 0.0f;
                    lastVisualBeatTime = bt;
                }
            }
            if ((songTime - lastVisualBeatTime) >= beatPeriod * 1.08f) {
                if ((songTime - lastVisualBeatTime) > beatPeriod * 2.5f) {
                    lastVisualBeatTime = songTime;
                } else {
                    lastVisualBeatTime += beatPeriod;
                }
                beatSideToggle = !beatSideToggle;
                gustinaState = beatSideToggle ? GUSTINA_BOP_RIGHT : GUSTINA_BOP_LEFT;
                gustinaFrame = 0;
                gustinaFrameTimer = 0.0f;
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
            if (currentLrcIndex != oldLrc) {
                lyricAnimTimer = 0.0f;
            }
        }
        if (lyricAnimTimer < 1.0f) {
            lyricAnimTimer += dt * CFG_LYRICS_TWEEN_SPEED;
            if (lyricAnimTimer > 1.0f) lyricAnimTimer = 1.0f;
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

            if (kHeld & KEY_TOUCH) {
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
            if (curVolDir != 0) {
                if (volHoldDir != curVolDir) {
                    volHoldDir = curVolDir;
                    volHoldTime = 0.0f;
                    volRepeatTimer = 0.0f;
                    volumeGain += (float)curVolDir * 0.10f;
                    if (volumeGain < 0.10f) volumeGain = 0.10f;
                    if (volumeGain > 3.0f) volumeGain = 3.0f;
                } else {
                    volHoldTime += dt;
                    if (volHoldTime >= 0.28f) {
                        volRepeatTimer += dt;
                        while (volRepeatTimer >= 0.05f) {
                            volRepeatTimer -= 0.05f;
                            volumeGain += (float)curVolDir * 0.05f;
                            if (volumeGain < 0.10f) volumeGain = 0.10f;
                            if (volumeGain > 3.0f) volumeGain = 3.0f;
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
                } else if (touch.py < 24 && touch.px < 110) {
                    bottomScreenMode = 1;
                } else if (touch.py < 24 && touch.px >= 110 && touch.px <= 232) {
                    lyricsEnabled = !lyricsEnabled;
                } else if (touch.px >= 248 && touch.py >= 195) {
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
                open_search_keyboard(&lastTick);
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
                    open_search_keyboard(&lastTick);
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
                    update_all_carousel_covers();
                } else {
                    play_track_by_index(selectedFile, TRANS_NEXT);
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
        C3D_FrameEnd(0);
    }

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
    if (isShellClosed) set_screens_backlight(true);
    if (hasPtmu) ptmuExit();
    aptSetSleepAllowed(true);
    gfxExit();
    romfsExit();

    return 0;
}
