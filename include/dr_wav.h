








































































































































#ifndef dr_wav_h
#define dr_wav_h

#ifdef __cplusplus
extern "C" {
#endif

#define DRWAV_STRINGIFY(x)      #x
#define DRWAV_XSTRINGIFY(x)     DRWAV_STRINGIFY(x)

#define DRWAV_VERSION_MAJOR     0
#define DRWAV_VERSION_MINOR     14
#define DRWAV_VERSION_REVISION  7
#define DRWAV_VERSION_STRING    DRWAV_XSTRINGIFY(DRWAV_VERSION_MAJOR) "." DRWAV_XSTRINGIFY(DRWAV_VERSION_MINOR) "." DRWAV_XSTRINGIFY(DRWAV_VERSION_REVISION)

#include <stddef.h> 


typedef   signed char           drwav_int8;
typedef unsigned char           drwav_uint8;
typedef   signed short          drwav_int16;
typedef unsigned short          drwav_uint16;
typedef   signed int            drwav_int32;
typedef unsigned int            drwav_uint32;
#if defined(_MSC_VER) && !defined(__clang__)
    typedef   signed __int64    drwav_int64;
    typedef unsigned __int64    drwav_uint64;
#else
    #if defined(__clang__) || (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wlong-long"
        #if defined(__clang__)
            #pragma GCC diagnostic ignored "-Wc++11-long-long"
        #endif
    #endif
    typedef   signed long long  drwav_int64;
    typedef unsigned long long  drwav_uint64;
    #if defined(__clang__) || (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))
        #pragma GCC diagnostic pop
    #endif
#endif
#if defined(__LP64__) || defined(_WIN64) || (defined(__x86_64__) && !defined(__ILP32__)) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(__powerpc64__)
    typedef drwav_uint64        drwav_uintptr;
#else
    typedef drwav_uint32        drwav_uintptr;
#endif
typedef drwav_uint8             drwav_bool8;
typedef drwav_uint32            drwav_bool32;
#define DRWAV_TRUE              1
#define DRWAV_FALSE             0



#if !defined(DRWAV_API)
    #if defined(DRWAV_DLL)
        #if defined(_WIN32)
            #define DRWAV_DLL_IMPORT  __declspec(dllimport)
            #define DRWAV_DLL_EXPORT  __declspec(dllexport)
            #define DRWAV_DLL_PRIVATE static
        #else
            #if defined(__GNUC__) && __GNUC__ >= 4
                #define DRWAV_DLL_IMPORT  __attribute__((visibility("default")))
                #define DRWAV_DLL_EXPORT  __attribute__((visibility("default")))
                #define DRWAV_DLL_PRIVATE __attribute__((visibility("hidden")))
            #else
                #define DRWAV_DLL_IMPORT
                #define DRWAV_DLL_EXPORT
                #define DRWAV_DLL_PRIVATE static
            #endif
        #endif

        #if defined(DR_WAV_IMPLEMENTATION) || defined(DRWAV_IMPLEMENTATION)
            #define DRWAV_API  DRWAV_DLL_EXPORT
        #else
            #define DRWAV_API  DRWAV_DLL_IMPORT
        #endif
        #define DRWAV_PRIVATE DRWAV_DLL_PRIVATE
    #else
        #define DRWAV_API extern
        #define DRWAV_PRIVATE static
    #endif
#endif



typedef drwav_int32 drwav_result;
#define DRWAV_SUCCESS                        0
#define DRWAV_ERROR                         -1   
#define DRWAV_INVALID_ARGS                  -2
#define DRWAV_INVALID_OPERATION             -3
#define DRWAV_OUT_OF_MEMORY                 -4
#define DRWAV_OUT_OF_RANGE                  -5
#define DRWAV_ACCESS_DENIED                 -6
#define DRWAV_DOES_NOT_EXIST                -7
#define DRWAV_ALREADY_EXISTS                -8
#define DRWAV_TOO_MANY_OPEN_FILES           -9
#define DRWAV_INVALID_FILE                  -10
#define DRWAV_TOO_BIG                       -11
#define DRWAV_PATH_TOO_LONG                 -12
#define DRWAV_NAME_TOO_LONG                 -13
#define DRWAV_NOT_DIRECTORY                 -14
#define DRWAV_IS_DIRECTORY                  -15
#define DRWAV_DIRECTORY_NOT_EMPTY           -16
#define DRWAV_END_OF_FILE                   -17
#define DRWAV_NO_SPACE                      -18
#define DRWAV_BUSY                          -19
#define DRWAV_IO_ERROR                      -20
#define DRWAV_INTERRUPT                     -21
#define DRWAV_UNAVAILABLE                   -22
#define DRWAV_ALREADY_IN_USE                -23
#define DRWAV_BAD_ADDRESS                   -24
#define DRWAV_BAD_SEEK                      -25
#define DRWAV_BAD_PIPE                      -26
#define DRWAV_DEADLOCK                      -27
#define DRWAV_TOO_MANY_LINKS                -28
#define DRWAV_NOT_IMPLEMENTED               -29
#define DRWAV_NO_MESSAGE                    -30
#define DRWAV_BAD_MESSAGE                   -31
#define DRWAV_NO_DATA_AVAILABLE             -32
#define DRWAV_INVALID_DATA                  -33
#define DRWAV_TIMEOUT                       -34
#define DRWAV_NO_NETWORK                    -35
#define DRWAV_NOT_UNIQUE                    -36
#define DRWAV_NOT_SOCKET                    -37
#define DRWAV_NO_ADDRESS                    -38
#define DRWAV_BAD_PROTOCOL                  -39
#define DRWAV_PROTOCOL_UNAVAILABLE          -40
#define DRWAV_PROTOCOL_NOT_SUPPORTED        -41
#define DRWAV_PROTOCOL_FAMILY_NOT_SUPPORTED -42
#define DRWAV_ADDRESS_FAMILY_NOT_SUPPORTED  -43
#define DRWAV_SOCKET_NOT_SUPPORTED          -44
#define DRWAV_CONNECTION_RESET              -45
#define DRWAV_ALREADY_CONNECTED             -46
#define DRWAV_NOT_CONNECTED                 -47
#define DRWAV_CONNECTION_REFUSED            -48
#define DRWAV_NO_HOST                       -49
#define DRWAV_IN_PROGRESS                   -50
#define DRWAV_CANCELLED                     -51
#define DRWAV_MEMORY_ALREADY_MAPPED         -52
#define DRWAV_AT_END                        -53



#define DR_WAVE_FORMAT_PCM          0x1
#define DR_WAVE_FORMAT_ADPCM        0x2
#define DR_WAVE_FORMAT_IEEE_FLOAT   0x3
#define DR_WAVE_FORMAT_ALAW         0x6
#define DR_WAVE_FORMAT_MULAW        0x7
#define DR_WAVE_FORMAT_DTS          0x8
#define DR_WAVE_FORMAT_DVI_ADPCM    0x11
#define DR_WAVE_FORMAT_EXTENSIBLE   0xFFFE


#define DRWAV_SEQUENTIAL            0x00000001
#define DRWAV_WITH_METADATA         0x00000002

DRWAV_API void drwav_version(drwav_uint32* pMajor, drwav_uint32* pMinor, drwav_uint32* pRevision);
DRWAV_API const char* drwav_version_string(void);


typedef struct
{
    void* pUserData;
    void* (* onMalloc)(size_t sz, void* pUserData);
    void* (* onRealloc)(void* p, size_t sz, void* pUserData);
    void  (* onFree)(void* p, void* pUserData);
} drwav_allocation_callbacks;


typedef enum
{
    DRWAV_SEEK_SET,
    DRWAV_SEEK_CUR,
    DRWAV_SEEK_END
} drwav_seek_origin;

typedef enum
{
    drwav_container_riff,
    drwav_container_rifx,
    drwav_container_w64,
    drwav_container_rf64,
    drwav_container_aiff
} drwav_container;

typedef struct
{
    union
    {
        drwav_uint8 fourcc[4];
        drwav_uint8 guid[16];
    } id;

    
    drwav_uint64 sizeInBytes;

    



    unsigned int paddingSize;
} drwav_chunk_header;

typedef struct
{
    



    drwav_uint16 formatTag;

    
    drwav_uint16 channels;

    
    drwav_uint32 sampleRate;

    
    drwav_uint32 avgBytesPerSec;

    
    drwav_uint16 blockAlign;

    
    drwav_uint16 bitsPerSample;

    
    drwav_uint16 extendedSize;

    




    drwav_uint16 validBitsPerSample;

    
    drwav_uint32 channelMask;

    
    drwav_uint8 subFormat[16];
} drwav_fmt;

DRWAV_API drwav_uint16 drwav_fmt_get_format(const drwav_fmt* pFMT);














typedef size_t (* drwav_read_proc)(void* pUserData, void* pBufferOut, size_t bytesToRead);












typedef size_t (* drwav_write_proc)(void* pUserData, const void* pData, size_t bytesToWrite);













typedef drwav_bool32 (* drwav_seek_proc)(void* pUserData, int offset, drwav_seek_origin origin);









typedef drwav_bool32 (* drwav_tell_proc)(void* pUserData, drwav_int64* pCursor);

























typedef drwav_uint64 (* drwav_chunk_proc)(void* pChunkUserData, drwav_read_proc onRead, drwav_seek_proc onSeek, void* pReadSeekUserData, const drwav_chunk_header* pChunkHeader, drwav_container container, const drwav_fmt* pFMT);



typedef struct
{
    const drwav_uint8* data;
    size_t dataSize;
    size_t currentReadPos;
} drwav__memory_stream;


typedef struct
{
    void** ppData;
    size_t* pDataSize;
    size_t dataSize;
    size_t dataCapacity;
    size_t currentWritePos;
} drwav__memory_stream_write;

typedef struct
{
    drwav_container container;  
    drwav_uint32 format;        
    drwav_uint32 channels;
    drwav_uint32 sampleRate;
    drwav_uint32 bitsPerSample;
} drwav_data_format;

typedef enum
{
    drwav_metadata_type_none                        = 0,

    






    drwav_metadata_type_unknown                     = 1 << 0,

    
    drwav_metadata_type_smpl                        = 1 << 1,
    drwav_metadata_type_inst                        = 1 << 2,
    drwav_metadata_type_cue                         = 1 << 3,
    drwav_metadata_type_acid                        = 1 << 4,
    drwav_metadata_type_bext                        = 1 << 5,

    






    drwav_metadata_type_list_label                  = 1 << 6,
    drwav_metadata_type_list_note                   = 1 << 7,
    drwav_metadata_type_list_labelled_cue_region    = 1 << 8,

    drwav_metadata_type_list_info_software          = 1 << 9,
    drwav_metadata_type_list_info_copyright         = 1 << 10,
    drwav_metadata_type_list_info_title             = 1 << 11,
    drwav_metadata_type_list_info_artist            = 1 << 12,
    drwav_metadata_type_list_info_comment           = 1 << 13,
    drwav_metadata_type_list_info_date              = 1 << 14,
    drwav_metadata_type_list_info_genre             = 1 << 15,
    drwav_metadata_type_list_info_album             = 1 << 16,
    drwav_metadata_type_list_info_tracknumber       = 1 << 17,
    drwav_metadata_type_list_info_location          = 1 << 18,
    drwav_metadata_type_list_info_organization      = 1 << 19,
    drwav_metadata_type_list_info_keywords          = 1 << 20,
    drwav_metadata_type_list_info_medium            = 1 << 21,
    drwav_metadata_type_list_info_description       = 1 << 22,

    
    drwav_metadata_type_list_all_info_strings       = drwav_metadata_type_list_info_software
                                                    | drwav_metadata_type_list_info_copyright
                                                    | drwav_metadata_type_list_info_title
                                                    | drwav_metadata_type_list_info_artist
                                                    | drwav_metadata_type_list_info_comment
                                                    | drwav_metadata_type_list_info_date
                                                    | drwav_metadata_type_list_info_genre
                                                    | drwav_metadata_type_list_info_album
                                                    | drwav_metadata_type_list_info_tracknumber
                                                    | drwav_metadata_type_list_info_location
                                                    | drwav_metadata_type_list_info_organization
                                                    | drwav_metadata_type_list_info_keywords
                                                    | drwav_metadata_type_list_info_medium
                                                    | drwav_metadata_type_list_info_description,

    drwav_metadata_type_list_all_adtl               = drwav_metadata_type_list_label
                                                    | drwav_metadata_type_list_note
                                                    | drwav_metadata_type_list_labelled_cue_region,

    drwav_metadata_type_all                         = -2,   
    drwav_metadata_type_all_including_unknown       = -1    
} drwav_metadata_type;







typedef enum
{
    drwav_smpl_loop_type_forward  = 0,
    drwav_smpl_loop_type_pingpong = 1,
    drwav_smpl_loop_type_backward = 2
} drwav_smpl_loop_type;

typedef struct
{
    
    drwav_uint32 cuePointId;

    
    drwav_uint32 type;

    
    drwav_uint32 firstSampleOffset;

    
    drwav_uint32 lastSampleOffset;

    
    drwav_uint32 sampleFraction;

    
    drwav_uint32 playCount;
} drwav_smpl_loop;

typedef struct
{
    
    drwav_uint32 manufacturerId;
    drwav_uint32 productId;

    
    drwav_uint32 samplePeriodNanoseconds;

    
    drwav_uint32 midiUnityNote;

    
    drwav_uint32 midiPitchFraction;

    
    drwav_uint32 smpteFormat;
    drwav_uint32 smpteOffset;

    
    drwav_uint32 sampleLoopCount;

    
    drwav_uint32 samplerSpecificDataSizeInBytes;

    drwav_smpl_loop* pLoops;
    drwav_uint8* pSamplerSpecificData;
} drwav_smpl;







typedef struct
{
    drwav_int8 midiUnityNote;   
    drwav_int8 fineTuneCents;   
    drwav_int8 gainDecibels;    
    drwav_int8 lowNote;         
    drwav_int8 highNote;        
    drwav_int8 lowVelocity;     
    drwav_int8 highVelocity;    
} drwav_inst;







typedef struct
{
    
    drwav_uint32 id;

    
    drwav_uint32 playOrderPosition;

    
    drwav_uint8 dataChunkId[4];

    
    drwav_uint32 chunkStart;

    
    drwav_uint32 blockStart;

    
    drwav_uint32 sampleOffset;
} drwav_cue_point;

typedef struct
{
    drwav_uint32 cuePointCount;
    drwav_cue_point *pCuePoints;
} drwav_cue;






typedef enum
{
    drwav_acid_flag_one_shot      = 1,  
    drwav_acid_flag_root_note_set = 2,
    drwav_acid_flag_stretch       = 4,
    drwav_acid_flag_disk_based    = 8,
    drwav_acid_flag_acidizer      = 16  
} drwav_acid_flag;

typedef struct
{
    
    drwav_uint32 flags;

    
    drwav_uint16 midiUnityNote;

    
    drwav_uint16 reserved1;
    float reserved2;

    
    drwav_uint32 numBeats;

    
    drwav_uint16 meterDenominator;
    drwav_uint16 meterNumerator;

    
    float tempo;
} drwav_acid;








typedef struct
{
    
    drwav_uint32 cuePointId;

    
    drwav_uint32 stringLength;

    
    char* pString;
} drwav_list_label_or_note;







typedef struct
{
    




    char* pDescription;                 
    char* pOriginatorName;              
    char* pOriginatorReference;         
    char  pOriginationDate[10];         
    char  pOriginationTime[8];          
    drwav_uint64 timeReference;         
    drwav_uint16 version;               

    



    char* pCodingHistory;
    drwav_uint32 codingHistorySize;

    
    drwav_uint8* pUMID;                  

    
    drwav_uint16 loudnessValue;         
    drwav_uint16 loudnessRange;         
    drwav_uint16 maxTruePeakLevel;      
    drwav_uint16 maxMomentaryLoudness;  
    drwav_uint16 maxShortTermLoudness;  
} drwav_bext;








typedef struct
{
    
    drwav_uint32 stringLength;

    
    char* pString;
} drwav_list_info_text;







typedef struct
{
    
    drwav_uint32 cuePointId;

    
    drwav_uint32 sampleLength;

    
    drwav_uint8 purposeId[4];

    
    drwav_uint16 country;
    drwav_uint16 language;
    drwav_uint16 dialect;
    drwav_uint16 codePage;

    
    drwav_uint32 stringLength;

    
    char* pString;
} drwav_list_labelled_cue_region;










typedef enum
{
    drwav_metadata_location_invalid,
    drwav_metadata_location_top_level,
    drwav_metadata_location_inside_info_list,
    drwav_metadata_location_inside_adtl_list
} drwav_metadata_location;

typedef struct
{
    drwav_uint8 id[4];
    drwav_metadata_location chunkLocation;
    drwav_uint32 dataSizeInBytes;
    drwav_uint8* pData;
} drwav_unknown_metadata;




typedef struct
{
    
    drwav_metadata_type type;

    union
    {
        drwav_cue cue;
        drwav_smpl smpl;
        drwav_acid acid;
        drwav_inst inst;
        drwav_bext bext;
        drwav_list_label_or_note labelOrNote;   
        drwav_list_labelled_cue_region labelledCueRegion;
        drwav_list_info_text infoText;          
        drwav_unknown_metadata unknown;
    } data;
} drwav_metadata;

typedef struct
{
    
    drwav_read_proc onRead;

    
    drwav_write_proc onWrite;

    
    drwav_seek_proc onSeek;

    
    drwav_tell_proc onTell;

    
    void* pUserData;

    
    drwav_allocation_callbacks allocationCallbacks;


    
    drwav_container container;


    
    drwav_fmt fmt;

    
    drwav_uint32 sampleRate;

    
    drwav_uint16 channels;

    
    drwav_uint16 bitsPerSample;

    
    drwav_uint16 translatedFormatTag;

    
    drwav_uint64 totalPCMFrameCount;


    
    drwav_uint64 dataChunkDataSize;

    
    drwav_uint64 dataChunkDataPos;

    
    drwav_uint64 bytesRemaining;

    
    drwav_uint64 readCursorInPCMFrames;


    



    drwav_uint64 dataChunkDataSizeTargetWrite;

    
    drwav_bool32 isSequentialWrite;


    
    drwav_metadata* pMetadata;
    drwav_uint32 metadataCount;


    
    drwav__memory_stream memoryStream;
    drwav__memory_stream_write memoryStreamWrite;


    
    struct
    {
        drwav_uint32 bytesRemainingInBlock;
        drwav_uint16 predictor[2];
        drwav_int32  delta[2];
        drwav_int32  cachedFrames[4];  
        drwav_uint32 cachedFrameCount;
        drwav_int32  prevFrames[2][2]; 
    } msadpcm;

    
    struct
    {
        drwav_uint32 bytesRemainingInBlock;
        drwav_int32  predictor[2];
        drwav_int32  stepIndex[2];
        drwav_int32  cachedFrames[16]; 
        drwav_uint32 cachedFrameCount;
    } ima;

    
    struct
    {
        drwav_bool8 isLE;   
        drwav_bool8 isUnsigned; 
    } aiff;
} drwav;































DRWAV_API drwav_bool32 drwav_init(drwav* pWav, drwav_read_proc onRead, drwav_seek_proc onSeek, drwav_tell_proc onTell, void* pUserData, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_ex(drwav* pWav, drwav_read_proc onRead, drwav_seek_proc onSeek, drwav_tell_proc onTell, drwav_chunk_proc onChunk, void* pReadSeekTellUserData, void* pChunkUserData, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_with_metadata(drwav* pWav, drwav_read_proc onRead, drwav_seek_proc onSeek, drwav_tell_proc onTell, void* pUserData, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks);





















DRWAV_API drwav_bool32 drwav_init_write(drwav* pWav, const drwav_data_format* pFormat, drwav_write_proc onWrite, drwav_seek_proc onSeek, void* pUserData, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_write_sequential(drwav* pWav, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, drwav_write_proc onWrite, void* pUserData, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_write_sequential_pcm_frames(drwav* pWav, const drwav_data_format* pFormat, drwav_uint64 totalPCMFrameCount, drwav_write_proc onWrite, void* pUserData, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_write_with_metadata(drwav* pWav, const drwav_data_format* pFormat, drwav_write_proc onWrite, drwav_seek_proc onSeek, void* pUserData, const drwav_allocation_callbacks* pAllocationCallbacks, drwav_metadata* pMetadata, drwav_uint32 metadataCount);














DRWAV_API drwav_uint64 drwav_target_write_size_bytes(const drwav_data_format* pFormat, drwav_uint64 totalFrameCount, drwav_metadata* pMetadata, drwav_uint32 metadataCount);








DRWAV_API drwav_metadata* drwav_take_ownership_of_metadata(drwav* pWav);






DRWAV_API drwav_result drwav_uninit(drwav* pWav);















DRWAV_API size_t drwav_read_raw(drwav* pWav, size_t bytesToRead, void* pBufferOut);















DRWAV_API drwav_uint64 drwav_read_pcm_frames(drwav* pWav, drwav_uint64 framesToRead, void* pBufferOut);
DRWAV_API drwav_uint64 drwav_read_pcm_frames_le(drwav* pWav, drwav_uint64 framesToRead, void* pBufferOut);
DRWAV_API drwav_uint64 drwav_read_pcm_frames_be(drwav* pWav, drwav_uint64 framesToRead, void* pBufferOut);






DRWAV_API drwav_bool32 drwav_seek_to_pcm_frame(drwav* pWav, drwav_uint64 targetFrameIndex);




DRWAV_API drwav_result drwav_get_cursor_in_pcm_frames(drwav* pWav, drwav_uint64* pCursor);




DRWAV_API drwav_result drwav_get_length_in_pcm_frames(drwav* pWav, drwav_uint64* pLength);







DRWAV_API size_t drwav_write_raw(drwav* pWav, size_t bytesToWrite, const void* pData);









DRWAV_API drwav_uint64 drwav_write_pcm_frames(drwav* pWav, drwav_uint64 framesToWrite, const void* pData);
DRWAV_API drwav_uint64 drwav_write_pcm_frames_le(drwav* pWav, drwav_uint64 framesToWrite, const void* pData);
DRWAV_API drwav_uint64 drwav_write_pcm_frames_be(drwav* pWav, drwav_uint64 framesToWrite, const void* pData);


#ifndef DR_WAV_NO_CONVERSION_API










DRWAV_API drwav_uint64 drwav_read_pcm_frames_s16(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut);
DRWAV_API drwav_uint64 drwav_read_pcm_frames_s16le(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut);
DRWAV_API drwav_uint64 drwav_read_pcm_frames_s16be(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut);


DRWAV_API void drwav_u8_to_s16(drwav_int16* pOut, const drwav_uint8* pIn, size_t sampleCount);


DRWAV_API void drwav_s24_to_s16(drwav_int16* pOut, const drwav_uint8* pIn, size_t sampleCount);


DRWAV_API void drwav_s32_to_s16(drwav_int16* pOut, const drwav_int32* pIn, size_t sampleCount);


DRWAV_API void drwav_f32_to_s16(drwav_int16* pOut, const float* pIn, size_t sampleCount);


DRWAV_API void drwav_f64_to_s16(drwav_int16* pOut, const double* pIn, size_t sampleCount);


DRWAV_API void drwav_alaw_to_s16(drwav_int16* pOut, const drwav_uint8* pIn, size_t sampleCount);


DRWAV_API void drwav_mulaw_to_s16(drwav_int16* pOut, const drwav_uint8* pIn, size_t sampleCount);











DRWAV_API drwav_uint64 drwav_read_pcm_frames_f32(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut);
DRWAV_API drwav_uint64 drwav_read_pcm_frames_f32le(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut);
DRWAV_API drwav_uint64 drwav_read_pcm_frames_f32be(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut);


DRWAV_API void drwav_u8_to_f32(float* pOut, const drwav_uint8* pIn, size_t sampleCount);


DRWAV_API void drwav_s16_to_f32(float* pOut, const drwav_int16* pIn, size_t sampleCount);


DRWAV_API void drwav_s24_to_f32(float* pOut, const drwav_uint8* pIn, size_t sampleCount);


DRWAV_API void drwav_s32_to_f32(float* pOut, const drwav_int32* pIn, size_t sampleCount);


DRWAV_API void drwav_f64_to_f32(float* pOut, const double* pIn, size_t sampleCount);


DRWAV_API void drwav_alaw_to_f32(float* pOut, const drwav_uint8* pIn, size_t sampleCount);


DRWAV_API void drwav_mulaw_to_f32(float* pOut, const drwav_uint8* pIn, size_t sampleCount);











DRWAV_API drwav_uint64 drwav_read_pcm_frames_s32(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut);
DRWAV_API drwav_uint64 drwav_read_pcm_frames_s32le(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut);
DRWAV_API drwav_uint64 drwav_read_pcm_frames_s32be(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut);


DRWAV_API void drwav_u8_to_s32(drwav_int32* pOut, const drwav_uint8* pIn, size_t sampleCount);


DRWAV_API void drwav_s16_to_s32(drwav_int32* pOut, const drwav_int16* pIn, size_t sampleCount);


DRWAV_API void drwav_s24_to_s32(drwav_int32* pOut, const drwav_uint8* pIn, size_t sampleCount);


DRWAV_API void drwav_f32_to_s32(drwav_int32* pOut, const float* pIn, size_t sampleCount);


DRWAV_API void drwav_f64_to_s32(drwav_int32* pOut, const double* pIn, size_t sampleCount);


DRWAV_API void drwav_alaw_to_s32(drwav_int32* pOut, const drwav_uint8* pIn, size_t sampleCount);


DRWAV_API void drwav_mulaw_to_s32(drwav_int32* pOut, const drwav_uint8* pIn, size_t sampleCount);

#endif  




#ifndef DR_WAV_NO_STDIO







DRWAV_API drwav_bool32 drwav_init_file(drwav* pWav, const char* filename, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_file_ex(drwav* pWav, const char* filename, drwav_chunk_proc onChunk, void* pChunkUserData, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_file_w(drwav* pWav, const wchar_t* filename, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_file_ex_w(drwav* pWav, const wchar_t* filename, drwav_chunk_proc onChunk, void* pChunkUserData, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_file_with_metadata(drwav* pWav, const char* filename, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_file_with_metadata_w(drwav* pWav, const wchar_t* filename, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks);









DRWAV_API drwav_bool32 drwav_init_file_write(drwav* pWav, const char* filename, const drwav_data_format* pFormat, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_file_write_sequential(drwav* pWav, const char* filename, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_file_write_sequential_pcm_frames(drwav* pWav, const char* filename, const drwav_data_format* pFormat, drwav_uint64 totalPCMFrameCount, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_file_write_w(drwav* pWav, const wchar_t* filename, const drwav_data_format* pFormat, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_file_write_sequential_w(drwav* pWav, const wchar_t* filename, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_file_write_sequential_pcm_frames_w(drwav* pWav, const wchar_t* filename, const drwav_data_format* pFormat, drwav_uint64 totalPCMFrameCount, const drwav_allocation_callbacks* pAllocationCallbacks);
#endif  









DRWAV_API drwav_bool32 drwav_init_memory(drwav* pWav, const void* data, size_t dataSize, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_memory_ex(drwav* pWav, const void* data, size_t dataSize, drwav_chunk_proc onChunk, void* pChunkUserData, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_memory_with_metadata(drwav* pWav, const void* data, size_t dataSize, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks);









DRWAV_API drwav_bool32 drwav_init_memory_write(drwav* pWav, void** ppData, size_t* pDataSize, const drwav_data_format* pFormat, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_memory_write_sequential(drwav* pWav, void** ppData, size_t* pDataSize, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_bool32 drwav_init_memory_write_sequential_pcm_frames(drwav* pWav, void** ppData, size_t* pDataSize, const drwav_data_format* pFormat, drwav_uint64 totalPCMFrameCount, const drwav_allocation_callbacks* pAllocationCallbacks);


#ifndef DR_WAV_NO_CONVERSION_API





DRWAV_API drwav_int16* drwav_open_and_read_pcm_frames_s16(drwav_read_proc onRead, drwav_seek_proc onSeek, drwav_tell_proc onTell, void* pUserData, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API float* drwav_open_and_read_pcm_frames_f32(drwav_read_proc onRead, drwav_seek_proc onSeek, drwav_tell_proc onTell, void* pUserData, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_int32* drwav_open_and_read_pcm_frames_s32(drwav_read_proc onRead, drwav_seek_proc onSeek, drwav_tell_proc onTell, void* pUserData, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks);
#ifndef DR_WAV_NO_STDIO





DRWAV_API drwav_int16* drwav_open_file_and_read_pcm_frames_s16(const char* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API float* drwav_open_file_and_read_pcm_frames_f32(const char* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_int32* drwav_open_file_and_read_pcm_frames_s32(const char* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_int16* drwav_open_file_and_read_pcm_frames_s16_w(const wchar_t* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API float* drwav_open_file_and_read_pcm_frames_f32_w(const wchar_t* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_int32* drwav_open_file_and_read_pcm_frames_s32_w(const wchar_t* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks);
#endif





DRWAV_API drwav_int16* drwav_open_memory_and_read_pcm_frames_s16(const void* data, size_t dataSize, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API float* drwav_open_memory_and_read_pcm_frames_f32(const void* data, size_t dataSize, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks);
DRWAV_API drwav_int32* drwav_open_memory_and_read_pcm_frames_s32(const void* data, size_t dataSize, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks);
#endif


DRWAV_API void drwav_free(void* p, const drwav_allocation_callbacks* pAllocationCallbacks);


DRWAV_API drwav_uint16 drwav_bytes_to_u16(const drwav_uint8* data);
DRWAV_API drwav_int16 drwav_bytes_to_s16(const drwav_uint8* data);
DRWAV_API drwav_uint32 drwav_bytes_to_u32(const drwav_uint8* data);
DRWAV_API drwav_int32 drwav_bytes_to_s32(const drwav_uint8* data);
DRWAV_API drwav_uint64 drwav_bytes_to_u64(const drwav_uint8* data);
DRWAV_API drwav_int64 drwav_bytes_to_s64(const drwav_uint8* data);
DRWAV_API float drwav_bytes_to_f32(const drwav_uint8* data);


DRWAV_API drwav_bool32 drwav_guid_equal(const drwav_uint8 a[16], const drwav_uint8 b[16]);


DRWAV_API drwav_bool32 drwav_fourcc_equal(const drwav_uint8* a, const char* b);

#ifdef __cplusplus
}
#endif
#endif  









#if defined(DR_WAV_IMPLEMENTATION) || defined(DRWAV_IMPLEMENTATION)
#ifndef dr_wav_c
#define dr_wav_c

#ifdef __MRC__

#pragma options opt off
#endif

#include <stdlib.h>
#include <string.h>
#include <limits.h> 

#ifndef DR_WAV_NO_STDIO
#include <stdio.h>
#ifndef DR_WAV_NO_WCHAR
#include <wchar.h>
#endif
#endif


#ifndef DRWAV_ASSERT
#include <assert.h>
#define DRWAV_ASSERT(expression)           assert(expression)
#endif
#ifndef DRWAV_MALLOC
#define DRWAV_MALLOC(sz)                   malloc((sz))
#endif
#ifndef DRWAV_REALLOC
#define DRWAV_REALLOC(p, sz)               realloc((p), (sz))
#endif
#ifndef DRWAV_FREE
#define DRWAV_FREE(p)                      free((p))
#endif
#ifndef DRWAV_COPY_MEMORY
#define DRWAV_COPY_MEMORY(dst, src, sz)    memcpy((dst), (src), (sz))
#endif
#ifndef DRWAV_ZERO_MEMORY
#define DRWAV_ZERO_MEMORY(p, sz)           memset((p), 0, (sz))
#endif
#ifndef DRWAV_ZERO_OBJECT
#define DRWAV_ZERO_OBJECT(p)               DRWAV_ZERO_MEMORY((p), sizeof(*p))
#endif

#define drwav_countof(x)                   (sizeof(x) / sizeof(x[0]))
#define drwav_align(x, a)                  ((((x) + (a) - 1) / (a)) * (a))
#define drwav_min(a, b)                    (((a) < (b)) ? (a) : (b))
#define drwav_max(a, b)                    (((a) > (b)) ? (a) : (b))
#define drwav_clamp(x, lo, hi)             (drwav_max((lo), drwav_min((hi), (x))))
#define drwav_offset_ptr(p, offset)        (((drwav_uint8*)(p)) + (offset))

#define DRWAV_MAX_SIMD_VECTOR_SIZE         32


#if defined(__x86_64__) || (defined(_M_X64) && !defined(_M_ARM64EC))
    #define DRWAV_X64
#elif defined(__i386) || defined(_M_IX86)
    #define DRWAV_X86
#elif defined(__arm__) || defined(_M_ARM)
    #define DRWAV_ARM
#endif



#ifdef _MSC_VER
    #define DRWAV_INLINE __forceinline
#elif defined(__GNUC__)
    






    #if defined(__STRICT_ANSI__)
        #define DRWAV_GNUC_INLINE_HINT __inline__
    #else
        #define DRWAV_GNUC_INLINE_HINT inline
    #endif

    #if (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 2)) || defined(__clang__)
        #define DRWAV_INLINE DRWAV_GNUC_INLINE_HINT __attribute__((always_inline))
    #else
        #define DRWAV_INLINE DRWAV_GNUC_INLINE_HINT
    #endif
#elif defined(__WATCOMC__)
    #define DRWAV_INLINE __inline
#else
    #define DRWAV_INLINE
#endif



#if defined(SIZE_MAX)
    #define DRWAV_SIZE_MAX  SIZE_MAX
#else
    #if defined(_WIN64) || defined(_LP64) || defined(__LP64__)
        #define DRWAV_SIZE_MAX  ((drwav_uint64)0xFFFFFFFFFFFFFFFF)
    #else
        #define DRWAV_SIZE_MAX  0xFFFFFFFF
    #endif
#endif



#define DRWAV_INT64_MIN ((drwav_int64) ((drwav_uint64)0x80000000 << 32))
#define DRWAV_INT64_MAX ((drwav_int64)(((drwav_uint64)0x7FFFFFFF << 32) | 0xFFFFFFFF))

#if defined(_MSC_VER) && _MSC_VER >= 1400
    #define DRWAV_HAS_BYTESWAP16_INTRINSIC
    #define DRWAV_HAS_BYTESWAP32_INTRINSIC
    #define DRWAV_HAS_BYTESWAP64_INTRINSIC
#elif defined(__clang__)
    #if defined(__has_builtin)
        #if __has_builtin(__builtin_bswap16)
            #define DRWAV_HAS_BYTESWAP16_INTRINSIC
        #endif
        #if __has_builtin(__builtin_bswap32)
            #define DRWAV_HAS_BYTESWAP32_INTRINSIC
        #endif
        #if __has_builtin(__builtin_bswap64)
            #define DRWAV_HAS_BYTESWAP64_INTRINSIC
        #endif
    #endif
#elif defined(__GNUC__)
    #if ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
        #define DRWAV_HAS_BYTESWAP32_INTRINSIC
        #define DRWAV_HAS_BYTESWAP64_INTRINSIC
    #endif
    #if ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8))
        #define DRWAV_HAS_BYTESWAP16_INTRINSIC
    #endif
#endif

DRWAV_API void drwav_version(drwav_uint32* pMajor, drwav_uint32* pMinor, drwav_uint32* pRevision)
{
    if (pMajor) {
        *pMajor = DRWAV_VERSION_MAJOR;
    }

    if (pMinor) {
        *pMinor = DRWAV_VERSION_MINOR;
    }

    if (pRevision) {
        *pRevision = DRWAV_VERSION_REVISION;
    }
}

DRWAV_API const char* drwav_version_string(void)
{
    return DRWAV_VERSION_STRING;
}





#ifndef DRWAV_MAX_SAMPLE_RATE
#define DRWAV_MAX_SAMPLE_RATE       384000
#endif
#ifndef DRWAV_MAX_CHANNELS
#define DRWAV_MAX_CHANNELS          256
#endif
#ifndef DRWAV_MAX_BITS_PER_SAMPLE
#define DRWAV_MAX_BITS_PER_SAMPLE   64
#endif

static const drwav_uint8 drwavGUID_W64_RIFF[16] = {0x72,0x69,0x66,0x66, 0x2E,0x91, 0xCF,0x11, 0xA5,0xD6, 0x28,0xDB,0x04,0xC1,0x00,0x00};    
static const drwav_uint8 drwavGUID_W64_WAVE[16] = {0x77,0x61,0x76,0x65, 0xF3,0xAC, 0xD3,0x11, 0x8C,0xD1, 0x00,0xC0,0x4F,0x8E,0xDB,0x8A};    
    
static const drwav_uint8 drwavGUID_W64_FMT [16] = {0x66,0x6D,0x74,0x20, 0xF3,0xAC, 0xD3,0x11, 0x8C,0xD1, 0x00,0xC0,0x4F,0x8E,0xDB,0x8A};    
static const drwav_uint8 drwavGUID_W64_FACT[16] = {0x66,0x61,0x63,0x74, 0xF3,0xAC, 0xD3,0x11, 0x8C,0xD1, 0x00,0xC0,0x4F,0x8E,0xDB,0x8A};    
static const drwav_uint8 drwavGUID_W64_DATA[16] = {0x64,0x61,0x74,0x61, 0xF3,0xAC, 0xD3,0x11, 0x8C,0xD1, 0x00,0xC0,0x4F,0x8E,0xDB,0x8A};    
    


static DRWAV_INLINE int drwav__is_little_endian(void)
{
#if defined(DRWAV_X86) || defined(DRWAV_X64)
    return DRWAV_TRUE;
#elif defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && __BYTE_ORDER == __LITTLE_ENDIAN
    return DRWAV_TRUE;
#else
    int n = 1;
    return (*(char*)&n) == 1;
#endif
}


static DRWAV_INLINE void drwav_bytes_to_guid(const drwav_uint8* data, drwav_uint8* guid)
{
    int i;
    for (i = 0; i < 16; ++i) {
        guid[i] = data[i];
    }
}


static DRWAV_INLINE drwav_uint16 drwav__bswap16(drwav_uint16 n)
{
#ifdef DRWAV_HAS_BYTESWAP16_INTRINSIC
    #if defined(_MSC_VER)
        return _byteswap_ushort(n);
    #elif defined(__GNUC__) || defined(__clang__)
        return __builtin_bswap16(n);
    #else
        #error "This compiler does not support the byte swap intrinsic."
    #endif
#else
    return ((n & 0xFF00) >> 8) |
           ((n & 0x00FF) << 8);
#endif
}

static DRWAV_INLINE drwav_uint32 drwav__bswap32(drwav_uint32 n)
{
#ifdef DRWAV_HAS_BYTESWAP32_INTRINSIC
    #if defined(_MSC_VER)
        return _byteswap_ulong(n);
    #elif defined(__GNUC__) || defined(__clang__)
        #if defined(DRWAV_ARM) && (defined(__ARM_ARCH) && __ARM_ARCH >= 6) && !defined(DRWAV_64BIT)   
            
            drwav_uint32 r;
            __asm__ __volatile__ (
            #if defined(DRWAV_64BIT)
                "rev %w[out], %w[in]" : [out]"=r"(r) : [in]"r"(n)   
            #else
                "rev %[out], %[in]" : [out]"=r"(r) : [in]"r"(n)
            #endif
            );
            return r;
        #else
            return __builtin_bswap32(n);
        #endif
    #else
        #error "This compiler does not support the byte swap intrinsic."
    #endif
#else
    return ((n & 0xFF000000) >> 24) |
           ((n & 0x00FF0000) >>  8) |
           ((n & 0x0000FF00) <<  8) |
           ((n & 0x000000FF) << 24);
#endif
}

static DRWAV_INLINE drwav_uint64 drwav__bswap64(drwav_uint64 n)
{
#ifdef DRWAV_HAS_BYTESWAP64_INTRINSIC
    #if defined(_MSC_VER)
        return _byteswap_uint64(n);
    #elif defined(__GNUC__) || defined(__clang__)
        return __builtin_bswap64(n);
    #else
        #error "This compiler does not support the byte swap intrinsic."
    #endif
#else
    
    return ((n & ((drwav_uint64)0xFF000000 << 32)) >> 56) |
           ((n & ((drwav_uint64)0x00FF0000 << 32)) >> 40) |
           ((n & ((drwav_uint64)0x0000FF00 << 32)) >> 24) |
           ((n & ((drwav_uint64)0x000000FF << 32)) >>  8) |
           ((n & ((drwav_uint64)0xFF000000      )) <<  8) |
           ((n & ((drwav_uint64)0x00FF0000      )) << 24) |
           ((n & ((drwav_uint64)0x0000FF00      )) << 40) |
           ((n & ((drwav_uint64)0x000000FF      )) << 56);
#endif
}


static DRWAV_INLINE drwav_int16 drwav__bswap_s16(drwav_int16 n)
{
    return (drwav_int16)drwav__bswap16((drwav_uint16)n);
}

static DRWAV_INLINE void drwav__bswap_samples_s16(drwav_int16* pSamples, drwav_uint64 sampleCount)
{
    drwav_uint64 iSample;
    for (iSample = 0; iSample < sampleCount; iSample += 1) {
        pSamples[iSample] = drwav__bswap_s16(pSamples[iSample]);
    }
}


static DRWAV_INLINE void drwav__bswap_s24(drwav_uint8* p)
{
    drwav_uint8 t;
    t = p[0];
    p[0] = p[2];
    p[2] = t;
}

static DRWAV_INLINE void drwav__bswap_samples_s24(drwav_uint8* pSamples, drwav_uint64 sampleCount)
{
    drwav_uint64 iSample;
    for (iSample = 0; iSample < sampleCount; iSample += 1) {
        drwav_uint8* pSample = pSamples + (iSample*3);
        drwav__bswap_s24(pSample);
    }
}


static DRWAV_INLINE drwav_int32 drwav__bswap_s32(drwav_int32 n)
{
    return (drwav_int32)drwav__bswap32((drwav_uint32)n);
}

static DRWAV_INLINE void drwav__bswap_samples_s32(drwav_int32* pSamples, drwav_uint64 sampleCount)
{
    drwav_uint64 iSample;
    for (iSample = 0; iSample < sampleCount; iSample += 1) {
        pSamples[iSample] = drwav__bswap_s32(pSamples[iSample]);
    }
}


static DRWAV_INLINE drwav_int64 drwav__bswap_s64(drwav_int64 n)
{
    return (drwav_int64)drwav__bswap64((drwav_uint64)n);
}

static DRWAV_INLINE void drwav__bswap_samples_s64(drwav_int64* pSamples, drwav_uint64 sampleCount)
{
    drwav_uint64 iSample;
    for (iSample = 0; iSample < sampleCount; iSample += 1) {
        pSamples[iSample] = drwav__bswap_s64(pSamples[iSample]);
    }
}


static DRWAV_INLINE float drwav__bswap_f32(float n)
{
    union {
        drwav_uint32 i;
        float f;
    } x;
    x.f = n;
    x.i = drwav__bswap32(x.i);

    return x.f;
}

static DRWAV_INLINE void drwav__bswap_samples_f32(float* pSamples, drwav_uint64 sampleCount)
{
    drwav_uint64 iSample;
    for (iSample = 0; iSample < sampleCount; iSample += 1) {
        pSamples[iSample] = drwav__bswap_f32(pSamples[iSample]);
    }
}


static DRWAV_INLINE void drwav__bswap_samples(void* pSamples, drwav_uint64 sampleCount, drwav_uint32 bytesPerSample)
{
    switch (bytesPerSample)
    {
        case 1:
        {
            
        } break;
        case 2:
        {
            drwav__bswap_samples_s16((drwav_int16*)pSamples, sampleCount);
        } break;
        case 3:
        {
            drwav__bswap_samples_s24((drwav_uint8*)pSamples, sampleCount);
        } break;
        case 4:
        {
            drwav__bswap_samples_s32((drwav_int32*)pSamples, sampleCount);
        } break;
        case 8:
        {
            drwav__bswap_samples_s64((drwav_int64*)pSamples, sampleCount);
        } break;
        default:
        {
            drwav_uint64 iSample;

            for (iSample = 0; iSample < sampleCount; iSample += 1) {
                drwav_uint8* pSample = (drwav_uint8*)pSamples + (iSample * bytesPerSample);
                drwav_uint32 iByte;

                for (iByte = 0; iByte < bytesPerSample / 2; iByte += 1) {
                    drwav_uint8 temp = pSample[iByte];
                    pSample[iByte] = pSample[bytesPerSample - iByte - 1];
                    pSample[bytesPerSample - iByte - 1] = temp;
                }
            }
        } break;
    }
}



DRWAV_PRIVATE DRWAV_INLINE drwav_bool32 drwav_is_container_be(drwav_container container)
{
    if (container == drwav_container_rifx || container == drwav_container_aiff) {
        return DRWAV_TRUE;
    } else {
        return DRWAV_FALSE;
    }
}


DRWAV_PRIVATE DRWAV_INLINE drwav_uint16 drwav_bytes_to_u16_le(const drwav_uint8* data)
{
    return ((drwav_uint16)data[0] << 0) | ((drwav_uint16)data[1] << 8);
}

DRWAV_PRIVATE DRWAV_INLINE drwav_uint16 drwav_bytes_to_u16_be(const drwav_uint8* data)
{
    return ((drwav_uint16)data[1] << 0) | ((drwav_uint16)data[0] << 8);
}

DRWAV_PRIVATE DRWAV_INLINE drwav_uint16 drwav_bytes_to_u16_ex(const drwav_uint8* data, drwav_container container)
{
    if (drwav_is_container_be(container)) {
        return drwav_bytes_to_u16_be(data);
    } else {
        return drwav_bytes_to_u16_le(data);
    }
}


DRWAV_PRIVATE DRWAV_INLINE drwav_uint32 drwav_bytes_to_u32_le(const drwav_uint8* data)
{
    return ((drwav_uint32)data[0] << 0) | ((drwav_uint32)data[1] << 8) | ((drwav_uint32)data[2] << 16) | ((drwav_uint32)data[3] << 24);
}

DRWAV_PRIVATE DRWAV_INLINE drwav_uint32 drwav_bytes_to_u32_be(const drwav_uint8* data)
{
    return ((drwav_uint32)data[3] << 0) | ((drwav_uint32)data[2] << 8) | ((drwav_uint32)data[1] << 16) | ((drwav_uint32)data[0] << 24);
}

DRWAV_PRIVATE DRWAV_INLINE drwav_uint32 drwav_bytes_to_u32_ex(const drwav_uint8* data, drwav_container container)
{
    if (drwav_is_container_be(container)) {
        return drwav_bytes_to_u32_be(data);
    } else {
        return drwav_bytes_to_u32_le(data);
    }
}



DRWAV_PRIVATE drwav_int64 drwav_aiff_extended_to_s64(const drwav_uint8* data)
{
    drwav_uint32 exponent = ((drwav_uint32)data[0] << 8) | data[1];
    drwav_uint64 hi = ((drwav_uint64)data[2] << 24) | ((drwav_uint64)data[3] << 16) | ((drwav_uint64)data[4] <<  8) | ((drwav_uint64)data[5] <<  0);
    drwav_uint64 lo = ((drwav_uint64)data[6] << 24) | ((drwav_uint64)data[7] << 16) | ((drwav_uint64)data[8] <<  8) | ((drwav_uint64)data[9] <<  0);
    drwav_uint64 significand = (hi << 32) | lo;
    int sign = exponent >> 15;

    
    exponent &= 0x7FFF;

    
    if (exponent == 0 && significand == 0) {
        return 0;
    } else if (exponent == 0x7FFF) {
        return sign ? DRWAV_INT64_MIN : DRWAV_INT64_MAX;    
    }

    exponent -= 16383;

    if (exponent >= 63) {
        return sign ? DRWAV_INT64_MIN : DRWAV_INT64_MAX;    
    } else if (exponent < 1) {
        return 0;  
    }

    significand >>= (63 - exponent);

    if (sign) {
        return -(drwav_int64)significand;
    } else {
        return  (drwav_int64)significand;
    }
}


DRWAV_PRIVATE void* drwav__malloc_default(size_t sz, void* pUserData)
{
    (void)pUserData;
    return DRWAV_MALLOC(sz);
}

DRWAV_PRIVATE void* drwav__realloc_default(void* p, size_t sz, void* pUserData)
{
    (void)pUserData;
    return DRWAV_REALLOC(p, sz);
}

DRWAV_PRIVATE void drwav__free_default(void* p, void* pUserData)
{
    (void)pUserData;
    DRWAV_FREE(p);
}


DRWAV_PRIVATE void* drwav__malloc_from_callbacks(size_t sz, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pAllocationCallbacks == NULL) {
        return NULL;
    }

    if (pAllocationCallbacks->onMalloc != NULL) {
        return pAllocationCallbacks->onMalloc(sz, pAllocationCallbacks->pUserData);
    }

    
    if (pAllocationCallbacks->onRealloc != NULL) {
        return pAllocationCallbacks->onRealloc(NULL, sz, pAllocationCallbacks->pUserData);
    }

    return NULL;
}

DRWAV_PRIVATE void* drwav__realloc_from_callbacks(void* p, size_t szNew, size_t szOld, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pAllocationCallbacks == NULL) {
        return NULL;
    }

    if (pAllocationCallbacks->onRealloc != NULL) {
        return pAllocationCallbacks->onRealloc(p, szNew, pAllocationCallbacks->pUserData);
    }

    
    if (pAllocationCallbacks->onMalloc != NULL && pAllocationCallbacks->onFree != NULL) {
        void* p2;

        p2 = pAllocationCallbacks->onMalloc(szNew, pAllocationCallbacks->pUserData);
        if (p2 == NULL) {
            return NULL;
        }

        if (p != NULL) {
            DRWAV_COPY_MEMORY(p2, p, szOld);
            pAllocationCallbacks->onFree(p, pAllocationCallbacks->pUserData);
        }

        return p2;
    }

    return NULL;
}

DRWAV_PRIVATE void drwav__free_from_callbacks(void* p, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (p == NULL || pAllocationCallbacks == NULL) {
        return;
    }

    if (pAllocationCallbacks->onFree != NULL) {
        pAllocationCallbacks->onFree(p, pAllocationCallbacks->pUserData);
    }
}


DRWAV_PRIVATE drwav_allocation_callbacks drwav_copy_allocation_callbacks_or_defaults(const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pAllocationCallbacks != NULL) {
        
        return *pAllocationCallbacks;
    } else {
        
        drwav_allocation_callbacks allocationCallbacks;
        allocationCallbacks.pUserData = NULL;
        allocationCallbacks.onMalloc  = drwav__malloc_default;
        allocationCallbacks.onRealloc = drwav__realloc_default;
        allocationCallbacks.onFree    = drwav__free_default;
        return allocationCallbacks;
    }
}


static DRWAV_INLINE drwav_bool32 drwav__is_compressed_format_tag(drwav_uint16 formatTag)
{
    return
        formatTag == DR_WAVE_FORMAT_ADPCM ||
        formatTag == DR_WAVE_FORMAT_DVI_ADPCM;
}

DRWAV_PRIVATE unsigned int drwav__chunk_padding_size_riff(drwav_uint64 chunkSize)
{
    return (unsigned int)(chunkSize % 2);
}

DRWAV_PRIVATE unsigned int drwav__chunk_padding_size_w64(drwav_uint64 chunkSize)
{
    return (unsigned int)(chunkSize % 8);
}

DRWAV_PRIVATE unsigned int drwav_calculate_padding_size(drwav_container container, drwav_uint64 chunkSize)
{
    if (container == drwav_container_riff || container == drwav_container_rf64) {
        return drwav__chunk_padding_size_riff(chunkSize);
    } else {
        return drwav__chunk_padding_size_w64(chunkSize);
    }
}

DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_s16__msadpcm(drwav* pWav, drwav_uint64 samplesToRead, drwav_int16* pBufferOut);
DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_s16__ima(drwav* pWav, drwav_uint64 samplesToRead, drwav_int16* pBufferOut);
DRWAV_PRIVATE drwav_bool32 drwav_init_write__internal(drwav* pWav, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount);

DRWAV_PRIVATE drwav_result drwav__read_chunk_header(drwav_read_proc onRead, void* pUserData, drwav_container container, drwav_uint64* pRunningBytesReadOut, drwav_chunk_header* pHeaderOut)
{
    if (container == drwav_container_riff || container == drwav_container_rifx || container == drwav_container_rf64 || container == drwav_container_aiff) {
        drwav_uint8 sizeInBytes[4];

        if (onRead(pUserData, pHeaderOut->id.fourcc, 4) != 4) {
            return DRWAV_AT_END;
        }

        if (onRead(pUserData, sizeInBytes, 4) != 4) {
            return DRWAV_INVALID_FILE;
        }

        pHeaderOut->sizeInBytes = drwav_bytes_to_u32_ex(sizeInBytes, container);
        pHeaderOut->paddingSize = drwav__chunk_padding_size_riff(pHeaderOut->sizeInBytes);

        *pRunningBytesReadOut += 8;
    } else if (container == drwav_container_w64) {
        drwav_uint8 sizeInBytes[8];

        if (onRead(pUserData, pHeaderOut->id.guid, 16) != 16) {
            return DRWAV_AT_END;
        }

        if (onRead(pUserData, sizeInBytes, 8) != 8) {
            return DRWAV_INVALID_FILE;
        }

        pHeaderOut->sizeInBytes = drwav_bytes_to_u64(sizeInBytes);

        
        if (pHeaderOut->sizeInBytes >= 24) {
            pHeaderOut->sizeInBytes -= 24;
        } else {
            return DRWAV_INVALID_FILE;
        }

        pHeaderOut->paddingSize = drwav__chunk_padding_size_w64(pHeaderOut->sizeInBytes);
        *pRunningBytesReadOut += 24;
    } else {
        return DRWAV_INVALID_FILE;
    }

    return DRWAV_SUCCESS;
}

DRWAV_PRIVATE drwav_bool32 drwav__seek_forward(drwav_seek_proc onSeek, drwav_uint64 offset, void* pUserData)
{
    drwav_uint64 bytesRemainingToSeek = offset;
    while (bytesRemainingToSeek > 0) {
        if (bytesRemainingToSeek > 0x7FFFFFFF) {
            if (!onSeek(pUserData, 0x7FFFFFFF, DRWAV_SEEK_CUR)) {
                return DRWAV_FALSE;
            }
            bytesRemainingToSeek -= 0x7FFFFFFF;
        } else {
            if (!onSeek(pUserData, (int)bytesRemainingToSeek, DRWAV_SEEK_CUR)) {
                return DRWAV_FALSE;
            }
            bytesRemainingToSeek = 0;
        }
    }

    return DRWAV_TRUE;
}

DRWAV_PRIVATE drwav_bool32 drwav__seek_from_start(drwav_seek_proc onSeek, drwav_uint64 offset, void* pUserData)
{
    if (offset <= 0x7FFFFFFF) {
        return onSeek(pUserData, (int)offset, DRWAV_SEEK_SET);
    }

    
    if (!onSeek(pUserData, 0x7FFFFFFF, DRWAV_SEEK_SET)) {
        return DRWAV_FALSE;
    }
    offset -= 0x7FFFFFFF;

    for (;;) {
        if (offset <= 0x7FFFFFFF) {
            return onSeek(pUserData, (int)offset, DRWAV_SEEK_CUR);
        }

        if (!onSeek(pUserData, 0x7FFFFFFF, DRWAV_SEEK_CUR)) {
            return DRWAV_FALSE;
        }
        offset -= 0x7FFFFFFF;
    }

    
    
}



DRWAV_PRIVATE size_t drwav__on_read(drwav_read_proc onRead, void* pUserData, void* pBufferOut, size_t bytesToRead, drwav_uint64* pCursor)
{
    size_t bytesRead;

    DRWAV_ASSERT(onRead != NULL);
    DRWAV_ASSERT(pCursor != NULL);

    bytesRead = onRead(pUserData, pBufferOut, bytesToRead);
    *pCursor += bytesRead;
    return bytesRead;
}

#if 0
DRWAV_PRIVATE drwav_bool32 drwav__on_seek(drwav_seek_proc onSeek, void* pUserData, int offset, drwav_seek_origin origin, drwav_uint64* pCursor)
{
    DRWAV_ASSERT(onSeek != NULL);
    DRWAV_ASSERT(pCursor != NULL);

    if (!onSeek(pUserData, offset, origin)) {
        return DRWAV_FALSE;
    }

    if (origin == DRWAV_SEEK_SET) {
        *pCursor = offset;
    } else {
        *pCursor += offset;
    }

    return DRWAV_TRUE;
}
#endif


#define DRWAV_SMPL_BYTES                    36
#define DRWAV_SMPL_LOOP_BYTES               24
#define DRWAV_INST_BYTES                    7
#define DRWAV_ACID_BYTES                    24
#define DRWAV_CUE_BYTES                     4
#define DRWAV_BEXT_BYTES                    602
#define DRWAV_BEXT_DESCRIPTION_BYTES        256
#define DRWAV_BEXT_ORIGINATOR_NAME_BYTES    32
#define DRWAV_BEXT_ORIGINATOR_REF_BYTES     32
#define DRWAV_BEXT_RESERVED_BYTES           180
#define DRWAV_BEXT_UMID_BYTES               64
#define DRWAV_CUE_POINT_BYTES               24
#define DRWAV_LIST_LABEL_OR_NOTE_BYTES      4
#define DRWAV_LIST_LABELLED_TEXT_BYTES      20

#define DRWAV_METADATA_ALIGNMENT            8

typedef enum
{
    drwav__metadata_parser_stage_count,
    drwav__metadata_parser_stage_read
} drwav__metadata_parser_stage;

typedef struct
{
    drwav_read_proc onRead;
    drwav_seek_proc onSeek;
    void *pReadSeekUserData;
    drwav__metadata_parser_stage stage;
    drwav_metadata *pMetadata;
    drwav_uint32 metadataCount;
    drwav_uint8 *pData;
    drwav_uint8 *pDataCursor;
    drwav_uint64 metadataCursor;
    drwav_uint64 extraCapacity;
} drwav__metadata_parser;

DRWAV_PRIVATE size_t drwav__metadata_memory_capacity(drwav__metadata_parser* pParser)
{
    drwav_uint64 cap = sizeof(drwav_metadata) * (drwav_uint64)pParser->metadataCount + pParser->extraCapacity;
    if (cap > DRWAV_SIZE_MAX) {
        return 0;   
    }

    return (size_t)cap; 
}

DRWAV_PRIVATE drwav_uint8* drwav__metadata_get_memory(drwav__metadata_parser* pParser, size_t size, size_t align)
{
    drwav_uint8* pResult;

    if (align) {
        drwav_uintptr modulo = (drwav_uintptr)pParser->pDataCursor % align;
        if (modulo != 0) {
            pParser->pDataCursor += align - modulo;
        }
    }

    pResult = pParser->pDataCursor;

    



    DRWAV_ASSERT((pResult + size) <= (pParser->pData + drwav__metadata_memory_capacity(pParser)));

    pParser->pDataCursor += size;
    return pResult;
}

DRWAV_PRIVATE void drwav__metadata_request_extra_memory_for_stage_2(drwav__metadata_parser* pParser, drwav_uint64 bytes, size_t align)
{
    pParser->extraCapacity += bytes;
    if (align > 0) {
        pParser->extraCapacity += align - 1;
    }
}

DRWAV_PRIVATE drwav_result drwav__metadata_alloc(drwav__metadata_parser* pParser, drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pParser->extraCapacity != 0 || pParser->metadataCount != 0) {
        size_t capacity;

        capacity = drwav__metadata_memory_capacity(pParser);
        if (capacity == 0) {
            return DRWAV_OUT_OF_MEMORY;
        }

        pAllocationCallbacks->onFree(pParser->pData, pAllocationCallbacks->pUserData);

        pParser->pData = (drwav_uint8*)pAllocationCallbacks->onMalloc(capacity, pAllocationCallbacks->pUserData);
        pParser->pDataCursor = pParser->pData;

        if (pParser->pData == NULL) {
            return DRWAV_OUT_OF_MEMORY;
        }

        




        pParser->pMetadata = (drwav_metadata*)drwav__metadata_get_memory(pParser, sizeof(drwav_metadata) * pParser->metadataCount, 1);
        pParser->metadataCursor = 0;
    }

    return DRWAV_SUCCESS;
}

DRWAV_PRIVATE drwav_metadata* drwav__metadata_get_next_object(drwav__metadata_parser* pParser)
{
    drwav_metadata* pMetadata;

    DRWAV_ASSERT(pParser->stage == drwav__metadata_parser_stage_read);

    if (pParser->metadataCursor >= pParser->metadataCount) {
        return NULL;
    }

    
    pMetadata = &pParser->pMetadata[pParser->metadataCursor];
    DRWAV_ZERO_OBJECT(pMetadata);

    return pMetadata;
}

DRWAV_PRIVATE size_t drwav__metadata_parser_read(drwav__metadata_parser* pParser, void* pBufferOut, size_t bytesToRead, drwav_uint64* pCursor)
{
    if (pCursor != NULL) {
        return drwav__on_read(pParser->onRead, pParser->pReadSeekUserData, pBufferOut, bytesToRead, pCursor);
    } else {
        return pParser->onRead(pParser->pReadSeekUserData, pBufferOut, bytesToRead);
    }
}

DRWAV_PRIVATE drwav_bool32 drwav__metadata_validate_smpl_chunk(const drwav_chunk_header* pChunkHeader, drwav_uint32 loopCount, drwav_uint32 samplerSpecificDataSizeInBytes, drwav_uint64* pTrailingDataSizeInBytes)
{
    drwav_uint64 remainingDataSizeInBytes;

    DRWAV_ASSERT(pChunkHeader != NULL);

    if (pChunkHeader->sizeInBytes < DRWAV_SMPL_BYTES) {
        return DRWAV_FALSE;
    }

    remainingDataSizeInBytes = pChunkHeader->sizeInBytes - DRWAV_SMPL_BYTES;
    if ((drwav_uint64)loopCount > remainingDataSizeInBytes / DRWAV_SMPL_LOOP_BYTES) {
        return DRWAV_FALSE;
    }

    remainingDataSizeInBytes -= (drwav_uint64)loopCount * DRWAV_SMPL_LOOP_BYTES;
    if ((drwav_uint64)samplerSpecificDataSizeInBytes > remainingDataSizeInBytes) {
        return DRWAV_FALSE;
    }

    if (pTrailingDataSizeInBytes != NULL) {
        *pTrailingDataSizeInBytes = remainingDataSizeInBytes - samplerSpecificDataSizeInBytes;
    }

    return DRWAV_TRUE;
}

DRWAV_PRIVATE drwav_uint64 drwav__read_smpl_to_metadata_obj(drwav__metadata_parser* pParser, const drwav_chunk_header* pChunkHeader, drwav_metadata* pMetadata, drwav_result* pResult)
{
    drwav_uint8 smplHeaderData[DRWAV_SMPL_BYTES];
    drwav_uint64 totalBytesRead = 0;
    size_t bytesJustRead;

    *pResult = DRWAV_INVALID_DATA;

    if (pMetadata == NULL) {
        return 0;
    }

    bytesJustRead = drwav__metadata_parser_read(pParser, smplHeaderData, sizeof(smplHeaderData), &totalBytesRead);

    DRWAV_ASSERT(pParser->stage == drwav__metadata_parser_stage_read);
    DRWAV_ASSERT(pChunkHeader != NULL);

    if (bytesJustRead == sizeof(smplHeaderData)) {
        drwav_uint32 iSampleLoop;
        drwav_uint32 loopCount;
        drwav_uint32 samplerSpecificDataSizeInBytes;
        drwav_uint64 trailingDataSizeInBytes;

        




        loopCount = drwav_bytes_to_u32(smplHeaderData + 28);
        samplerSpecificDataSizeInBytes = drwav_bytes_to_u32(smplHeaderData + 32);
        if (!drwav__metadata_validate_smpl_chunk(pChunkHeader, loopCount, samplerSpecificDataSizeInBytes, &trailingDataSizeInBytes)) {
            return totalBytesRead;
        }

        pMetadata->type                                     = drwav_metadata_type_smpl;
        pMetadata->data.smpl.manufacturerId                 = drwav_bytes_to_u32(smplHeaderData + 0);
        pMetadata->data.smpl.productId                      = drwav_bytes_to_u32(smplHeaderData + 4);
        pMetadata->data.smpl.samplePeriodNanoseconds        = drwav_bytes_to_u32(smplHeaderData + 8);
        pMetadata->data.smpl.midiUnityNote                  = drwav_bytes_to_u32(smplHeaderData + 12);
        pMetadata->data.smpl.midiPitchFraction              = drwav_bytes_to_u32(smplHeaderData + 16);
        pMetadata->data.smpl.smpteFormat                    = drwav_bytes_to_u32(smplHeaderData + 20);
        pMetadata->data.smpl.smpteOffset                    = drwav_bytes_to_u32(smplHeaderData + 24);
        pMetadata->data.smpl.sampleLoopCount                = loopCount;
        pMetadata->data.smpl.samplerSpecificDataSizeInBytes = samplerSpecificDataSizeInBytes;

        pMetadata->data.smpl.pLoops = (drwav_smpl_loop*)drwav__metadata_get_memory(pParser, sizeof(drwav_smpl_loop) * pMetadata->data.smpl.sampleLoopCount, DRWAV_METADATA_ALIGNMENT);

        for (iSampleLoop = 0; iSampleLoop < pMetadata->data.smpl.sampleLoopCount; ++iSampleLoop) {
            drwav_uint8 smplLoopData[DRWAV_SMPL_LOOP_BYTES];
            bytesJustRead = drwav__metadata_parser_read(pParser, smplLoopData, sizeof(smplLoopData), &totalBytesRead);

            if (bytesJustRead == sizeof(smplLoopData)) {
                pMetadata->data.smpl.pLoops[iSampleLoop].cuePointId        = drwav_bytes_to_u32(smplLoopData + 0);
                pMetadata->data.smpl.pLoops[iSampleLoop].type              = drwav_bytes_to_u32(smplLoopData + 4);
                pMetadata->data.smpl.pLoops[iSampleLoop].firstSampleOffset = drwav_bytes_to_u32(smplLoopData + 8);
                pMetadata->data.smpl.pLoops[iSampleLoop].lastSampleOffset  = drwav_bytes_to_u32(smplLoopData + 12);
                pMetadata->data.smpl.pLoops[iSampleLoop].sampleFraction    = drwav_bytes_to_u32(smplLoopData + 16);
                pMetadata->data.smpl.pLoops[iSampleLoop].playCount         = drwav_bytes_to_u32(smplLoopData + 20);
            } else {
                return totalBytesRead;
            }
        }

        if (pMetadata->data.smpl.samplerSpecificDataSizeInBytes > 0) {
            pMetadata->data.smpl.pSamplerSpecificData = drwav__metadata_get_memory(pParser, pMetadata->data.smpl.samplerSpecificDataSizeInBytes, 1);
            DRWAV_ASSERT(pMetadata->data.smpl.pSamplerSpecificData != NULL);

            bytesJustRead = drwav__metadata_parser_read(pParser, pMetadata->data.smpl.pSamplerSpecificData, pMetadata->data.smpl.samplerSpecificDataSizeInBytes, &totalBytesRead);
            if (bytesJustRead != pMetadata->data.smpl.samplerSpecificDataSizeInBytes) {
                return totalBytesRead;
            }
        }

        if (trailingDataSizeInBytes > 0) {
            if (!drwav__seek_forward(pParser->onSeek, trailingDataSizeInBytes, pParser->pReadSeekUserData)) {
                return totalBytesRead;
            }
            totalBytesRead += trailingDataSizeInBytes;
        }

        *pResult = DRWAV_SUCCESS;
    }

    return totalBytesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav__read_cue_to_metadata_obj(drwav__metadata_parser* pParser, const drwav_chunk_header* pChunkHeader, drwav_metadata* pMetadata, drwav_result* pResult)
{
    drwav_uint8 cueHeaderSectionData[DRWAV_CUE_BYTES];
    drwav_uint64 totalBytesRead = 0;
    size_t bytesJustRead;

    *pResult = DRWAV_INVALID_DATA;

    if (pMetadata == NULL) {
        return 0;
    }

    bytesJustRead = drwav__metadata_parser_read(pParser, cueHeaderSectionData, sizeof(cueHeaderSectionData), &totalBytesRead);

    DRWAV_ASSERT(pParser->stage == drwav__metadata_parser_stage_read);

    if (bytesJustRead == sizeof(cueHeaderSectionData)) {
        pMetadata->type                   = drwav_metadata_type_cue;
        pMetadata->data.cue.cuePointCount = drwav_bytes_to_u32(cueHeaderSectionData);

        



        if (pMetadata->data.cue.cuePointCount == (pChunkHeader->sizeInBytes - DRWAV_CUE_BYTES) / DRWAV_CUE_POINT_BYTES) {
            pMetadata->data.cue.pCuePoints    = (drwav_cue_point*)drwav__metadata_get_memory(pParser, sizeof(drwav_cue_point) * pMetadata->data.cue.cuePointCount, DRWAV_METADATA_ALIGNMENT);
            DRWAV_ASSERT(pMetadata->data.cue.pCuePoints != NULL);

            if (pMetadata->data.cue.cuePointCount > 0) {
                drwav_uint32 iCuePoint;

                for (iCuePoint = 0; iCuePoint < pMetadata->data.cue.cuePointCount; ++iCuePoint) {
                    drwav_uint8 cuePointData[DRWAV_CUE_POINT_BYTES];
                    bytesJustRead = drwav__metadata_parser_read(pParser, cuePointData, sizeof(cuePointData), &totalBytesRead);

                    if (bytesJustRead == sizeof(cuePointData)) {
                        pMetadata->data.cue.pCuePoints[iCuePoint].id                = drwav_bytes_to_u32(cuePointData + 0);
                        pMetadata->data.cue.pCuePoints[iCuePoint].playOrderPosition = drwav_bytes_to_u32(cuePointData + 4);
                        pMetadata->data.cue.pCuePoints[iCuePoint].dataChunkId[0]    = cuePointData[8];
                        pMetadata->data.cue.pCuePoints[iCuePoint].dataChunkId[1]    = cuePointData[9];
                        pMetadata->data.cue.pCuePoints[iCuePoint].dataChunkId[2]    = cuePointData[10];
                        pMetadata->data.cue.pCuePoints[iCuePoint].dataChunkId[3]    = cuePointData[11];
                        pMetadata->data.cue.pCuePoints[iCuePoint].chunkStart        = drwav_bytes_to_u32(cuePointData + 12);
                        pMetadata->data.cue.pCuePoints[iCuePoint].blockStart        = drwav_bytes_to_u32(cuePointData + 16);
                        pMetadata->data.cue.pCuePoints[iCuePoint].sampleOffset      = drwav_bytes_to_u32(cuePointData + 20);
                    } else {
                        return totalBytesRead;
                    }
                }
            }

            *pResult = DRWAV_SUCCESS;
        }
    }

    return totalBytesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav__read_inst_to_metadata_obj(drwav__metadata_parser* pParser, drwav_metadata* pMetadata)
{
    drwav_uint8 instData[DRWAV_INST_BYTES];
    drwav_uint64 bytesRead;

    if (pMetadata == NULL) {
        return 0;
    }

    bytesRead = drwav__metadata_parser_read(pParser, instData, sizeof(instData), NULL);

    DRWAV_ASSERT(pParser->stage == drwav__metadata_parser_stage_read);

    if (bytesRead == sizeof(instData)) {
        pMetadata->type                    = drwav_metadata_type_inst;
        pMetadata->data.inst.midiUnityNote = (drwav_int8)instData[0];
        pMetadata->data.inst.fineTuneCents = (drwav_int8)instData[1];
        pMetadata->data.inst.gainDecibels  = (drwav_int8)instData[2];
        pMetadata->data.inst.lowNote       = (drwav_int8)instData[3];
        pMetadata->data.inst.highNote      = (drwav_int8)instData[4];
        pMetadata->data.inst.lowVelocity   = (drwav_int8)instData[5];
        pMetadata->data.inst.highVelocity  = (drwav_int8)instData[6];
    }

    return bytesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav__read_acid_to_metadata_obj(drwav__metadata_parser* pParser, drwav_metadata* pMetadata)
{
    drwav_uint8 acidData[DRWAV_ACID_BYTES];
    drwav_uint64 bytesRead;

    if (pMetadata == NULL) {
        return 0;
    }

    bytesRead = drwav__metadata_parser_read(pParser, acidData, sizeof(acidData), NULL);

    DRWAV_ASSERT(pParser->stage == drwav__metadata_parser_stage_read);

    if (bytesRead == sizeof(acidData)) {
        pMetadata->type                       = drwav_metadata_type_acid;
        pMetadata->data.acid.flags            = drwav_bytes_to_u32(acidData + 0);
        pMetadata->data.acid.midiUnityNote    = drwav_bytes_to_u16(acidData + 4);
        pMetadata->data.acid.reserved1        = drwav_bytes_to_u16(acidData + 6);
        pMetadata->data.acid.reserved2        = drwav_bytes_to_f32(acidData + 8);
        pMetadata->data.acid.numBeats         = drwav_bytes_to_u32(acidData + 12);
        pMetadata->data.acid.meterDenominator = drwav_bytes_to_u16(acidData + 16);
        pMetadata->data.acid.meterNumerator   = drwav_bytes_to_u16(acidData + 18);
        pMetadata->data.acid.tempo            = drwav_bytes_to_f32(acidData + 20);
    }

    return bytesRead;
}

DRWAV_PRIVATE size_t drwav__strlen(const char* str)
{
    size_t result = 0;

    while (*str++) {
        result += 1;
    }

    return result;
}

DRWAV_PRIVATE size_t drwav__strlen_clamped(const char* str, size_t maxToRead)
{
    size_t result = 0;

    while (*str++ && result < maxToRead) {
        result += 1;
    }

    return result;
}

DRWAV_PRIVATE char* drwav__metadata_copy_string(drwav__metadata_parser* pParser, const char* str, size_t maxToRead)
{
    size_t len = drwav__strlen_clamped(str, maxToRead);

    if (len) {
        char* result = (char*)drwav__metadata_get_memory(pParser, len + 1, 1);
        DRWAV_ASSERT(result != NULL);

        DRWAV_COPY_MEMORY(result, str, len);
        result[len] = '\0';

        return result;
    } else {
        return NULL;
    }
}

typedef struct
{
    const void* pBuffer;
    size_t sizeInBytes;
    size_t cursor;
} drwav_buffer_reader;

DRWAV_PRIVATE drwav_result drwav_buffer_reader_init(const void* pBuffer, size_t sizeInBytes, drwav_buffer_reader* pReader)
{
    DRWAV_ASSERT(pBuffer != NULL);
    DRWAV_ASSERT(pReader != NULL);

    DRWAV_ZERO_OBJECT(pReader);

    pReader->pBuffer     = pBuffer;
    pReader->sizeInBytes = sizeInBytes;
    pReader->cursor      = 0;

    return DRWAV_SUCCESS;
}

DRWAV_PRIVATE const void* drwav_buffer_reader_ptr(const drwav_buffer_reader* pReader)
{
    DRWAV_ASSERT(pReader != NULL);

    return drwav_offset_ptr(pReader->pBuffer, pReader->cursor);
}

DRWAV_PRIVATE drwav_result drwav_buffer_reader_seek(drwav_buffer_reader* pReader, size_t bytesToSeek)
{
    DRWAV_ASSERT(pReader != NULL);

    if (pReader->cursor + bytesToSeek > pReader->sizeInBytes) {
        return DRWAV_BAD_SEEK;  
    }

    pReader->cursor += bytesToSeek;

    return DRWAV_SUCCESS;
}

DRWAV_PRIVATE drwav_result drwav_buffer_reader_read(drwav_buffer_reader* pReader, void* pDst, size_t bytesToRead, size_t* pBytesRead)
{
    drwav_result result = DRWAV_SUCCESS;
    size_t bytesRemaining;

    DRWAV_ASSERT(pReader != NULL);

    if (pBytesRead != NULL) {
        *pBytesRead = 0;
    }

    bytesRemaining = (pReader->sizeInBytes - pReader->cursor);
    if (bytesToRead > bytesRemaining) {
        bytesToRead = bytesRemaining;
    }

    if (pDst == NULL) {
        
        result = drwav_buffer_reader_seek(pReader, bytesToRead);
    } else {
        
        DRWAV_COPY_MEMORY(pDst, drwav_buffer_reader_ptr(pReader), bytesToRead);
        pReader->cursor += bytesToRead;
    }

    DRWAV_ASSERT(pReader->cursor <= pReader->sizeInBytes);

    if (result == DRWAV_SUCCESS) {
        if (pBytesRead != NULL) {
            *pBytesRead = bytesToRead;
        }
    }

    return DRWAV_SUCCESS;
}

DRWAV_PRIVATE drwav_result drwav_buffer_reader_read_u16(drwav_buffer_reader* pReader, drwav_uint16* pDst)
{
    drwav_result result;
    size_t bytesRead;
    drwav_uint8 data[2];

    DRWAV_ASSERT(pReader != NULL);
    DRWAV_ASSERT(pDst != NULL);

    *pDst = 0;  

    result = drwav_buffer_reader_read(pReader, data, sizeof(*pDst), &bytesRead);
    if (result != DRWAV_SUCCESS || bytesRead != sizeof(*pDst)) {
        return result;
    }

    *pDst = drwav_bytes_to_u16(data);

    return DRWAV_SUCCESS;
}

DRWAV_PRIVATE drwav_result drwav_buffer_reader_read_u32(drwav_buffer_reader* pReader, drwav_uint32* pDst)
{
    drwav_result result;
    size_t bytesRead;
    drwav_uint8 data[4];

    DRWAV_ASSERT(pReader != NULL);
    DRWAV_ASSERT(pDst != NULL);

    *pDst = 0;  

    result = drwav_buffer_reader_read(pReader, data, sizeof(*pDst), &bytesRead);
    if (result != DRWAV_SUCCESS || bytesRead != sizeof(*pDst)) {
        return result;
    }

    *pDst = drwav_bytes_to_u32(data);

    return DRWAV_SUCCESS;
}



DRWAV_PRIVATE drwav_uint64 drwav__read_bext_to_metadata_obj(drwav__metadata_parser* pParser, drwav_metadata* pMetadata, drwav_uint64 chunkSize)
{
    drwav_uint8 bextData[DRWAV_BEXT_BYTES];
    size_t bytesRead;

    if (pMetadata == NULL) {
        return 0;
    }

    bytesRead = drwav__metadata_parser_read(pParser, bextData, sizeof(bextData), NULL);

    DRWAV_ASSERT(pParser->stage == drwav__metadata_parser_stage_read);

    if (bytesRead == sizeof(bextData)) {
        drwav_buffer_reader reader;
        drwav_uint32 timeReferenceLow;
        drwav_uint32 timeReferenceHigh;
        size_t extraBytes;

        pMetadata->type = drwav_metadata_type_bext;

        if (drwav_buffer_reader_init(bextData, bytesRead, &reader) == DRWAV_SUCCESS) {
            pMetadata->data.bext.pDescription = drwav__metadata_copy_string(pParser, (const char*)drwav_buffer_reader_ptr(&reader), DRWAV_BEXT_DESCRIPTION_BYTES);
            drwav_buffer_reader_seek(&reader, DRWAV_BEXT_DESCRIPTION_BYTES);

            pMetadata->data.bext.pOriginatorName = drwav__metadata_copy_string(pParser, (const char*)drwav_buffer_reader_ptr(&reader), DRWAV_BEXT_ORIGINATOR_NAME_BYTES);
            drwav_buffer_reader_seek(&reader, DRWAV_BEXT_ORIGINATOR_NAME_BYTES);

            pMetadata->data.bext.pOriginatorReference = drwav__metadata_copy_string(pParser, (const char*)drwav_buffer_reader_ptr(&reader), DRWAV_BEXT_ORIGINATOR_REF_BYTES);
            drwav_buffer_reader_seek(&reader, DRWAV_BEXT_ORIGINATOR_REF_BYTES);

            drwav_buffer_reader_read(&reader, pMetadata->data.bext.pOriginationDate, sizeof(pMetadata->data.bext.pOriginationDate), NULL);
            drwav_buffer_reader_read(&reader, pMetadata->data.bext.pOriginationTime, sizeof(pMetadata->data.bext.pOriginationTime), NULL);

            drwav_buffer_reader_read_u32(&reader, &timeReferenceLow);
            drwav_buffer_reader_read_u32(&reader, &timeReferenceHigh);
            pMetadata->data.bext.timeReference = ((drwav_uint64)timeReferenceHigh << 32) + timeReferenceLow;

            drwav_buffer_reader_read_u16(&reader, &pMetadata->data.bext.version);

            pMetadata->data.bext.pUMID = drwav__metadata_get_memory(pParser, DRWAV_BEXT_UMID_BYTES, 1);
            drwav_buffer_reader_read(&reader, pMetadata->data.bext.pUMID, DRWAV_BEXT_UMID_BYTES, NULL);

            drwav_buffer_reader_read_u16(&reader, &pMetadata->data.bext.loudnessValue);
            drwav_buffer_reader_read_u16(&reader, &pMetadata->data.bext.loudnessRange);
            drwav_buffer_reader_read_u16(&reader, &pMetadata->data.bext.maxTruePeakLevel);
            drwav_buffer_reader_read_u16(&reader, &pMetadata->data.bext.maxMomentaryLoudness);
            drwav_buffer_reader_read_u16(&reader, &pMetadata->data.bext.maxShortTermLoudness);

            DRWAV_ASSERT((drwav_offset_ptr(drwav_buffer_reader_ptr(&reader), DRWAV_BEXT_RESERVED_BYTES)) == (bextData + DRWAV_BEXT_BYTES));

            extraBytes = (size_t)(chunkSize - DRWAV_BEXT_BYTES);
            if (extraBytes > 0) {
                pMetadata->data.bext.pCodingHistory = (char*)drwav__metadata_get_memory(pParser, extraBytes + 1, 1);
                DRWAV_ASSERT(pMetadata->data.bext.pCodingHistory != NULL);

                pMetadata->data.bext.codingHistorySize = (drwav_uint32)drwav__metadata_parser_read(pParser, pMetadata->data.bext.pCodingHistory, extraBytes, NULL);
                pMetadata->data.bext.pCodingHistory[pMetadata->data.bext.codingHistorySize] = '\0'; 
                
                bytesRead += pMetadata->data.bext.codingHistorySize;
            } else {
                pMetadata->data.bext.pCodingHistory    = NULL;
                pMetadata->data.bext.codingHistorySize = 0;
            }
        }
    }

    return bytesRead;
}

DRWAV_PRIVATE size_t drwav__metadata_read_string(drwav__metadata_parser* pParser, drwav_uint32 stringSize, char** ppString, drwav_uint32* pStringLength)
{
    size_t bytesRead;
    drwav_uint32 stringLength;
    char* pString;
    
    *ppString = NULL;
    *pStringLength = 0;

    if (stringSize == 0) {
        return 0;
    }

    pString = (char*)drwav__metadata_get_memory(pParser, (size_t)stringSize + 1, 1);

    bytesRead = drwav__metadata_parser_read(pParser, pString, stringSize, NULL);
    pString[bytesRead] = '\0';

    stringLength = (drwav_uint32)bytesRead;
    if (bytesRead > 0 && pString[bytesRead - 1] == '\0') {
        stringLength -= 1;
    }

    *ppString = pString;
    *pStringLength = stringLength;

    return bytesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav__read_list_label_or_note_to_metadata_obj(drwav__metadata_parser* pParser, drwav_metadata* pMetadata, drwav_uint64 chunkSize, drwav_metadata_type type)
{
    drwav_uint8 cueIDBuffer[DRWAV_LIST_LABEL_OR_NOTE_BYTES];
    drwav_uint64 totalBytesRead = 0;
    size_t bytesJustRead;

    if (pMetadata == NULL) {
        return 0;
    }

    bytesJustRead = drwav__metadata_parser_read(pParser, cueIDBuffer, sizeof(cueIDBuffer), &totalBytesRead);

    DRWAV_ASSERT(pParser->stage == drwav__metadata_parser_stage_read);

    if (bytesJustRead == sizeof(cueIDBuffer)) {
        pMetadata->type = type;
        pMetadata->data.labelOrNote.cuePointId = drwav_bytes_to_u32(cueIDBuffer);

        totalBytesRead += drwav__metadata_read_string(pParser, ((drwav_uint32)chunkSize - DRWAV_LIST_LABEL_OR_NOTE_BYTES), &pMetadata->data.labelOrNote.pString, &pMetadata->data.labelOrNote.stringLength);
    }

    return totalBytesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav__read_list_labelled_cue_region_to_metadata_obj(drwav__metadata_parser* pParser, drwav_metadata* pMetadata, drwav_uint64 chunkSize)
{
    drwav_uint8 buffer[DRWAV_LIST_LABELLED_TEXT_BYTES];
    drwav_uint64 totalBytesRead = 0;
    size_t bytesJustRead;

    if (pMetadata == NULL) {
        return 0;
    }

    bytesJustRead = drwav__metadata_parser_read(pParser, buffer, sizeof(buffer), &totalBytesRead);

    DRWAV_ASSERT(pParser->stage == drwav__metadata_parser_stage_read);

    if (bytesJustRead == sizeof(buffer)) {
        pMetadata->type                                = drwav_metadata_type_list_labelled_cue_region;
        pMetadata->data.labelledCueRegion.cuePointId   = drwav_bytes_to_u32(buffer + 0);
        pMetadata->data.labelledCueRegion.sampleLength = drwav_bytes_to_u32(buffer + 4);
        pMetadata->data.labelledCueRegion.purposeId[0] = buffer[8];
        pMetadata->data.labelledCueRegion.purposeId[1] = buffer[9];
        pMetadata->data.labelledCueRegion.purposeId[2] = buffer[10];
        pMetadata->data.labelledCueRegion.purposeId[3] = buffer[11];
        pMetadata->data.labelledCueRegion.country      = drwav_bytes_to_u16(buffer + 12);
        pMetadata->data.labelledCueRegion.language     = drwav_bytes_to_u16(buffer + 14);
        pMetadata->data.labelledCueRegion.dialect      = drwav_bytes_to_u16(buffer + 16);
        pMetadata->data.labelledCueRegion.codePage     = drwav_bytes_to_u16(buffer + 18);

        totalBytesRead += drwav__metadata_read_string(pParser, ((drwav_uint32)chunkSize - DRWAV_LIST_LABELLED_TEXT_BYTES), &pMetadata->data.labelledCueRegion.pString, &pMetadata->data.labelledCueRegion.stringLength);
    }

    return totalBytesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav__metadata_process_info_text_chunk(drwav__metadata_parser* pParser, drwav_uint64 chunkSize, drwav_metadata_type type)
{
    drwav_uint64 bytesRead = 0;

    if (pParser->stage == drwav__metadata_parser_stage_count) {
        pParser->metadataCount += 1;
        drwav__metadata_request_extra_memory_for_stage_2(pParser, chunkSize + 1, 1);
    } else {
        drwav_metadata* pMetadata;

        pMetadata = drwav__metadata_get_next_object(pParser);
        if (pMetadata == NULL) {
            return 0;
        }

        pMetadata->type = type;

        bytesRead = drwav__metadata_read_string(pParser, (drwav_uint32)chunkSize, &pMetadata->data.infoText.pString, &pMetadata->data.infoText.stringLength);
        if (bytesRead == chunkSize) {
            pParser->metadataCursor += 1;
        }
    }

    return bytesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav__metadata_process_unknown_chunk(drwav__metadata_parser* pParser, const drwav_uint8* pChunkId, drwav_uint64 chunkSize, drwav_metadata_location location)
{
    drwav_uint64 bytesRead = 0;

    if (location == drwav_metadata_location_invalid) {
        return 0;
    }

    if (drwav_fourcc_equal(pChunkId, "data") || drwav_fourcc_equal(pChunkId, "fmt ") || drwav_fourcc_equal(pChunkId, "fact")) {
        return 0;
    }

    if (pParser->stage == drwav__metadata_parser_stage_count) {
        pParser->metadataCount += 1;
        drwav__metadata_request_extra_memory_for_stage_2(pParser, (size_t)chunkSize, 1);
    } else {
        drwav_metadata* pMetadata;

        pMetadata = drwav__metadata_get_next_object(pParser);
        if (pMetadata == NULL) {
            return 0;
        }

        pMetadata->type                         = drwav_metadata_type_unknown;
        pMetadata->data.unknown.chunkLocation   = location;
        pMetadata->data.unknown.id[0]           = pChunkId[0];
        pMetadata->data.unknown.id[1]           = pChunkId[1];
        pMetadata->data.unknown.id[2]           = pChunkId[2];
        pMetadata->data.unknown.id[3]           = pChunkId[3];
        pMetadata->data.unknown.dataSizeInBytes = (drwav_uint32)chunkSize;
        pMetadata->data.unknown.pData           = (drwav_uint8 *)drwav__metadata_get_memory(pParser, (size_t)chunkSize, 1);
        DRWAV_ASSERT(pMetadata->data.unknown.pData != NULL);

        bytesRead = drwav__metadata_parser_read(pParser, pMetadata->data.unknown.pData, pMetadata->data.unknown.dataSizeInBytes, NULL);
        if (bytesRead == pMetadata->data.unknown.dataSizeInBytes) {
            pParser->metadataCursor += 1;
        } else {
            
        }
    }

    return bytesRead;
}

DRWAV_PRIVATE drwav_bool32 drwav__chunk_matches(drwav_metadata_type allowedMetadataTypes, const drwav_uint8* pChunkID, drwav_metadata_type type, const char* pID)
{
    return (allowedMetadataTypes & type) && drwav_fourcc_equal(pChunkID, pID);
}

DRWAV_PRIVATE drwav_uint64 drwav__metadata_process_chunk(drwav__metadata_parser* pParser, const drwav_chunk_header* pChunkHeader, drwav_metadata_type allowedMetadataTypes)
{
    const drwav_uint8 *pChunkID = pChunkHeader->id.fourcc;
    drwav_uint64 bytesRead = 0;

    if (drwav__chunk_matches(allowedMetadataTypes, pChunkID, drwav_metadata_type_smpl, "smpl")) {
        if (pChunkHeader->sizeInBytes >= DRWAV_SMPL_BYTES) {
            if (pParser->stage == drwav__metadata_parser_stage_count) {
                drwav_uint8 buffer[4];
                size_t bytesJustRead;

                if (!pParser->onSeek(pParser->pReadSeekUserData, 28, DRWAV_SEEK_CUR)) {
                    return bytesRead;
                }
                bytesRead += 28;

                bytesJustRead = drwav__metadata_parser_read(pParser, buffer, sizeof(buffer), &bytesRead);
                if (bytesJustRead == sizeof(buffer)) {
                    drwav_uint32 loopCount = drwav_bytes_to_u32(buffer);

                    bytesJustRead = drwav__metadata_parser_read(pParser, buffer, sizeof(buffer), &bytesRead);
                    if (bytesJustRead == sizeof(buffer)) {
                        drwav_uint32 samplerSpecificDataSizeInBytes = drwav_bytes_to_u32(buffer);

                        if (drwav__metadata_validate_smpl_chunk(pChunkHeader, loopCount, samplerSpecificDataSizeInBytes, NULL)) {
                            pParser->metadataCount += 1;
                            drwav__metadata_request_extra_memory_for_stage_2(pParser, sizeof(drwav_smpl_loop) * loopCount, DRWAV_METADATA_ALIGNMENT);
                            drwav__metadata_request_extra_memory_for_stage_2(pParser, samplerSpecificDataSizeInBytes, 1);
                        } else {
                            
                        }
                    }
                }
            } else {
                drwav_result result;

                bytesRead = drwav__read_smpl_to_metadata_obj(pParser, pChunkHeader, drwav__metadata_get_next_object(pParser), &result);
                if (result == DRWAV_SUCCESS && bytesRead == pChunkHeader->sizeInBytes) {
                    pParser->metadataCursor += 1;
                } else {
                    
                }
            }
        } else {
            
        }
    } else if (drwav__chunk_matches(allowedMetadataTypes, pChunkID, drwav_metadata_type_inst, "inst")) {
        if (pChunkHeader->sizeInBytes == DRWAV_INST_BYTES) {
            if (pParser->stage == drwav__metadata_parser_stage_count) {
                pParser->metadataCount += 1;
            } else {
                bytesRead = drwav__read_inst_to_metadata_obj(pParser, drwav__metadata_get_next_object(pParser));
                if (bytesRead == pChunkHeader->sizeInBytes) {
                    pParser->metadataCursor += 1;
                } else {
                    
                }
            }
        } else {
            
        }
    } else if (drwav__chunk_matches(allowedMetadataTypes, pChunkID, drwav_metadata_type_acid, "acid")) {
        if (pChunkHeader->sizeInBytes == DRWAV_ACID_BYTES) {
            if (pParser->stage == drwav__metadata_parser_stage_count) {
                pParser->metadataCount += 1;
            } else {
                bytesRead = drwav__read_acid_to_metadata_obj(pParser, drwav__metadata_get_next_object(pParser));
                if (bytesRead == pChunkHeader->sizeInBytes) {
                    pParser->metadataCursor += 1;
                } else {
                    
                }
            }
        } else {
            
        }
    } else if (drwav__chunk_matches(allowedMetadataTypes, pChunkID, drwav_metadata_type_cue, "cue ")) {
        if (pChunkHeader->sizeInBytes >= DRWAV_CUE_BYTES) {
            if (pParser->stage == drwav__metadata_parser_stage_count) {
                size_t cueCount;

                pParser->metadataCount += 1;
                cueCount = (size_t)(pChunkHeader->sizeInBytes - DRWAV_CUE_BYTES) / DRWAV_CUE_POINT_BYTES;
                drwav__metadata_request_extra_memory_for_stage_2(pParser, sizeof(drwav_cue_point) * cueCount, DRWAV_METADATA_ALIGNMENT);
            } else {
                drwav_result result;

                bytesRead = drwav__read_cue_to_metadata_obj(pParser, pChunkHeader, drwav__metadata_get_next_object(pParser), &result);
                if (result == DRWAV_SUCCESS && bytesRead == pChunkHeader->sizeInBytes) {
                    pParser->metadataCursor += 1;
                } else {
                    
                }
            }
        } else {
            
        }
    } else if (drwav__chunk_matches(allowedMetadataTypes, pChunkID, drwav_metadata_type_bext, "bext")) {
        if (pChunkHeader->sizeInBytes >= DRWAV_BEXT_BYTES) {
            if (pParser->stage == drwav__metadata_parser_stage_count) {
                
                char buffer[DRWAV_BEXT_DESCRIPTION_BYTES + 1];
                size_t allocSizeNeeded = DRWAV_BEXT_UMID_BYTES; 
                size_t bytesJustRead;

                buffer[DRWAV_BEXT_DESCRIPTION_BYTES] = '\0';
                bytesJustRead = drwav__metadata_parser_read(pParser, buffer, DRWAV_BEXT_DESCRIPTION_BYTES, &bytesRead);
                if (bytesJustRead != DRWAV_BEXT_DESCRIPTION_BYTES) {
                    return bytesRead;
                }
                allocSizeNeeded += drwav__strlen(buffer) + 1;

                buffer[DRWAV_BEXT_ORIGINATOR_NAME_BYTES] = '\0';
                bytesJustRead = drwav__metadata_parser_read(pParser, buffer, DRWAV_BEXT_ORIGINATOR_NAME_BYTES, &bytesRead);
                if (bytesJustRead != DRWAV_BEXT_ORIGINATOR_NAME_BYTES) {
                    return bytesRead;
                }
                allocSizeNeeded += drwav__strlen(buffer) + 1;

                buffer[DRWAV_BEXT_ORIGINATOR_REF_BYTES] = '\0';
                bytesJustRead = drwav__metadata_parser_read(pParser, buffer, DRWAV_BEXT_ORIGINATOR_REF_BYTES, &bytesRead);
                if (bytesJustRead != DRWAV_BEXT_ORIGINATOR_REF_BYTES) {
                    return bytesRead;
                }
                allocSizeNeeded += drwav__strlen(buffer) + 1;

                
                allocSizeNeeded += (size_t)pChunkHeader->sizeInBytes - DRWAV_BEXT_BYTES + 1;

                drwav__metadata_request_extra_memory_for_stage_2(pParser, allocSizeNeeded, 1);

                pParser->metadataCount += 1;
            } else {
                bytesRead = drwav__read_bext_to_metadata_obj(pParser, drwav__metadata_get_next_object(pParser), pChunkHeader->sizeInBytes);
                if (bytesRead == pChunkHeader->sizeInBytes) {
                    pParser->metadataCursor += 1;
                } else {
                    
                }
            }
        } else {
            
        }
    } else if (drwav_fourcc_equal(pChunkID, "LIST") || drwav_fourcc_equal(pChunkID, "list")) {
        drwav_metadata_location listType = drwav_metadata_location_invalid;
        while (bytesRead < pChunkHeader->sizeInBytes) {
            drwav_uint8 subchunkId[4];
            drwav_uint8 subchunkSizeBuffer[4];
            drwav_uint64 subchunkDataSize;
            drwav_uint64 subchunkBytesRead = 0;
            drwav_uint64 bytesJustRead;

            if (pChunkHeader->sizeInBytes - bytesRead < sizeof(subchunkId)) {
                break;
            }

            bytesJustRead = drwav__metadata_parser_read(pParser, subchunkId, sizeof(subchunkId), &bytesRead);
            if (bytesJustRead != sizeof(subchunkId)) {
                break;
            }

            










            if (drwav_fourcc_equal(subchunkId, "adtl")) {
                listType = drwav_metadata_location_inside_adtl_list;
                continue;
            } else if (drwav_fourcc_equal(subchunkId, "INFO")) {
                listType = drwav_metadata_location_inside_info_list;
                continue;
            }

            if (pChunkHeader->sizeInBytes - bytesRead < sizeof(subchunkSizeBuffer)) {
                break;
            }

            bytesJustRead = drwav__metadata_parser_read(pParser, subchunkSizeBuffer, sizeof(subchunkSizeBuffer), &bytesRead);
            if (bytesJustRead != sizeof(subchunkSizeBuffer)) {
                break;
            }
            subchunkDataSize = drwav_bytes_to_u32(subchunkSizeBuffer);

            
            if (subchunkDataSize + (subchunkDataSize % 2) > pChunkHeader->sizeInBytes - bytesRead) {
                break;
            }

            if (drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_label, "labl") || drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_note, "note")) {
                if (subchunkDataSize >= DRWAV_LIST_LABEL_OR_NOTE_BYTES) {
                    if (pParser->stage == drwav__metadata_parser_stage_count) {
                        pParser->metadataCount += 1;
                        drwav__metadata_request_extra_memory_for_stage_2(pParser, (subchunkDataSize - DRWAV_LIST_LABEL_OR_NOTE_BYTES) + 1, 1);
                    } else {
                        subchunkBytesRead = drwav__read_list_label_or_note_to_metadata_obj(pParser, drwav__metadata_get_next_object(pParser), subchunkDataSize, drwav_fourcc_equal(subchunkId, "labl") ? drwav_metadata_type_list_label : drwav_metadata_type_list_note);
                        if (subchunkBytesRead == subchunkDataSize) {
                            pParser->metadataCursor += 1;
                        } else {
                            
                        }
                    }
                } else {
                    
                }
            } else if (drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_labelled_cue_region, "ltxt")) {
                if (subchunkDataSize >= DRWAV_LIST_LABELLED_TEXT_BYTES) {
                    if (pParser->stage == drwav__metadata_parser_stage_count) {
                        pParser->metadataCount += 1;
                        drwav__metadata_request_extra_memory_for_stage_2(pParser, (subchunkDataSize - DRWAV_LIST_LABELLED_TEXT_BYTES) + 1, 1);
                    } else {
                        subchunkBytesRead = drwav__read_list_labelled_cue_region_to_metadata_obj(pParser, drwav__metadata_get_next_object(pParser), subchunkDataSize);
                        if (subchunkBytesRead == subchunkDataSize) {
                            pParser->metadataCursor += 1;
                        } else {
                            
                        }
                    }
                } else {
                    
                }
            } else if (drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_info_software, "ISFT")) {
                subchunkBytesRead = drwav__metadata_process_info_text_chunk(pParser, subchunkDataSize,  drwav_metadata_type_list_info_software);
            } else if (drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_info_copyright, "ICOP")) {
                subchunkBytesRead = drwav__metadata_process_info_text_chunk(pParser, subchunkDataSize,  drwav_metadata_type_list_info_copyright);
            } else if (drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_info_title, "INAM")) {
                subchunkBytesRead = drwav__metadata_process_info_text_chunk(pParser, subchunkDataSize,  drwav_metadata_type_list_info_title);
            } else if (drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_info_artist, "IART")) {
                subchunkBytesRead = drwav__metadata_process_info_text_chunk(pParser, subchunkDataSize,  drwav_metadata_type_list_info_artist);
            } else if (drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_info_comment, "ICMT")) {
                subchunkBytesRead = drwav__metadata_process_info_text_chunk(pParser, subchunkDataSize,  drwav_metadata_type_list_info_comment);
            } else if (drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_info_date, "ICRD")) {
                subchunkBytesRead = drwav__metadata_process_info_text_chunk(pParser, subchunkDataSize,  drwav_metadata_type_list_info_date);
            } else if (drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_info_genre, "IGNR")) {
                subchunkBytesRead = drwav__metadata_process_info_text_chunk(pParser, subchunkDataSize,  drwav_metadata_type_list_info_genre);
            } else if (drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_info_album, "IPRD")) {
                subchunkBytesRead = drwav__metadata_process_info_text_chunk(pParser, subchunkDataSize,  drwav_metadata_type_list_info_album);
            } else if (drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_info_tracknumber, "ITRK")) {
                subchunkBytesRead = drwav__metadata_process_info_text_chunk(pParser, subchunkDataSize,  drwav_metadata_type_list_info_tracknumber);
            } else if (drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_info_location, "IARL")) {
                subchunkBytesRead = drwav__metadata_process_info_text_chunk(pParser, subchunkDataSize,  drwav_metadata_type_list_info_location);
            } else if (drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_info_organization, "ICMS")) {
                subchunkBytesRead = drwav__metadata_process_info_text_chunk(pParser, subchunkDataSize,  drwav_metadata_type_list_info_organization);
            } else if (drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_info_keywords, "IKEY")) {
                subchunkBytesRead = drwav__metadata_process_info_text_chunk(pParser, subchunkDataSize,  drwav_metadata_type_list_info_keywords);
            } else if (drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_info_medium, "IMED")) {
                subchunkBytesRead = drwav__metadata_process_info_text_chunk(pParser, subchunkDataSize,  drwav_metadata_type_list_info_medium);
            } else if (drwav__chunk_matches(allowedMetadataTypes, subchunkId, drwav_metadata_type_list_info_description, "ISBJ")) {
                subchunkBytesRead = drwav__metadata_process_info_text_chunk(pParser, subchunkDataSize,  drwav_metadata_type_list_info_description);
            } else if ((allowedMetadataTypes & drwav_metadata_type_unknown) != 0) {
                subchunkBytesRead = drwav__metadata_process_unknown_chunk(pParser, subchunkId, subchunkDataSize, listType);
            }

            bytesRead += subchunkBytesRead;
            DRWAV_ASSERT(subchunkBytesRead <= subchunkDataSize);

            if (subchunkBytesRead < subchunkDataSize) {
                drwav_uint64 bytesToSeek = subchunkDataSize - subchunkBytesRead;

                if (!pParser->onSeek(pParser->pReadSeekUserData, (int)bytesToSeek, DRWAV_SEEK_CUR)) {
                    break;
                }
                bytesRead += bytesToSeek;
            }

            if ((subchunkDataSize % 2) == 1) {
                if (!pParser->onSeek(pParser->pReadSeekUserData, 1, DRWAV_SEEK_CUR)) {
                    break;
                }
                bytesRead += 1;
            }
        }
    } else if ((allowedMetadataTypes & drwav_metadata_type_unknown) != 0) {
        bytesRead = drwav__metadata_process_unknown_chunk(pParser, pChunkID, pChunkHeader->sizeInBytes, drwav_metadata_location_top_level);
    }

    return bytesRead;
}


DRWAV_PRIVATE drwav_uint32 drwav_get_bytes_per_pcm_frame(drwav* pWav)
{
    drwav_uint32 bytesPerFrame;

    



    if ((pWav->bitsPerSample & 0x7) == 0) {
        
        bytesPerFrame = (pWav->bitsPerSample * pWav->fmt.channels) >> 3;
    } else {
        bytesPerFrame = pWav->fmt.blockAlign;
    }

    
    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ALAW || pWav->translatedFormatTag == DR_WAVE_FORMAT_MULAW) {
        if (bytesPerFrame != pWav->fmt.channels) {
            return 0;   
        }
    }

    return bytesPerFrame;
}

DRWAV_API drwav_uint16 drwav_fmt_get_format(const drwav_fmt* pFMT)
{
    if (pFMT == NULL) {
        return 0;
    }

    if (pFMT->formatTag != DR_WAVE_FORMAT_EXTENSIBLE) {
        return pFMT->formatTag;
    } else {
        return drwav_bytes_to_u16(pFMT->subFormat);    
    }
}

DRWAV_PRIVATE drwav_bool32 drwav_preinit(drwav* pWav, drwav_read_proc onRead, drwav_seek_proc onSeek, drwav_tell_proc onTell, void* pReadSeekTellUserData, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pWav == NULL || onRead == NULL || onSeek == NULL) { 
        return DRWAV_FALSE;
    }

    DRWAV_ZERO_MEMORY(pWav, sizeof(*pWav));
    pWav->onRead    = onRead;
    pWav->onSeek    = onSeek;
    pWav->onTell    = onTell;
    pWav->pUserData = pReadSeekTellUserData;
    pWav->allocationCallbacks = drwav_copy_allocation_callbacks_or_defaults(pAllocationCallbacks);

    if (pWav->allocationCallbacks.onFree == NULL || (pWav->allocationCallbacks.onMalloc == NULL && pWav->allocationCallbacks.onRealloc == NULL)) {
        return DRWAV_FALSE;    
    }

    return DRWAV_TRUE;
}

DRWAV_PRIVATE drwav_bool32 drwav_init__internal(drwav* pWav, drwav_chunk_proc onChunk, void* pChunkUserData, drwav_uint32 flags)
{
    
    drwav_result result;
    drwav_uint64 cursor;    
    drwav_bool32 sequential;
    drwav_uint8 riff[4];
    drwav_fmt fmt;
    unsigned short translatedFormatTag;
    drwav_uint64 dataChunkSize = 0;             
    drwav_uint64 sampleCountFromFactChunk = 0;  
    drwav_uint64 metadataStartPos;
    drwav__metadata_parser metadataParser;
    drwav_bool8 isProcessingMetadata = DRWAV_FALSE;
    drwav_bool8 foundChunk_fmt  = DRWAV_FALSE;
    drwav_bool8 foundChunk_data = DRWAV_FALSE;
    drwav_bool8 isAIFCFormType = DRWAV_FALSE;   
    drwav_uint64 aiffFrameCount = 0;
    drwav_int64 fileSize = 0;
    drwav_bool32 hasKnownFileSize = DRWAV_FALSE;
    drwav_bool32 isDataChunkSizeUnknown = DRWAV_FALSE;

    cursor = 0;
    sequential = (flags & DRWAV_SEQUENTIAL) != 0;

    




    isProcessingMetadata = !sequential && ((flags & DRWAV_WITH_METADATA) != 0);

    
    if (!sequential && pWav->onTell != NULL) {
        if (pWav->onSeek(pWav->pUserData, 0, DRWAV_SEEK_END) && pWav->onTell(pWav->pUserData, &fileSize) && fileSize >= 0) {
            hasKnownFileSize = DRWAV_TRUE;
        }

        if (!pWav->onSeek(pWav->pUserData, 0, DRWAV_SEEK_SET)) {
            return DRWAV_FALSE;
        }
    }

    DRWAV_ZERO_OBJECT(&fmt);

    
    if (drwav__on_read(pWav->onRead, pWav->pUserData, riff, sizeof(riff), &cursor) != sizeof(riff)) {
        return DRWAV_FALSE;
    }

    



    if (drwav_fourcc_equal(riff, "RIFF")) {
        pWav->container = drwav_container_riff;
    } else if (drwav_fourcc_equal(riff, "RIFX")) {
        pWav->container = drwav_container_rifx;
    } else if (drwav_fourcc_equal(riff, "riff")) {
        int i;
        drwav_uint8 riff2[12];

        pWav->container = drwav_container_w64;

        
        if (drwav__on_read(pWav->onRead, pWav->pUserData, riff2, sizeof(riff2), &cursor) != sizeof(riff2)) {
            return DRWAV_FALSE;
        }

        for (i = 0; i < 12; ++i) {
            if (riff2[i] != drwavGUID_W64_RIFF[i+4]) {
                return DRWAV_FALSE;
            }
        }
    } else if (drwav_fourcc_equal(riff, "RF64")) {
        pWav->container = drwav_container_rf64;
    } else if (drwav_fourcc_equal(riff, "FORM")) {
        pWav->container = drwav_container_aiff;
    } else {
        return DRWAV_FALSE;   
    }


    if (pWav->container == drwav_container_riff || pWav->container == drwav_container_rifx || pWav->container == drwav_container_rf64) {
        drwav_uint8 chunkSizeBytes[4];
        drwav_uint8 wave[4];

        if (drwav__on_read(pWav->onRead, pWav->pUserData, chunkSizeBytes, sizeof(chunkSizeBytes), &cursor) != sizeof(chunkSizeBytes)) {
            return DRWAV_FALSE;
        }

        if (pWav->container == drwav_container_riff || pWav->container == drwav_container_rifx) {
            if (drwav_bytes_to_u32_ex(chunkSizeBytes, pWav->container) < 36) {
                





                    
            }
        } else if (pWav->container == drwav_container_rf64) {
            if (drwav_bytes_to_u32_le(chunkSizeBytes) != 0xFFFFFFFF) {
                return DRWAV_FALSE;    
            }
        } else {
            return DRWAV_FALSE; 
        }

        if (drwav__on_read(pWav->onRead, pWav->pUserData, wave, sizeof(wave), &cursor) != sizeof(wave)) {
            return DRWAV_FALSE;
        }

        if (!drwav_fourcc_equal(wave, "WAVE")) {
            return DRWAV_FALSE;    
        }
    } else if (pWav->container == drwav_container_w64) {
        drwav_uint8 chunkSizeBytes[8];
        drwav_uint8 wave[16];

        if (drwav__on_read(pWav->onRead, pWav->pUserData, chunkSizeBytes, sizeof(chunkSizeBytes), &cursor) != sizeof(chunkSizeBytes)) {
            return DRWAV_FALSE;
        }

        if (drwav_bytes_to_u64(chunkSizeBytes) < 80) {
            return DRWAV_FALSE;
        }

        if (drwav__on_read(pWav->onRead, pWav->pUserData, wave, sizeof(wave), &cursor) != sizeof(wave)) {
            return DRWAV_FALSE;
        }

        if (!drwav_guid_equal(wave, drwavGUID_W64_WAVE)) {
            return DRWAV_FALSE;
        }
    } else if (pWav->container == drwav_container_aiff) {
        drwav_uint8 chunkSizeBytes[4];
        drwav_uint8 aiff[4];

        if (drwav__on_read(pWav->onRead, pWav->pUserData, chunkSizeBytes, sizeof(chunkSizeBytes), &cursor) != sizeof(chunkSizeBytes)) {
            return DRWAV_FALSE;
        }

        if (drwav_bytes_to_u32_be(chunkSizeBytes) < 18) {
            return DRWAV_FALSE;
        }

        if (drwav__on_read(pWav->onRead, pWav->pUserData, aiff, sizeof(aiff), &cursor) != sizeof(aiff)) {
            return DRWAV_FALSE;
        }

        if (drwav_fourcc_equal(aiff, "AIFF")) {
            isAIFCFormType = DRWAV_FALSE;
        } else if (drwav_fourcc_equal(aiff, "AIFC")) {
            isAIFCFormType = DRWAV_TRUE;
        } else {
            return DRWAV_FALSE; 
        }
    } else {
        return DRWAV_FALSE;
    }


    
    if (pWav->container == drwav_container_rf64) {
        drwav_uint8 sizeBytes[8];
        drwav_uint64 bytesRemainingInChunk;
        drwav_chunk_header header;
        result = drwav__read_chunk_header(pWav->onRead, pWav->pUserData, pWav->container, &cursor, &header);
        if (result != DRWAV_SUCCESS) {
            return DRWAV_FALSE;
        }

        if (!drwav_fourcc_equal(header.id.fourcc, "ds64")) {
            return DRWAV_FALSE; 
        }

        bytesRemainingInChunk = header.sizeInBytes + header.paddingSize;

        
        if (!drwav__seek_forward(pWav->onSeek, 8, pWav->pUserData)) {
            return DRWAV_FALSE;
        }
        bytesRemainingInChunk -= 8;
        cursor += 8;


        
        if (drwav__on_read(pWav->onRead, pWav->pUserData, sizeBytes, sizeof(sizeBytes), &cursor) != sizeof(sizeBytes)) {
            return DRWAV_FALSE;
        }
        bytesRemainingInChunk -= 8;
        dataChunkSize = drwav_bytes_to_u64(sizeBytes);


        
        if (drwav__on_read(pWav->onRead, pWav->pUserData, sizeBytes, sizeof(sizeBytes), &cursor) != sizeof(sizeBytes)) {
            return DRWAV_FALSE;
        }
        bytesRemainingInChunk -= 8;
        sampleCountFromFactChunk = drwav_bytes_to_u64(sizeBytes);


        
        if (!drwav__seek_forward(pWav->onSeek, bytesRemainingInChunk, pWav->pUserData)) {
            return DRWAV_FALSE;
        }
        cursor += bytesRemainingInChunk;
    }


    metadataStartPos = cursor;

    
    if (pWav->container != drwav_container_riff && pWav->container != drwav_container_rf64) {
        isProcessingMetadata = DRWAV_FALSE;
    }

    DRWAV_ZERO_MEMORY(&metadataParser, sizeof(metadataParser));
    if (isProcessingMetadata) {
        metadataParser.onRead = pWav->onRead;
        metadataParser.onSeek = pWav->onSeek;
        metadataParser.pReadSeekUserData = pWav->pUserData;
        metadataParser.stage  = drwav__metadata_parser_stage_count;
    }


    




    for (;;) {  
        drwav_chunk_header header;
        drwav_uint64 chunkSize;

        result = drwav__read_chunk_header(pWav->onRead, pWav->pUserData, pWav->container, &cursor, &header);
        if (result != DRWAV_SUCCESS) {
            break;
        }

        chunkSize = header.sizeInBytes;


        



        if (!sequential && onChunk != NULL) {
            drwav_uint64 callbackBytesRead = onChunk(pChunkUserData, pWav->onRead, pWav->onSeek, pWav->pUserData, &header, pWav->container, &fmt);

            



            if (callbackBytesRead > 0) {
                if (drwav__seek_from_start(pWav->onSeek, cursor, pWav->pUserData) == DRWAV_FALSE) {
                    return DRWAV_FALSE;
                }
            }
        }


        

        
        if (((pWav->container == drwav_container_riff || pWav->container == drwav_container_rifx || pWav->container == drwav_container_rf64) && drwav_fourcc_equal(header.id.fourcc, "fmt ")) ||
            ((pWav->container == drwav_container_w64) && drwav_guid_equal(header.id.guid, drwavGUID_W64_FMT))) {
            drwav_uint8 fmtData[16];

            if (header.sizeInBytes < sizeof(fmtData)) {
                return DRWAV_FALSE; 
            }

            foundChunk_fmt = DRWAV_TRUE;

            if (pWav->onRead(pWav->pUserData, fmtData, sizeof(fmtData)) != sizeof(fmtData)) {
                return DRWAV_FALSE;
            }
            cursor += sizeof(fmtData);

            fmt.formatTag      = drwav_bytes_to_u16_ex(fmtData + 0,  pWav->container);
            fmt.channels       = drwav_bytes_to_u16_ex(fmtData + 2,  pWav->container);
            fmt.sampleRate     = drwav_bytes_to_u32_ex(fmtData + 4,  pWav->container);
            fmt.avgBytesPerSec = drwav_bytes_to_u32_ex(fmtData + 8,  pWav->container);
            fmt.blockAlign     = drwav_bytes_to_u16_ex(fmtData + 12, pWav->container);
            fmt.bitsPerSample  = drwav_bytes_to_u16_ex(fmtData + 14, pWav->container);

            fmt.extendedSize       = 0;
            fmt.validBitsPerSample = 0;
            fmt.channelMask        = 0;
            DRWAV_ZERO_MEMORY(fmt.subFormat, sizeof(fmt.subFormat));

            if (header.sizeInBytes > 16) {
                drwav_uint8 fmt_cbSize[2];
                int bytesReadSoFar = 0;
                drwav_uint64 leftoverBytes;

                if (pWav->onRead(pWav->pUserData, fmt_cbSize, sizeof(fmt_cbSize)) != sizeof(fmt_cbSize)) {
                    return DRWAV_FALSE;    
                }
                cursor += sizeof(fmt_cbSize);

                bytesReadSoFar = 18;

                fmt.extendedSize = drwav_bytes_to_u16_ex(fmt_cbSize, pWav->container);
                if (fmt.extendedSize > 0) {
                    
                    if (fmt.formatTag == DR_WAVE_FORMAT_EXTENSIBLE) {
                        if (fmt.extendedSize != 22) {
                            return DRWAV_FALSE;
                        }
                    }

                    if (fmt.formatTag == DR_WAVE_FORMAT_EXTENSIBLE) {
                        drwav_uint8 fmtext[22];

                        if (pWav->onRead(pWav->pUserData, fmtext, fmt.extendedSize) != fmt.extendedSize) {
                            return DRWAV_FALSE;    
                        }

                        fmt.validBitsPerSample = drwav_bytes_to_u16_ex(fmtext + 0, pWav->container);
                        fmt.channelMask        = drwav_bytes_to_u32_ex(fmtext + 2, pWav->container);
                        drwav_bytes_to_guid(fmtext + 6, fmt.subFormat);
                    } else {
                        if (pWav->onSeek(pWav->pUserData, fmt.extendedSize, DRWAV_SEEK_CUR) == DRWAV_FALSE) {
                            return DRWAV_FALSE;
                        }
                    }
                    cursor += fmt.extendedSize;

                    bytesReadSoFar += fmt.extendedSize;
                }

                
                leftoverBytes = header.sizeInBytes - bytesReadSoFar;
                if (leftoverBytes > 0x7FFFFFFF) {
                    return DRWAV_FALSE;
                }

                if (pWav->onSeek(pWav->pUserData, (int)leftoverBytes, DRWAV_SEEK_CUR) == DRWAV_FALSE) {
                    return DRWAV_FALSE;
                }

                cursor += leftoverBytes;
            }

            if (header.paddingSize > 0) {
                if (drwav__seek_forward(pWav->onSeek, header.paddingSize, pWav->pUserData) == DRWAV_FALSE) {
                    break;
                }
                cursor += header.paddingSize;
            }

            
            continue;
        }

        
        if (((pWav->container == drwav_container_riff || pWav->container == drwav_container_rifx || pWav->container == drwav_container_rf64) && drwav_fourcc_equal(header.id.fourcc, "data")) ||
            ((pWav->container == drwav_container_w64) && drwav_guid_equal(header.id.guid, drwavGUID_W64_DATA))) {
            foundChunk_data = DRWAV_TRUE;

            pWav->dataChunkDataPos  = cursor;

            if (pWav->container != drwav_container_rf64) {  
                dataChunkSize = chunkSize;
            }

            
            isDataChunkSizeUnknown = (pWav->container == drwav_container_riff || pWav->container == drwav_container_rifx) && dataChunkSize == 0xFFFFFFFF;
            if (sequential || !isProcessingMetadata || isDataChunkSizeUnknown) {
                break;
            } else {
                chunkSize += header.paddingSize;    
                if (drwav__seek_forward(pWav->onSeek, chunkSize, pWav->pUserData) == DRWAV_FALSE) {
                    break;
                }
                cursor += chunkSize;

                continue;   
            }
        }

        
        if (((pWav->container == drwav_container_riff || pWav->container == drwav_container_rifx || pWav->container == drwav_container_rf64) && drwav_fourcc_equal(header.id.fourcc, "fact")) ||
            ((pWav->container == drwav_container_w64) && drwav_guid_equal(header.id.guid, drwavGUID_W64_FACT))) {
            if (pWav->container == drwav_container_riff || pWav->container == drwav_container_rifx) {
                drwav_uint8 sampleCount[4];

                if (chunkSize < 4) {
                    return DRWAV_FALSE;
                }

                if (drwav__on_read(pWav->onRead, pWav->pUserData, &sampleCount, 4, &cursor) != 4) {
                    return DRWAV_FALSE;
                }

                chunkSize -= 4;

                sampleCountFromFactChunk = drwav_bytes_to_u32_ex(sampleCount, pWav->container);
            } else if (pWav->container == drwav_container_w64) {
                drwav_uint8 sampleCount[8];

                if (chunkSize < sizeof(sampleCount)) {
                    return DRWAV_FALSE;
                }

                if (drwav__on_read(pWav->onRead, pWav->pUserData, sampleCount, sizeof(sampleCount), &cursor) != sizeof(sampleCount)) {
                    return DRWAV_FALSE;
                }

                chunkSize -= sizeof(sampleCount);
                sampleCountFromFactChunk = drwav_bytes_to_u64(sampleCount);
            } else if (pWav->container == drwav_container_rf64) {
                
            }

            
            chunkSize += header.paddingSize;    
            if (drwav__seek_forward(pWav->onSeek, chunkSize, pWav->pUserData) == DRWAV_FALSE) {
                break;
            }
            cursor += chunkSize;

            continue;
        }


        
        if (pWav->container == drwav_container_aiff && drwav_fourcc_equal(header.id.fourcc, "COMM")) {
            drwav_uint8 commData[24];
            drwav_uint32 commDataBytesToRead;
            drwav_uint16 channels;
            drwav_uint32 frameCount;
            drwav_uint16 sampleSizeInBits;
            drwav_int64  sampleRate;
            drwav_uint16 compressionFormat;

            foundChunk_fmt = DRWAV_TRUE;

            if (isAIFCFormType) {
                commDataBytesToRead = 24;
                if (header.sizeInBytes < commDataBytesToRead) {
                    return DRWAV_FALSE; 
                }
            } else {
                commDataBytesToRead = 18;
                if (header.sizeInBytes != commDataBytesToRead) {
                    return DRWAV_FALSE; 
                }
            }

            if (drwav__on_read(pWav->onRead, pWav->pUserData, commData, commDataBytesToRead, &cursor) != commDataBytesToRead) {
                return DRWAV_FALSE;
            }


            channels         = drwav_bytes_to_u16_ex     (commData + 0, pWav->container);
            frameCount       = drwav_bytes_to_u32_ex     (commData + 2, pWav->container);
            sampleSizeInBits = drwav_bytes_to_u16_ex     (commData + 6, pWav->container);
            sampleRate       = drwav_aiff_extended_to_s64(commData + 8);

            if (sampleRate < 0 || sampleRate > 0xFFFFFFFF) {
                return DRWAV_FALSE; 
            }

            if (isAIFCFormType) {
                const drwav_uint8* type = commData + 18;

                if (drwav_fourcc_equal(type, "NONE")) {
                    compressionFormat = DR_WAVE_FORMAT_PCM; 
                } else if (drwav_fourcc_equal(type, "raw ")) {
                    compressionFormat = DR_WAVE_FORMAT_PCM;

                    
                    if (sampleSizeInBits == 8) {
                        pWav->aiff.isUnsigned = DRWAV_TRUE;
                    }
                } else if (drwav_fourcc_equal(type, "sowt")) {
                    compressionFormat = DR_WAVE_FORMAT_PCM; 
                    pWav->aiff.isLE = DRWAV_TRUE;
                } else if (drwav_fourcc_equal(type, "fl32") || drwav_fourcc_equal(type, "fl64") || drwav_fourcc_equal(type, "FL32") || drwav_fourcc_equal(type, "FL64")) {
                    compressionFormat = DR_WAVE_FORMAT_IEEE_FLOAT;
                } else if (drwav_fourcc_equal(type, "alaw") || drwav_fourcc_equal(type, "ALAW")) {
                    compressionFormat = DR_WAVE_FORMAT_ALAW;
                } else if (drwav_fourcc_equal(type, "ulaw") || drwav_fourcc_equal(type, "ULAW")) {
                    compressionFormat = DR_WAVE_FORMAT_MULAW;
                } else if (drwav_fourcc_equal(type, "ima4")) {
                    compressionFormat = DR_WAVE_FORMAT_DVI_ADPCM;
                    sampleSizeInBits  = 4;

                    



                    (void)compressionFormat;
                    (void)sampleSizeInBits;

                    return DRWAV_FALSE;
                } else {
                    return DRWAV_FALSE; 
                }
            } else {
                compressionFormat = DR_WAVE_FORMAT_PCM; 
            }

            
            aiffFrameCount = frameCount;

            
            fmt.formatTag      = compressionFormat;
            fmt.channels       = channels;
            fmt.sampleRate     = (drwav_uint32)sampleRate;
            fmt.bitsPerSample  = (sampleSizeInBits + 7) & ~7;   
            fmt.blockAlign     = (drwav_uint16)((drwav_uint32)fmt.channels * fmt.bitsPerSample / 8);
            fmt.avgBytesPerSec = fmt.blockAlign * fmt.sampleRate;

            



            if (compressionFormat == DR_WAVE_FORMAT_ALAW || compressionFormat == DR_WAVE_FORMAT_MULAW) {
                if (fmt.bitsPerSample > 8) {
                    fmt.bitsPerSample = 8;
                    fmt.blockAlign = fmt.channels;
                }
            }

            
            if (isAIFCFormType) {
                if (drwav__seek_forward(pWav->onSeek, (chunkSize - commDataBytesToRead), pWav->pUserData) == DRWAV_FALSE) {
                    return DRWAV_FALSE;
                }
                cursor += (chunkSize - commDataBytesToRead);
            }

            
            continue;
        }


        
        if (pWav->container == drwav_container_aiff && drwav_fourcc_equal(header.id.fourcc, "SSND")) {
            drwav_uint8 offsetAndBlockSizeData[8];
            drwav_uint32 offset;

            foundChunk_data = DRWAV_TRUE;

            if (drwav__on_read(pWav->onRead, pWav->pUserData, offsetAndBlockSizeData, sizeof(offsetAndBlockSizeData), &cursor) != sizeof(offsetAndBlockSizeData)) {
                return DRWAV_FALSE;
            }

            
            offset = drwav_bytes_to_u32_ex(offsetAndBlockSizeData + 0, pWav->container);
            pWav->dataChunkDataPos = cursor + offset;

            
            dataChunkSize = chunkSize;
            if (dataChunkSize  > offset) {
                dataChunkSize -= offset;
            } else {
                dataChunkSize = 0;
            }

            if (sequential) {
                if (foundChunk_fmt) {   
                    







                    if (drwav__seek_forward(pWav->onSeek, offset, pWav->pUserData) == DRWAV_FALSE) {
                        return DRWAV_FALSE;
                    }
                    cursor += offset;

                    break;
                } else {
                    




                    return DRWAV_FALSE;
                }
            } else {
                chunkSize += header.paddingSize;                
                chunkSize -= sizeof(offsetAndBlockSizeData);    

                if (drwav__seek_forward(pWav->onSeek, chunkSize, pWav->pUserData) == DRWAV_FALSE) {
                    break;
                }
                cursor += chunkSize;

                continue;   
            }
        }


        
        if (isProcessingMetadata) {
            if (hasKnownFileSize && header.sizeInBytes > (drwav_uint64)fileSize) {
                return DRWAV_FALSE;
            }

            drwav__metadata_process_chunk(&metadataParser, &header, drwav_metadata_type_all_including_unknown);

            
            if (drwav__seek_from_start(pWav->onSeek, cursor, pWav->pUserData) == DRWAV_FALSE) {
                break;  
            }
        }


        
        chunkSize += header.paddingSize;    
        if (drwav__seek_forward(pWav->onSeek, chunkSize, pWav->pUserData) == DRWAV_FALSE) {
            break;
        }
        cursor += chunkSize;
    }

    
    if (!foundChunk_fmt || !foundChunk_data) {
        return DRWAV_FALSE;
    }

    
    if ((fmt.sampleRate    == 0 || fmt.sampleRate    > DRWAV_MAX_SAMPLE_RATE    ) ||
        (fmt.channels      == 0 || fmt.channels      > DRWAV_MAX_CHANNELS       ) ||
        (fmt.bitsPerSample == 0 || fmt.bitsPerSample > DRWAV_MAX_BITS_PER_SAMPLE) ||
        fmt.blockAlign == 0) {
        return DRWAV_FALSE; 
    }

    
    translatedFormatTag = fmt.formatTag;
    if (translatedFormatTag == DR_WAVE_FORMAT_EXTENSIBLE) {
        translatedFormatTag = drwav_bytes_to_u16_ex(fmt.subFormat + 0, pWav->container);
    }

    
    if ((pWav->container == drwav_container_riff || pWav->container == drwav_container_rifx) && translatedFormatTag != DR_WAVE_FORMAT_ADPCM) {
        sampleCountFromFactChunk = 0;
    }

    if (hasKnownFileSize) {
        drwav_uint64 bytesAvailable;
        
        if (pWav->dataChunkDataPos > (drwav_uint64)fileSize) {
            return DRWAV_FALSE;
        }

        bytesAvailable = (drwav_uint64)fileSize - pWav->dataChunkDataPos;
        if (isDataChunkSizeUnknown || dataChunkSize > bytesAvailable) {
            dataChunkSize = bytesAvailable;
        }
    } else if (isDataChunkSizeUnknown && !sequential) {
        
        dataChunkSize = 0;
        for (;;) {
            drwav_uint8 temp[4096];
            size_t bytesRead;

            bytesRead = pWav->onRead(pWav->pUserData, temp, sizeof(temp));
            dataChunkSize += bytesRead;
            
            if (bytesRead < sizeof(temp)) {
                break;
            }
        }
    }


    






    if (isProcessingMetadata && metadataParser.metadataCount > 0) {
        if (drwav__seek_from_start(pWav->onSeek, metadataStartPos, pWav->pUserData) == DRWAV_FALSE) {
            return DRWAV_FALSE;
        }

        cursor = metadataStartPos;

        result = drwav__metadata_alloc(&metadataParser, &pWav->allocationCallbacks);
        if (result != DRWAV_SUCCESS) {
            return DRWAV_FALSE;
        }

        metadataParser.stage = drwav__metadata_parser_stage_read;

        for (;;) {
            drwav_chunk_header header;
            drwav_uint64 metadataBytesRead;

            result = drwav__read_chunk_header(pWav->onRead, pWav->pUserData, pWav->container, &cursor, &header);
            if (result != DRWAV_SUCCESS) {
                break;
            }

            metadataBytesRead = drwav__metadata_process_chunk(&metadataParser, &header, drwav_metadata_type_all_including_unknown);

            if (metadataParser.metadataCursor == metadataParser.metadataCount) {
                break;
            }

            
            if (drwav__seek_forward(pWav->onSeek, (header.sizeInBytes + header.paddingSize) - metadataBytesRead, pWav->pUserData) == DRWAV_FALSE) {
                drwav_free(metadataParser.pMetadata, &pWav->allocationCallbacks);
                return DRWAV_FALSE;
            }
        }

        
        pWav->pMetadata     = metadataParser.pMetadata;
        pWav->metadataCount = (drwav_uint32)metadataParser.metadataCursor;
    }

    if (!sequential && !drwav__seek_from_start(pWav->onSeek, pWav->dataChunkDataPos, pWav->pUserData)) {
        drwav_free(pWav->pMetadata, &pWav->allocationCallbacks);
        return DRWAV_FALSE;
    }


    pWav->fmt                 = fmt;
    pWav->sampleRate          = fmt.sampleRate;
    pWav->channels            = fmt.channels;
    pWav->bitsPerSample       = fmt.bitsPerSample;
    pWav->translatedFormatTag = translatedFormatTag;

    









    if (!drwav__is_compressed_format_tag(translatedFormatTag)) {
        drwav_uint32 bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
        if (bytesPerFrame > 0) {
            dataChunkSize -= (dataChunkSize % bytesPerFrame);
        }
    }

    pWav->bytesRemaining      = dataChunkSize;
    pWav->dataChunkDataSize   = dataChunkSize;

    if (sampleCountFromFactChunk != 0) {
        pWav->totalPCMFrameCount = sampleCountFromFactChunk;
    } else if (aiffFrameCount != 0) {
        pWav->totalPCMFrameCount = aiffFrameCount;
    } else {
        drwav_uint32 bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
        if (bytesPerFrame == 0) {
            drwav_free(pWav->pMetadata, &pWav->allocationCallbacks);
            return DRWAV_FALSE; 
        }

        pWav->totalPCMFrameCount = dataChunkSize / bytesPerFrame;

        if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ADPCM) {
            drwav_uint64 totalBlockHeaderSizeInBytes;
            drwav_uint64 blockCount = dataChunkSize / fmt.blockAlign;

            
            if ((blockCount * fmt.blockAlign) < dataChunkSize) {
                blockCount += 1;
            }

            
            totalBlockHeaderSizeInBytes = blockCount * (6*fmt.channels);
            if (totalBlockHeaderSizeInBytes >= dataChunkSize) {  
                drwav_free(pWav->pMetadata, &pWav->allocationCallbacks);
                return DRWAV_FALSE; 
            }

            pWav->totalPCMFrameCount = ((dataChunkSize - totalBlockHeaderSizeInBytes) * 2) / fmt.channels;
        }
        if (pWav->translatedFormatTag == DR_WAVE_FORMAT_DVI_ADPCM) {
            drwav_uint64 totalBlockHeaderSizeInBytes;
            drwav_uint64 blockCount = dataChunkSize / fmt.blockAlign;

            
            if ((blockCount * fmt.blockAlign) < dataChunkSize) {
                blockCount += 1;
            }

            
            totalBlockHeaderSizeInBytes = blockCount * (4*fmt.channels);
            if (totalBlockHeaderSizeInBytes >= dataChunkSize) {  
                drwav_free(pWav->pMetadata, &pWav->allocationCallbacks);
                return DRWAV_FALSE; 
            }

            pWav->totalPCMFrameCount = ((dataChunkSize - totalBlockHeaderSizeInBytes) * 2) / fmt.channels;

            
            pWav->totalPCMFrameCount += blockCount;
        }
    }

    
    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ADPCM || pWav->translatedFormatTag == DR_WAVE_FORMAT_DVI_ADPCM) {
        if (pWav->channels > 2) {
            drwav_free(pWav->pMetadata, &pWav->allocationCallbacks);
            return DRWAV_FALSE;
        }
    }

    
    if (drwav_get_bytes_per_pcm_frame(pWav) == 0) {
        drwav_free(pWav->pMetadata, &pWav->allocationCallbacks);
        return DRWAV_FALSE;
    }

    return DRWAV_TRUE;
}

DRWAV_API drwav_bool32 drwav_init(drwav* pWav, drwav_read_proc onRead, drwav_seek_proc onSeek, drwav_tell_proc onTell, void* pUserData, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_ex(pWav, onRead, onSeek, onTell, NULL, pUserData, NULL, 0, pAllocationCallbacks);
}

DRWAV_API drwav_bool32 drwav_init_ex(drwav* pWav, drwav_read_proc onRead, drwav_seek_proc onSeek, drwav_tell_proc onTell, drwav_chunk_proc onChunk, void* pReadSeekTellUserData, void* pChunkUserData, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (!drwav_preinit(pWav, onRead, onSeek, onTell, pReadSeekTellUserData, pAllocationCallbacks)) {
        return DRWAV_FALSE;
    }

    return drwav_init__internal(pWav, onChunk, pChunkUserData, flags);
}

DRWAV_API drwav_bool32 drwav_init_with_metadata(drwav* pWav, drwav_read_proc onRead, drwav_seek_proc onSeek, drwav_tell_proc onTell, void* pUserData, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (!drwav_preinit(pWav, onRead, onSeek, onTell, pUserData, pAllocationCallbacks)) {
        return DRWAV_FALSE;
    }

    return drwav_init__internal(pWav, NULL, NULL, flags | DRWAV_WITH_METADATA);
}

DRWAV_API drwav_metadata* drwav_take_ownership_of_metadata(drwav* pWav)
{
    drwav_metadata *result = pWav->pMetadata;

    pWav->pMetadata     = NULL;
    pWav->metadataCount = 0;

    return result;
}


DRWAV_PRIVATE size_t drwav__write(drwav* pWav, const void* pData, size_t dataSize)
{
    DRWAV_ASSERT(pWav          != NULL);
    DRWAV_ASSERT(pWav->onWrite != NULL);

    
    return pWav->onWrite(pWav->pUserData, pData, dataSize);
}

DRWAV_PRIVATE size_t drwav__write_byte(drwav* pWav, drwav_uint8 byte)
{
    DRWAV_ASSERT(pWav          != NULL);
    DRWAV_ASSERT(pWav->onWrite != NULL);

    return pWav->onWrite(pWav->pUserData, &byte, 1);
}

DRWAV_PRIVATE size_t drwav__write_u16ne_to_le(drwav* pWav, drwav_uint16 value)
{
    DRWAV_ASSERT(pWav          != NULL);
    DRWAV_ASSERT(pWav->onWrite != NULL);

    if (!drwav__is_little_endian()) {
        value = drwav__bswap16(value);
    }

    return drwav__write(pWav, &value, 2);
}

DRWAV_PRIVATE size_t drwav__write_u32ne_to_le(drwav* pWav, drwav_uint32 value)
{
    DRWAV_ASSERT(pWav          != NULL);
    DRWAV_ASSERT(pWav->onWrite != NULL);

    if (!drwav__is_little_endian()) {
        value = drwav__bswap32(value);
    }

    return drwav__write(pWav, &value, 4);
}

DRWAV_PRIVATE size_t drwav__write_u64ne_to_le(drwav* pWav, drwav_uint64 value)
{
    DRWAV_ASSERT(pWav          != NULL);
    DRWAV_ASSERT(pWav->onWrite != NULL);

    if (!drwav__is_little_endian()) {
        value = drwav__bswap64(value);
    }

    return drwav__write(pWav, &value, 8);
}

DRWAV_PRIVATE size_t drwav__write_f32ne_to_le(drwav* pWav, float value)
{
    union {
       drwav_uint32 u32;
       float f32;
    } u;

    DRWAV_ASSERT(pWav          != NULL);
    DRWAV_ASSERT(pWav->onWrite != NULL);

    u.f32 = value;

    if (!drwav__is_little_endian()) {
        u.u32 = drwav__bswap32(u.u32);
    }

    return drwav__write(pWav, &u.u32, 4);
}

DRWAV_PRIVATE size_t drwav__write_or_count(drwav* pWav, const void* pData, size_t dataSize)
{
    if (pWav == NULL) {
        return dataSize;
    }

    return drwav__write(pWav, pData, dataSize);
}

DRWAV_PRIVATE size_t drwav__write_or_count_byte(drwav* pWav, drwav_uint8 byte)
{
    if (pWav == NULL) {
        return 1;
    }

    return drwav__write_byte(pWav, byte);
}

DRWAV_PRIVATE size_t drwav__write_or_count_u16ne_to_le(drwav* pWav, drwav_uint16 value)
{
    if (pWav == NULL) {
        return 2;
    }

    return drwav__write_u16ne_to_le(pWav, value);
}

DRWAV_PRIVATE size_t drwav__write_or_count_u32ne_to_le(drwav* pWav, drwav_uint32 value)
{
    if (pWav == NULL) {
        return 4;
    }

    return drwav__write_u32ne_to_le(pWav, value);
}

#if 0   
DRWAV_PRIVATE size_t drwav__write_or_count_u64ne_to_le(drwav* pWav, drwav_uint64 value)
{
    if (pWav == NULL) {
        return 8;
    }

    return drwav__write_u64ne_to_le(pWav, value);
}
#endif

DRWAV_PRIVATE size_t drwav__write_or_count_f32ne_to_le(drwav* pWav, float value)
{
    if (pWav == NULL) {
        return 4;
    }

    return drwav__write_f32ne_to_le(pWav, value);
}

DRWAV_PRIVATE size_t drwav__write_or_count_string_to_fixed_size_buf(drwav* pWav, char* str, size_t bufFixedSize)
{
    size_t len;

    if (pWav == NULL) {
        return bufFixedSize;
    }

    len = drwav__strlen_clamped(str, bufFixedSize);
    drwav__write_or_count(pWav, str, len);

    if (len < bufFixedSize) {
        size_t i;
        for (i = 0; i < bufFixedSize - len; ++i) {
            drwav__write_byte(pWav, 0);
        }
    }

    return bufFixedSize;
}



DRWAV_PRIVATE size_t drwav__write_or_count_metadata(drwav* pWav, drwav_metadata* pMetadatas, drwav_uint32 metadataCount)
{
    size_t bytesWritten = 0;
    drwav_bool32 hasListAdtl = DRWAV_FALSE;
    drwav_bool32 hasListInfo = DRWAV_FALSE;
    drwav_uint32 iMetadata;

    if (pMetadatas == NULL || metadataCount == 0) {
        return 0;
    }

    for (iMetadata = 0; iMetadata < metadataCount; ++iMetadata) {
        drwav_metadata* pMetadata = &pMetadatas[iMetadata];
        drwav_uint32 chunkSize = 0;

        if ((pMetadata->type & drwav_metadata_type_list_all_info_strings) || (pMetadata->type == drwav_metadata_type_unknown && pMetadata->data.unknown.chunkLocation == drwav_metadata_location_inside_info_list)) {
            hasListInfo = DRWAV_TRUE;
        }

        if ((pMetadata->type & drwav_metadata_type_list_all_adtl) || (pMetadata->type == drwav_metadata_type_unknown && pMetadata->data.unknown.chunkLocation == drwav_metadata_location_inside_adtl_list)) {
            hasListAdtl = DRWAV_TRUE;
        }

        switch (pMetadata->type) {
            case drwav_metadata_type_smpl:
            {
                drwav_uint32 iLoop;

                chunkSize = DRWAV_SMPL_BYTES + DRWAV_SMPL_LOOP_BYTES * pMetadata->data.smpl.sampleLoopCount + pMetadata->data.smpl.samplerSpecificDataSizeInBytes;

                bytesWritten += drwav__write_or_count(pWav, "smpl", 4);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, chunkSize);

                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.smpl.manufacturerId);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.smpl.productId);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.smpl.samplePeriodNanoseconds);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.smpl.midiUnityNote);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.smpl.midiPitchFraction);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.smpl.smpteFormat);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.smpl.smpteOffset);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.smpl.sampleLoopCount);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.smpl.samplerSpecificDataSizeInBytes);

                for (iLoop = 0; iLoop < pMetadata->data.smpl.sampleLoopCount; ++iLoop) {
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.smpl.pLoops[iLoop].cuePointId);
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.smpl.pLoops[iLoop].type);
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.smpl.pLoops[iLoop].firstSampleOffset);
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.smpl.pLoops[iLoop].lastSampleOffset);
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.smpl.pLoops[iLoop].sampleFraction);
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.smpl.pLoops[iLoop].playCount);
                }

                if (pMetadata->data.smpl.samplerSpecificDataSizeInBytes > 0) {
                    bytesWritten += drwav__write_or_count(pWav, pMetadata->data.smpl.pSamplerSpecificData, pMetadata->data.smpl.samplerSpecificDataSizeInBytes);
                }
            } break;

            case drwav_metadata_type_inst:
            {
                chunkSize = DRWAV_INST_BYTES;

                bytesWritten += drwav__write_or_count(pWav, "inst", 4);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, chunkSize);
                bytesWritten += drwav__write_or_count(pWav, &pMetadata->data.inst.midiUnityNote, 1);
                bytesWritten += drwav__write_or_count(pWav, &pMetadata->data.inst.fineTuneCents, 1);
                bytesWritten += drwav__write_or_count(pWav, &pMetadata->data.inst.gainDecibels, 1);
                bytesWritten += drwav__write_or_count(pWav, &pMetadata->data.inst.lowNote, 1);
                bytesWritten += drwav__write_or_count(pWav, &pMetadata->data.inst.highNote, 1);
                bytesWritten += drwav__write_or_count(pWav, &pMetadata->data.inst.lowVelocity, 1);
                bytesWritten += drwav__write_or_count(pWav, &pMetadata->data.inst.highVelocity, 1);
            } break;

            case drwav_metadata_type_cue:
            {
                drwav_uint32 iCuePoint;

                chunkSize = DRWAV_CUE_BYTES + DRWAV_CUE_POINT_BYTES * pMetadata->data.cue.cuePointCount;

                bytesWritten += drwav__write_or_count(pWav, "cue ", 4);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, chunkSize);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.cue.cuePointCount);
                for (iCuePoint = 0; iCuePoint < pMetadata->data.cue.cuePointCount; ++iCuePoint) {
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.cue.pCuePoints[iCuePoint].id);
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.cue.pCuePoints[iCuePoint].playOrderPosition);
                    bytesWritten += drwav__write_or_count(pWav, pMetadata->data.cue.pCuePoints[iCuePoint].dataChunkId, 4);
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.cue.pCuePoints[iCuePoint].chunkStart);
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.cue.pCuePoints[iCuePoint].blockStart);
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.cue.pCuePoints[iCuePoint].sampleOffset);
                }
            } break;

            case drwav_metadata_type_acid:
            {
                chunkSize = DRWAV_ACID_BYTES;

                bytesWritten += drwav__write_or_count(pWav, "acid", 4);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, chunkSize);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.acid.flags);
                bytesWritten += drwav__write_or_count_u16ne_to_le(pWav, pMetadata->data.acid.midiUnityNote);
                bytesWritten += drwav__write_or_count_u16ne_to_le(pWav, pMetadata->data.acid.reserved1);
                bytesWritten += drwav__write_or_count_f32ne_to_le(pWav, pMetadata->data.acid.reserved2);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.acid.numBeats);
                bytesWritten += drwav__write_or_count_u16ne_to_le(pWav, pMetadata->data.acid.meterDenominator);
                bytesWritten += drwav__write_or_count_u16ne_to_le(pWav, pMetadata->data.acid.meterNumerator);
                bytesWritten += drwav__write_or_count_f32ne_to_le(pWav, pMetadata->data.acid.tempo);
            } break;

            case drwav_metadata_type_bext:
            {
                char reservedBuf[DRWAV_BEXT_RESERVED_BYTES];
                drwav_uint32 timeReferenceLow;
                drwav_uint32 timeReferenceHigh;

                chunkSize = DRWAV_BEXT_BYTES + pMetadata->data.bext.codingHistorySize;

                bytesWritten += drwav__write_or_count(pWav, "bext", 4);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, chunkSize);

                bytesWritten += drwav__write_or_count_string_to_fixed_size_buf(pWav, pMetadata->data.bext.pDescription, DRWAV_BEXT_DESCRIPTION_BYTES);
                bytesWritten += drwav__write_or_count_string_to_fixed_size_buf(pWav, pMetadata->data.bext.pOriginatorName, DRWAV_BEXT_ORIGINATOR_NAME_BYTES);
                bytesWritten += drwav__write_or_count_string_to_fixed_size_buf(pWav, pMetadata->data.bext.pOriginatorReference, DRWAV_BEXT_ORIGINATOR_REF_BYTES);
                bytesWritten += drwav__write_or_count(pWav, pMetadata->data.bext.pOriginationDate, sizeof(pMetadata->data.bext.pOriginationDate));
                bytesWritten += drwav__write_or_count(pWav, pMetadata->data.bext.pOriginationTime, sizeof(pMetadata->data.bext.pOriginationTime));

                timeReferenceLow  = (drwav_uint32)(pMetadata->data.bext.timeReference & 0xFFFFFFFF);
                timeReferenceHigh = (drwav_uint32)(pMetadata->data.bext.timeReference >> 32);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, timeReferenceLow);
                bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, timeReferenceHigh);

                bytesWritten += drwav__write_or_count_u16ne_to_le(pWav, pMetadata->data.bext.version);
                bytesWritten += drwav__write_or_count(pWav, pMetadata->data.bext.pUMID, DRWAV_BEXT_UMID_BYTES);
                bytesWritten += drwav__write_or_count_u16ne_to_le(pWav, pMetadata->data.bext.loudnessValue);
                bytesWritten += drwav__write_or_count_u16ne_to_le(pWav, pMetadata->data.bext.loudnessRange);
                bytesWritten += drwav__write_or_count_u16ne_to_le(pWav, pMetadata->data.bext.maxTruePeakLevel);
                bytesWritten += drwav__write_or_count_u16ne_to_le(pWav, pMetadata->data.bext.maxMomentaryLoudness);
                bytesWritten += drwav__write_or_count_u16ne_to_le(pWav, pMetadata->data.bext.maxShortTermLoudness);

                DRWAV_ZERO_MEMORY(reservedBuf, sizeof(reservedBuf));
                bytesWritten += drwav__write_or_count(pWav, reservedBuf, sizeof(reservedBuf));

                if (pMetadata->data.bext.codingHistorySize > 0) {
                    bytesWritten += drwav__write_or_count(pWav, pMetadata->data.bext.pCodingHistory, pMetadata->data.bext.codingHistorySize);
                }
            } break;

            case drwav_metadata_type_unknown:
            {
                if (pMetadata->data.unknown.chunkLocation == drwav_metadata_location_top_level) {
                    chunkSize = pMetadata->data.unknown.dataSizeInBytes;

                    bytesWritten += drwav__write_or_count(pWav, pMetadata->data.unknown.id, 4);
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, chunkSize);
                    bytesWritten += drwav__write_or_count(pWav, pMetadata->data.unknown.pData, pMetadata->data.unknown.dataSizeInBytes);
                }
            } break;

            default: break;
        }
        if ((chunkSize % 2) != 0) {
            bytesWritten += drwav__write_or_count_byte(pWav, 0);
        }
    }

    if (hasListInfo) {
        drwav_uint32 chunkSize = 4; 
        for (iMetadata = 0; iMetadata < metadataCount; ++iMetadata) {
            drwav_metadata* pMetadata = &pMetadatas[iMetadata];

            if ((pMetadata->type & drwav_metadata_type_list_all_info_strings)) {
                chunkSize += 8; 
                chunkSize += pMetadata->data.infoText.stringLength + 1; 
            } else if (pMetadata->type == drwav_metadata_type_unknown && pMetadata->data.unknown.chunkLocation == drwav_metadata_location_inside_info_list) {
                chunkSize += 8; 
                chunkSize += pMetadata->data.unknown.dataSizeInBytes;
            }

            if ((chunkSize % 2) != 0) {
                chunkSize += 1;
            }
        }

        bytesWritten += drwav__write_or_count(pWav, "LIST", 4);
        bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, chunkSize);
        bytesWritten += drwav__write_or_count(pWav, "INFO", 4);

        for (iMetadata = 0; iMetadata < metadataCount; ++iMetadata) {
            drwav_metadata* pMetadata = &pMetadatas[iMetadata];
            drwav_uint32 subchunkSize = 0;

            if (pMetadata->type & drwav_metadata_type_list_all_info_strings) {
                const char* pID = NULL;

                switch (pMetadata->type) {
                    case drwav_metadata_type_list_info_software:     pID = "ISFT"; break;
                    case drwav_metadata_type_list_info_copyright:    pID = "ICOP"; break;
                    case drwav_metadata_type_list_info_title:        pID = "INAM"; break;
                    case drwav_metadata_type_list_info_artist:       pID = "IART"; break;
                    case drwav_metadata_type_list_info_comment:      pID = "ICMT"; break;
                    case drwav_metadata_type_list_info_date:         pID = "ICRD"; break;
                    case drwav_metadata_type_list_info_genre:        pID = "IGNR"; break;
                    case drwav_metadata_type_list_info_album:        pID = "IPRD"; break;
                    case drwav_metadata_type_list_info_tracknumber:  pID = "ITRK"; break;
                    case drwav_metadata_type_list_info_location:     pID = "IARL"; break;
                    case drwav_metadata_type_list_info_organization: pID = "ICMS"; break;
                    case drwav_metadata_type_list_info_keywords:     pID = "IKEY"; break;
                    case drwav_metadata_type_list_info_medium:       pID = "IMED"; break;
                    case drwav_metadata_type_list_info_description:  pID = "ISBJ"; break;
                    default: break;
                }

                DRWAV_ASSERT(pID != NULL);

                if (pMetadata->data.infoText.stringLength) {
                    subchunkSize = pMetadata->data.infoText.stringLength + 1;
                    bytesWritten += drwav__write_or_count(pWav, pID, 4);
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, subchunkSize);
                    bytesWritten += drwav__write_or_count(pWav, pMetadata->data.infoText.pString, pMetadata->data.infoText.stringLength);
                    bytesWritten += drwav__write_or_count_byte(pWav, '\0');
                }
            } else if (pMetadata->type == drwav_metadata_type_unknown && pMetadata->data.unknown.chunkLocation == drwav_metadata_location_inside_info_list) {
                if (pMetadata->data.unknown.dataSizeInBytes) {
                    subchunkSize = pMetadata->data.unknown.dataSizeInBytes;

                    bytesWritten += drwav__write_or_count(pWav, pMetadata->data.unknown.id, 4);
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.unknown.dataSizeInBytes);
                    bytesWritten += drwav__write_or_count(pWav, pMetadata->data.unknown.pData, subchunkSize);
                }
            }

            if ((subchunkSize % 2) != 0) {
                bytesWritten += drwav__write_or_count_byte(pWav, 0);
            }
        }
    }

    if (hasListAdtl) {
        drwav_uint32 chunkSize = 4; 

        for (iMetadata = 0; iMetadata < metadataCount; ++iMetadata) {
            drwav_metadata* pMetadata = &pMetadatas[iMetadata];

            switch (pMetadata->type)
            {
                case drwav_metadata_type_list_label:
                case drwav_metadata_type_list_note:
                {
                    chunkSize += 8; 
                    chunkSize += DRWAV_LIST_LABEL_OR_NOTE_BYTES;

                    if (pMetadata->data.labelOrNote.stringLength > 0) {
                        chunkSize += pMetadata->data.labelOrNote.stringLength + 1;
                    }
                } break;

                case drwav_metadata_type_list_labelled_cue_region:
                {
                    chunkSize += 8; 
                    chunkSize += DRWAV_LIST_LABELLED_TEXT_BYTES;

                    if (pMetadata->data.labelledCueRegion.stringLength > 0) {
                        chunkSize += pMetadata->data.labelledCueRegion.stringLength + 1;
                    }
                } break;

                case drwav_metadata_type_unknown:
                {
                    if (pMetadata->data.unknown.chunkLocation == drwav_metadata_location_inside_adtl_list) {
                        chunkSize += 8; 
                        chunkSize += pMetadata->data.unknown.dataSizeInBytes;
                    }
                } break;

                default: break;
            }

            if ((chunkSize % 2) != 0) {
                chunkSize += 1;
            }
        }

        bytesWritten += drwav__write_or_count(pWav, "LIST", 4);
        bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, chunkSize);
        bytesWritten += drwav__write_or_count(pWav, "adtl", 4);

        for (iMetadata = 0; iMetadata < metadataCount; ++iMetadata) {
            drwav_metadata* pMetadata = &pMetadatas[iMetadata];
            drwav_uint32 subchunkSize = 0;

            switch (pMetadata->type)
            {
                case drwav_metadata_type_list_label:
                case drwav_metadata_type_list_note:
                {
                    if (pMetadata->data.labelOrNote.stringLength > 0) {
                        const char *pID = NULL;

                        if (pMetadata->type == drwav_metadata_type_list_label) {
                            pID = "labl";
                        }
                        else if (pMetadata->type == drwav_metadata_type_list_note) {
                            pID = "note";
                        }

                        DRWAV_ASSERT(pID != NULL);
                        DRWAV_ASSERT(pMetadata->data.labelOrNote.pString != NULL);

                        subchunkSize = DRWAV_LIST_LABEL_OR_NOTE_BYTES;

                        bytesWritten += drwav__write_or_count(pWav, pID, 4);
                        subchunkSize += pMetadata->data.labelOrNote.stringLength + 1;
                        bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, subchunkSize);

                        bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.labelOrNote.cuePointId);
                        bytesWritten += drwav__write_or_count(pWav, pMetadata->data.labelOrNote.pString, pMetadata->data.labelOrNote.stringLength);
                        bytesWritten += drwav__write_or_count_byte(pWav, '\0');
                    }
                } break;

                case drwav_metadata_type_list_labelled_cue_region:
                {
                    subchunkSize = DRWAV_LIST_LABELLED_TEXT_BYTES;

                    bytesWritten += drwav__write_or_count(pWav, "ltxt", 4);
                    if (pMetadata->data.labelledCueRegion.stringLength > 0) {
                        subchunkSize += pMetadata->data.labelledCueRegion.stringLength + 1;
                    }
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, subchunkSize);
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.labelledCueRegion.cuePointId);
                    bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, pMetadata->data.labelledCueRegion.sampleLength);
                    bytesWritten += drwav__write_or_count(pWav, pMetadata->data.labelledCueRegion.purposeId, 4);
                    bytesWritten += drwav__write_or_count_u16ne_to_le(pWav, pMetadata->data.labelledCueRegion.country);
                    bytesWritten += drwav__write_or_count_u16ne_to_le(pWav, pMetadata->data.labelledCueRegion.language);
                    bytesWritten += drwav__write_or_count_u16ne_to_le(pWav, pMetadata->data.labelledCueRegion.dialect);
                    bytesWritten += drwav__write_or_count_u16ne_to_le(pWav, pMetadata->data.labelledCueRegion.codePage);

                    if (pMetadata->data.labelledCueRegion.stringLength > 0) {
                        DRWAV_ASSERT(pMetadata->data.labelledCueRegion.pString != NULL);

                        bytesWritten += drwav__write_or_count(pWav, pMetadata->data.labelledCueRegion.pString, pMetadata->data.labelledCueRegion.stringLength);
                        bytesWritten += drwav__write_or_count_byte(pWav, '\0');
                    }
                } break;

                case drwav_metadata_type_unknown:
                {
                    if (pMetadata->data.unknown.chunkLocation == drwav_metadata_location_inside_adtl_list) {
                        subchunkSize = pMetadata->data.unknown.dataSizeInBytes;

                        DRWAV_ASSERT(pMetadata->data.unknown.pData != NULL);
                        bytesWritten += drwav__write_or_count(pWav, pMetadata->data.unknown.id, 4);
                        bytesWritten += drwav__write_or_count_u32ne_to_le(pWav, subchunkSize);
                        bytesWritten += drwav__write_or_count(pWav, pMetadata->data.unknown.pData, subchunkSize);
                    }
                } break;

                default: break;
            }

            if ((subchunkSize % 2) != 0) {
                bytesWritten += drwav__write_or_count_byte(pWav, 0);
            }
        }
    }

    DRWAV_ASSERT((bytesWritten % 2) == 0);

    return bytesWritten;
}

