#ifndef APP_STATE_H
#define APP_STATE_H

#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>

#include <tremor/ivorbisfile.h> //someone is rlly watching this?
#include <opus/opusfile.h>

#include "sprites.h"
#include "qrcodegen.h"
#include "m4a_demux.h"
#include "neaacdec.h"

#define CFG_QUAD_X0             22.0f
#define CFG_QUAD_Y0             26.0f
#define CFG_QUAD_X1             142.0f
#define CFG_QUAD_Y1             64.0f
#define CFG_QUAD_X2             142.0f
#define CFG_QUAD_Y2             172.0f
#define CFG_QUAD_X3             22.0f
#define CFG_QUAD_Y3             206.0f

#define CFG_PERSPECTIVE_SLICES  16      // fun
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
#define MAX_FILES 256
#define MAX_LRC_LINES 512

#define PAUSE_BGM_CHANNEL   1
#define PAUSE_NUM_BUFS      4
#define PAUSE_BUF_FRAMES    4096

#define MAX_PENDING_BEATS 64
#define OSF_LEN 768

#define LASTFM_CFG_PATH     "sdmc:/3ds/MusicPlayer/lastfm.cfg"
#define LASTFM_QUEUE_PATH   "sdmc:/3ds/MusicPlayer/lfm_queue.txt"
#define LASTFM_WORKER_BASE  "http://mycoollyricsapi.kirb2109.workers.dev"

#define CFG_LASTFM_BTN_X        282.0f
#define CFG_LASTFM_BTN_Y        216.0f
#define CFG_LASTFM_BTN_SCALE    0.58f