DRWAV_PRIVATE drwav_uint32 drwav__riff_chunk_size_riff(drwav_uint64 dataChunkSize, drwav_metadata* pMetadata, drwav_uint32 metadataCount)
{
    drwav_uint64 chunkSize = 4 + 24 + (drwav_uint64)drwav__write_or_count_metadata(NULL, pMetadata, metadataCount) + 8 + dataChunkSize + drwav__chunk_padding_size_riff(dataChunkSize); 
    if (chunkSize > 0xFFFFFFFFUL) {
        chunkSize = 0xFFFFFFFFUL;
    }

    return (drwav_uint32)chunkSize; 
}

DRWAV_PRIVATE drwav_uint32 drwav__data_chunk_size_riff(drwav_uint64 dataChunkSize)
{
    if (dataChunkSize <= 0xFFFFFFFFUL) {
        return (drwav_uint32)dataChunkSize;
    } else {
        return 0xFFFFFFFFUL;
    }
}

DRWAV_PRIVATE drwav_uint64 drwav__riff_chunk_size_w64(drwav_uint64 dataChunkSize)
{
    drwav_uint64 dataSubchunkPaddingSize = drwav__chunk_padding_size_w64(dataChunkSize);

    return 80 + 24 + dataChunkSize + dataSubchunkPaddingSize;   
}

DRWAV_PRIVATE drwav_uint64 drwav__data_chunk_size_w64(drwav_uint64 dataChunkSize)
{
    return 24 + dataChunkSize;        
}

DRWAV_PRIVATE drwav_uint64 drwav__riff_chunk_size_rf64(drwav_uint64 dataChunkSize, drwav_metadata *metadata, drwav_uint32 numMetadata)
{
    drwav_uint64 chunkSize = 4 + 36 + 24 + (drwav_uint64)drwav__write_or_count_metadata(NULL, metadata, numMetadata) + 8 + dataChunkSize + drwav__chunk_padding_size_riff(dataChunkSize); 
    if (chunkSize > 0xFFFFFFFFUL) {
        chunkSize = 0xFFFFFFFFUL;
    }

    return chunkSize;
}

DRWAV_PRIVATE drwav_uint64 drwav__data_chunk_size_rf64(drwav_uint64 dataChunkSize)
{
    return dataChunkSize;
}



DRWAV_PRIVATE drwav_bool32 drwav_preinit_write(drwav* pWav, const drwav_data_format* pFormat, drwav_bool32 isSequential, drwav_write_proc onWrite, drwav_seek_proc onSeek, void* pUserData, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pWav == NULL || onWrite == NULL) {
        return DRWAV_FALSE;
    }

    if (!isSequential && onSeek == NULL) {
        return DRWAV_FALSE; 
    }

    
    if (pFormat->format == DR_WAVE_FORMAT_EXTENSIBLE) {
        return DRWAV_FALSE;
    }
    if (pFormat->format == DR_WAVE_FORMAT_ADPCM || pFormat->format == DR_WAVE_FORMAT_DVI_ADPCM) {
        return DRWAV_FALSE;
    }

    DRWAV_ZERO_MEMORY(pWav, sizeof(*pWav));
    pWav->onWrite   = onWrite;
    pWav->onSeek    = onSeek;
    pWav->pUserData = pUserData;
    pWav->allocationCallbacks = drwav_copy_allocation_callbacks_or_defaults(pAllocationCallbacks);

    if (pWav->allocationCallbacks.onFree == NULL || (pWav->allocationCallbacks.onMalloc == NULL && pWav->allocationCallbacks.onRealloc == NULL)) {
        return DRWAV_FALSE;    
    }

    pWav->fmt.formatTag = (drwav_uint16)pFormat->format;
    pWav->fmt.channels = (drwav_uint16)pFormat->channels;
    pWav->fmt.sampleRate = pFormat->sampleRate;
    pWav->fmt.avgBytesPerSec = (drwav_uint32)((pFormat->bitsPerSample * pFormat->sampleRate * pFormat->channels) / 8);
    pWav->fmt.blockAlign = (drwav_uint16)((pFormat->channels * pFormat->bitsPerSample) / 8);
    pWav->fmt.bitsPerSample = (drwav_uint16)pFormat->bitsPerSample;
    pWav->fmt.extendedSize = 0;
    pWav->isSequentialWrite = isSequential;

    return DRWAV_TRUE;
}