typedef enum {
    FORMAT_NONE,
    FORMAT_MP3,
    FORMAT_WAV,
    FORMAT_FLAC, //i wanna kill you
    FORMAT_OGG,
    FORMAT_OPUS,
    FORMAT_M4A,
    FORMAT_AAC
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

typedef struct {
    char songPath[1024];
    char title[256];
    char artist[256];
    char album[256];
    int durationSec;
    uint32_t generation;
    u64 enqueueTimeMs;
} LyricsFetchJob;

typedef enum {
    TRANS_NONE, //i love transfems
    TRANS_NEXT,
    TRANS_PREV
} TransitionDir;

//i dont rlly wanna rename all 
extern C3D_Tex customGustinaTex[10];
extern Tex3DS_SubTexture customGustinaSubtex[10];
extern bool hasCustomGustina[10];

extern ndspWaveBuf waveBuf[NUM_WAVE_BUFS];
extern int16_t* audioBuffer[NUM_WAVE_BUFS];
extern ndspWaveBuf pauseBgmWaveBuf[PAUSE_NUM_BUFS];
extern int16_t* pauseBgmStreamBuf[PAUSE_NUM_BUFS];
extern int16_t* pauseBgmBuffer;
extern u32 pauseBgmTotalFrames;
extern u16 pauseBgmChannels;
extern u32 pauseBgmCursor;
extern bool hasPauseBgm;
extern volatile bool pauseBgmPlaying;
extern float pauseBgmVol;
extern float pauseBgmLastSentVol;
extern u32 waveBufSamples[NUM_WAVE_BUFS];

extern FILE* currentAudioFile;
extern AudioFormat currentFormat;
extern volatile uint64_t samplesCompleted;
extern volatile uint64_t samplesFed;
extern volatile bool audioStreamEOF;
extern volatile bool trackFinished;

extern float pendingBeatTimes[MAX_PENDING_BEATS];
extern volatile int beatQueueHead;
extern volatile int beatQueueTail;
extern float beatLpBass;
extern float beatLpMid;
extern float beatPrevBass;
extern float beatPrevMid;
extern float beatAvgEnergy;
extern float lastDetectedBeatTime;
extern float nextVisualBeatTime;
extern bool visualBeatLocked;
extern bool beatSideToggle;
extern bool hasTrackBpmTag;
extern float osfBuf[OSF_LEN];
extern int osfWriteIdx;
extern int osfCount;
extern int osfFramesSinceCalc;
extern float bpmLastCand;
extern int bpmCandVotes;

extern Thread audioThread;
extern volatile bool audioThreadRun;
extern LightLock audioLock;
extern volatile bool isAppSuspended;
extern aptHookCookie appAptHookCookie;

extern uint8_t* mp3ReadBuffer;
extern size_t mp3BufferLen;
extern size_t mp3BufferPos;
extern int mp3PcmPos;
extern int mp3PcmCount;
extern int mp3PcmChannels;
extern long mp3DataStartOffset;
extern long mp3DataSize;

extern bool isWavOpen;
extern FILE* flacFile;
extern OggVorbis_File oggFile;
extern M4ADemuxer m4aDemuxer;
extern bool isM4aOpen;
extern NeAACDecHandle aacDec;
extern uint8_t aacPacketBuf[16384];
extern int16_t aacPcmResid[4096 * 2];
extern int aacPcmPos;
extern int aacPcmCount;
extern int aacPcmChannels;
extern bool isOggOpen;
extern OggOpusFile* opusFile;

extern volatile bool isPlaying;
extern volatile bool isPaused;
extern char currentTrackPath[1024];
extern char currentTrackName[256];
extern char currentTrackTitle[256];
extern char currentTrackArtist[256];
extern char currentTrackAlbum[256];
extern int currentSampleRate;
extern int currentChannels;
extern float volumeGain;
extern float volumeLimEnv;
extern bool headphonesConnected;
extern int headphonePollCounter;
extern bool bottomWaveThingyMode;
extern int deferredPrevCoverIdx;
extern int deferredNextCoverIdx;
extern bool coverCacheDirReady;
extern volatile float songTime;
extern float songDuration;
extern float detectedBPM;
extern int lastBeat;

extern float fullSongWaveform[NUM_WAVE_SAMPLES];
extern int16_t waveScanChunk[512 * 2];

extern LrcLine lrcLines[MAX_LRC_LINES];
extern int lrcLineCount;
extern int currentLrcIndex;
extern float lyricScrollPos;
extern bool lyricsEnabled;

extern Thread lyricsThread;
extern volatile bool lyricsThreadRun;
extern LightLock lyricsLock;
extern LyricsFetchJob pendingLyricsJob;
extern volatile bool hasPendingLyricsJob;
extern volatile bool downloadedLrcReady;
extern volatile uint32_t currentTrackGeneration;
extern bool hasAcService;
extern bool hasHttpcService;
extern bool isWifiConnected;
extern char lyricsDebugStatus[24];

extern char lastfm_session_key[128];
extern char lastfm_username[64];
extern bool lastfm_has_session;
extern bool lastfm_scrobbled_track;
extern float lastfm_played_seconds;
extern u32 lastfm_track_start_ts;
extern LightLock lastfmLock;

extern uint8_t lfm_qrcode[qrcodegen_BUFFER_LEN_FOR_VERSION(10)];
extern bool lfm_qr_ready;
extern char lfm_auth_token[128];
extern float lfm_poll_timer;
extern float lfm_done_timer;
extern bool g_lastfm_login_active;
extern bool g_lastfm_login_done;
extern bool g_lastfm_login_cancelled;
extern char g_lastfm_login_status[64];

extern FileEntry allFileList[MAX_FILES];
extern int allFileCount;
extern FileEntry fileList[MAX_FILES];
extern int fileCount;
extern int selectedFile;
extern int scrollOffset;
extern char currentDir[1024];
extern char searchFilter[64];
extern float uiElapsedTime;

extern C3D_RenderTarget* topTarget;
extern C3D_RenderTarget* botTarget;
extern C2D_SpriteSheet spriteSheet;
extern C2D_SpriteSheet fontSheet1;
extern C2D_SpriteSheet fontSheet2;
extern C3D_Tex* fontTex1;
extern C3D_Tex* fontTex2;
extern Tex3DS_SubTexture fontGlyphs[96];
extern C2D_TextBuf sysTextBuf;
extern C2D_TextBuf sysMeasureBuf;

extern C3D_Tex coverTextures[4];
extern C3D_Tex coverBlurTextures[4];
extern C3D_Tex coverPeekNext2Tex;
extern C3D_Tex coverPeekPrev2Tex;
extern bool hasPeekNext2Tex;
extern bool hasPeekPrev2Tex;
extern int peekNext2SongIdx;
extern int peekPrev2SongIdx;
extern int deferredNext2CoverIdx;
extern int deferredPrev2CoverIdx;
extern Tex3DS_SubTexture cover64Subtex;
extern bool hasCoverTex[4];
extern int slotSongIdx[4];
extern uint8_t coverAvgR[4];
extern uint8_t coverAvgG[4];
extern uint8_t coverAvgB[4];
extern int slotCurrent;
extern int slotPrev;
extern int slotNext;
extern int slotOld;

#define coverTexCurrent (coverTextures[slotCurrent])
#define coverBlurTexCurrent (coverBlurTextures[slotCurrent])
#define coverTexPrev    (coverTextures[slotPrev])
#define coverTexNext    (coverTextures[slotNext])
#define coverTexOld     (coverTextures[slotOld])
#define hasCoverCurrent (hasCoverTex[slotCurrent])
#define hasCoverPrev    (hasCoverTex[slotPrev])
#define hasCoverNext    (hasCoverTex[slotNext])
#define hasCoverOld     (hasCoverTex[slotOld])
extern Tex3DS_SubTexture coverSubtex;
extern Tex3DS_SubTexture coverBlurSubtex;
extern uint8_t coverResizeBuf[128 * 128 * 4];

extern TransitionDir coverTransDir;
extern float coverTransProgress;
extern int playingFileIdx;
extern int coverScrollQueue[8];
extern int coverScrollCount;
extern int coverScrollPos;
extern TransitionDir coverScrollDir;
extern float coverBgPanX;
extern float coverBgPanY;
extern float noLyricsFocus;
extern float longBreakFocus;
extern float idleDimFocus;
extern float idleTimer;
extern float repeatFade;
extern float speakerVolumeGain;
extern float headphoneVolumeGain;
extern bool prevHeadphonesConnected;
void save_app_state(void);
void mark_app_state_dirty(void);
extern float beatPulseVal;

extern GustinaState gustinaState;
extern int gustinaFrame;
extern float gustinaFrameTimer;
extern int global10FpsFrame;
extern float global10FpsTimer;
extern float bgScrollX;
extern float bgScrollY;

extern int bottomScreenMode;
extern int wheelTouchSprite;
extern bool repeatEnabled;
extern bool shuffleEnabled;
extern int upcomingShuffleIdx;
extern int cachedNextIdx;
extern bool cachedNextRepeat;
extern char cachedNextDir[1024];
extern uint32_t cachedNextGen;
extern char cachedNextDisplay[256];

// helper shit
bool has_extension(const char* fn, const char* ext);
void nuke_weird_utf8_chars(const char* src, char* dst, size_t dstSize);
void strip_extension_copy(const char* src, char* dst, size_t dstSize);
void utf8_truncate_chars(const char* src, char* dst, size_t dstSize, int maxChars);
void init_custom_font(void);
float measure_custom_font(float scale, const char* str);
void draw_custom_font(float x, float y, float scale, u32 color, const char* str);
void draw_custom_font_centered(float cx, float y, float scale, u32 color, const char* str);
void draw_marquee_font(float x, float y, float maxW, float scale, u32 color, const char* str, float elapsed);
void draw_wrapped_lyrics(float cx, float cy, float maxW, float scale, u32 color, const char* text);
void draw_bouncing_loading_text(float x, float y, float scale, float animTime, u8 alpha);

// texture annnd cover stuff
typedef void (*OggCommentVisitor)(const char* comment, int len, void* userData);
bool ogg_scan(const char* filePath, size_t maxPacketBytes, OggCommentVisitor visitor, void* userData);
void upload_rgba8_to_tex(C3D_Tex* tex, const uint8_t* linear_rgba, int w, int h, int tex_w, int tex_h);
void reupload_rgba8_to_tex(C3D_Tex* tex, const uint8_t* linear_rgba, int w, int h, int tex_w, int tex_h);
void load_song_cover_tex(const char* filePath, C3D_Tex* tex, bool* hasTex, uint8_t accentR, uint8_t accentG, uint8_t accentB);
void load_song_cover_lowres(const char* filePath, C3D_Tex* tex, bool* hasTex, uint8_t accentR, uint8_t accentG, uint8_t accentB);
void refresh_the_3_covers(void);
void advance_cover_slot_step(int targetSongIdx, TransitionDir dir, bool fastFlyBy);

// fnf lyrics 2021
void build_lrc_path(const char* songPath, char* lrcPath, size_t lrcPathSize);
bool lrc_file_exists(const char* songPath);
void load_lrc_file(const char* songPath);
void url_encode_str(const char* src, char* dst, size_t dstSize);
void clean_query_title(const char* src, char* dst, size_t dstSize);
bool is_nothingburger_title(const char* title);
char* httpc_fetch_json(const char* startUrl, uint32_t activeGen, Result* outRet, u32* outHttpStatus, size_t* outLen);
void fnf_lyrics_slave(void* arg);

// i hate lastfm btw
void draw_qr_code_centered(float cx, float topY);
void lastfm_load_cfg(void);
int lastfm_poll_qr_auth(void);
void tell_lastfm_im_listening(void);
void snitch_song_to_lastfm(void);
void lastfm_dump_offline_shit(void);
void lastfm_process_async(void);
void render_lastfm_login_screen(void);
void lastfm_open_login(void);

// oh... another fnf reference....
void reset_beat_tracking(bool resetTag);
void do_the_bpm_thingy(const int16_t* pcm, int frames);

// audio playback stuff
void ear_destroyer_limiter(int16_t* dest, int frames, float gain);
void stop_playback(void);
void start_playback(const char* filePath, const char* fileName, TransitionDir transDirection);
void audio_thread_entry(void* arg);
void update_audio_stream_fallback(void);
void seek_current_track(float targetSec);
void init_pause_bgm(void);
void update_pause_bgm(float dt);

// metadata?
void apply_file_filter(void);
void load_directory(const char* path);
void apply_oggshit_kv(const char* comment, int len,
                      char* outTitle, size_t titleSz,
                      char* outArtist, size_t artistSz,
                      char* outAlbum, size_t albumSz,
                      float* outBpm, bool* outHasBpm);
void parse_oggshits_array(char** comments, const int* lengths, int count,
                          char* outTitle, size_t titleSz,
                          char* outArtist, size_t artistSz,
                          char* outAlbum, size_t albumSz,
                          float* outBpm, bool* outHasBpm);
void parse_mp3_id3v2_meta(FILE* f, long* outAudioOffset, float* outTlenSec);
void parse_mp3_id3v2_meta_into(FILE* f,
                               char* outTitle, size_t titleSz,
                               char* outArtist, size_t artistSz,
                               char* outAlbum, size_t albumSz,
                               float* outBpm, bool* outHasBpm,
                               long* outAudioOffset, float* outTlenSec);
const char* get_next_song_display(int targetIdx, bool repeating);
int find_next_song_index(int fromIdx, int step);
int find_random_song_index(int currentIdx);
void play_track_by_index(int idx, TransitionDir dir);

// ui!
void render_top_screen(void);
void render_bottom_screen(void);
void open_3ds_keyboard_thingy(u64* lastTickPtr);
void load_gustina_mod_slop(void);

#endif // app state shii