DRWAV_PRIVATE drwav_bool32 drwav_init_write__internal(drwav* pWav, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount)
{
    

    size_t runningPos = 0;
    drwav_uint64 initialDataChunkSize = 0;
    drwav_uint64 chunkSizeFMT;

    




    if (pWav->isSequentialWrite) {
        initialDataChunkSize = (totalSampleCount * pWav->fmt.bitsPerSample) / 8;

        



        if (pFormat->container == drwav_container_riff) {
            if (initialDataChunkSize > (0xFFFFFFFFUL - 36)) {
                return DRWAV_FALSE; 
            }
        }
    }

    pWav->dataChunkDataSizeTargetWrite = initialDataChunkSize;


    
    if (pFormat->container == drwav_container_riff) {
        drwav_uint32 chunkSizeRIFF = 36 + (drwav_uint32)initialDataChunkSize;   
        runningPos += drwav__write(pWav, "RIFF", 4);
        runningPos += drwav__write_u32ne_to_le(pWav, chunkSizeRIFF);
        runningPos += drwav__write(pWav, "WAVE", 4);
    } else if (pFormat->container == drwav_container_w64) {
        drwav_uint64 chunkSizeRIFF = 80 + 24 + initialDataChunkSize;            
        runningPos += drwav__write(pWav, drwavGUID_W64_RIFF, 16);
        runningPos += drwav__write_u64ne_to_le(pWav, chunkSizeRIFF);
        runningPos += drwav__write(pWav, drwavGUID_W64_WAVE, 16);
    } else if (pFormat->container == drwav_container_rf64) {
        runningPos += drwav__write(pWav, "RF64", 4);
        runningPos += drwav__write_u32ne_to_le(pWav, 0xFFFFFFFF);               
        runningPos += drwav__write(pWav, "WAVE", 4);
    } else {
        return DRWAV_FALSE; 
    }


    
    if (pFormat->container == drwav_container_rf64) {
        drwav_uint32 initialds64ChunkSize = 28;                                 
        drwav_uint64 initialRiffChunkSize = 8 + initialds64ChunkSize + initialDataChunkSize;    

        runningPos += drwav__write(pWav, "ds64", 4);
        runningPos += drwav__write_u32ne_to_le(pWav, initialds64ChunkSize);     
        runningPos += drwav__write_u64ne_to_le(pWav, initialRiffChunkSize);     
        runningPos += drwav__write_u64ne_to_le(pWav, initialDataChunkSize);     
        runningPos += drwav__write_u64ne_to_le(pWav, totalSampleCount);         
        runningPos += drwav__write_u32ne_to_le(pWav, 0);                        
    }


    
    if (pFormat->container == drwav_container_riff || pFormat->container == drwav_container_rf64) {
        chunkSizeFMT = 16;
        runningPos += drwav__write(pWav, "fmt ", 4);
        runningPos += drwav__write_u32ne_to_le(pWav, (drwav_uint32)chunkSizeFMT);
    } else if (pFormat->container == drwav_container_w64) {
        chunkSizeFMT = 40;
        runningPos += drwav__write(pWav, drwavGUID_W64_FMT, 16);
        runningPos += drwav__write_u64ne_to_le(pWav, chunkSizeFMT);
    }

    runningPos += drwav__write_u16ne_to_le(pWav, pWav->fmt.formatTag);
    runningPos += drwav__write_u16ne_to_le(pWav, pWav->fmt.channels);
    runningPos += drwav__write_u32ne_to_le(pWav, pWav->fmt.sampleRate);
    runningPos += drwav__write_u32ne_to_le(pWav, pWav->fmt.avgBytesPerSec);
    runningPos += drwav__write_u16ne_to_le(pWav, pWav->fmt.blockAlign);
    runningPos += drwav__write_u16ne_to_le(pWav, pWav->fmt.bitsPerSample);

    

    if (!pWav->isSequentialWrite && pWav->pMetadata != NULL && pWav->metadataCount > 0 && (pFormat->container == drwav_container_riff || pFormat->container == drwav_container_rf64)) {
        runningPos += drwav__write_or_count_metadata(pWav, pWav->pMetadata, pWav->metadataCount);
    }

    pWav->dataChunkDataPos = runningPos;

    
    if (pFormat->container == drwav_container_riff) {
        drwav_uint32 chunkSizeDATA = (drwav_uint32)initialDataChunkSize;
        runningPos += drwav__write(pWav, "data", 4);
        runningPos += drwav__write_u32ne_to_le(pWav, chunkSizeDATA);
    } else if (pFormat->container == drwav_container_w64) {
        drwav_uint64 chunkSizeDATA = 24 + initialDataChunkSize;     
        runningPos += drwav__write(pWav, drwavGUID_W64_DATA, 16);
        runningPos += drwav__write_u64ne_to_le(pWav, chunkSizeDATA);
    } else if (pFormat->container == drwav_container_rf64) {
        runningPos += drwav__write(pWav, "data", 4);
        runningPos += drwav__write_u32ne_to_le(pWav, 0xFFFFFFFF);   
    }

    
    pWav->container = pFormat->container;
    pWav->channels = (drwav_uint16)pFormat->channels;
    pWav->sampleRate = pFormat->sampleRate;
    pWav->bitsPerSample = (drwav_uint16)pFormat->bitsPerSample;
    pWav->translatedFormatTag = (drwav_uint16)pFormat->format;
    pWav->dataChunkDataPos = runningPos;

    return DRWAV_TRUE;
}


DRWAV_API drwav_bool32 drwav_init_write(drwav* pWav, const drwav_data_format* pFormat, drwav_write_proc onWrite, drwav_seek_proc onSeek, void* pUserData, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (!drwav_preinit_write(pWav, pFormat, DRWAV_FALSE, onWrite, onSeek, pUserData, pAllocationCallbacks)) {
        return DRWAV_FALSE;
    }

    return drwav_init_write__internal(pWav, pFormat, 0);               
}

DRWAV_API drwav_bool32 drwav_init_write_sequential(drwav* pWav, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, drwav_write_proc onWrite, void* pUserData, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (!drwav_preinit_write(pWav, pFormat, DRWAV_TRUE, onWrite, NULL, pUserData, pAllocationCallbacks)) {
        return DRWAV_FALSE;
    }

    return drwav_init_write__internal(pWav, pFormat, totalSampleCount); 
}

DRWAV_API drwav_bool32 drwav_init_write_sequential_pcm_frames(drwav* pWav, const drwav_data_format* pFormat, drwav_uint64 totalPCMFrameCount, drwav_write_proc onWrite, void* pUserData, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pFormat == NULL) {
        return DRWAV_FALSE;
    }

    return drwav_init_write_sequential(pWav, pFormat, totalPCMFrameCount*pFormat->channels, onWrite, pUserData, pAllocationCallbacks);
}

DRWAV_API drwav_bool32 drwav_init_write_with_metadata(drwav* pWav, const drwav_data_format* pFormat, drwav_write_proc onWrite, drwav_seek_proc onSeek, void* pUserData, const drwav_allocation_callbacks* pAllocationCallbacks, drwav_metadata* pMetadata, drwav_uint32 metadataCount)
{
    if (!drwav_preinit_write(pWav, pFormat, DRWAV_FALSE, onWrite, onSeek, pUserData, pAllocationCallbacks)) {
        return DRWAV_FALSE;
    }

    pWav->pMetadata     = pMetadata;
    pWav->metadataCount = metadataCount;

    return drwav_init_write__internal(pWav, pFormat, 0);
}


DRWAV_API drwav_uint64 drwav_target_write_size_bytes(const drwav_data_format* pFormat, drwav_uint64 totalFrameCount, drwav_metadata* pMetadata, drwav_uint32 metadataCount)
{
    
    drwav_uint64 targetDataSizeBytes = (drwav_uint64)((drwav_int64)totalFrameCount * pFormat->channels * pFormat->bitsPerSample/8.0);
    drwav_uint64 riffChunkSizeBytes;
    drwav_uint64 fileSizeBytes = 0;

    if (pFormat->container == drwav_container_riff) {
        riffChunkSizeBytes = drwav__riff_chunk_size_riff(targetDataSizeBytes, pMetadata, metadataCount);
        fileSizeBytes = (8 + riffChunkSizeBytes);   
    } else if (pFormat->container == drwav_container_w64) {
        riffChunkSizeBytes = drwav__riff_chunk_size_w64(targetDataSizeBytes);
        fileSizeBytes = riffChunkSizeBytes;
    } else if (pFormat->container == drwav_container_rf64) {
        riffChunkSizeBytes = drwav__riff_chunk_size_rf64(targetDataSizeBytes, pMetadata, metadataCount);
        fileSizeBytes = (8 + riffChunkSizeBytes);   
    }

    return fileSizeBytes;
}


#ifndef DR_WAV_NO_STDIO



#include <errno.h>
DRWAV_PRIVATE drwav_result drwav_result_from_errno(int e)
{
    switch (e)
    {
        case 0: return DRWAV_SUCCESS;
    #ifdef EPERM
        case EPERM: return DRWAV_INVALID_OPERATION;
    #endif
    #ifdef ENOENT
        case ENOENT: return DRWAV_DOES_NOT_EXIST;
    #endif
    #ifdef ESRCH
        case ESRCH: return DRWAV_DOES_NOT_EXIST;
    #endif
    #ifdef EINTR
        case EINTR: return DRWAV_INTERRUPT;
    #endif
    #ifdef EIO
        case EIO: return DRWAV_IO_ERROR;
    #endif
    #ifdef ENXIO
        case ENXIO: return DRWAV_DOES_NOT_EXIST;
    #endif
    #ifdef E2BIG
        case E2BIG: return DRWAV_INVALID_ARGS;
    #endif
    #ifdef ENOEXEC
        case ENOEXEC: return DRWAV_INVALID_FILE;
    #endif
    #ifdef EBADF
        case EBADF: return DRWAV_INVALID_FILE;
    #endif
    #ifdef ECHILD
        case ECHILD: return DRWAV_ERROR;
    #endif
    #ifdef EAGAIN
        case EAGAIN: return DRWAV_UNAVAILABLE;
    #endif
    #ifdef ENOMEM
        case ENOMEM: return DRWAV_OUT_OF_MEMORY;
    #endif
    #ifdef EACCES
        case EACCES: return DRWAV_ACCESS_DENIED;
    #endif
    #ifdef EFAULT
        case EFAULT: return DRWAV_BAD_ADDRESS;
    #endif
    #ifdef ENOTBLK
        case ENOTBLK: return DRWAV_ERROR;
    #endif
    #ifdef EBUSY
        case EBUSY: return DRWAV_BUSY;
    #endif
    #ifdef EEXIST
        case EEXIST: return DRWAV_ALREADY_EXISTS;
    #endif
    #ifdef EXDEV
        case EXDEV: return DRWAV_ERROR;
    #endif
    #ifdef ENODEV
        case ENODEV: return DRWAV_DOES_NOT_EXIST;
    #endif
    #ifdef ENOTDIR
        case ENOTDIR: return DRWAV_NOT_DIRECTORY;
    #endif
    #ifdef EISDIR
        case EISDIR: return DRWAV_IS_DIRECTORY;
    #endif
    #ifdef EINVAL
        case EINVAL: return DRWAV_INVALID_ARGS;
    #endif
    #ifdef ENFILE
        case ENFILE: return DRWAV_TOO_MANY_OPEN_FILES;
    #endif
    #ifdef EMFILE
        case EMFILE: return DRWAV_TOO_MANY_OPEN_FILES;
    #endif
    #ifdef ENOTTY
        case ENOTTY: return DRWAV_INVALID_OPERATION;
    #endif
    #ifdef ETXTBSY
        case ETXTBSY: return DRWAV_BUSY;
    #endif
    #ifdef EFBIG
        case EFBIG: return DRWAV_TOO_BIG;
    #endif
    #ifdef ENOSPC
        case ENOSPC: return DRWAV_NO_SPACE;
    #endif
    #ifdef ESPIPE
        case ESPIPE: return DRWAV_BAD_SEEK;
    #endif
    #ifdef EROFS
        case EROFS: return DRWAV_ACCESS_DENIED;
    #endif
    #ifdef EMLINK
        case EMLINK: return DRWAV_TOO_MANY_LINKS;
    #endif
    #ifdef EPIPE
        case EPIPE: return DRWAV_BAD_PIPE;
    #endif
    #ifdef EDOM
        case EDOM: return DRWAV_OUT_OF_RANGE;
    #endif
    #ifdef ERANGE
        case ERANGE: return DRWAV_OUT_OF_RANGE;
    #endif
    #ifdef EDEADLK
        case EDEADLK: return DRWAV_DEADLOCK;
    #endif
    #ifdef ENAMETOOLONG
        case ENAMETOOLONG: return DRWAV_PATH_TOO_LONG;
    #endif
    #ifdef ENOLCK
        case ENOLCK: return DRWAV_ERROR;
    #endif
    #ifdef ENOSYS
        case ENOSYS: return DRWAV_NOT_IMPLEMENTED;
    #endif
    #if defined(ENOTEMPTY) && ENOTEMPTY != EEXIST   
        case ENOTEMPTY: return DRWAV_DIRECTORY_NOT_EMPTY;
    #endif
    #ifdef ELOOP
        case ELOOP: return DRWAV_TOO_MANY_LINKS;
    #endif
    #ifdef ENOMSG
        case ENOMSG: return DRWAV_NO_MESSAGE;
    #endif
    #ifdef EIDRM
        case EIDRM: return DRWAV_ERROR;
    #endif
    #ifdef ECHRNG
        case ECHRNG: return DRWAV_ERROR;
    #endif
    #ifdef EL2NSYNC
        case EL2NSYNC: return DRWAV_ERROR;
    #endif
    #ifdef EL3HLT
        case EL3HLT: return DRWAV_ERROR;
    #endif
    #ifdef EL3RST
        case EL3RST: return DRWAV_ERROR;
    #endif
    #ifdef ELNRNG
        case ELNRNG: return DRWAV_OUT_OF_RANGE;
    #endif
    #ifdef EUNATCH
        case EUNATCH: return DRWAV_ERROR;
    #endif
    #ifdef ENOCSI
        case ENOCSI: return DRWAV_ERROR;
    #endif
    #ifdef EL2HLT
        case EL2HLT: return DRWAV_ERROR;
    #endif
    #ifdef EBADE
        case EBADE: return DRWAV_ERROR;
    #endif
    #ifdef EBADR
        case EBADR: return DRWAV_ERROR;
    #endif
    #ifdef EXFULL
        case EXFULL: return DRWAV_ERROR;
    #endif
    #ifdef ENOANO
        case ENOANO: return DRWAV_ERROR;
    #endif
    #ifdef EBADRQC
        case EBADRQC: return DRWAV_ERROR;
    #endif
    #ifdef EBADSLT
        case EBADSLT: return DRWAV_ERROR;
    #endif
    #ifdef EBFONT
        case EBFONT: return DRWAV_INVALID_FILE;
    #endif
    #ifdef ENOSTR
        case ENOSTR: return DRWAV_ERROR;
    #endif
    #ifdef ENODATA
        case ENODATA: return DRWAV_NO_DATA_AVAILABLE;
    #endif
    #ifdef ETIME
        case ETIME: return DRWAV_TIMEOUT;
    #endif
    #ifdef ENOSR
        case ENOSR: return DRWAV_NO_DATA_AVAILABLE;
    #endif
    #ifdef ENONET
        case ENONET: return DRWAV_NO_NETWORK;
    #endif
    #ifdef ENOPKG
        case ENOPKG: return DRWAV_ERROR;
    #endif
    #ifdef EREMOTE
        case EREMOTE: return DRWAV_ERROR;
    #endif
    #ifdef ENOLINK
        case ENOLINK: return DRWAV_ERROR;
    #endif
    #ifdef EADV
        case EADV: return DRWAV_ERROR;
    #endif
    #ifdef ESRMNT
        case ESRMNT: return DRWAV_ERROR;
    #endif
    #ifdef ECOMM
        case ECOMM: return DRWAV_ERROR;
    #endif
    #ifdef EPROTO
        case EPROTO: return DRWAV_ERROR;
    #endif
    #ifdef EMULTIHOP
        case EMULTIHOP: return DRWAV_ERROR;
    #endif
    #ifdef EDOTDOT
        case EDOTDOT: return DRWAV_ERROR;
    #endif
    #ifdef EBADMSG
        case EBADMSG: return DRWAV_BAD_MESSAGE;
    #endif
    #ifdef EOVERFLOW
        case EOVERFLOW: return DRWAV_TOO_BIG;
    #endif
    #ifdef ENOTUNIQ
        case ENOTUNIQ: return DRWAV_NOT_UNIQUE;
    #endif
    #ifdef EBADFD
        case EBADFD: return DRWAV_ERROR;
    #endif
    #ifdef EREMCHG
        case EREMCHG: return DRWAV_ERROR;
    #endif
    #ifdef ELIBACC
        case ELIBACC: return DRWAV_ACCESS_DENIED;
    #endif
    #ifdef ELIBBAD
        case ELIBBAD: return DRWAV_INVALID_FILE;
    #endif
    #ifdef ELIBSCN
        case ELIBSCN: return DRWAV_INVALID_FILE;
    #endif
    #ifdef ELIBMAX
        case ELIBMAX: return DRWAV_ERROR;
    #endif
    #ifdef ELIBEXEC
        case ELIBEXEC: return DRWAV_ERROR;
    #endif
    #ifdef EILSEQ
        case EILSEQ: return DRWAV_INVALID_DATA;
    #endif
    #ifdef ERESTART
        case ERESTART: return DRWAV_ERROR;
    #endif
    #ifdef ESTRPIPE
        case ESTRPIPE: return DRWAV_ERROR;
    #endif
    #ifdef EUSERS
        case EUSERS: return DRWAV_ERROR;
    #endif
    #ifdef ENOTSOCK
        case ENOTSOCK: return DRWAV_NOT_SOCKET;
    #endif
    #ifdef EDESTADDRREQ
        case EDESTADDRREQ: return DRWAV_NO_ADDRESS;
    #endif
    #ifdef EMSGSIZE
        case EMSGSIZE: return DRWAV_TOO_BIG;
    #endif
    #ifdef EPROTOTYPE
        case EPROTOTYPE: return DRWAV_BAD_PROTOCOL;
    #endif
    #ifdef ENOPROTOOPT
        case ENOPROTOOPT: return DRWAV_PROTOCOL_UNAVAILABLE;
    #endif
    #ifdef EPROTONOSUPPORT
        case EPROTONOSUPPORT: return DRWAV_PROTOCOL_NOT_SUPPORTED;
    #endif
    #ifdef ESOCKTNOSUPPORT
        case ESOCKTNOSUPPORT: return DRWAV_SOCKET_NOT_SUPPORTED;
    #endif
    #ifdef EOPNOTSUPP
        case EOPNOTSUPP: return DRWAV_INVALID_OPERATION;
    #endif
    #ifdef EPFNOSUPPORT
        case EPFNOSUPPORT: return DRWAV_PROTOCOL_FAMILY_NOT_SUPPORTED;
    #endif
    #ifdef EAFNOSUPPORT
        case EAFNOSUPPORT: return DRWAV_ADDRESS_FAMILY_NOT_SUPPORTED;
    #endif
    #ifdef EADDRINUSE
        case EADDRINUSE: return DRWAV_ALREADY_IN_USE;
    #endif
    #ifdef EADDRNOTAVAIL
        case EADDRNOTAVAIL: return DRWAV_ERROR;
    #endif
    #ifdef ENETDOWN
        case ENETDOWN: return DRWAV_NO_NETWORK;
    #endif
    #ifdef ENETUNREACH
        case ENETUNREACH: return DRWAV_NO_NETWORK;
    #endif
    #ifdef ENETRESET
        case ENETRESET: return DRWAV_NO_NETWORK;
    #endif
    #ifdef ECONNABORTED
        case ECONNABORTED: return DRWAV_NO_NETWORK;
    #endif
    #ifdef ECONNRESET
        case ECONNRESET: return DRWAV_CONNECTION_RESET;
    #endif
    #ifdef ENOBUFS
        case ENOBUFS: return DRWAV_NO_SPACE;
    #endif
    #ifdef EISCONN
        case EISCONN: return DRWAV_ALREADY_CONNECTED;
    #endif
    #ifdef ENOTCONN
        case ENOTCONN: return DRWAV_NOT_CONNECTED;
    #endif
    #ifdef ESHUTDOWN
        case ESHUTDOWN: return DRWAV_ERROR;
    #endif
    #ifdef ETOOMANYREFS
        case ETOOMANYREFS: return DRWAV_ERROR;
    #endif
    #ifdef ETIMEDOUT
        case ETIMEDOUT: return DRWAV_TIMEOUT;
    #endif
    #ifdef ECONNREFUSED
        case ECONNREFUSED: return DRWAV_CONNECTION_REFUSED;
    #endif
    #ifdef EHOSTDOWN
        case EHOSTDOWN: return DRWAV_NO_HOST;
    #endif
    #ifdef EHOSTUNREACH
        case EHOSTUNREACH: return DRWAV_NO_HOST;
    #endif
    #ifdef EALREADY
        case EALREADY: return DRWAV_IN_PROGRESS;
    #endif
    #ifdef EINPROGRESS
        case EINPROGRESS: return DRWAV_IN_PROGRESS;
    #endif
    #ifdef ESTALE
        case ESTALE: return DRWAV_INVALID_FILE;
    #endif
    #ifdef EUCLEAN
        case EUCLEAN: return DRWAV_ERROR;
    #endif
    #ifdef ENOTNAM
        case ENOTNAM: return DRWAV_ERROR;
    #endif
    #ifdef ENAVAIL
        case ENAVAIL: return DRWAV_ERROR;
    #endif
    #ifdef EISNAM
        case EISNAM: return DRWAV_ERROR;
    #endif
    #ifdef EREMOTEIO
        case EREMOTEIO: return DRWAV_IO_ERROR;
    #endif
    #ifdef EDQUOT
        case EDQUOT: return DRWAV_NO_SPACE;
    #endif
    #ifdef ENOMEDIUM
        case ENOMEDIUM: return DRWAV_DOES_NOT_EXIST;
    #endif
    #ifdef EMEDIUMTYPE
        case EMEDIUMTYPE: return DRWAV_ERROR;
    #endif
    #ifdef ECANCELED
        case ECANCELED: return DRWAV_CANCELLED;
    #endif
    #ifdef ENOKEY
        case ENOKEY: return DRWAV_ERROR;
    #endif
    #ifdef EKEYEXPIRED
        case EKEYEXPIRED: return DRWAV_ERROR;
    #endif
    #ifdef EKEYREVOKED
        case EKEYREVOKED: return DRWAV_ERROR;
    #endif
    #ifdef EKEYREJECTED
        case EKEYREJECTED: return DRWAV_ERROR;
    #endif
    #ifdef EOWNERDEAD
        case EOWNERDEAD: return DRWAV_ERROR;
    #endif
    #ifdef ENOTRECOVERABLE
        case ENOTRECOVERABLE: return DRWAV_ERROR;
    #endif
    #ifdef ERFKILL
        case ERFKILL: return DRWAV_ERROR;
    #endif
    #ifdef EHWPOISON
        case EHWPOISON: return DRWAV_ERROR;
    #endif
        default: return DRWAV_ERROR;
    }
}



DRWAV_PRIVATE drwav_result drwav_fopen(FILE** ppFile, const char* pFilePath, const char* pOpenMode)
{
#if defined(_MSC_VER) && _MSC_VER >= 1400
    errno_t err;
#endif

    if (ppFile != NULL) {
        *ppFile = NULL;  
    }

    if (pFilePath == NULL || pOpenMode == NULL || ppFile == NULL) {
        return DRWAV_INVALID_ARGS;
    }

#if defined(_MSC_VER) && _MSC_VER >= 1400
    err = fopen_s(ppFile, pFilePath, pOpenMode);
    if (err != 0) {
        return drwav_result_from_errno(err);
    }
#else
#if defined(_WIN32) || defined(__APPLE__)
    *ppFile = fopen(pFilePath, pOpenMode);
#else
    #if defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS == 64 && defined(_LARGEFILE64_SOURCE)
        *ppFile = fopen64(pFilePath, pOpenMode);
    #else
        *ppFile = fopen(pFilePath, pOpenMode);
    #endif
#endif
    if (*ppFile == NULL) {
        drwav_result result = drwav_result_from_errno(errno);
        if (result == DRWAV_SUCCESS) {
            result = DRWAV_ERROR;   
        }

        return result;
    }
#endif

    return DRWAV_SUCCESS;
}













#if defined(_WIN32)
    #if defined(_MSC_VER) || defined(__MINGW64__) || (!defined(__STRICT_ANSI__) && !defined(_NO_EXT_KEYS))
        #define DRWAV_HAS_WFOPEN
    #endif
#endif

#ifndef DR_WAV_NO_WCHAR
DRWAV_PRIVATE drwav_result drwav_wfopen(FILE** ppFile, const wchar_t* pFilePath, const wchar_t* pOpenMode, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (ppFile != NULL) {
        *ppFile = NULL;  
    }

    if (pFilePath == NULL || pOpenMode == NULL || ppFile == NULL) {
        return DRWAV_INVALID_ARGS;
    }

#if defined(DRWAV_HAS_WFOPEN)
    {
        
    #if defined(_MSC_VER) && _MSC_VER >= 1400
        errno_t err = _wfopen_s(ppFile, pFilePath, pOpenMode);
        if (err != 0) {
            return drwav_result_from_errno(err);
        }
    #else
        *ppFile = _wfopen(pFilePath, pOpenMode);
        if (*ppFile == NULL) {
            return drwav_result_from_errno(errno);
        }
    #endif
        (void)pAllocationCallbacks;
    }
#else
	







	




	#if defined(__DJGPP__)
	{
		
	}
	#else
    {
        mbstate_t mbs;
        size_t lenMB;
        const wchar_t* pFilePathTemp = pFilePath;
        char* pFilePathMB = NULL;
        char pOpenModeMB[32] = {0};

        
        DRWAV_ZERO_OBJECT(&mbs);
        lenMB = wcsrtombs(NULL, &pFilePathTemp, 0, &mbs);
        if (lenMB == (size_t)-1) {
            return drwav_result_from_errno(errno);
        }

        pFilePathMB = (char*)drwav__malloc_from_callbacks(lenMB + 1, pAllocationCallbacks);
        if (pFilePathMB == NULL) {
            return DRWAV_OUT_OF_MEMORY;
        }

        pFilePathTemp = pFilePath;
        DRWAV_ZERO_OBJECT(&mbs);
        wcsrtombs(pFilePathMB, &pFilePathTemp, lenMB + 1, &mbs);

        
        {
            size_t i = 0;
            for (;;) {
                if (pOpenMode[i] == 0) {
                    pOpenModeMB[i] = '\0';
                    break;
                }

                pOpenModeMB[i] = (char)pOpenMode[i];
                i += 1;
            }
        }

        *ppFile = fopen(pFilePathMB, pOpenModeMB);

        drwav__free_from_callbacks(pFilePathMB, pAllocationCallbacks);
    }
	#endif

    if (*ppFile == NULL) {
        return DRWAV_ERROR;
    }
#endif

    return DRWAV_SUCCESS;
}
#endif



DRWAV_PRIVATE size_t drwav__on_read_stdio(void* pUserData, void* pBufferOut, size_t bytesToRead)
{
    return fread(pBufferOut, 1, bytesToRead, (FILE*)pUserData);
}

DRWAV_PRIVATE size_t drwav__on_write_stdio(void* pUserData, const void* pData, size_t bytesToWrite)
{
    return fwrite(pData, 1, bytesToWrite, (FILE*)pUserData);
}

DRWAV_PRIVATE drwav_bool32 drwav__on_seek_stdio(void* pUserData, int offset, drwav_seek_origin origin)
{
    int whence = SEEK_SET;
    if (origin == DRWAV_SEEK_CUR) {
        whence = SEEK_CUR;
    } else if (origin == DRWAV_SEEK_END) {
        whence = SEEK_END;
    }

    return fseek((FILE*)pUserData, offset, whence) == 0;
}

DRWAV_PRIVATE drwav_bool32 drwav__on_tell_stdio(void* pUserData, drwav_int64* pCursor)
{
    FILE* pFileStdio = (FILE*)pUserData;
    drwav_int64 result;

    
    DRWAV_ASSERT(pFileStdio != NULL);
    DRWAV_ASSERT(pCursor    != NULL);

#if defined(_WIN32) && !defined(NXDK)
    #if defined(_MSC_VER) && _MSC_VER > 1200
        result = _ftelli64(pFileStdio);
    #else
        result = ftell(pFileStdio);
    #endif
#else
    result = ftell(pFileStdio);
#endif

    if (result < 0) {
        return DRWAV_FALSE;
    }

    *pCursor = result;

    return DRWAV_TRUE;
}

DRWAV_API drwav_bool32 drwav_init_file(drwav* pWav, const char* filename, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_file_ex(pWav, filename, NULL, NULL, 0, pAllocationCallbacks);
}


DRWAV_PRIVATE drwav_bool32 drwav_init_file__internal_FILE(drwav* pWav, FILE* pFile, drwav_chunk_proc onChunk, void* pChunkUserData, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav_bool32 result;

    result = drwav_preinit(pWav, drwav__on_read_stdio, drwav__on_seek_stdio, drwav__on_tell_stdio, (void*)pFile, pAllocationCallbacks);
    if (result != DRWAV_TRUE) {
        fclose(pFile);
        return result;
    }

    result = drwav_init__internal(pWav, onChunk, pChunkUserData, flags);
    if (result != DRWAV_TRUE) {
        fclose(pFile);
        return result;
    }

    return DRWAV_TRUE;
}

DRWAV_API drwav_bool32 drwav_init_file_ex(drwav* pWav, const char* filename, drwav_chunk_proc onChunk, void* pChunkUserData, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    FILE* pFile;
    if (drwav_fopen(&pFile, filename, "rb") != DRWAV_SUCCESS) {
        return DRWAV_FALSE;
    }

    
    return drwav_init_file__internal_FILE(pWav, pFile, onChunk, pChunkUserData, flags, pAllocationCallbacks);
}

#ifndef DR_WAV_NO_WCHAR
DRWAV_API drwav_bool32 drwav_init_file_w(drwav* pWav, const wchar_t* filename, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_file_ex_w(pWav, filename, NULL, NULL, 0, pAllocationCallbacks);
}

DRWAV_API drwav_bool32 drwav_init_file_ex_w(drwav* pWav, const wchar_t* filename, drwav_chunk_proc onChunk, void* pChunkUserData, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    FILE* pFile;
    if (drwav_wfopen(&pFile, filename, L"rb", pAllocationCallbacks) != DRWAV_SUCCESS) {
        return DRWAV_FALSE;
    }

    
    return drwav_init_file__internal_FILE(pWav, pFile, onChunk, pChunkUserData, flags, pAllocationCallbacks);
}
#endif

DRWAV_API drwav_bool32 drwav_init_file_with_metadata(drwav* pWav, const char* filename, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    FILE* pFile;
    if (drwav_fopen(&pFile, filename, "rb") != DRWAV_SUCCESS) {
        return DRWAV_FALSE;
    }

    
    return drwav_init_file__internal_FILE(pWav, pFile, NULL, NULL, flags | DRWAV_WITH_METADATA, pAllocationCallbacks);
}

#ifndef DR_WAV_NO_WCHAR
DRWAV_API drwav_bool32 drwav_init_file_with_metadata_w(drwav* pWav, const wchar_t* filename, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    FILE* pFile;
    if (drwav_wfopen(&pFile, filename, L"rb", pAllocationCallbacks) != DRWAV_SUCCESS) {
        return DRWAV_FALSE;
    }

    
    return drwav_init_file__internal_FILE(pWav, pFile, NULL, NULL, flags | DRWAV_WITH_METADATA, pAllocationCallbacks);
}
#endif


DRWAV_PRIVATE drwav_bool32 drwav_init_file_write__internal_FILE(drwav* pWav, FILE* pFile, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, drwav_bool32 isSequential, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav_bool32 result;

    result = drwav_preinit_write(pWav, pFormat, isSequential, drwav__on_write_stdio, drwav__on_seek_stdio, (void*)pFile, pAllocationCallbacks);
    if (result != DRWAV_TRUE) {
        fclose(pFile);
        return result;
    }

    result = drwav_init_write__internal(pWav, pFormat, totalSampleCount);
    if (result != DRWAV_TRUE) {
        fclose(pFile);
        return result;
    }

    return DRWAV_TRUE;
}

DRWAV_PRIVATE drwav_bool32 drwav_init_file_write__internal(drwav* pWav, const char* filename, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, drwav_bool32 isSequential, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    FILE* pFile;
    if (drwav_fopen(&pFile, filename, "wb") != DRWAV_SUCCESS) {
        return DRWAV_FALSE;
    }

    
    return drwav_init_file_write__internal_FILE(pWav, pFile, pFormat, totalSampleCount, isSequential, pAllocationCallbacks);
}

#ifndef DR_WAV_NO_WCHAR
DRWAV_PRIVATE drwav_bool32 drwav_init_file_write_w__internal(drwav* pWav, const wchar_t* filename, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, drwav_bool32 isSequential, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    FILE* pFile;
    if (drwav_wfopen(&pFile, filename, L"wb", pAllocationCallbacks) != DRWAV_SUCCESS) {
        return DRWAV_FALSE;
    }

    
    return drwav_init_file_write__internal_FILE(pWav, pFile, pFormat, totalSampleCount, isSequential, pAllocationCallbacks);
}
#endif

DRWAV_API drwav_bool32 drwav_init_file_write(drwav* pWav, const char* filename, const drwav_data_format* pFormat, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_file_write__internal(pWav, filename, pFormat, 0, DRWAV_FALSE, pAllocationCallbacks);
}

DRWAV_API drwav_bool32 drwav_init_file_write_sequential(drwav* pWav, const char* filename, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_file_write__internal(pWav, filename, pFormat, totalSampleCount, DRWAV_TRUE, pAllocationCallbacks);
}

DRWAV_API drwav_bool32 drwav_init_file_write_sequential_pcm_frames(drwav* pWav, const char* filename, const drwav_data_format* pFormat, drwav_uint64 totalPCMFrameCount, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pFormat == NULL) {
        return DRWAV_FALSE;
    }

    return drwav_init_file_write_sequential(pWav, filename, pFormat, totalPCMFrameCount*pFormat->channels, pAllocationCallbacks);
}

#ifndef DR_WAV_NO_WCHAR
DRWAV_API drwav_bool32 drwav_init_file_write_w(drwav* pWav, const wchar_t* filename, const drwav_data_format* pFormat, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_file_write_w__internal(pWav, filename, pFormat, 0, DRWAV_FALSE, pAllocationCallbacks);
}

DRWAV_API drwav_bool32 drwav_init_file_write_sequential_w(drwav* pWav, const wchar_t* filename, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_file_write_w__internal(pWav, filename, pFormat, totalSampleCount, DRWAV_TRUE, pAllocationCallbacks);
}

DRWAV_API drwav_bool32 drwav_init_file_write_sequential_pcm_frames_w(drwav* pWav, const wchar_t* filename, const drwav_data_format* pFormat, drwav_uint64 totalPCMFrameCount, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pFormat == NULL) {
        return DRWAV_FALSE;
    }

    return drwav_init_file_write_sequential_w(pWav, filename, pFormat, totalPCMFrameCount*pFormat->channels, pAllocationCallbacks);
}
#endif
#endif  


DRWAV_PRIVATE size_t drwav__on_read_memory(void* pUserData, void* pBufferOut, size_t bytesToRead)
{
    drwav* pWav = (drwav*)pUserData;
    size_t bytesRemaining;

    DRWAV_ASSERT(pWav != NULL);
    DRWAV_ASSERT(pWav->memoryStream.dataSize >= pWav->memoryStream.currentReadPos);

    bytesRemaining = pWav->memoryStream.dataSize - pWav->memoryStream.currentReadPos;
    if (bytesToRead > bytesRemaining) {
        bytesToRead = bytesRemaining;
    }

    if (bytesToRead > 0) {
        DRWAV_COPY_MEMORY(pBufferOut, pWav->memoryStream.data + pWav->memoryStream.currentReadPos, bytesToRead);
        pWav->memoryStream.currentReadPos += bytesToRead;
    }

    return bytesToRead;
}

DRWAV_PRIVATE drwav_bool32 drwav__on_seek_memory(void* pUserData, int offset, drwav_seek_origin origin)
{
    drwav* pWav = (drwav*)pUserData;
    drwav_int64 newCursor;

    DRWAV_ASSERT(pWav != NULL);

    if (origin == DRWAV_SEEK_SET) {
        newCursor = 0;
    } else if (origin == DRWAV_SEEK_CUR) {
        newCursor = (drwav_int64)pWav->memoryStream.currentReadPos;
    } else if (origin == DRWAV_SEEK_END) {
        newCursor = (drwav_int64)pWav->memoryStream.dataSize;
    } else {
        DRWAV_ASSERT(!"Invalid seek origin");
        return DRWAV_FALSE;
    }

    newCursor += offset;

    if (newCursor < 0) {
        return DRWAV_FALSE;  
    }
    if ((size_t)newCursor > pWav->memoryStream.dataSize) {
        return DRWAV_FALSE;  
    }

    pWav->memoryStream.currentReadPos = (size_t)newCursor;

    return DRWAV_TRUE;
}

DRWAV_PRIVATE size_t drwav__on_write_memory(void* pUserData, const void* pDataIn, size_t bytesToWrite)
{
    drwav* pWav = (drwav*)pUserData;
    size_t bytesRemaining;

    DRWAV_ASSERT(pWav != NULL);
    DRWAV_ASSERT(pWav->memoryStreamWrite.dataCapacity >= pWav->memoryStreamWrite.currentWritePos);

    bytesRemaining = pWav->memoryStreamWrite.dataCapacity - pWav->memoryStreamWrite.currentWritePos;
    if (bytesRemaining < bytesToWrite) {
        
        void* pNewData;
        size_t newDataCapacity = (pWav->memoryStreamWrite.dataCapacity == 0) ? 256 : pWav->memoryStreamWrite.dataCapacity * 2;

        
        if ((newDataCapacity - pWav->memoryStreamWrite.currentWritePos) < bytesToWrite) {
            newDataCapacity = pWav->memoryStreamWrite.currentWritePos + bytesToWrite;
        }

        pNewData = drwav__realloc_from_callbacks(*pWav->memoryStreamWrite.ppData, newDataCapacity, pWav->memoryStreamWrite.dataCapacity, &pWav->allocationCallbacks);
        if (pNewData == NULL) {
            return 0;
        }

        *pWav->memoryStreamWrite.ppData = pNewData;
        pWav->memoryStreamWrite.dataCapacity = newDataCapacity;
    }

    DRWAV_COPY_MEMORY(((drwav_uint8*)(*pWav->memoryStreamWrite.ppData)) + pWav->memoryStreamWrite.currentWritePos, pDataIn, bytesToWrite);

    pWav->memoryStreamWrite.currentWritePos += bytesToWrite;
    if (pWav->memoryStreamWrite.dataSize < pWav->memoryStreamWrite.currentWritePos) {
        pWav->memoryStreamWrite.dataSize = pWav->memoryStreamWrite.currentWritePos;
    }

    *pWav->memoryStreamWrite.pDataSize = pWav->memoryStreamWrite.dataSize;

    return bytesToWrite;
}

DRWAV_PRIVATE drwav_bool32 drwav__on_seek_memory_write(void* pUserData, int offset, drwav_seek_origin origin)
{
    drwav* pWav = (drwav*)pUserData;
    drwav_int64 newCursor;

    DRWAV_ASSERT(pWav != NULL);

    if (origin == DRWAV_SEEK_SET) {
        newCursor = 0;
    } else if (origin == DRWAV_SEEK_CUR) {
        newCursor = (drwav_int64)pWav->memoryStreamWrite.currentWritePos;
    } else if (origin == DRWAV_SEEK_END) {
        newCursor = (drwav_int64)pWav->memoryStreamWrite.dataSize;
    } else {
        DRWAV_ASSERT(!"Invalid seek origin");
        return DRWAV_FALSE;
    }

    newCursor += offset;

    if (newCursor < 0) {
        return DRWAV_FALSE;  
    }
    if ((size_t)newCursor > pWav->memoryStreamWrite.dataSize) {
        return DRWAV_FALSE;  
    }

    pWav->memoryStreamWrite.currentWritePos = (size_t)newCursor;

    return DRWAV_TRUE;
}

DRWAV_PRIVATE drwav_bool32 drwav__on_tell_memory(void* pUserData, drwav_int64* pCursor)
{
    drwav* pWav = (drwav*)pUserData;

    DRWAV_ASSERT(pWav != NULL);
    DRWAV_ASSERT(pCursor != NULL);

    *pCursor = (drwav_int64)pWav->memoryStream.currentReadPos;
    return DRWAV_TRUE;
}

DRWAV_API drwav_bool32 drwav_init_memory(drwav* pWav, const void* data, size_t dataSize, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_memory_ex(pWav, data, dataSize, NULL, NULL, 0, pAllocationCallbacks);
}

DRWAV_API drwav_bool32 drwav_init_memory_ex(drwav* pWav, const void* data, size_t dataSize, drwav_chunk_proc onChunk, void* pChunkUserData, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (data == NULL || dataSize == 0) {
        return DRWAV_FALSE;
    }

    if (!drwav_preinit(pWav, drwav__on_read_memory, drwav__on_seek_memory, drwav__on_tell_memory, pWav, pAllocationCallbacks)) {
        return DRWAV_FALSE;
    }

    pWav->memoryStream.data = (const drwav_uint8*)data;
    pWav->memoryStream.dataSize = dataSize;
    pWav->memoryStream.currentReadPos = 0;

    return drwav_init__internal(pWav, onChunk, pChunkUserData, flags);
}

DRWAV_API drwav_bool32 drwav_init_memory_with_metadata(drwav* pWav, const void* data, size_t dataSize, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (data == NULL || dataSize == 0) {
        return DRWAV_FALSE;
    }

    if (!drwav_preinit(pWav, drwav__on_read_memory, drwav__on_seek_memory, drwav__on_tell_memory, pWav, pAllocationCallbacks)) {
        return DRWAV_FALSE;
    }

    pWav->memoryStream.data = (const drwav_uint8*)data;
    pWav->memoryStream.dataSize = dataSize;
    pWav->memoryStream.currentReadPos = 0;

    return drwav_init__internal(pWav, NULL, NULL, flags | DRWAV_WITH_METADATA);
}


DRWAV_PRIVATE drwav_bool32 drwav_init_memory_write__internal(drwav* pWav, void** ppData, size_t* pDataSize, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, drwav_bool32 isSequential, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (ppData == NULL || pDataSize == NULL) {
        return DRWAV_FALSE;
    }

    *ppData = NULL; 
    *pDataSize = 0;

    if (!drwav_preinit_write(pWav, pFormat, isSequential, drwav__on_write_memory, drwav__on_seek_memory_write, pWav, pAllocationCallbacks)) {
        return DRWAV_FALSE;
    }

    pWav->memoryStreamWrite.ppData = ppData;
    pWav->memoryStreamWrite.pDataSize = pDataSize;
    pWav->memoryStreamWrite.dataSize = 0;
    pWav->memoryStreamWrite.dataCapacity = 0;
    pWav->memoryStreamWrite.currentWritePos = 0;

    return drwav_init_write__internal(pWav, pFormat, totalSampleCount);
}

DRWAV_API drwav_bool32 drwav_init_memory_write(drwav* pWav, void** ppData, size_t* pDataSize, const drwav_data_format* pFormat, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_memory_write__internal(pWav, ppData, pDataSize, pFormat, 0, DRWAV_FALSE, pAllocationCallbacks);
}

DRWAV_API drwav_bool32 drwav_init_memory_write_sequential(drwav* pWav, void** ppData, size_t* pDataSize, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_memory_write__internal(pWav, ppData, pDataSize, pFormat, totalSampleCount, DRWAV_TRUE, pAllocationCallbacks);
}

DRWAV_API drwav_bool32 drwav_init_memory_write_sequential_pcm_frames(drwav* pWav, void** ppData, size_t* pDataSize, const drwav_data_format* pFormat, drwav_uint64 totalPCMFrameCount, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pFormat == NULL) {
        return DRWAV_FALSE;
    }

    return drwav_init_memory_write_sequential(pWav, ppData, pDataSize, pFormat, totalPCMFrameCount*pFormat->channels, pAllocationCallbacks);
}

DRWAV_PRIVATE drwav_uint32 drwav_write_padding(drwav* pWav)
{
    
    drwav_uint32 paddingSize = drwav_calculate_padding_size(pWav->container, pWav->dataChunkDataSize);

    if (paddingSize > 0) {
        drwav_uint64 paddingData = 0;
        drwav__write(pWav, &paddingData, paddingSize);  
    }

    return paddingSize;
}

DRWAV_PRIVATE void drwav_write_chunk_sizes(drwav* pWav)
{
    



    if (pWav->onSeek != NULL && !pWav->isSequentialWrite) {
        if (pWav->container == drwav_container_riff) {
            
            if (pWav->onSeek(pWav->pUserData, 4, DRWAV_SEEK_SET)) {
                drwav_uint32 riffChunkSize = drwav__riff_chunk_size_riff(pWav->dataChunkDataSize, pWav->pMetadata, pWav->metadataCount);
                drwav__write_u32ne_to_le(pWav, riffChunkSize);
            }

            
            if (pWav->onSeek(pWav->pUserData, (int)pWav->dataChunkDataPos - 4, DRWAV_SEEK_SET)) {
                drwav_uint32 dataChunkSize = drwav__data_chunk_size_riff(pWav->dataChunkDataSize);
                drwav__write_u32ne_to_le(pWav, dataChunkSize);
            }
        } else if (pWav->container == drwav_container_w64) {
            
            if (pWav->onSeek(pWav->pUserData, 16, DRWAV_SEEK_SET)) {
                drwav_uint64 riffChunkSize = drwav__riff_chunk_size_w64(pWav->dataChunkDataSize);
                drwav__write_u64ne_to_le(pWav, riffChunkSize);
            }

            
            if (pWav->onSeek(pWav->pUserData, (int)pWav->dataChunkDataPos - 8, DRWAV_SEEK_SET)) {
                drwav_uint64 dataChunkSize = drwav__data_chunk_size_w64(pWav->dataChunkDataSize);
                drwav__write_u64ne_to_le(pWav, dataChunkSize);
            }
        } else if (pWav->container == drwav_container_rf64) {
            
            int ds64BodyPos = 12 + 8;

            
            if (pWav->onSeek(pWav->pUserData, ds64BodyPos + 0, DRWAV_SEEK_SET)) {
                drwav_uint64 riffChunkSize = drwav__riff_chunk_size_rf64(pWav->dataChunkDataSize, pWav->pMetadata, pWav->metadataCount);
                drwav__write_u64ne_to_le(pWav, riffChunkSize);
            }

            
            if (pWav->onSeek(pWav->pUserData, ds64BodyPos + 8, DRWAV_SEEK_SET)) {
                drwav_uint64 dataChunkSize = drwav__data_chunk_size_rf64(pWav->dataChunkDataSize);
                drwav__write_u64ne_to_le(pWav, dataChunkSize);
            }
        }
    }
}

DRWAV_API drwav_result drwav_uninit(drwav* pWav)
{
    drwav_result result = DRWAV_SUCCESS;

    if (pWav == NULL) {
        return DRWAV_INVALID_ARGS;
    }

    if (pWav->onWrite != NULL) {
        if (pWav->isSequentialWrite) {
            



            drwav_write_padding(pWav);

            
            if (pWav->dataChunkDataSize != pWav->dataChunkDataSizeTargetWrite) {
                result = DRWAV_INVALID_FILE;
            }
        }
    } else {
        drwav_free(pWav->pMetadata, &pWav->allocationCallbacks);
    }

#ifndef DR_WAV_NO_STDIO
    



    if (pWav->onRead == drwav__on_read_stdio || pWav->onWrite == drwav__on_write_stdio) {
        fclose((FILE*)pWav->pUserData);
    }
#endif

    return result;
}



DRWAV_API size_t drwav_read_raw(drwav* pWav, size_t bytesToRead, void* pBufferOut)
{
    size_t bytesRead;
    drwav_uint32 bytesPerFrame;

    if (pWav == NULL || bytesToRead == 0) {
        return 0;   
    }

    if (bytesToRead > pWav->bytesRemaining) {
        bytesToRead = (size_t)pWav->bytesRemaining;
    }

    if (bytesToRead == 0) {
        return 0;   
    }

    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;   
    }

    if (pBufferOut != NULL) {
        bytesRead = pWav->onRead(pWav->pUserData, pBufferOut, bytesToRead);
    } else {
        
        bytesRead = 0;
        while (bytesRead < bytesToRead) {
            size_t bytesToSeek = (bytesToRead - bytesRead);
            if (bytesToSeek > 0x7FFFFFFF) {
                bytesToSeek = 0x7FFFFFFF;
            }

            if (pWav->onSeek(pWav->pUserData, (int)bytesToSeek, DRWAV_SEEK_CUR) == DRWAV_FALSE) {
                break;
            }

            bytesRead += bytesToSeek;
        }

        
        while (bytesRead < bytesToRead) {
            drwav_uint8 buffer[4096];
            size_t bytesSeeked;
            size_t bytesToSeek = (bytesToRead - bytesRead);
            if (bytesToSeek > sizeof(buffer)) {
                bytesToSeek = sizeof(buffer);
            }

            bytesSeeked = pWav->onRead(pWav->pUserData, buffer, bytesToSeek);
            bytesRead += bytesSeeked;

            if (bytesSeeked < bytesToSeek) {
                break;  
            }
        }
    }

    pWav->bytesRemaining -= bytesRead;
    pWav->readCursorInPCMFrames = (pWav->dataChunkDataSize - pWav->bytesRemaining) / bytesPerFrame;

    return bytesRead;
}



DRWAV_API drwav_uint64 drwav_read_pcm_frames_le(drwav* pWav, drwav_uint64 framesToRead, void* pBufferOut)
{
    drwav_uint32 bytesPerFrame;
    drwav_uint64 bytesToRead;   
    drwav_uint64 framesRemainingInFile;

    if (pWav == NULL || framesToRead == 0) {
        return 0;
    }

    
    if (drwav__is_compressed_format_tag(pWav->translatedFormatTag)) {
        return 0;
    }

    framesRemainingInFile = pWav->totalPCMFrameCount - pWav->readCursorInPCMFrames;
    if (framesToRead > framesRemainingInFile) {
        framesToRead = framesRemainingInFile;
    }

    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    
    bytesToRead = framesToRead * bytesPerFrame;
    if (bytesToRead > DRWAV_SIZE_MAX) {
        bytesToRead = (DRWAV_SIZE_MAX / bytesPerFrame) * bytesPerFrame; 
    }

    



    if (bytesToRead == 0) {
        return 0;
    }

    return drwav_read_raw(pWav, (size_t)bytesToRead, pBufferOut) / bytesPerFrame;
}

DRWAV_API drwav_uint64 drwav_read_pcm_frames_be(drwav* pWav, drwav_uint64 framesToRead, void* pBufferOut)
{
    drwav_uint64 framesRead = drwav_read_pcm_frames_le(pWav, framesToRead, pBufferOut);

    if (pBufferOut != NULL) {
        drwav_uint32 bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
        if (bytesPerFrame == 0) {
            return 0;   
        }

        drwav__bswap_samples(pBufferOut, framesRead*pWav->channels, bytesPerFrame/pWav->channels);
    }

    return framesRead;
}

DRWAV_API drwav_uint64 drwav_read_pcm_frames(drwav* pWav, drwav_uint64 framesToRead, void* pBufferOut)
{
    drwav_uint64 framesRead = 0;

    if (drwav_is_container_be(pWav->container)) {
        




        if (pWav->container != drwav_container_aiff || pWav->aiff.isLE == DRWAV_FALSE) {
            if (drwav__is_little_endian()) {
                framesRead = drwav_read_pcm_frames_be(pWav, framesToRead, pBufferOut);
            } else {
                framesRead = drwav_read_pcm_frames_le(pWav, framesToRead, pBufferOut);
            }

            goto post_process;
        }
    }

    
    if (drwav__is_little_endian()) {
        framesRead = drwav_read_pcm_frames_le(pWav, framesToRead, pBufferOut);
    } else {
        framesRead = drwav_read_pcm_frames_be(pWav, framesToRead, pBufferOut);
    }

    



    post_process:
    {
        if (pWav->container == drwav_container_aiff && pWav->translatedFormatTag == DR_WAVE_FORMAT_PCM && pWav->bitsPerSample == 8 && pWav->aiff.isUnsigned == DRWAV_FALSE) {
            if (pBufferOut != NULL) {
                drwav_uint64 iSample;

                for (iSample = 0; iSample < framesRead * pWav->channels; iSample += 1) {
                    ((drwav_uint8*)pBufferOut)[iSample] += 128;
                }
            }
        }
    }

    return framesRead;
}



DRWAV_PRIVATE drwav_bool32 drwav_seek_to_first_pcm_frame(drwav* pWav)
{
    if (pWav->onWrite != NULL) {
        return DRWAV_FALSE; 
    }

    if (!pWav->onSeek(pWav->pUserData, (int)pWav->dataChunkDataPos, DRWAV_SEEK_SET)) {
        return DRWAV_FALSE;
    }

    if (drwav__is_compressed_format_tag(pWav->translatedFormatTag)) {
        
        if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ADPCM) {
            DRWAV_ZERO_OBJECT(&pWav->msadpcm);
        } else if (pWav->translatedFormatTag == DR_WAVE_FORMAT_DVI_ADPCM) {
            DRWAV_ZERO_OBJECT(&pWav->ima);
        } else {
            DRWAV_ASSERT(DRWAV_FALSE);  
        }
    }

    pWav->readCursorInPCMFrames = 0;
    pWav->bytesRemaining = pWav->dataChunkDataSize;

    return DRWAV_TRUE;
}

DRWAV_API drwav_bool32 drwav_seek_to_pcm_frame(drwav* pWav, drwav_uint64 targetFrameIndex)
{
    

    if (pWav == NULL || pWav->onSeek == NULL) {
        return DRWAV_FALSE;
    }

    
    if (pWav->onWrite != NULL) {
        return DRWAV_FALSE;
    }

    
    if (pWav->totalPCMFrameCount == 0) {
        return DRWAV_TRUE;
    }

    
    if (targetFrameIndex > pWav->totalPCMFrameCount) {
        targetFrameIndex = pWav->totalPCMFrameCount;
    }

    



    if (drwav__is_compressed_format_tag(pWav->translatedFormatTag)) {
        

        



        if (targetFrameIndex < pWav->readCursorInPCMFrames) {
            if (!drwav_seek_to_first_pcm_frame(pWav)) {
                return DRWAV_FALSE;
            }
        }

        if (targetFrameIndex > pWav->readCursorInPCMFrames) {
            drwav_uint64 offsetInFrames = targetFrameIndex - pWav->readCursorInPCMFrames;

            drwav_int16 devnull[2048];
            while (offsetInFrames > 0) {
                drwav_uint64 framesRead = 0;
                drwav_uint64 framesToRead = offsetInFrames;
                if (framesToRead > drwav_countof(devnull)/pWav->channels) {
                    framesToRead = drwav_countof(devnull)/pWav->channels;
                }

                if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ADPCM) {
                    framesRead = drwav_read_pcm_frames_s16__msadpcm(pWav, framesToRead, devnull);
                } else if (pWav->translatedFormatTag == DR_WAVE_FORMAT_DVI_ADPCM) {
                    framesRead = drwav_read_pcm_frames_s16__ima(pWav, framesToRead, devnull);
                } else {
                    DRWAV_ASSERT(DRWAV_FALSE);  
                }

                if (framesRead != framesToRead) {
                    return DRWAV_FALSE;
                }

                offsetInFrames -= framesRead;
            }
        }
    } else {
        drwav_uint64 currentBytePos;
        drwav_uint64 targetBytePos;
        drwav_uint64 offset;
        drwav_uint32 bytesPerFrame;

        bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
        if (bytesPerFrame == 0) {
            return DRWAV_FALSE; 
        }

        currentBytePos = pWav->dataChunkDataSize - pWav->bytesRemaining;
        targetBytePos  = targetFrameIndex * bytesPerFrame;

        pWav->readCursorInPCMFrames = currentBytePos / bytesPerFrame;

        if (currentBytePos < targetBytePos) {
            
            offset = (targetBytePos - currentBytePos);
        } else {
            
            if (!drwav_seek_to_first_pcm_frame(pWav)) {
                return DRWAV_FALSE;
            }
            offset = targetBytePos;
        }

        while (offset > 0) {
            int offset32 = ((offset > INT_MAX) ? INT_MAX : (int)offset);
            if (!pWav->onSeek(pWav->pUserData, offset32, DRWAV_SEEK_CUR)) {
                return DRWAV_FALSE;
            }

            pWav->bytesRemaining        -= offset32;
            pWav->readCursorInPCMFrames  = (pWav->dataChunkDataSize - pWav->bytesRemaining) / bytesPerFrame;
            offset                      -= offset32;
        }
    }

    return DRWAV_TRUE;
}

DRWAV_API drwav_result drwav_get_cursor_in_pcm_frames(drwav* pWav, drwav_uint64* pCursor)
{
    if (pCursor == NULL) {
        return DRWAV_INVALID_ARGS;
    }

    *pCursor = 0;   

    if (pWav == NULL) {
        return DRWAV_INVALID_ARGS;
    }

    *pCursor = pWav->readCursorInPCMFrames;

    return DRWAV_SUCCESS;
}

DRWAV_API drwav_result drwav_get_length_in_pcm_frames(drwav* pWav, drwav_uint64* pLength)
{
    if (pLength == NULL) {
        return DRWAV_INVALID_ARGS;
    }

    *pLength = 0;   

    if (pWav == NULL) {
        return DRWAV_INVALID_ARGS;
    }

    *pLength = pWav->totalPCMFrameCount;

    return DRWAV_SUCCESS;
}


DRWAV_API size_t drwav_write_raw(drwav* pWav, size_t bytesToWrite, const void* pData)
{
    size_t bytesWritten;

    if (pWav == NULL || bytesToWrite == 0 || pData == NULL) {
        return 0;
    }

    bytesWritten = pWav->onWrite(pWav->pUserData, pData, bytesToWrite);
    pWav->dataChunkDataSize += bytesWritten;

    if (!pWav->isSequentialWrite) {
        drwav_uint32 padding;

        
        padding = drwav_write_padding(pWav);

        
        drwav_write_chunk_sizes(pWav);

        
        if (pWav->onSeek != NULL) {
            pWav->onSeek(pWav->pUserData, -(int)padding, DRWAV_SEEK_END);   
        }
    }

    return bytesWritten;
}

DRWAV_API drwav_uint64 drwav_write_pcm_frames_le(drwav* pWav, drwav_uint64 framesToWrite, const void* pData)
{
    drwav_uint64 bytesToWrite;
    drwav_uint64 bytesWritten;
    const drwav_uint8* pRunningData;

    if (pWav == NULL || framesToWrite == 0 || pData == NULL) {
        return 0;
    }

    bytesToWrite = ((framesToWrite * pWav->channels * pWav->bitsPerSample) / 8);
    if (bytesToWrite > DRWAV_SIZE_MAX) {
        return 0;
    }

    bytesWritten = 0;
    pRunningData = (const drwav_uint8*)pData;

    while (bytesToWrite > 0) {
        size_t bytesJustWritten;
        drwav_uint64 bytesToWriteThisIteration;

        bytesToWriteThisIteration = bytesToWrite;
        DRWAV_ASSERT(bytesToWriteThisIteration <= DRWAV_SIZE_MAX);  

        bytesJustWritten = drwav_write_raw(pWav, (size_t)bytesToWriteThisIteration, pRunningData);
        if (bytesJustWritten == 0) {
            break;
        }

        bytesToWrite -= bytesJustWritten;
        bytesWritten += bytesJustWritten;
        pRunningData += bytesJustWritten;
    }

    return (bytesWritten * 8) / pWav->bitsPerSample / pWav->channels;
}

DRWAV_API drwav_uint64 drwav_write_pcm_frames_be(drwav* pWav, drwav_uint64 framesToWrite, const void* pData)
{
    drwav_uint64 bytesToWrite;
    drwav_uint64 bytesWritten;
    drwav_uint32 bytesPerSample;
    const drwav_uint8* pRunningData;

    if (pWav == NULL || framesToWrite == 0 || pData == NULL) {
        return 0;
    }

    bytesToWrite = ((framesToWrite * pWav->channels * pWav->bitsPerSample) / 8);
    if (bytesToWrite > DRWAV_SIZE_MAX) {
        return 0;
    }

    bytesWritten = 0;
    pRunningData = (const drwav_uint8*)pData;

    bytesPerSample = drwav_get_bytes_per_pcm_frame(pWav) / pWav->channels;
    if (bytesPerSample == 0) {
        return 0;   
    }

    while (bytesToWrite > 0) {
        drwav_uint8 temp[4096];
        drwav_uint32 sampleCount;
        size_t bytesJustWritten;
        drwav_uint64 bytesToWriteThisIteration;

        bytesToWriteThisIteration = bytesToWrite;
        DRWAV_ASSERT(bytesToWriteThisIteration <= DRWAV_SIZE_MAX);  

        



        sampleCount = sizeof(temp)/bytesPerSample;

        if (bytesToWriteThisIteration > ((drwav_uint64)sampleCount)*bytesPerSample) {
            bytesToWriteThisIteration = ((drwav_uint64)sampleCount)*bytesPerSample;
        }

        DRWAV_COPY_MEMORY(temp, pRunningData, (size_t)bytesToWriteThisIteration);
        drwav__bswap_samples(temp, sampleCount, bytesPerSample);

        bytesJustWritten = drwav_write_raw(pWav, (size_t)bytesToWriteThisIteration, temp);
        if (bytesJustWritten == 0) {
            break;
        }

        bytesToWrite -= bytesJustWritten;
        bytesWritten += bytesJustWritten;
        pRunningData += bytesJustWritten;
    }

    return (bytesWritten * 8) / pWav->bitsPerSample / pWav->channels;
}

DRWAV_API drwav_uint64 drwav_write_pcm_frames(drwav* pWav, drwav_uint64 framesToWrite, const void* pData)
{
    if (drwav__is_little_endian()) {
        return drwav_write_pcm_frames_le(pWav, framesToWrite, pData);
    } else {
        return drwav_write_pcm_frames_be(pWav, framesToWrite, pData);
    }
}


DRWAV_PRIVATE DRWAV_INLINE drwav_int32 drwav__msadpcm_calc_sample(drwav_int32 prevSample0, drwav_int32 prevSample1, drwav_int32 coeff1, drwav_int32 coeff2, drwav_int32 nibble, drwav_int32 delta)
{
    drwav_int32 prediction;

    prediction = ((prevSample1 * coeff1) + (prevSample0 * coeff2)) >> 8;

    
    if (delta > (0x7FFFFFFF - 98304) / 8) {
        return (drwav_int32)drwav_clamp(prediction + ((drwav_int64)nibble * delta), -32768, 32767);
    } else {
        return drwav_clamp(prediction + (nibble * delta), -32768, 32767);
    }
}

DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_s16__msadpcm(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    drwav_uint64 totalFramesRead = 0;

    static const drwav_int32 adaptationTable[] = {
        230, 230, 230, 230, 307, 409, 512, 614,
        768, 614, 512, 409, 307, 230, 230, 230
    };
    static const drwav_int32 coeff1Table[] = { 256, 512, 0, 192, 240, 460,  392 };
    static const drwav_int32 coeff2Table[] = { 0,  -256, 0, 64,  0,  -208, -232 };

    DRWAV_ASSERT(pWav != NULL);
    DRWAV_ASSERT(framesToRead > 0);

    

    while (pWav->readCursorInPCMFrames < pWav->totalPCMFrameCount) {
        DRWAV_ASSERT(framesToRead > 0); 

        
        if (pWav->msadpcm.cachedFrameCount == 0 && pWav->msadpcm.bytesRemainingInBlock == 0) {
            if (pWav->channels == 1) {
                
                drwav_uint8 header[7];
                if (pWav->onRead(pWav->pUserData, header, sizeof(header)) != sizeof(header)) {
                    return totalFramesRead;
                }
                pWav->msadpcm.bytesRemainingInBlock = pWav->fmt.blockAlign - sizeof(header);

                pWav->msadpcm.predictor[0]     = header[0];
                pWav->msadpcm.delta[0]         = drwav_bytes_to_s16(header + 1);
                pWav->msadpcm.prevFrames[0][1] = (drwav_int32)drwav_bytes_to_s16(header + 3);
                pWav->msadpcm.prevFrames[0][0] = (drwav_int32)drwav_bytes_to_s16(header + 5);
                pWav->msadpcm.cachedFrames[2]  = pWav->msadpcm.prevFrames[0][0];
                pWav->msadpcm.cachedFrames[3]  = pWav->msadpcm.prevFrames[0][1];
                pWav->msadpcm.cachedFrameCount = 2;

                
                if (pWav->msadpcm.predictor[0] >= drwav_countof(coeff1Table) || pWav->msadpcm.predictor[0] >= drwav_countof(coeff2Table)) {
                    return totalFramesRead; 
                }
            } else {
                
                drwav_uint8 header[14];
                if (pWav->onRead(pWav->pUserData, header, sizeof(header)) != sizeof(header)) {
                    return totalFramesRead;
                }
                pWav->msadpcm.bytesRemainingInBlock = pWav->fmt.blockAlign - sizeof(header);

                pWav->msadpcm.predictor[0] = header[0];
                pWav->msadpcm.predictor[1] = header[1];
                pWav->msadpcm.delta[0] = drwav_bytes_to_s16(header + 2);
                pWav->msadpcm.delta[1] = drwav_bytes_to_s16(header + 4);
                pWav->msadpcm.prevFrames[0][1] = (drwav_int32)drwav_bytes_to_s16(header + 6);
                pWav->msadpcm.prevFrames[1][1] = (drwav_int32)drwav_bytes_to_s16(header + 8);
                pWav->msadpcm.prevFrames[0][0] = (drwav_int32)drwav_bytes_to_s16(header + 10);
                pWav->msadpcm.prevFrames[1][0] = (drwav_int32)drwav_bytes_to_s16(header + 12);

                pWav->msadpcm.cachedFrames[0] = pWav->msadpcm.prevFrames[0][0];
                pWav->msadpcm.cachedFrames[1] = pWav->msadpcm.prevFrames[1][0];
                pWav->msadpcm.cachedFrames[2] = pWav->msadpcm.prevFrames[0][1];
                pWav->msadpcm.cachedFrames[3] = pWav->msadpcm.prevFrames[1][1];
                pWav->msadpcm.cachedFrameCount = 2;

                
                if (pWav->msadpcm.predictor[0] >= drwav_countof(coeff1Table) || pWav->msadpcm.predictor[0] >= drwav_countof(coeff2Table) ||
                    pWav->msadpcm.predictor[1] >= drwav_countof(coeff1Table) || pWav->msadpcm.predictor[1] >= drwav_countof(coeff2Table)) {
                    return totalFramesRead; 
                }
            }
        }

        
        while (framesToRead > 0 && pWav->msadpcm.cachedFrameCount > 0 && pWav->readCursorInPCMFrames < pWav->totalPCMFrameCount) {
            if (pBufferOut != NULL) {
                drwav_uint32 iSample = 0;
                for (iSample = 0; iSample < pWav->channels; iSample += 1) {
                    pBufferOut[iSample] = (drwav_int16)pWav->msadpcm.cachedFrames[(drwav_countof(pWav->msadpcm.cachedFrames) - (pWav->msadpcm.cachedFrameCount*pWav->channels)) + iSample];
                }

                pBufferOut += pWav->channels;
            }

            framesToRead    -= 1;
            totalFramesRead += 1;
            pWav->readCursorInPCMFrames += 1;
            pWav->msadpcm.cachedFrameCount -= 1;
        }

        if (framesToRead == 0) {
            break;
        }


        



        if (pWav->msadpcm.cachedFrameCount == 0) {
            if (pWav->msadpcm.bytesRemainingInBlock == 0) {
                continue;
            } else {
                drwav_uint8 nibbles;
                drwav_int32 nibble0;
                drwav_int32 nibble1;

                if (pWav->onRead(pWav->pUserData, &nibbles, 1) != 1) {
                    return totalFramesRead;
                }
                pWav->msadpcm.bytesRemainingInBlock -= 1;

                
                nibble0 = ((nibbles & 0xF0) >> 4); if ((nibbles & 0x80)) { nibble0 |= 0xFFFFFFF0UL; }
                nibble1 = ((nibbles & 0x0F) >> 0); if ((nibbles & 0x08)) { nibble1 |= 0xFFFFFFF0UL; }

                if (pWav->channels == 1) {
                    
                    drwav_int32 newSample0;
                    drwav_int32 newSample1;

                    
                    if (pWav->msadpcm.predictor[0] >= drwav_countof(coeff1Table) || pWav->msadpcm.predictor[0] >= drwav_countof(coeff2Table)) {
                        return totalFramesRead;
                    }

                    newSample0 = drwav__msadpcm_calc_sample(pWav->msadpcm.prevFrames[0][0], pWav->msadpcm.prevFrames[0][1], coeff1Table[pWav->msadpcm.predictor[0]], coeff2Table[pWav->msadpcm.predictor[0]], nibble0, pWav->msadpcm.delta[0]);

                    pWav->msadpcm.delta[0] = (drwav_int32)drwav_clamp(((drwav_int64)adaptationTable[((nibbles & 0xF0) >> 4)] * pWav->msadpcm.delta[0]) >> 8, 16, 0x7FFFFFFF);
 
                    pWav->msadpcm.prevFrames[0][0] = pWav->msadpcm.prevFrames[0][1];
                    pWav->msadpcm.prevFrames[0][1] = newSample0;


                    newSample1 = drwav__msadpcm_calc_sample(pWav->msadpcm.prevFrames[0][0], pWav->msadpcm.prevFrames[0][1], coeff1Table[pWav->msadpcm.predictor[0]], coeff2Table[pWav->msadpcm.predictor[0]], nibble1, pWav->msadpcm.delta[0]);

                    pWav->msadpcm.delta[0] = (drwav_int32)drwav_clamp(((drwav_int64)adaptationTable[((nibbles & 0x0F) >> 0)] * pWav->msadpcm.delta[0]) >> 8, 16, 0x7FFFFFFF);

                    pWav->msadpcm.prevFrames[0][0] = pWav->msadpcm.prevFrames[0][1];
                    pWav->msadpcm.prevFrames[0][1] = newSample1;

                    pWav->msadpcm.cachedFrames[2] = newSample0;
                    pWav->msadpcm.cachedFrames[3] = newSample1;
                    pWav->msadpcm.cachedFrameCount = 2;
                } else {
                    
                    drwav_int32 newSample0;
                    drwav_int32 newSample1;

                    
                    if (pWav->msadpcm.predictor[0] >= drwav_countof(coeff1Table) || pWav->msadpcm.predictor[0] >= drwav_countof(coeff2Table)) {
                        return totalFramesRead; 
                    }

                    newSample0 = drwav__msadpcm_calc_sample(pWav->msadpcm.prevFrames[0][0], pWav->msadpcm.prevFrames[0][1], coeff1Table[pWav->msadpcm.predictor[0]], coeff2Table[pWav->msadpcm.predictor[0]], nibble0, pWav->msadpcm.delta[0]);

                    pWav->msadpcm.delta[0] = (drwav_int32)drwav_clamp(((drwav_int64)adaptationTable[((nibbles & 0xF0) >> 4)] * pWav->msadpcm.delta[0]) >> 8, 16, 0x7FFFFFFF);

                    pWav->msadpcm.prevFrames[0][0] = pWav->msadpcm.prevFrames[0][1];
                    pWav->msadpcm.prevFrames[0][1] = newSample0;


                    
                    if (pWav->msadpcm.predictor[1] >= drwav_countof(coeff1Table) || pWav->msadpcm.predictor[1] >= drwav_countof(coeff2Table)) {
                        return totalFramesRead; 
                    }

                    newSample1 = drwav__msadpcm_calc_sample(pWav->msadpcm.prevFrames[1][0], pWav->msadpcm.prevFrames[1][1], coeff1Table[pWav->msadpcm.predictor[1]], coeff2Table[pWav->msadpcm.predictor[1]], nibble1, pWav->msadpcm.delta[1]);

                    pWav->msadpcm.delta[1] = (drwav_int32)drwav_clamp(((drwav_int64)adaptationTable[((nibbles & 0x0F) >> 0)] * pWav->msadpcm.delta[1]) >> 8, 16, 0x7FFFFFFF);

                    pWav->msadpcm.prevFrames[1][0] = pWav->msadpcm.prevFrames[1][1];
                    pWav->msadpcm.prevFrames[1][1] = newSample1;

                    pWav->msadpcm.cachedFrames[2] = newSample0;
                    pWav->msadpcm.cachedFrames[3] = newSample1;
                    pWav->msadpcm.cachedFrameCount = 1;
                }
            }
        }
    }

    return totalFramesRead;
}


DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_s16__ima(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    drwav_uint64 totalFramesRead = 0;
    drwav_uint32 iChannel;

    static const drwav_int32 indexTable[16] = {
        -1, -1, -1, -1, 2, 4, 6, 8,
        -1, -1, -1, -1, 2, 4, 6, 8
    };

    static const drwav_int32 stepTable[89] = {
        7,     8,     9,     10,    11,    12,    13,    14,    16,    17,
        19,    21,    23,    25,    28,    31,    34,    37,    41,    45,
        50,    55,    60,    66,    73,    80,    88,    97,    107,   118,
        130,   143,   157,   173,   190,   209,   230,   253,   279,   307,
        337,   371,   408,   449,   494,   544,   598,   658,   724,   796,
        876,   963,   1060,  1166,  1282,  1411,  1552,  1707,  1878,  2066,
        2272,  2499,  2749,  3024,  3327,  3660,  4026,  4428,  4871,  5358,
        5894,  6484,  7132,  7845,  8630,  9493,  10442, 11487, 12635, 13899,
        15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
    };

    DRWAV_ASSERT(pWav != NULL);
    DRWAV_ASSERT(framesToRead > 0);

    

    while (pWav->readCursorInPCMFrames < pWav->totalPCMFrameCount) {
        DRWAV_ASSERT(framesToRead > 0); 

        
        if (pWav->ima.cachedFrameCount == 0 && pWav->ima.bytesRemainingInBlock == 0) {
            if (pWav->channels == 1) {
                
                drwav_uint8 header[4];
                if (pWav->onRead(pWav->pUserData, header, sizeof(header)) != sizeof(header)) {
                    return totalFramesRead;
                }
                pWav->ima.bytesRemainingInBlock = pWav->fmt.blockAlign - sizeof(header);

                if (header[2] >= drwav_countof(stepTable)) {
                    pWav->onSeek(pWav->pUserData, pWav->ima.bytesRemainingInBlock, DRWAV_SEEK_CUR);
                    pWav->ima.bytesRemainingInBlock = 0;
                    return totalFramesRead; 
                }

                pWav->ima.predictor[0] = (drwav_int16)drwav_bytes_to_u16(header + 0);
                pWav->ima.stepIndex[0] = drwav_clamp(header[2], 0, (drwav_int32)drwav_countof(stepTable)-1);    
                pWav->ima.cachedFrames[drwav_countof(pWav->ima.cachedFrames) - 1] = pWav->ima.predictor[0];
                pWav->ima.cachedFrameCount = 1;
            } else {
                
                drwav_uint8 header[8];
                if (pWav->onRead(pWav->pUserData, header, sizeof(header)) != sizeof(header)) {
                    return totalFramesRead;
                }
                pWav->ima.bytesRemainingInBlock = pWav->fmt.blockAlign - sizeof(header);

                if (header[2] >= drwav_countof(stepTable) || header[6] >= drwav_countof(stepTable)) {
                    pWav->onSeek(pWav->pUserData, pWav->ima.bytesRemainingInBlock, DRWAV_SEEK_CUR);
                    pWav->ima.bytesRemainingInBlock = 0;
                    return totalFramesRead; 
                }

                pWav->ima.predictor[0] = drwav_bytes_to_s16(header + 0);
                pWav->ima.stepIndex[0] = drwav_clamp(header[2], 0, (drwav_int32)drwav_countof(stepTable)-1);    
                pWav->ima.predictor[1] = drwav_bytes_to_s16(header + 4);
                pWav->ima.stepIndex[1] = drwav_clamp(header[6], 0, (drwav_int32)drwav_countof(stepTable)-1);    

                pWav->ima.cachedFrames[drwav_countof(pWav->ima.cachedFrames) - 2] = pWav->ima.predictor[0];
                pWav->ima.cachedFrames[drwav_countof(pWav->ima.cachedFrames) - 1] = pWav->ima.predictor[1];
                pWav->ima.cachedFrameCount = 1;
            }
        }

        
        while (framesToRead > 0 && pWav->ima.cachedFrameCount > 0 && pWav->readCursorInPCMFrames < pWav->totalPCMFrameCount) {
            if (pBufferOut != NULL) {
                drwav_uint32 iSample;
                for (iSample = 0; iSample < pWav->channels; iSample += 1) {
                    pBufferOut[iSample] = (drwav_int16)pWav->ima.cachedFrames[(drwav_countof(pWav->ima.cachedFrames) - (pWav->ima.cachedFrameCount*pWav->channels)) + iSample];
                }
                pBufferOut += pWav->channels;
            }

            framesToRead    -= 1;
            totalFramesRead += 1;
            pWav->readCursorInPCMFrames += 1;
            pWav->ima.cachedFrameCount -= 1;
        }

        if (framesToRead == 0) {
            break;
        }

        



        if (pWav->ima.cachedFrameCount == 0) {
            if (pWav->ima.bytesRemainingInBlock == 0) {
                continue;
            } else {
                



                pWav->ima.cachedFrameCount = 8;
                for (iChannel = 0; iChannel < pWav->channels; ++iChannel) {
                    drwav_uint32 iByte;
                    drwav_uint8 nibbles[4];
                    if (pWav->onRead(pWav->pUserData, &nibbles, 4) != 4) {
                        pWav->ima.cachedFrameCount = 0;
                        return totalFramesRead;
                    }
                    pWav->ima.bytesRemainingInBlock -= 4;

                    for (iByte = 0; iByte < 4; ++iByte) {
                        drwav_uint8 nibble0 = ((nibbles[iByte] & 0x0F) >> 0);
                        drwav_uint8 nibble1 = ((nibbles[iByte] & 0xF0) >> 4);

                        drwav_int32 step      = stepTable[pWav->ima.stepIndex[iChannel]];
                        drwav_int32 predictor = pWav->ima.predictor[iChannel];

                        drwav_int32      diff  = step >> 3;
                        if (nibble0 & 1) diff += step >> 2;
                        if (nibble0 & 2) diff += step >> 1;
                        if (nibble0 & 4) diff += step;
                        if (nibble0 & 8) diff  = -diff;

                        predictor = drwav_clamp(predictor + diff, -32768, 32767);
                        pWav->ima.predictor[iChannel] = predictor;
                        pWav->ima.stepIndex[iChannel] = drwav_clamp(pWav->ima.stepIndex[iChannel] + indexTable[nibble0], 0, (drwav_int32)drwav_countof(stepTable)-1);
                        pWav->ima.cachedFrames[(drwav_countof(pWav->ima.cachedFrames) - (pWav->ima.cachedFrameCount*pWav->channels)) + (iByte*2+0)*pWav->channels + iChannel] = predictor;


                        step      = stepTable[pWav->ima.stepIndex[iChannel]];
                        predictor = pWav->ima.predictor[iChannel];

                                         diff  = step >> 3;
                        if (nibble1 & 1) diff += step >> 2;
                        if (nibble1 & 2) diff += step >> 1;
                        if (nibble1 & 4) diff += step;
                        if (nibble1 & 8) diff  = -diff;

                        predictor = drwav_clamp(predictor + diff, -32768, 32767);
                        pWav->ima.predictor[iChannel] = predictor;
                        pWav->ima.stepIndex[iChannel] = drwav_clamp(pWav->ima.stepIndex[iChannel] + indexTable[nibble1], 0, (drwav_int32)drwav_countof(stepTable)-1);
                        pWav->ima.cachedFrames[(drwav_countof(pWav->ima.cachedFrames) - (pWav->ima.cachedFrameCount*pWav->channels)) + (iByte*2+1)*pWav->channels + iChannel] = predictor;
                    }
                }
            }
        }
    }

    return totalFramesRead;
}


#ifndef DR_WAV_NO_CONVERSION_API
static const unsigned short g_drwavAlawTable[256] = {
    0xEA80, 0xEB80, 0xE880, 0xE980, 0xEE80, 0xEF80, 0xEC80, 0xED80, 0xE280, 0xE380, 0xE080, 0xE180, 0xE680, 0xE780, 0xE480, 0xE580,
    0xF540, 0xF5C0, 0xF440, 0xF4C0, 0xF740, 0xF7C0, 0xF640, 0xF6C0, 0xF140, 0xF1C0, 0xF040, 0xF0C0, 0xF340, 0xF3C0, 0xF240, 0xF2C0,
    0xAA00, 0xAE00, 0xA200, 0xA600, 0xBA00, 0xBE00, 0xB200, 0xB600, 0x8A00, 0x8E00, 0x8200, 0x8600, 0x9A00, 0x9E00, 0x9200, 0x9600,
    0xD500, 0xD700, 0xD100, 0xD300, 0xDD00, 0xDF00, 0xD900, 0xDB00, 0xC500, 0xC700, 0xC100, 0xC300, 0xCD00, 0xCF00, 0xC900, 0xCB00,
    0xFEA8, 0xFEB8, 0xFE88, 0xFE98, 0xFEE8, 0xFEF8, 0xFEC8, 0xFED8, 0xFE28, 0xFE38, 0xFE08, 0xFE18, 0xFE68, 0xFE78, 0xFE48, 0xFE58,
    0xFFA8, 0xFFB8, 0xFF88, 0xFF98, 0xFFE8, 0xFFF8, 0xFFC8, 0xFFD8, 0xFF28, 0xFF38, 0xFF08, 0xFF18, 0xFF68, 0xFF78, 0xFF48, 0xFF58,
    0xFAA0, 0xFAE0, 0xFA20, 0xFA60, 0xFBA0, 0xFBE0, 0xFB20, 0xFB60, 0xF8A0, 0xF8E0, 0xF820, 0xF860, 0xF9A0, 0xF9E0, 0xF920, 0xF960,
    0xFD50, 0xFD70, 0xFD10, 0xFD30, 0xFDD0, 0xFDF0, 0xFD90, 0xFDB0, 0xFC50, 0xFC70, 0xFC10, 0xFC30, 0xFCD0, 0xFCF0, 0xFC90, 0xFCB0,
    0x1580, 0x1480, 0x1780, 0x1680, 0x1180, 0x1080, 0x1380, 0x1280, 0x1D80, 0x1C80, 0x1F80, 0x1E80, 0x1980, 0x1880, 0x1B80, 0x1A80,
    0x0AC0, 0x0A40, 0x0BC0, 0x0B40, 0x08C0, 0x0840, 0x09C0, 0x0940, 0x0EC0, 0x0E40, 0x0FC0, 0x0F40, 0x0CC0, 0x0C40, 0x0DC0, 0x0D40,
    0x5600, 0x5200, 0x5E00, 0x5A00, 0x4600, 0x4200, 0x4E00, 0x4A00, 0x7600, 0x7200, 0x7E00, 0x7A00, 0x6600, 0x6200, 0x6E00, 0x6A00,
    0x2B00, 0x2900, 0x2F00, 0x2D00, 0x2300, 0x2100, 0x2700, 0x2500, 0x3B00, 0x3900, 0x3F00, 0x3D00, 0x3300, 0x3100, 0x3700, 0x3500,
    0x0158, 0x0148, 0x0178, 0x0168, 0x0118, 0x0108, 0x0138, 0x0128, 0x01D8, 0x01C8, 0x01F8, 0x01E8, 0x0198, 0x0188, 0x01B8, 0x01A8,
    0x0058, 0x0048, 0x0078, 0x0068, 0x0018, 0x0008, 0x0038, 0x0028, 0x00D8, 0x00C8, 0x00F8, 0x00E8, 0x0098, 0x0088, 0x00B8, 0x00A8,
    0x0560, 0x0520, 0x05E0, 0x05A0, 0x0460, 0x0420, 0x04E0, 0x04A0, 0x0760, 0x0720, 0x07E0, 0x07A0, 0x0660, 0x0620, 0x06E0, 0x06A0,
    0x02B0, 0x0290, 0x02F0, 0x02D0, 0x0230, 0x0210, 0x0270, 0x0250, 0x03B0, 0x0390, 0x03F0, 0x03D0, 0x0330, 0x0310, 0x0370, 0x0350
};

static const unsigned short g_drwavMulawTable[256] = {
    0x8284, 0x8684, 0x8A84, 0x8E84, 0x9284, 0x9684, 0x9A84, 0x9E84, 0xA284, 0xA684, 0xAA84, 0xAE84, 0xB284, 0xB684, 0xBA84, 0xBE84,
    0xC184, 0xC384, 0xC584, 0xC784, 0xC984, 0xCB84, 0xCD84, 0xCF84, 0xD184, 0xD384, 0xD584, 0xD784, 0xD984, 0xDB84, 0xDD84, 0xDF84,
    0xE104, 0xE204, 0xE304, 0xE404, 0xE504, 0xE604, 0xE704, 0xE804, 0xE904, 0xEA04, 0xEB04, 0xEC04, 0xED04, 0xEE04, 0xEF04, 0xF004,
    0xF0C4, 0xF144, 0xF1C4, 0xF244, 0xF2C4, 0xF344, 0xF3C4, 0xF444, 0xF4C4, 0xF544, 0xF5C4, 0xF644, 0xF6C4, 0xF744, 0xF7C4, 0xF844,
    0xF8A4, 0xF8E4, 0xF924, 0xF964, 0xF9A4, 0xF9E4, 0xFA24, 0xFA64, 0xFAA4, 0xFAE4, 0xFB24, 0xFB64, 0xFBA4, 0xFBE4, 0xFC24, 0xFC64,
    0xFC94, 0xFCB4, 0xFCD4, 0xFCF4, 0xFD14, 0xFD34, 0xFD54, 0xFD74, 0xFD94, 0xFDB4, 0xFDD4, 0xFDF4, 0xFE14, 0xFE34, 0xFE54, 0xFE74,
    0xFE8C, 0xFE9C, 0xFEAC, 0xFEBC, 0xFECC, 0xFEDC, 0xFEEC, 0xFEFC, 0xFF0C, 0xFF1C, 0xFF2C, 0xFF3C, 0xFF4C, 0xFF5C, 0xFF6C, 0xFF7C,
    0xFF88, 0xFF90, 0xFF98, 0xFFA0, 0xFFA8, 0xFFB0, 0xFFB8, 0xFFC0, 0xFFC8, 0xFFD0, 0xFFD8, 0xFFE0, 0xFFE8, 0xFFF0, 0xFFF8, 0x0000,
    0x7D7C, 0x797C, 0x757C, 0x717C, 0x6D7C, 0x697C, 0x657C, 0x617C, 0x5D7C, 0x597C, 0x557C, 0x517C, 0x4D7C, 0x497C, 0x457C, 0x417C,
    0x3E7C, 0x3C7C, 0x3A7C, 0x387C, 0x367C, 0x347C, 0x327C, 0x307C, 0x2E7C, 0x2C7C, 0x2A7C, 0x287C, 0x267C, 0x247C, 0x227C, 0x207C,
    0x1EFC, 0x1DFC, 0x1CFC, 0x1BFC, 0x1AFC, 0x19FC, 0x18FC, 0x17FC, 0x16FC, 0x15FC, 0x14FC, 0x13FC, 0x12FC, 0x11FC, 0x10FC, 0x0FFC,
    0x0F3C, 0x0EBC, 0x0E3C, 0x0DBC, 0x0D3C, 0x0CBC, 0x0C3C, 0x0BBC, 0x0B3C, 0x0ABC, 0x0A3C, 0x09BC, 0x093C, 0x08BC, 0x083C, 0x07BC,
    0x075C, 0x071C, 0x06DC, 0x069C, 0x065C, 0x061C, 0x05DC, 0x059C, 0x055C, 0x051C, 0x04DC, 0x049C, 0x045C, 0x041C, 0x03DC, 0x039C,
    0x036C, 0x034C, 0x032C, 0x030C, 0x02EC, 0x02CC, 0x02AC, 0x028C, 0x026C, 0x024C, 0x022C, 0x020C, 0x01EC, 0x01CC, 0x01AC, 0x018C,
    0x0174, 0x0164, 0x0154, 0x0144, 0x0134, 0x0124, 0x0114, 0x0104, 0x00F4, 0x00E4, 0x00D4, 0x00C4, 0x00B4, 0x00A4, 0x0094, 0x0084,
    0x0078, 0x0070, 0x0068, 0x0060, 0x0058, 0x0050, 0x0048, 0x0040, 0x0038, 0x0030, 0x0028, 0x0020, 0x0018, 0x0010, 0x0008, 0x0000
};

static DRWAV_INLINE drwav_int16 drwav__alaw_to_s16(drwav_uint8 sampleIn)
{
    return (short)g_drwavAlawTable[sampleIn];
}

static DRWAV_INLINE drwav_int16 drwav__mulaw_to_s16(drwav_uint8 sampleIn)
{
    return (short)g_drwavMulawTable[sampleIn];
}



DRWAV_PRIVATE void drwav__pcm_to_s16(drwav_int16* pOut, const drwav_uint8* pIn, size_t totalSampleCount, unsigned int bytesPerSample)
{
    size_t i;

    
    if (bytesPerSample == 1) {
        drwav_u8_to_s16(pOut, pIn, totalSampleCount);
        return;
    }


    
    if (bytesPerSample == 2) {
        for (i = 0; i < totalSampleCount; ++i) {
           *pOut++ = ((const drwav_int16*)pIn)[i];
        }
        return;
    }
    if (bytesPerSample == 3) {
        drwav_s24_to_s16(pOut, pIn, totalSampleCount);
        return;
    }
    if (bytesPerSample == 4) {
        drwav_s32_to_s16(pOut, (const drwav_int32*)pIn, totalSampleCount);
        return;
    }


    
    if (bytesPerSample > 8) {
        DRWAV_ZERO_MEMORY(pOut, totalSampleCount * sizeof(*pOut));
        return;
    }


    
    for (i = 0; i < totalSampleCount; ++i) {
        drwav_uint64 sample = 0;
        unsigned int shift  = (8 - bytesPerSample) * 8;

        unsigned int j;
        for (j = 0; j < bytesPerSample; j += 1) {
            DRWAV_ASSERT(j < 8);
            sample |= (drwav_uint64)(pIn[j]) << shift;
            shift  += 8;
        }

        if (!drwav__is_little_endian()) {
            sample = drwav__bswap64(sample);
        }

        pIn += j;
        *pOut++ = (drwav_int16)((drwav_int64)sample >> 48);
    }
}

DRWAV_PRIVATE void drwav__ieee_to_s16(drwav_int16* pOut, const drwav_uint8* pIn, size_t totalSampleCount, unsigned int bytesPerSample)
{
    if (bytesPerSample == 4) {
        drwav_f32_to_s16(pOut, (const float*)pIn, totalSampleCount);
        return;
    } else if (bytesPerSample == 8) {
        drwav_f64_to_s16(pOut, (const double*)pIn, totalSampleCount);
        return;
    } else {
        
        DRWAV_ZERO_MEMORY(pOut, totalSampleCount * sizeof(*pOut));
        return;
    }
}

DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_s16__pcm(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    drwav_uint8 sampleData[4096] = {0};
    drwav_uint32 bytesPerFrame;
    drwav_uint32 bytesPerSample;
    drwav_uint64 samplesRead;

    
    if ((pWav->translatedFormatTag == DR_WAVE_FORMAT_PCM && pWav->bitsPerSample == 16) || pBufferOut == NULL) {
        return drwav_read_pcm_frames(pWav, framesToRead, pBufferOut);
    }

    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    bytesPerSample = bytesPerFrame / pWav->channels;
    if (bytesPerSample == 0 || (bytesPerFrame % pWav->channels) != 0) {
        return 0;   
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesToReadThisIteration = drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame);
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, framesToReadThisIteration, sampleData);
        if (framesRead == 0) {
            break;
        }

        DRWAV_ASSERT(framesRead <= framesToReadThisIteration);   

        
        samplesRead = framesRead * pWav->channels;
        if ((samplesRead * bytesPerSample) > sizeof(sampleData)) {
            DRWAV_ASSERT(DRWAV_FALSE);  
            break;
        }

        drwav__pcm_to_s16(pBufferOut, sampleData, (size_t)samplesRead, bytesPerSample);

        pBufferOut      += samplesRead;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_s16__ieee(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    drwav_uint8 sampleData[4096] = {0};
    drwav_uint32 bytesPerFrame;
    drwav_uint32 bytesPerSample;
    drwav_uint64 samplesRead;

    if (pBufferOut == NULL) {
        return drwav_read_pcm_frames(pWav, framesToRead, NULL);
    }

    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    bytesPerSample = bytesPerFrame / pWav->channels;
    if (bytesPerSample == 0 || (bytesPerFrame % pWav->channels) != 0) {
        return 0;   
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesToReadThisIteration = drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame);
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, framesToReadThisIteration, sampleData);
        if (framesRead == 0) {
            break;
        }

        DRWAV_ASSERT(framesRead <= framesToReadThisIteration);   

        
        samplesRead = framesRead * pWav->channels;
        if ((samplesRead * bytesPerSample) > sizeof(sampleData)) {
            DRWAV_ASSERT(DRWAV_FALSE);  
            break;
        }

        drwav__ieee_to_s16(pBufferOut, sampleData, (size_t)samplesRead, bytesPerSample);    

        pBufferOut      += samplesRead;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_s16__alaw(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    drwav_uint8 sampleData[4096] = {0};
    drwav_uint32 bytesPerFrame;
    drwav_uint32 bytesPerSample;
    drwav_uint64 samplesRead;

    if (pBufferOut == NULL) {
        return drwav_read_pcm_frames(pWav, framesToRead, NULL);
    }

    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    bytesPerSample = bytesPerFrame / pWav->channels;
    if (bytesPerSample == 0 || (bytesPerFrame % pWav->channels) != 0) {
        return 0;   
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesToReadThisIteration = drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame);
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, framesToReadThisIteration, sampleData);
        if (framesRead == 0) {
            break;
        }

        DRWAV_ASSERT(framesRead <= framesToReadThisIteration);   

        
        samplesRead = framesRead * pWav->channels;
        if ((samplesRead * bytesPerSample) > sizeof(sampleData)) {
            DRWAV_ASSERT(DRWAV_FALSE);  
            break;
        }

        drwav_alaw_to_s16(pBufferOut, sampleData, (size_t)samplesRead);

        pBufferOut      += samplesRead;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_s16__mulaw(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    drwav_uint8 sampleData[4096] = {0};
    drwav_uint32 bytesPerFrame;
    drwav_uint32 bytesPerSample;
    drwav_uint64 samplesRead;

    if (pBufferOut == NULL) {
        return drwav_read_pcm_frames(pWav, framesToRead, NULL);
    }

    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    bytesPerSample = bytesPerFrame / pWav->channels;
    if (bytesPerSample == 0 || (bytesPerFrame % pWav->channels) != 0) {
        return 0;   
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesToReadThisIteration = drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame);
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, framesToReadThisIteration, sampleData);
        if (framesRead == 0) {
            break;
        }

        DRWAV_ASSERT(framesRead <= framesToReadThisIteration);   

        
        samplesRead = framesRead * pWav->channels;
        if ((samplesRead * bytesPerSample) > sizeof(sampleData)) {
            DRWAV_ASSERT(DRWAV_FALSE);  
            break;
        }

        drwav_mulaw_to_s16(pBufferOut, sampleData, (size_t)samplesRead);

        pBufferOut      += samplesRead;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}


DRWAV_PRIVATE drwav_uint64 drwav_seek_forward(drwav* pWav, drwav_uint64 framesToSeek)
{
    DRWAV_ASSERT(pWav != NULL);
    DRWAV_ASSERT(framesToSeek > 0);

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ADPCM) {
        return drwav_read_pcm_frames_s16__msadpcm(pWav, framesToSeek, NULL);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_DVI_ADPCM) {
        return drwav_read_pcm_frames_s16__ima(pWav, framesToSeek, NULL);
    }

    return drwav_read_pcm_frames(pWav, framesToSeek, NULL);
}

DRWAV_API drwav_uint64 drwav_read_pcm_frames_s16(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    if (pWav == NULL || framesToRead == 0) {
        return 0;
    }

    if (pBufferOut == NULL) {
        return drwav_seek_forward(pWav, framesToRead);
    }

    
    if (framesToRead * pWav->channels * sizeof(drwav_int16) > DRWAV_SIZE_MAX) {
        framesToRead = DRWAV_SIZE_MAX / sizeof(drwav_int16) / pWav->channels;
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_PCM) {
        return drwav_read_pcm_frames_s16__pcm(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_IEEE_FLOAT) {
        return drwav_read_pcm_frames_s16__ieee(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ALAW) {
        return drwav_read_pcm_frames_s16__alaw(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_MULAW) {
        return drwav_read_pcm_frames_s16__mulaw(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ADPCM) {
        return drwav_read_pcm_frames_s16__msadpcm(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_DVI_ADPCM) {
        return drwav_read_pcm_frames_s16__ima(pWav, framesToRead, pBufferOut);
    }

    return 0;
}

DRWAV_API drwav_uint64 drwav_read_pcm_frames_s16le(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    drwav_uint64 framesRead = drwav_read_pcm_frames_s16(pWav, framesToRead, pBufferOut);
    if (pBufferOut != NULL && drwav__is_little_endian() == DRWAV_FALSE) {
        drwav__bswap_samples_s16(pBufferOut, framesRead*pWav->channels);
    }

    return framesRead;
}

DRWAV_API drwav_uint64 drwav_read_pcm_frames_s16be(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    drwav_uint64 framesRead = drwav_read_pcm_frames_s16(pWav, framesToRead, pBufferOut);
    if (pBufferOut != NULL && drwav__is_little_endian() == DRWAV_TRUE) {
        drwav__bswap_samples_s16(pBufferOut, framesRead*pWav->channels);
    }

    return framesRead;
}


DRWAV_API void drwav_u8_to_s16(drwav_int16* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    int r;
    size_t i;
    for (i = 0; i < sampleCount; ++i) {
        int x = pIn[i];
        r = x << 8;
        r = r - 32768;
        pOut[i] = (short)r;
    }
}

static DRWAV_INLINE drwav_int32 drwav__s24_to_s32(const drwav_uint8* pIn)
{
    if (drwav__is_little_endian()) {
        return (drwav_int32)(((drwav_uint32)pIn[0] << 8) | ((drwav_uint32)pIn[1] << 16) | ((drwav_uint32)pIn[2] << 24));
    } else {
        return (drwav_int32)(((drwav_uint32)pIn[2] << 8) | ((drwav_uint32)pIn[1] << 16) | ((drwav_uint32)pIn[0] << 24));
    }
}

DRWAV_API void drwav_s24_to_s16(drwav_int16* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;
    for (i = 0; i < sampleCount; ++i) {
        pOut[i] = (drwav_int16)(drwav__s24_to_s32(pIn + i*3) >> 16);
    }
}

DRWAV_API void drwav_s32_to_s16(drwav_int16* pOut, const drwav_int32* pIn, size_t sampleCount)
{
    int r;
    size_t i;
    for (i = 0; i < sampleCount; ++i) {
        int x = pIn[i];
        r = x >> 16;
        pOut[i] = (short)r;
    }
}

DRWAV_API void drwav_f32_to_s16(drwav_int16* pOut, const float* pIn, size_t sampleCount)
{
    size_t i;
    for (i = 0; i < sampleCount; ++i) {
        float x = pIn[i];
        if (x != x) {
            pOut[i] = 0;    
        } else if (x <= -1) {
            pOut[i] = (-32767 - 1);
        } else if (x >= 1) {
            pOut[i] = 32767;
        } else {
            pOut[i] = (drwav_int16)(x * 32768.0f);
        }
    }
}

DRWAV_API void drwav_f64_to_s16(drwav_int16* pOut, const double* pIn, size_t sampleCount)
{
    size_t i;
    for (i = 0; i < sampleCount; ++i) {
        double x = pIn[i];
        if (x != x) {
            pOut[i] = 0;    
        } else if (x <= -1) {
            pOut[i] = (-32767 - 1);
        } else if (x >= 1) {
            pOut[i] = 32767;
        } else {
            pOut[i] = (drwav_int16)(x * 32768.0);
        }
    }
}

DRWAV_API void drwav_alaw_to_s16(drwav_int16* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;
    for (i = 0; i < sampleCount; ++i) {
        pOut[i] = drwav__alaw_to_s16(pIn[i]);
    }
}

DRWAV_API void drwav_mulaw_to_s16(drwav_int16* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;
    for (i = 0; i < sampleCount; ++i) {
        pOut[i] = drwav__mulaw_to_s16(pIn[i]);
    }
}


DRWAV_PRIVATE void drwav__pcm_to_f32(float* pOut, const drwav_uint8* pIn, size_t sampleCount, unsigned int bytesPerSample)
{
    unsigned int i;

    
    if (bytesPerSample == 1) {
        drwav_u8_to_f32(pOut, pIn, sampleCount);
        return;
    }

    
    if (bytesPerSample == 2) {
        drwav_s16_to_f32(pOut, (const drwav_int16*)pIn, sampleCount);
        return;
    }
    if (bytesPerSample == 3) {
        drwav_s24_to_f32(pOut, pIn, sampleCount);
        return;
    }
    if (bytesPerSample == 4) {
        drwav_s32_to_f32(pOut, (const drwav_int32*)pIn, sampleCount);
        return;
    }


    
    if (bytesPerSample > 8) {
        DRWAV_ZERO_MEMORY(pOut, sampleCount * sizeof(*pOut));
        return;
    }


    
    for (i = 0; i < sampleCount; ++i) {
        drwav_uint64 sample = 0;
        unsigned int shift  = (8 - bytesPerSample) * 8;

        unsigned int j;
        for (j = 0; j < bytesPerSample; j += 1) {
            DRWAV_ASSERT(j < 8);
            sample |= (drwav_uint64)(pIn[j]) << shift;
            shift  += 8;
        }

        if (!drwav__is_little_endian()) {
            sample = drwav__bswap64(sample);
        }

        pIn += j;
        *pOut++ = (float)((drwav_int64)sample / 9223372036854775807.0);
    }
}

DRWAV_PRIVATE void drwav__ieee_to_f32(float* pOut, const drwav_uint8* pIn, size_t sampleCount, unsigned int bytesPerSample)
{
    if (bytesPerSample == 4) {
        unsigned int i;
        for (i = 0; i < sampleCount; ++i) {
            *pOut++ = ((const float*)pIn)[i];
        }
        return;
    } else if (bytesPerSample == 8) {
        drwav_f64_to_f32(pOut, (const double*)pIn, sampleCount);
        return;
    } else {
        
        DRWAV_ZERO_MEMORY(pOut, sampleCount * sizeof(*pOut));
        return;
    }
}


DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_f32__pcm(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    drwav_uint8 sampleData[4096] = {0};
    drwav_uint32 bytesPerFrame;
    drwav_uint32 bytesPerSample;
    drwav_uint64 samplesRead;

    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    bytesPerSample = bytesPerFrame / pWav->channels;
    if (bytesPerSample == 0 || (bytesPerFrame % pWav->channels) != 0) {
        return 0;   
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesToReadThisIteration = drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame);
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, framesToReadThisIteration, sampleData);
        if (framesRead == 0) {
            break;
        }

        DRWAV_ASSERT(framesRead <= framesToReadThisIteration);   

        
        samplesRead = framesRead * pWav->channels;
        if ((samplesRead * bytesPerSample) > sizeof(sampleData)) {
            DRWAV_ASSERT(DRWAV_FALSE);  
            break;
        }

        drwav__pcm_to_f32(pBufferOut, sampleData, (size_t)samplesRead, bytesPerSample);

        pBufferOut      += samplesRead;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_f32__msadpcm_ima(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    



    drwav_uint64 totalFramesRead;
    drwav_int16 samples16[2048];

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesToReadThisIteration = drwav_min(framesToRead, drwav_countof(samples16)/pWav->channels);
        drwav_uint64 framesRead = drwav_read_pcm_frames_s16(pWav, framesToReadThisIteration, samples16);
        if (framesRead == 0) {
            break;
        }

        DRWAV_ASSERT(framesRead <= framesToReadThisIteration);   

        drwav_s16_to_f32(pBufferOut, samples16, (size_t)(framesRead*pWav->channels));   

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_f32__ieee(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    drwav_uint8 sampleData[4096] = {0};
    drwav_uint32 bytesPerFrame;
    drwav_uint32 bytesPerSample;
    drwav_uint64 samplesRead;

    
    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_IEEE_FLOAT && pWav->bitsPerSample == 32) {
        return drwav_read_pcm_frames(pWav, framesToRead, pBufferOut);
    }

    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    bytesPerSample = bytesPerFrame / pWav->channels;
    if (bytesPerSample == 0 || (bytesPerFrame % pWav->channels) != 0) {
        return 0;   
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesToReadThisIteration = drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame);
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, framesToReadThisIteration, sampleData);
        if (framesRead == 0) {
            break;
        }

        DRWAV_ASSERT(framesRead <= framesToReadThisIteration);   

        
        samplesRead = framesRead * pWav->channels;
        if ((samplesRead * bytesPerSample) > sizeof(sampleData)) {
            DRWAV_ASSERT(DRWAV_FALSE);  
            break;
        }

        drwav__ieee_to_f32(pBufferOut, sampleData, (size_t)samplesRead, bytesPerSample);

        pBufferOut      += samplesRead;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_f32__alaw(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    drwav_uint8 sampleData[4096] = {0};
    drwav_uint32 bytesPerFrame;
    drwav_uint32 bytesPerSample;
    drwav_uint64 samplesRead;

    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    bytesPerSample = bytesPerFrame / pWav->channels;
    if (bytesPerSample == 0 || (bytesPerFrame % pWav->channels) != 0) {
        return 0;   
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesToReadThisIteration = drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame);
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, framesToReadThisIteration, sampleData);
        if (framesRead == 0) {
            break;
        }

        DRWAV_ASSERT(framesRead <= framesToReadThisIteration);   

        
        samplesRead = framesRead * pWav->channels;
        if ((samplesRead * bytesPerSample) > sizeof(sampleData)) {
            DRWAV_ASSERT(DRWAV_FALSE);  
            break;
        }

        drwav_alaw_to_f32(pBufferOut, sampleData, (size_t)samplesRead);

        pBufferOut      += samplesRead;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_f32__mulaw(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    drwav_uint8 sampleData[4096] = {0};
    drwav_uint32 bytesPerFrame;
    drwav_uint32 bytesPerSample;
    drwav_uint64 samplesRead;

    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    bytesPerSample = bytesPerFrame / pWav->channels;
    if (bytesPerSample == 0 || (bytesPerFrame % pWav->channels) != 0) {
        return 0;   
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesToReadThisIteration = drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame);
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, framesToReadThisIteration, sampleData);
        if (framesRead == 0) {
            break;
        }

        DRWAV_ASSERT(framesRead <= framesToReadThisIteration);   

        
        samplesRead = framesRead * pWav->channels;
        if ((samplesRead * bytesPerSample) > sizeof(sampleData)) {
            DRWAV_ASSERT(DRWAV_FALSE);  
            break;
        }

        drwav_mulaw_to_f32(pBufferOut, sampleData, (size_t)samplesRead);

        pBufferOut      += samplesRead;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

DRWAV_API drwav_uint64 drwav_read_pcm_frames_f32(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    if (pWav == NULL || framesToRead == 0) {
        return 0;
    }

    if (pBufferOut == NULL) {
        return drwav_seek_forward(pWav, framesToRead);
    }

    
    if (framesToRead * pWav->channels * sizeof(float) > DRWAV_SIZE_MAX) {
        framesToRead = DRWAV_SIZE_MAX / sizeof(float) / pWav->channels;
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_PCM) {
        return drwav_read_pcm_frames_f32__pcm(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ADPCM || pWav->translatedFormatTag == DR_WAVE_FORMAT_DVI_ADPCM) {
        return drwav_read_pcm_frames_f32__msadpcm_ima(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_IEEE_FLOAT) {
        return drwav_read_pcm_frames_f32__ieee(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ALAW) {
        return drwav_read_pcm_frames_f32__alaw(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_MULAW) {
        return drwav_read_pcm_frames_f32__mulaw(pWav, framesToRead, pBufferOut);
    }

    return 0;
}

DRWAV_API drwav_uint64 drwav_read_pcm_frames_f32le(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    drwav_uint64 framesRead = drwav_read_pcm_frames_f32(pWav, framesToRead, pBufferOut);
    if (pBufferOut != NULL && drwav__is_little_endian() == DRWAV_FALSE) {
        drwav__bswap_samples_f32(pBufferOut, framesRead*pWav->channels);
    }

    return framesRead;
}

DRWAV_API drwav_uint64 drwav_read_pcm_frames_f32be(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    drwav_uint64 framesRead = drwav_read_pcm_frames_f32(pWav, framesToRead, pBufferOut);
    if (pBufferOut != NULL && drwav__is_little_endian() == DRWAV_TRUE) {
        drwav__bswap_samples_f32(pBufferOut, framesRead*pWav->channels);
    }

    return framesRead;
}


DRWAV_API void drwav_u8_to_f32(float* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = ((int)pIn[i] - 128) * 0.0078125f;
    }
}

DRWAV_API void drwav_s16_to_f32(float* pOut, const drwav_int16* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = pIn[i] * 0.000030517578125f;
    }
}

DRWAV_API void drwav_s24_to_f32(float* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = (float)(drwav__s24_to_s32(pIn + i*3) / 2147483648.0);
    }
}

DRWAV_API void drwav_s32_to_f32(float* pOut, const drwav_int32* pIn, size_t sampleCount)
{
    size_t i;
    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = (float)(pIn[i] / 2147483648.0);
    }
}

DRWAV_API void drwav_f64_to_f32(float* pOut, const double* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = (float)pIn[i];
    }
}

DRWAV_API void drwav_alaw_to_f32(float* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = drwav__alaw_to_s16(pIn[i]) / 32768.0f;
    }
}

DRWAV_API void drwav_mulaw_to_f32(float* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = drwav__mulaw_to_s16(pIn[i]) / 32768.0f;
    }
}



DRWAV_PRIVATE void drwav__pcm_to_s32(drwav_int32* pOut, const drwav_uint8* pIn, size_t totalSampleCount, unsigned int bytesPerSample)
{
    unsigned int i;

    
    if (bytesPerSample == 1) {
        drwav_u8_to_s32(pOut, pIn, totalSampleCount);
        return;
    }

    
    if (bytesPerSample == 2) {
        drwav_s16_to_s32(pOut, (const drwav_int16*)pIn, totalSampleCount);
        return;
    }
    if (bytesPerSample == 3) {
        drwav_s24_to_s32(pOut, pIn, totalSampleCount);
        return;
    }
    if (bytesPerSample == 4) {
        for (i = 0; i < totalSampleCount; ++i) {
           *pOut++ = ((const drwav_int32*)pIn)[i];
        }
        return;
    }


    
    if (bytesPerSample > 8) {
        DRWAV_ZERO_MEMORY(pOut, totalSampleCount * sizeof(*pOut));
        return;
    }


    
    for (i = 0; i < totalSampleCount; ++i) {
        drwav_uint64 sample = 0;
        unsigned int shift  = (8 - bytesPerSample) * 8;

        unsigned int j;
        for (j = 0; j < bytesPerSample; j += 1) {
            DRWAV_ASSERT(j < 8);
            sample |= (drwav_uint64)(pIn[j]) << shift;
            shift  += 8;
        }

        if (!drwav__is_little_endian()) {
            sample = drwav__bswap64(sample);
        }

        pIn += j;
        *pOut++ = (drwav_int32)((drwav_int64)sample >> 32);
    }
}

DRWAV_PRIVATE void drwav__ieee_to_s32(drwav_int32* pOut, const drwav_uint8* pIn, size_t totalSampleCount, unsigned int bytesPerSample)
{
    if (bytesPerSample == 4) {
        drwav_f32_to_s32(pOut, (const float*)pIn, totalSampleCount);
        return;
    } else if (bytesPerSample == 8) {
        drwav_f64_to_s32(pOut, (const double*)pIn, totalSampleCount);
        return;
    } else {
        
        DRWAV_ZERO_MEMORY(pOut, totalSampleCount * sizeof(*pOut));
        return;
    }
}


DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_s32__pcm(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    drwav_uint8 sampleData[4096] = {0};
    drwav_uint32 bytesPerFrame;
    drwav_uint32 bytesPerSample;
    drwav_uint64 samplesRead;

    
    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_PCM && pWav->bitsPerSample == 32) {
        return drwav_read_pcm_frames(pWav, framesToRead, pBufferOut);
    }

    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    bytesPerSample = bytesPerFrame / pWav->channels;
    if (bytesPerSample == 0 || (bytesPerFrame % pWav->channels) != 0) {
        return 0;   
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesToReadThisIteration = drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame);
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, framesToReadThisIteration, sampleData);
        if (framesRead == 0) {
            break;
        }

        DRWAV_ASSERT(framesRead <= framesToReadThisIteration);   

        
        samplesRead = framesRead * pWav->channels;
        if ((samplesRead * bytesPerSample) > sizeof(sampleData)) {
            DRWAV_ASSERT(DRWAV_FALSE);  
            break;
        }

        drwav__pcm_to_s32(pBufferOut, sampleData, (size_t)samplesRead, bytesPerSample);

        pBufferOut      += samplesRead;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_s32__msadpcm_ima(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    



    drwav_uint64 totalFramesRead = 0;
    drwav_int16 samples16[2048];

    while (framesToRead > 0) {
        drwav_uint64 framesToReadThisIteration = drwav_min(framesToRead, drwav_countof(samples16)/pWav->channels);
        drwav_uint64 framesRead = drwav_read_pcm_frames_s16(pWav, framesToReadThisIteration, samples16);
        if (framesRead == 0) {
            break;
        }

        DRWAV_ASSERT(framesRead <= framesToReadThisIteration);   

        drwav_s16_to_s32(pBufferOut, samples16, (size_t)(framesRead*pWav->channels));   

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_s32__ieee(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    drwav_uint8 sampleData[4096] = {0};
    drwav_uint32 bytesPerFrame;
    drwav_uint32 bytesPerSample;
    drwav_uint64 samplesRead;

    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    bytesPerSample = bytesPerFrame / pWav->channels;
    if (bytesPerSample == 0 || (bytesPerFrame % pWav->channels) != 0) {
        return 0;   
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesToReadThisIteration = drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame);
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, framesToReadThisIteration, sampleData);
        if (framesRead == 0) {
            break;
        }

        DRWAV_ASSERT(framesRead <= framesToReadThisIteration);   

        
        samplesRead = framesRead * pWav->channels;
        if ((samplesRead * bytesPerSample) > sizeof(sampleData)) {
            DRWAV_ASSERT(DRWAV_FALSE);  
            break;
        }

        drwav__ieee_to_s32(pBufferOut, sampleData, (size_t)samplesRead, bytesPerSample);

        pBufferOut      += samplesRead;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_s32__alaw(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    drwav_uint8 sampleData[4096] = {0};
    drwav_uint32 bytesPerFrame;
    drwav_uint32 bytesPerSample;
    drwav_uint64 samplesRead;

    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    bytesPerSample = bytesPerFrame / pWav->channels;
    if (bytesPerSample == 0 || (bytesPerFrame % pWav->channels) != 0) {
        return 0;   
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesToReadThisIteration = drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame);
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, framesToReadThisIteration, sampleData);
        if (framesRead == 0) {
            break;
        }

        DRWAV_ASSERT(framesRead <= framesToReadThisIteration);   

        
        samplesRead = framesRead * pWav->channels;
        if ((samplesRead * bytesPerSample) > sizeof(sampleData)) {
            DRWAV_ASSERT(DRWAV_FALSE);  
            break;
        }

        drwav_alaw_to_s32(pBufferOut, sampleData, (size_t)samplesRead);

        pBufferOut      += samplesRead;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

DRWAV_PRIVATE drwav_uint64 drwav_read_pcm_frames_s32__mulaw(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    drwav_uint8 sampleData[4096] = {0};
    drwav_uint32 bytesPerFrame;
    drwav_uint32 bytesPerSample;
    drwav_uint64 samplesRead;

    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    bytesPerSample = bytesPerFrame / pWav->channels;
    if (bytesPerSample == 0 || (bytesPerFrame % pWav->channels) != 0) {
        return 0;   
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesToReadThisIteration = drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame);
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, framesToReadThisIteration, sampleData);
        if (framesRead == 0) {
            break;
        }

        DRWAV_ASSERT(framesRead <= framesToReadThisIteration);   

        
        samplesRead = framesRead * pWav->channels;
        if ((samplesRead * bytesPerSample) > sizeof(sampleData)) {
            DRWAV_ASSERT(DRWAV_FALSE);  
            break;
        }

        drwav_mulaw_to_s32(pBufferOut, sampleData, (size_t)samplesRead);

        pBufferOut      += samplesRead;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

DRWAV_API drwav_uint64 drwav_read_pcm_frames_s32(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    if (pWav == NULL || framesToRead == 0) {
        return 0;
    }

    if (pBufferOut == NULL) {
        return drwav_seek_forward(pWav, framesToRead);
    }

    
    if (framesToRead * pWav->channels * sizeof(drwav_int32) > DRWAV_SIZE_MAX) {
        framesToRead = DRWAV_SIZE_MAX / sizeof(drwav_int32) / pWav->channels;
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_PCM) {
        return drwav_read_pcm_frames_s32__pcm(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ADPCM || pWav->translatedFormatTag == DR_WAVE_FORMAT_DVI_ADPCM) {
        return drwav_read_pcm_frames_s32__msadpcm_ima(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_IEEE_FLOAT) {
        return drwav_read_pcm_frames_s32__ieee(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ALAW) {
        return drwav_read_pcm_frames_s32__alaw(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_MULAW) {
        return drwav_read_pcm_frames_s32__mulaw(pWav, framesToRead, pBufferOut);
    }

    return 0;
}

DRWAV_API drwav_uint64 drwav_read_pcm_frames_s32le(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    drwav_uint64 framesRead = drwav_read_pcm_frames_s32(pWav, framesToRead, pBufferOut);
    if (pBufferOut != NULL && drwav__is_little_endian() == DRWAV_FALSE) {
        drwav__bswap_samples_s32(pBufferOut, framesRead*pWav->channels);
    }

    return framesRead;
}

DRWAV_API drwav_uint64 drwav_read_pcm_frames_s32be(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    drwav_uint64 framesRead = drwav_read_pcm_frames_s32(pWav, framesToRead, pBufferOut);
    if (pBufferOut != NULL && drwav__is_little_endian() == DRWAV_TRUE) {
        drwav__bswap_samples_s32(pBufferOut, framesRead*pWav->channels);
    }

    return framesRead;
}


DRWAV_API void drwav_u8_to_s32(drwav_int32* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = ((int)pIn[i] - 128) * 16777216;
    }
}

DRWAV_API void drwav_s16_to_s32(drwav_int32* pOut, const drwav_int16* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = (drwav_int32)pIn[i] * 65536;
    }
}

DRWAV_API void drwav_s24_to_s32(drwav_int32* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = drwav__s24_to_s32(pIn + i*3);
    }
}

DRWAV_API void drwav_f32_to_s32(drwav_int32* pOut, const float* pIn, size_t sampleCount)
{
    size_t i;
    for (i = 0; i < sampleCount; ++i) {
        float x = pIn[i];
        if (x != x) {
            pOut[i] = 0;    
        } else if (x <= -1) {
            pOut[i] = (-2147483647 - 1);
        } else if (x >= 1) {
            pOut[i] = 2147483647;
        } else {
            pOut[i] = (drwav_int32)(x * 2147483648.0f);
        }
    }
}

DRWAV_API void drwav_f64_to_s32(drwav_int32* pOut, const double* pIn, size_t sampleCount)
{
    size_t i;
    for (i = 0; i < sampleCount; ++i) {
        double x = pIn[i];
        if (x != x) {
            pOut[i] = 0;    
        } else if (x <= -1) {
            pOut[i] = (-2147483647 - 1);
        } else if (x >= 1) {
            pOut[i] = 2147483647;
        } else {
            pOut[i] = (drwav_int32)(x * 2147483648.0);
        }
    }
}

DRWAV_API void drwav_alaw_to_s32(drwav_int32* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = (drwav_int32)drwav__alaw_to_s16(pIn[i]) * 65536;
    }
}

DRWAV_API void drwav_mulaw_to_s32(drwav_int32* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i= 0; i < sampleCount; ++i) {
        *pOut++ = (drwav_int32)drwav__mulaw_to_s16(pIn[i]) * 65536;
    }
}



DRWAV_PRIVATE drwav_int16* drwav__read_pcm_frames_and_close_s16(drwav* pWav, unsigned int* channels, unsigned int* sampleRate, drwav_uint64* totalFrameCount)
{
    drwav_uint64 sampleDataSize;
    drwav_int16* pSampleData;
    drwav_uint64 framesRead;

    DRWAV_ASSERT(pWav != NULL);

    
    if (pWav->channels == 0 || pWav->totalPCMFrameCount > DRWAV_SIZE_MAX / pWav->channels / sizeof(drwav_int16)) {
        drwav_uninit(pWav);
        return NULL;    
    }

    sampleDataSize = pWav->totalPCMFrameCount * pWav->channels * sizeof(drwav_int16);
    if (sampleDataSize > DRWAV_SIZE_MAX) {
        drwav_uninit(pWav);
        return NULL;    
    }

    pSampleData = (drwav_int16*)drwav__malloc_from_callbacks((size_t)sampleDataSize, &pWav->allocationCallbacks); 
    if (pSampleData == NULL) {
        drwav_uninit(pWav);
        return NULL;    
    }

    framesRead = drwav_read_pcm_frames_s16(pWav, (size_t)pWav->totalPCMFrameCount, pSampleData);
    if (framesRead != pWav->totalPCMFrameCount) {
        drwav__free_from_callbacks(pSampleData, &pWav->allocationCallbacks);
        drwav_uninit(pWav);
        return NULL;    
    }

    drwav_uninit(pWav);

    if (sampleRate) {
        *sampleRate = pWav->sampleRate;
    }
    if (channels) {
        *channels = pWav->channels;
    }
    if (totalFrameCount) {
        *totalFrameCount = pWav->totalPCMFrameCount;
    }

    return pSampleData;
}

DRWAV_PRIVATE float* drwav__read_pcm_frames_and_close_f32(drwav* pWav, unsigned int* channels, unsigned int* sampleRate, drwav_uint64* totalFrameCount)
{
    drwav_uint64 sampleDataSize;
    float* pSampleData;
    drwav_uint64 framesRead;

    DRWAV_ASSERT(pWav != NULL);

    
    if (pWav->channels == 0 || pWav->totalPCMFrameCount > DRWAV_SIZE_MAX / pWav->channels / sizeof(float)) {
        drwav_uninit(pWav);
        return NULL;    
    }

    sampleDataSize = pWav->totalPCMFrameCount * pWav->channels * sizeof(float);
    if (sampleDataSize > DRWAV_SIZE_MAX) {
        drwav_uninit(pWav);
        return NULL;    
    }

    pSampleData = (float*)drwav__malloc_from_callbacks((size_t)sampleDataSize, &pWav->allocationCallbacks); 
    if (pSampleData == NULL) {
        drwav_uninit(pWav);
        return NULL;    
    }

    framesRead = drwav_read_pcm_frames_f32(pWav, (size_t)pWav->totalPCMFrameCount, pSampleData);
    if (framesRead != pWav->totalPCMFrameCount) {
        drwav__free_from_callbacks(pSampleData, &pWav->allocationCallbacks);
        drwav_uninit(pWav);
        return NULL;    
    }

    drwav_uninit(pWav);

    if (sampleRate) {
        *sampleRate = pWav->sampleRate;
    }
    if (channels) {
        *channels = pWav->channels;
    }
    if (totalFrameCount) {
        *totalFrameCount = pWav->totalPCMFrameCount;
    }

    return pSampleData;
}

DRWAV_PRIVATE drwav_int32* drwav__read_pcm_frames_and_close_s32(drwav* pWav, unsigned int* channels, unsigned int* sampleRate, drwav_uint64* totalFrameCount)
{
    drwav_uint64 sampleDataSize;
    drwav_int32* pSampleData;
    drwav_uint64 framesRead;

    DRWAV_ASSERT(pWav != NULL);

    
    if (pWav->channels == 0 || pWav->totalPCMFrameCount > DRWAV_SIZE_MAX / pWav->channels / sizeof(drwav_int32)) {
        drwav_uninit(pWav);
        return NULL;    
    }

    sampleDataSize = pWav->totalPCMFrameCount * pWav->channels * sizeof(drwav_int32);
    if (sampleDataSize > DRWAV_SIZE_MAX) {
        drwav_uninit(pWav);
        return NULL;    
    }

    pSampleData = (drwav_int32*)drwav__malloc_from_callbacks((size_t)sampleDataSize, &pWav->allocationCallbacks); 
    if (pSampleData == NULL) {
        drwav_uninit(pWav);
        return NULL;    
    }

    framesRead = drwav_read_pcm_frames_s32(pWav, (size_t)pWav->totalPCMFrameCount, pSampleData);
    if (framesRead != pWav->totalPCMFrameCount) {
        drwav__free_from_callbacks(pSampleData, &pWav->allocationCallbacks);
        drwav_uninit(pWav);
        return NULL;    
    }

    drwav_uninit(pWav);

    if (sampleRate) {
        *sampleRate = pWav->sampleRate;
    }
    if (channels) {
        *channels = pWav->channels;
    }
    if (totalFrameCount) {
        *totalFrameCount = pWav->totalPCMFrameCount;
    }

    return pSampleData;
}



DRWAV_API drwav_int16* drwav_open_and_read_pcm_frames_s16(drwav_read_proc onRead, drwav_seek_proc onSeek, drwav_tell_proc onTell, void* pUserData, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init(&wav, onRead, onSeek, onTell, pUserData, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_s16(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

DRWAV_API float* drwav_open_and_read_pcm_frames_f32(drwav_read_proc onRead, drwav_seek_proc onSeek, drwav_tell_proc onTell, void* pUserData, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init(&wav, onRead, onSeek, onTell, pUserData, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_f32(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

DRWAV_API drwav_int32* drwav_open_and_read_pcm_frames_s32(drwav_read_proc onRead, drwav_seek_proc onSeek, drwav_tell_proc onTell, void* pUserData, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init(&wav, onRead, onSeek, onTell, pUserData, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_s32(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

#ifndef DR_WAV_NO_STDIO
DRWAV_API drwav_int16* drwav_open_file_and_read_pcm_frames_s16(const char* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_file(&wav, filename, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_s16(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

DRWAV_API float* drwav_open_file_and_read_pcm_frames_f32(const char* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_file(&wav, filename, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_f32(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

DRWAV_API drwav_int32* drwav_open_file_and_read_pcm_frames_s32(const char* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_file(&wav, filename, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_s32(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}


#ifndef DR_WAV_NO_WCHAR
DRWAV_API drwav_int16* drwav_open_file_and_read_pcm_frames_s16_w(const wchar_t* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (channelsOut) {
        *channelsOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_file_w(&wav, filename, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_s16(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

DRWAV_API float* drwav_open_file_and_read_pcm_frames_f32_w(const wchar_t* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (channelsOut) {
        *channelsOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_file_w(&wav, filename, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_f32(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

DRWAV_API drwav_int32* drwav_open_file_and_read_pcm_frames_s32_w(const wchar_t* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (channelsOut) {
        *channelsOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_file_w(&wav, filename, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_s32(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}
#endif 
#endif 

DRWAV_API drwav_int16* drwav_open_memory_and_read_pcm_frames_s16(const void* data, size_t dataSize, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_memory(&wav, data, dataSize, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_s16(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

DRWAV_API float* drwav_open_memory_and_read_pcm_frames_f32(const void* data, size_t dataSize, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_memory(&wav, data, dataSize, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_f32(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

DRWAV_API drwav_int32* drwav_open_memory_and_read_pcm_frames_s32(const void* data, size_t dataSize, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_memory(&wav, data, dataSize, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_s32(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}
#endif  


DRWAV_API void drwav_free(void* p, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pAllocationCallbacks != NULL) {
        drwav__free_from_callbacks(p, pAllocationCallbacks);
    } else {
        drwav__free_default(p, NULL);
    }
}

DRWAV_API drwav_uint16 drwav_bytes_to_u16(const drwav_uint8* data)
{
    return ((drwav_uint16)data[0] << 0) | ((drwav_uint16)data[1] << 8);
}

DRWAV_API drwav_int16 drwav_bytes_to_s16(const drwav_uint8* data)
{
    return (drwav_int16)drwav_bytes_to_u16(data);
}

DRWAV_API drwav_uint32 drwav_bytes_to_u32(const drwav_uint8* data)
{
    return drwav_bytes_to_u32_le(data);
}

DRWAV_API float drwav_bytes_to_f32(const drwav_uint8* data)
{
    union {
        drwav_uint32 u32;
        float f32;
    } value;

    value.u32 = drwav_bytes_to_u32(data);
    return value.f32;
}

DRWAV_API drwav_int32 drwav_bytes_to_s32(const drwav_uint8* data)
{
    return (drwav_int32)drwav_bytes_to_u32(data);
}

DRWAV_API drwav_uint64 drwav_bytes_to_u64(const drwav_uint8* data)
{
    return
        ((drwav_uint64)data[0] <<  0) | ((drwav_uint64)data[1] <<  8) | ((drwav_uint64)data[2] << 16) | ((drwav_uint64)data[3] << 24) |
        ((drwav_uint64)data[4] << 32) | ((drwav_uint64)data[5] << 40) | ((drwav_uint64)data[6] << 48) | ((drwav_uint64)data[7] << 56);
}

DRWAV_API drwav_int64 drwav_bytes_to_s64(const drwav_uint8* data)
{
    return (drwav_int64)drwav_bytes_to_u64(data);
}


DRWAV_API drwav_bool32 drwav_guid_equal(const drwav_uint8 a[16], const drwav_uint8 b[16])
{
    int i;
    for (i = 0; i < 16; i += 1) {
        if (a[i] != b[i]) {
            return DRWAV_FALSE;
        }
    }

    return DRWAV_TRUE;
}

DRWAV_API drwav_bool32 drwav_fourcc_equal(const drwav_uint8* a, const char* b)
{
    return
        a[0] == b[0] &&
        a[1] == b[1] &&
        a[2] == b[2] &&
        a[3] == b[3];
}

#ifdef __MRC__

#pragma options opt reset
#endif

#endif  
#endif  


















































