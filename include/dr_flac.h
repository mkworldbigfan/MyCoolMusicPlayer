



















































































































#ifndef dr_flac_h
#define dr_flac_h

#ifdef __cplusplus
extern "C" {
#endif

#define DRFLAC_STRINGIFY(x)      #x
#define DRFLAC_XSTRINGIFY(x)     DRFLAC_STRINGIFY(x)

#define DRFLAC_VERSION_MAJOR     0
#define DRFLAC_VERSION_MINOR     13
#define DRFLAC_VERSION_REVISION  5
#define DRFLAC_VERSION_STRING    DRFLAC_XSTRINGIFY(DRFLAC_VERSION_MAJOR) "." DRFLAC_XSTRINGIFY(DRFLAC_VERSION_MINOR) "." DRFLAC_XSTRINGIFY(DRFLAC_VERSION_REVISION)

#include <stddef.h> 


typedef   signed char           drflac_int8;
typedef unsigned char           drflac_uint8;
typedef   signed short          drflac_int16;
typedef unsigned short          drflac_uint16;
typedef   signed int            drflac_int32;
typedef unsigned int            drflac_uint32;
#if defined(_MSC_VER) && !defined(__clang__)
    typedef   signed __int64    drflac_int64;
    typedef unsigned __int64    drflac_uint64;
#else
    #if defined(__clang__) || (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wlong-long"
        #if defined(__clang__)
            #pragma GCC diagnostic ignored "-Wc++11-long-long"
        #endif
    #endif
    typedef   signed long long  drflac_int64;
    typedef unsigned long long  drflac_uint64;
    #if defined(__clang__) || (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))
        #pragma GCC diagnostic pop
    #endif
#endif
#if defined(__LP64__) || defined(_WIN64) || (defined(__x86_64__) && !defined(__ILP32__)) || defined(_M_X64) || defined(__ia64) || defined(_M_IA64) || defined(__aarch64__) || defined(_M_ARM64) || defined(__powerpc64__)
    typedef drflac_uint64       drflac_uintptr;
#else
    typedef drflac_uint32       drflac_uintptr;
#endif
typedef drflac_uint8            drflac_bool8;
typedef drflac_uint32           drflac_bool32;
#define DRFLAC_TRUE             1
#define DRFLAC_FALSE            0



#if !defined(DRFLAC_API)
    #if defined(DRFLAC_DLL)
        #if defined(_WIN32)
            #define DRFLAC_DLL_IMPORT  __declspec(dllimport)
            #define DRFLAC_DLL_EXPORT  __declspec(dllexport)
            #define DRFLAC_DLL_PRIVATE static
        #else
            #if defined(__GNUC__) && __GNUC__ >= 4
                #define DRFLAC_DLL_IMPORT  __attribute__((visibility("default")))
                #define DRFLAC_DLL_EXPORT  __attribute__((visibility("default")))
                #define DRFLAC_DLL_PRIVATE __attribute__((visibility("hidden")))
            #else
                #define DRFLAC_DLL_IMPORT
                #define DRFLAC_DLL_EXPORT
                #define DRFLAC_DLL_PRIVATE static
            #endif
        #endif

        #if defined(DR_FLAC_IMPLEMENTATION) || defined(DRFLAC_IMPLEMENTATION)
            #define DRFLAC_API  DRFLAC_DLL_EXPORT
        #else
            #define DRFLAC_API  DRFLAC_DLL_IMPORT
        #endif
        #define DRFLAC_PRIVATE DRFLAC_DLL_PRIVATE
    #else
        #define DRFLAC_API extern
        #define DRFLAC_PRIVATE static
    #endif
#endif


#if defined(_MSC_VER) && _MSC_VER >= 1700   
    #define DRFLAC_DEPRECATED       __declspec(deprecated)
#elif (defined(__GNUC__) && __GNUC__ >= 4)  
    #define DRFLAC_DEPRECATED       __attribute__((deprecated))
#elif defined(__has_feature)                
    #if __has_feature(attribute_deprecated)
        #define DRFLAC_DEPRECATED   __attribute__((deprecated))
    #else
        #define DRFLAC_DEPRECATED
    #endif
#else
    #define DRFLAC_DEPRECATED
#endif

DRFLAC_API void drflac_version(drflac_uint32* pMajor, drflac_uint32* pMinor, drflac_uint32* pRevision);
DRFLAC_API const char* drflac_version_string(void);


typedef struct
{
    void* pUserData;
    void* (* onMalloc)(size_t sz, void* pUserData);
    void* (* onRealloc)(void* p, size_t sz, void* pUserData);
    void  (* onFree)(void* p, void* pUserData);
} drflac_allocation_callbacks;






#ifndef DR_FLAC_BUFFER_SIZE
#define DR_FLAC_BUFFER_SIZE   4096
#endif



#if defined(_WIN64) || defined(_LP64) || defined(__LP64__)
#define DRFLAC_64BIT
#endif

#if defined(__x86_64__) || (defined(_M_X64) && !defined(_M_ARM64EC))
    #define DRFLAC_X64
#elif defined(__i386) || defined(_M_IX86)
    #define DRFLAC_X86
#elif defined(__arm__) || defined(_M_ARM) || defined(__arm64) || defined(__arm64__) || defined(__aarch64__) || defined(_M_ARM64) || defined(_M_ARM64EC)
    #define DRFLAC_ARM
#endif



#ifdef DRFLAC_64BIT
typedef drflac_uint64 drflac_cache_t;
#else
typedef drflac_uint32 drflac_cache_t;
#endif


#define DRFLAC_METADATA_BLOCK_TYPE_STREAMINFO       0
#define DRFLAC_METADATA_BLOCK_TYPE_PADDING          1
#define DRFLAC_METADATA_BLOCK_TYPE_APPLICATION      2
#define DRFLAC_METADATA_BLOCK_TYPE_SEEKTABLE        3
#define DRFLAC_METADATA_BLOCK_TYPE_VORBIS_COMMENT   4
#define DRFLAC_METADATA_BLOCK_TYPE_CUESHEET         5
#define DRFLAC_METADATA_BLOCK_TYPE_PICTURE          6
#define DRFLAC_METADATA_BLOCK_TYPE_INVALID          127


#define DRFLAC_PICTURE_TYPE_OTHER                   0
#define DRFLAC_PICTURE_TYPE_FILE_ICON               1
#define DRFLAC_PICTURE_TYPE_OTHER_FILE_ICON         2
#define DRFLAC_PICTURE_TYPE_COVER_FRONT             3
#define DRFLAC_PICTURE_TYPE_COVER_BACK              4
#define DRFLAC_PICTURE_TYPE_LEAFLET_PAGE            5
#define DRFLAC_PICTURE_TYPE_MEDIA                   6
#define DRFLAC_PICTURE_TYPE_LEAD_ARTIST             7
#define DRFLAC_PICTURE_TYPE_ARTIST                  8
#define DRFLAC_PICTURE_TYPE_CONDUCTOR               9
#define DRFLAC_PICTURE_TYPE_BAND                    10
#define DRFLAC_PICTURE_TYPE_COMPOSER                11
#define DRFLAC_PICTURE_TYPE_LYRICIST                12
#define DRFLAC_PICTURE_TYPE_RECORDING_LOCATION      13
#define DRFLAC_PICTURE_TYPE_DURING_RECORDING        14
#define DRFLAC_PICTURE_TYPE_DURING_PERFORMANCE      15
#define DRFLAC_PICTURE_TYPE_SCREEN_CAPTURE          16
#define DRFLAC_PICTURE_TYPE_BRIGHT_COLORED_FISH     17
#define DRFLAC_PICTURE_TYPE_ILLUSTRATION            18
#define DRFLAC_PICTURE_TYPE_BAND_LOGOTYPE           19
#define DRFLAC_PICTURE_TYPE_PUBLISHER_LOGOTYPE      20

typedef enum
{
    drflac_container_native,
    drflac_container_ogg,
    drflac_container_unknown
} drflac_container;

typedef enum
{
    DRFLAC_SEEK_SET,
    DRFLAC_SEEK_CUR,
    DRFLAC_SEEK_END
} drflac_seek_origin;


typedef struct
{
    drflac_uint64 firstPCMFrame;
    drflac_uint64 flacFrameOffset;   
    drflac_uint16 pcmFrameCount;
} drflac_seekpoint;

typedef struct
{
    drflac_uint16 minBlockSizeInPCMFrames;
    drflac_uint16 maxBlockSizeInPCMFrames;
    drflac_uint32 minFrameSizeInPCMFrames;
    drflac_uint32 maxFrameSizeInPCMFrames;
    drflac_uint32 sampleRate;
    drflac_uint8  channels;
    drflac_uint8  bitsPerSample;
    drflac_uint64 totalPCMFrameCount;
    drflac_uint8  md5[16];
} drflac_streaminfo;

typedef struct
{
    



    drflac_uint32 type;

    
    drflac_uint32 rawDataSize;

    
    drflac_uint64 rawDataOffset;

    




    const void* pRawData;

    union
    {
        drflac_streaminfo streaminfo;

        struct
        {
            int unused;
        } padding;

        struct
        {
            drflac_uint32 id;
            const void* pData;
            drflac_uint32 dataSize;
        } application;

        struct
        {
            drflac_uint32 seekpointCount;
            const drflac_seekpoint* pSeekpoints;
        } seektable;

        struct
        {
            drflac_uint32 vendorLength;
            const char* vendor;
            drflac_uint32 commentCount;
            const void* pComments;
        } vorbis_comment;

        struct
        {
            char catalog[128];
            drflac_uint64 leadInSampleCount;
            drflac_bool32 isCD;
            drflac_uint8 trackCount;
            const void* pTrackData;
        } cuesheet;

        struct
        {
            drflac_uint32 type;
            drflac_uint32 mimeLength;
            const char* mime;
            drflac_uint32 descriptionLength;
            const char* description;
            drflac_uint32 width;
            drflac_uint32 height;
            drflac_uint32 colorDepth;
            drflac_uint32 indexColorCount;
            drflac_uint32 pictureDataSize;
            drflac_uint64 pictureDataOffset;  
            const drflac_uint8* pPictureData;
        } picture;
    } data;
} drflac_metadata;




























typedef size_t (* drflac_read_proc)(void* pUserData, void* pBufferOut, size_t bytesToRead);





























typedef drflac_bool32 (* drflac_seek_proc)(void* pUserData, int offset, drflac_seek_origin origin);


















typedef drflac_bool32 (* drflac_tell_proc)(void* pUserData, drflac_int64* pCursor);



















typedef void (* drflac_meta_proc)(void* pUserData, drflac_metadata* pMetadata);



typedef struct
{
    const drflac_uint8* data;
    size_t dataSize;
    size_t currentReadPos;
} drflac__memory_stream;


typedef struct
{
    
    drflac_read_proc onRead;

    
    drflac_seek_proc onSeek;

    
    drflac_tell_proc onTell;

    
    void* pUserData;


    




    size_t unalignedByteCount;

    
    drflac_cache_t unalignedCache;

    
    drflac_uint32 nextL2Line;

    
    drflac_uint32 consumedBits;

    



    drflac_cache_t cacheL2[DR_FLAC_BUFFER_SIZE/sizeof(drflac_cache_t)];
    drflac_cache_t cache;

    



    drflac_uint16 crc16;
    drflac_cache_t crc16Cache;              
    drflac_uint32 crc16CacheIgnoredBytes;   
} drflac_bs;

typedef union
{
    drflac_int32* pS32;
    drflac_int64* pS64;
} drflac_samples;

typedef struct
{
    
    drflac_uint8 subframeType;

    
    drflac_uint8 wastedBitsPerSample;

    
    drflac_uint8 lpcOrder;

    
    drflac_samples samples;
} drflac_subframe;

typedef struct
{
    



    drflac_uint64 pcmFrameNumber;

    



    drflac_uint32 flacFrameNumber;

    
    drflac_uint32 sampleRate;

    
    drflac_uint16 blockSizeInPCMFrames;

    



    drflac_uint8 channelAssignment;

    
    drflac_uint8 bitsPerSample;

    
    drflac_uint8 crc8;
} drflac_frame_header;

typedef struct
{
    
    drflac_frame_header header;

    



    drflac_uint32 pcmFramesRemaining;

    
    drflac_subframe subframes[8];
} drflac_frame;

typedef struct
{
    
    drflac_meta_proc onMeta;

    
    void* pUserDataMD;

    
    drflac_allocation_callbacks allocationCallbacks;


    
    drflac_uint32 sampleRate;

    



    drflac_uint8 channels;

    
    drflac_uint8 bitsPerSample;

    
    drflac_uint16 maxBlockSizeInPCMFrames;

    



    drflac_uint64 totalPCMFrameCount;


    
    drflac_container container;

    
    drflac_uint32 seekpointCount;


    
    drflac_frame currentFLACFrame;


    
    drflac_uint64 currentPCMFrame;

    
    drflac_uint64 firstFLACFramePosInBytes;


    
    drflac__memory_stream memoryStream;


    
    drflac_samples decodedSamples;

    
    void* pDecodedSamplesAllocation;
    drflac_uint16 decodedSamplesCapacityInPCMFrames;

    
    drflac_seekpoint* pSeekpoints;

    
    void* _oggbs;

    
    drflac_bool32 _noSeekTableSeek    : 1;
    drflac_bool32 _noBinarySearchSeek : 1;
    drflac_bool32 _noBruteForceSeek   : 1;

    
    drflac_bs bs;

    
    drflac_uint8 pExtraData[1];
} drflac;





















































DRFLAC_API drflac* drflac_open(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks);











































DRFLAC_API drflac* drflac_open_relaxed(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, drflac_container container, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks);



























































DRFLAC_API drflac* drflac_open_with_metadata(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, drflac_meta_proc onMeta, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks);









DRFLAC_API drflac* drflac_open_with_metadata_relaxed(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, drflac_meta_proc onMeta, drflac_container container, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks);



























DRFLAC_API void drflac_close(drflac* pFlac);



























DRFLAC_API drflac_uint64 drflac_read_pcm_frames_s32(drflac* pFlac, drflac_uint64 framesToRead, drflac_int32* pBufferOut);





























DRFLAC_API drflac_uint64 drflac_read_pcm_frames_s16(drflac* pFlac, drflac_uint64 framesToRead, drflac_int16* pBufferOut);




























DRFLAC_API drflac_uint64 drflac_read_pcm_frames_f32(drflac* pFlac, drflac_uint64 framesToRead, float* pBufferOut);


















DRFLAC_API drflac_bool32 drflac_seek_to_pcm_frame(drflac* pFlac, drflac_uint64 pcmFrameIndex);



#ifndef DR_FLAC_NO_STDIO



































DRFLAC_API drflac* drflac_open_file(const char* pFileName, const drflac_allocation_callbacks* pAllocationCallbacks);
DRFLAC_API drflac* drflac_open_file_w(const wchar_t* pFileName, const drflac_allocation_callbacks* pAllocationCallbacks);


































DRFLAC_API drflac* drflac_open_file_with_metadata(const char* pFileName, drflac_meta_proc onMeta, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks);
DRFLAC_API drflac* drflac_open_file_with_metadata_w(const wchar_t* pFileName, drflac_meta_proc onMeta, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks);
#endif
































DRFLAC_API drflac* drflac_open_memory(const void* pData, size_t dataSize, const drflac_allocation_callbacks* pAllocationCallbacks);


































DRFLAC_API drflac* drflac_open_memory_with_metadata(const void* pData, size_t dataSize, drflac_meta_proc onMeta, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks);

















DRFLAC_API drflac_int32* drflac_open_and_read_pcm_frames_s32(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, void* pUserData, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);


DRFLAC_API drflac_int16* drflac_open_and_read_pcm_frames_s16(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, void* pUserData, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);


DRFLAC_API float* drflac_open_and_read_pcm_frames_f32(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, void* pUserData, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);

#ifndef DR_FLAC_NO_STDIO

DRFLAC_API drflac_int32* drflac_open_file_and_read_pcm_frames_s32(const char* filename, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);


DRFLAC_API drflac_int16* drflac_open_file_and_read_pcm_frames_s16(const char* filename, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);


DRFLAC_API float* drflac_open_file_and_read_pcm_frames_f32(const char* filename, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);
#endif


DRFLAC_API drflac_int32* drflac_open_memory_and_read_pcm_frames_s32(const void* data, size_t dataSize, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);


DRFLAC_API drflac_int16* drflac_open_memory_and_read_pcm_frames_s16(const void* data, size_t dataSize, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);


DRFLAC_API float* drflac_open_memory_and_read_pcm_frames_f32(const void* data, size_t dataSize, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);






DRFLAC_API void drflac_free(void* p, const drflac_allocation_callbacks* pAllocationCallbacks);



typedef struct
{
    drflac_uint32 countRemaining;
    const char* pRunningData;
} drflac_vorbis_comment_iterator;





DRFLAC_API void drflac_init_vorbis_comment_iterator(drflac_vorbis_comment_iterator* pIter, drflac_uint32 commentCount, const void* pComments);





DRFLAC_API const char* drflac_next_vorbis_comment(drflac_vorbis_comment_iterator* pIter, drflac_uint32* pCommentLengthOut);



typedef struct
{
    drflac_uint32 countRemaining;
    const char* pRunningData;
} drflac_cuesheet_track_iterator;


typedef struct
{
    drflac_uint64 offset;
    drflac_uint8 index;
    drflac_uint8 reserved[3];
} drflac_cuesheet_track_index;

typedef struct
{
    drflac_uint64 offset;
    drflac_uint8 trackNumber;
    char ISRC[12];
    drflac_bool8 isAudio;
    drflac_bool8 preEmphasis;
    drflac_uint8 indexCount;
    const drflac_cuesheet_track_index* pIndexPoints;
} drflac_cuesheet_track;





DRFLAC_API void drflac_init_cuesheet_track_iterator(drflac_cuesheet_track_iterator* pIter, drflac_uint32 trackCount, const void* pTrackData);


DRFLAC_API drflac_bool32 drflac_next_cuesheet_track(drflac_cuesheet_track_iterator* pIter, drflac_cuesheet_track* pCuesheetTrack);


#ifdef __cplusplus
}
#endif
#endif  









#if defined(DR_FLAC_IMPLEMENTATION) || defined(DRFLAC_IMPLEMENTATION)
#ifndef dr_flac_c
#define dr_flac_c


#if defined(__clang__) || (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))
    #pragma GCC diagnostic push
    #if __GNUC__ >= 7
    #pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
    #endif
#endif

#ifdef __linux__
    #ifndef _BSD_SOURCE
        #define _BSD_SOURCE
    #endif
    #ifndef _DEFAULT_SOURCE
        #define _DEFAULT_SOURCE
    #endif
    #ifndef __USE_BSD
        #define __USE_BSD
    #endif
    #include <endian.h>
#endif

#include <stdlib.h>
#include <string.h>


#ifdef _MSC_VER
    #define DRFLAC_INLINE __forceinline
#elif defined(__GNUC__)
    






    #if defined(__STRICT_ANSI__)
        #define DRFLAC_GNUC_INLINE_HINT __inline__
    #else
        #define DRFLAC_GNUC_INLINE_HINT inline
    #endif

    #if (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 2)) || defined(__clang__)
        #define DRFLAC_INLINE DRFLAC_GNUC_INLINE_HINT __attribute__((always_inline))
    #else
        #define DRFLAC_INLINE DRFLAC_GNUC_INLINE_HINT
    #endif
#elif defined(__WATCOMC__)
    #define DRFLAC_INLINE __inline
#else
    #define DRFLAC_INLINE
#endif











#if !defined(DR_FLAC_NO_SIMD)
    #if defined(DRFLAC_X64) || defined(DRFLAC_X86)
        #if defined(_MSC_VER) && !defined(__clang__)
            
            #if _MSC_VER >= 1400 && !defined(DRFLAC_NO_SSE2)    
                #define DRFLAC_SUPPORT_SSE2
            #endif
            #if _MSC_VER >= 1600 && !defined(DRFLAC_NO_SSE41)   
                #define DRFLAC_SUPPORT_SSE41
            #endif
        #elif defined(__clang__) || (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3)))
            
            #if defined(__SSE2__) && !defined(DRFLAC_NO_SSE2)
                #define DRFLAC_SUPPORT_SSE2
            #endif
            #if defined(__SSE4_1__) && !defined(DRFLAC_NO_SSE41)
                #define DRFLAC_SUPPORT_SSE41
            #endif
        #endif

        
        #if !defined(__GNUC__) && !defined(__clang__) && defined(__has_include)
            #if !defined(DRFLAC_SUPPORT_SSE2) && !defined(DRFLAC_NO_SSE2) && __has_include(<emmintrin.h>)
                #define DRFLAC_SUPPORT_SSE2
            #endif
            #if !defined(DRFLAC_SUPPORT_SSE41) && !defined(DRFLAC_NO_SSE41) && __has_include(<smmintrin.h>)
                #define DRFLAC_SUPPORT_SSE41
            #endif
        #endif

        #if defined(DRFLAC_SUPPORT_SSE41)
            #include <smmintrin.h>
        #elif defined(DRFLAC_SUPPORT_SSE2)
            #include <emmintrin.h>
        #endif
    #endif

    #if defined(DRFLAC_ARM)
        #if !defined(DRFLAC_NO_NEON) && (defined(__ARM_NEON) || defined(__aarch64__) || defined(_M_ARM64))
            #define DRFLAC_SUPPORT_NEON
            #include <arm_neon.h>
        #endif
    #endif
#endif


#if !defined(DR_FLAC_NO_SIMD) && (defined(DRFLAC_X86) || defined(DRFLAC_X64))
    #if defined(_MSC_VER) && !defined(__clang__)
        #if _MSC_VER >= 1400
            #include <intrin.h>
            static void drflac__cpuid(int info[4], int fid)
            {
                __cpuid(info, fid);
            }
        #else
            #define DRFLAC_NO_CPUID
        #endif
    #else
        #if defined(__GNUC__) || defined(__clang__)
            static void drflac__cpuid(int info[4], int fid)
            {
                






                #if defined(DRFLAC_X86) && defined(__PIC__)
                    __asm__ __volatile__ (
                        "xchg{l} {%%}ebx, %k1;"
                        "cpuid;"
                        "xchg{l} {%%}ebx, %k1;"
                        : "=a"(info[0]), "=&r"(info[1]), "=c"(info[2]), "=d"(info[3]) : "a"(fid), "c"(0)
                    );
                #else
                    __asm__ __volatile__ (
                        "cpuid" : "=a"(info[0]), "=b"(info[1]), "=c"(info[2]), "=d"(info[3]) : "a"(fid), "c"(0)
                    );
                #endif
            }
        #else
            #define DRFLAC_NO_CPUID
        #endif
    #endif
#else
    #define DRFLAC_NO_CPUID
#endif

static DRFLAC_INLINE drflac_bool32 drflac_has_sse2(void)
{
#if defined(DRFLAC_SUPPORT_SSE2)
    #if (defined(DRFLAC_X64) || defined(DRFLAC_X86)) && !defined(DRFLAC_NO_SSE2)
        #if defined(DRFLAC_X64)
            return DRFLAC_TRUE;    
        #elif (defined(_M_IX86_FP) && _M_IX86_FP == 2) || defined(__SSE2__)
            return DRFLAC_TRUE;    
        #else
            #if defined(DRFLAC_NO_CPUID)
                return DRFLAC_FALSE;
            #else
                int info[4];
                drflac__cpuid(info, 1);
                return (info[3] & (1 << 26)) != 0;
            #endif
        #endif
    #else
        return DRFLAC_FALSE;       
    #endif
#else
    return DRFLAC_FALSE;           
#endif
}

static DRFLAC_INLINE drflac_bool32 drflac_has_sse41(void)
{
#if defined(DRFLAC_SUPPORT_SSE41)
    #if (defined(DRFLAC_X64) || defined(DRFLAC_X86)) && !defined(DRFLAC_NO_SSE41)
        #if defined(__SSE4_1__) || defined(__AVX__)
            return DRFLAC_TRUE;    
        #else
            #if defined(DRFLAC_NO_CPUID)
                return DRFLAC_FALSE;
            #else
                int info[4];
                drflac__cpuid(info, 1);
                return (info[2] & (1 << 19)) != 0;
            #endif
        #endif
    #else
        return DRFLAC_FALSE;       
    #endif
#else
    return DRFLAC_FALSE;           
#endif
}


#if defined(_MSC_VER) && _MSC_VER >= 1500 && (defined(DRFLAC_X86) || defined(DRFLAC_X64)) && !defined(__clang__)
    #define DRFLAC_HAS_LZCNT_INTRINSIC
#elif (defined(__GNUC__) && ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)))
    #define DRFLAC_HAS_LZCNT_INTRINSIC
#elif defined(__clang__)
    #if defined(__has_builtin)
        #if __has_builtin(__builtin_clzll) || __has_builtin(__builtin_clzl)
            #define DRFLAC_HAS_LZCNT_INTRINSIC
        #endif
    #endif
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1400 && !defined(__clang__)
    #define DRFLAC_HAS_BYTESWAP16_INTRINSIC
    #define DRFLAC_HAS_BYTESWAP32_INTRINSIC
    #define DRFLAC_HAS_BYTESWAP64_INTRINSIC
#elif defined(__clang__)
    #if defined(__has_builtin)
        #if __has_builtin(__builtin_bswap16)
            #define DRFLAC_HAS_BYTESWAP16_INTRINSIC
        #endif
        #if __has_builtin(__builtin_bswap32)
            #define DRFLAC_HAS_BYTESWAP32_INTRINSIC
        #endif
        #if __has_builtin(__builtin_bswap64)
            #define DRFLAC_HAS_BYTESWAP64_INTRINSIC
        #endif
    #endif
#elif defined(__GNUC__)
    #if ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
        #define DRFLAC_HAS_BYTESWAP32_INTRINSIC
        #define DRFLAC_HAS_BYTESWAP64_INTRINSIC
    #endif
    #if ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8))
        #define DRFLAC_HAS_BYTESWAP16_INTRINSIC
    #endif
#elif defined(__WATCOMC__) && defined(__386__)
    #define DRFLAC_HAS_BYTESWAP16_INTRINSIC
    #define DRFLAC_HAS_BYTESWAP32_INTRINSIC
    #define DRFLAC_HAS_BYTESWAP64_INTRINSIC
    extern __inline drflac_uint16 _watcom_bswap16(drflac_uint16);
    extern __inline drflac_uint32 _watcom_bswap32(drflac_uint32);
    extern __inline drflac_uint64 _watcom_bswap64(drflac_uint64);
#pragma aux _watcom_bswap16 = \
    "xchg al, ah" \
    parm  [ax]    \
    value [ax]    \
    modify nomemory;
#pragma aux _watcom_bswap32 = \
    "bswap eax" \
    parm  [eax] \
    value [eax] \
    modify nomemory;
#pragma aux _watcom_bswap64 = \
    "bswap eax"     \
    "bswap edx"     \
    "xchg eax,edx"  \
    parm [eax edx]  \
    value [eax edx] \
    modify nomemory;
#endif



#ifndef DRFLAC_ASSERT
#include <assert.h>
#define DRFLAC_ASSERT(expression)           assert(expression)
#endif
#ifndef DRFLAC_MALLOC
#define DRFLAC_MALLOC(sz)                   malloc((sz))
#endif
#ifndef DRFLAC_REALLOC
#define DRFLAC_REALLOC(p, sz)               realloc((p), (sz))
#endif
#ifndef DRFLAC_FREE
#define DRFLAC_FREE(p)                      free((p))
#endif
#ifndef DRFLAC_COPY_MEMORY
#define DRFLAC_COPY_MEMORY(dst, src, sz)    memcpy((dst), (src), (sz))
#endif
#ifndef DRFLAC_ZERO_MEMORY
#define DRFLAC_ZERO_MEMORY(p, sz)           memset((p), 0, (sz))
#endif
#ifndef DRFLAC_ZERO_OBJECT
#define DRFLAC_ZERO_OBJECT(p)               DRFLAC_ZERO_MEMORY((p), sizeof(*(p)))
#endif

#define DRFLAC_MIN(a, b)                    (((a) < (b)) ? (a) : (b))

#define DRFLAC_MAX_SIMD_VECTOR_SIZE                     64  


typedef drflac_int32 drflac_result;
#define DRFLAC_SUCCESS                                   0
#define DRFLAC_ERROR                                    -1   
#define DRFLAC_INVALID_ARGS                             -2
#define DRFLAC_INVALID_OPERATION                        -3
#define DRFLAC_OUT_OF_MEMORY                            -4
#define DRFLAC_OUT_OF_RANGE                             -5
#define DRFLAC_ACCESS_DENIED                            -6
#define DRFLAC_DOES_NOT_EXIST                           -7
#define DRFLAC_ALREADY_EXISTS                           -8
#define DRFLAC_TOO_MANY_OPEN_FILES                      -9
#define DRFLAC_INVALID_FILE                             -10
#define DRFLAC_TOO_BIG                                  -11
#define DRFLAC_PATH_TOO_LONG                            -12
#define DRFLAC_NAME_TOO_LONG                            -13
#define DRFLAC_NOT_DIRECTORY                            -14
#define DRFLAC_IS_DIRECTORY                             -15
#define DRFLAC_DIRECTORY_NOT_EMPTY                      -16
#define DRFLAC_END_OF_FILE                              -17
#define DRFLAC_NO_SPACE                                 -18
#define DRFLAC_BUSY                                     -19
#define DRFLAC_IO_ERROR                                 -20
#define DRFLAC_INTERRUPT                                -21
#define DRFLAC_UNAVAILABLE                              -22
#define DRFLAC_ALREADY_IN_USE                           -23
#define DRFLAC_BAD_ADDRESS                              -24
#define DRFLAC_BAD_SEEK                                 -25
#define DRFLAC_BAD_PIPE                                 -26
#define DRFLAC_DEADLOCK                                 -27
#define DRFLAC_TOO_MANY_LINKS                           -28
#define DRFLAC_NOT_IMPLEMENTED                          -29
#define DRFLAC_NO_MESSAGE                               -30
#define DRFLAC_BAD_MESSAGE                              -31
#define DRFLAC_NO_DATA_AVAILABLE                        -32
#define DRFLAC_INVALID_DATA                             -33
#define DRFLAC_TIMEOUT                                  -34
#define DRFLAC_NO_NETWORK                               -35
#define DRFLAC_NOT_UNIQUE                               -36
#define DRFLAC_NOT_SOCKET                               -37
#define DRFLAC_NO_ADDRESS                               -38
#define DRFLAC_BAD_PROTOCOL                             -39
#define DRFLAC_PROTOCOL_UNAVAILABLE                     -40
#define DRFLAC_PROTOCOL_NOT_SUPPORTED                   -41
#define DRFLAC_PROTOCOL_FAMILY_NOT_SUPPORTED            -42
#define DRFLAC_ADDRESS_FAMILY_NOT_SUPPORTED             -43
#define DRFLAC_SOCKET_NOT_SUPPORTED                     -44
#define DRFLAC_CONNECTION_RESET                         -45
#define DRFLAC_ALREADY_CONNECTED                        -46
#define DRFLAC_NOT_CONNECTED                            -47
#define DRFLAC_CONNECTION_REFUSED                       -48
#define DRFLAC_NO_HOST                                  -49
#define DRFLAC_IN_PROGRESS                              -50
#define DRFLAC_CANCELLED                                -51
#define DRFLAC_MEMORY_ALREADY_MAPPED                    -52
#define DRFLAC_AT_END                                   -53

#define DRFLAC_CRC_MISMATCH                             -100



#define DRFLAC_SUBFRAME_CONSTANT                        0
#define DRFLAC_SUBFRAME_VERBATIM                        1
#define DRFLAC_SUBFRAME_FIXED                           8
#define DRFLAC_SUBFRAME_LPC                             32
#define DRFLAC_SUBFRAME_RESERVED                        255

#define DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE  0
#define DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE2 1

#define DRFLAC_CHANNEL_ASSIGNMENT_INDEPENDENT_STEREO    1
#define DRFLAC_CHANNEL_ASSIGNMENT_LEFT_SIDE             8
#define DRFLAC_CHANNEL_ASSIGNMENT_RIGHT_SIDE            9
#define DRFLAC_CHANNEL_ASSIGNMENT_MID_SIDE              10

#define drflac_align(x, a)                              ((((x) + (a) - 1) / (a)) * (a))

#define DRFLAC_SEEKPOINT_SIZE_IN_BYTES                  18
#define DRFLAC_CUESHEET_TRACK_SIZE_IN_BYTES             36
#define DRFLAC_CUESHEET_TRACK_INDEX_SIZE_IN_BYTES       12
#define DRFLAC_CUESHEET_TRACK_PADDED_SIZE_IN_BYTES      drflac_align(DRFLAC_CUESHEET_TRACK_SIZE_IN_BYTES, sizeof(drflac_uint64))


DRFLAC_API void drflac_version(drflac_uint32* pMajor, drflac_uint32* pMinor, drflac_uint32* pRevision)
{
    if (pMajor) {
        *pMajor = DRFLAC_VERSION_MAJOR;
    }

    if (pMinor) {
        *pMinor = DRFLAC_VERSION_MINOR;
    }

    if (pRevision) {
        *pRevision = DRFLAC_VERSION_REVISION;
    }
}

DRFLAC_API const char* drflac_version_string(void)
{
    return DRFLAC_VERSION_STRING;
}



#if defined(__has_feature)
    #if __has_feature(thread_sanitizer)
        #define DRFLAC_NO_THREAD_SANITIZE __attribute__((no_sanitize("thread")))
    #else
        #define DRFLAC_NO_THREAD_SANITIZE
    #endif
#else
    #define DRFLAC_NO_THREAD_SANITIZE
#endif

#if defined(DRFLAC_HAS_LZCNT_INTRINSIC)
static drflac_bool32 drflac__gIsLZCNTSupported = DRFLAC_FALSE;
#endif

#ifndef DRFLAC_NO_CPUID
static drflac_bool32 drflac__gIsSSE2Supported  = DRFLAC_FALSE;
static drflac_bool32 drflac__gIsSSE41Supported = DRFLAC_FALSE;







DRFLAC_NO_THREAD_SANITIZE static void drflac__init_cpu_caps(void)
{
    static drflac_bool32 isCPUCapsInitialized = DRFLAC_FALSE;

    if (!isCPUCapsInitialized) {
        
#if defined(DRFLAC_HAS_LZCNT_INTRINSIC)
        int info[4] = {0};
        drflac__cpuid(info, 0x80000001);
        drflac__gIsLZCNTSupported = (info[2] & (1 << 5)) != 0;
#endif

        
        drflac__gIsSSE2Supported = drflac_has_sse2();

        
        drflac__gIsSSE41Supported = drflac_has_sse41();

        
        isCPUCapsInitialized = DRFLAC_TRUE;
    }
}
#else
static drflac_bool32 drflac__gIsNEONSupported  = DRFLAC_FALSE;

static DRFLAC_INLINE drflac_bool32 drflac__has_neon(void)
{
#if defined(DRFLAC_SUPPORT_NEON)
    #if defined(DRFLAC_ARM) && !defined(DRFLAC_NO_NEON)
        #if (defined(__ARM_NEON) || defined(__aarch64__) || defined(_M_ARM64))
            return DRFLAC_TRUE;    
        #else
            
            return DRFLAC_FALSE;
        #endif
    #else
        return DRFLAC_FALSE;       
    #endif
#else
    return DRFLAC_FALSE;           
#endif
}

DRFLAC_NO_THREAD_SANITIZE static void drflac__init_cpu_caps(void)
{
    drflac__gIsNEONSupported = drflac__has_neon();

#if defined(DRFLAC_HAS_LZCNT_INTRINSIC) && defined(DRFLAC_ARM) && (defined(__ARM_ARCH) && __ARM_ARCH >= 5)
    drflac__gIsLZCNTSupported = DRFLAC_TRUE;
#endif
}
#endif



static DRFLAC_INLINE drflac_bool32 drflac__is_little_endian(void)
{
#if defined(DRFLAC_X86) || defined(DRFLAC_X64)
    return DRFLAC_TRUE;
#elif defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && __BYTE_ORDER == __LITTLE_ENDIAN
    return DRFLAC_TRUE;
#else
    int n = 1;
    return (*(char*)&n) == 1;
#endif
}

static DRFLAC_INLINE drflac_uint16 drflac__swap_endian_uint16(drflac_uint16 n)
{
#ifdef DRFLAC_HAS_BYTESWAP16_INTRINSIC
    #if defined(_MSC_VER) && !defined(__clang__)
        return _byteswap_ushort(n);
    #elif defined(__GNUC__) || defined(__clang__)
        return __builtin_bswap16(n);
    #elif defined(__WATCOMC__) && defined(__386__)
        return _watcom_bswap16(n);
    #else
        #error "This compiler does not support the byte swap intrinsic."
    #endif
#else
    return ((n & 0xFF00) >> 8) |
           ((n & 0x00FF) << 8);
#endif
}

static DRFLAC_INLINE drflac_uint32 drflac__swap_endian_uint32(drflac_uint32 n)
{
#ifdef DRFLAC_HAS_BYTESWAP32_INTRINSIC
    #if defined(_MSC_VER) && !defined(__clang__)
        return _byteswap_ulong(n);
    #elif defined(__GNUC__) || defined(__clang__)
        #if defined(DRFLAC_ARM) && (defined(__ARM_ARCH) && __ARM_ARCH >= 6) && !defined(__ARM_ARCH_6M__) && !defined(DRFLAC_64BIT)   
            
            drflac_uint32 r;
            __asm__ __volatile__ (
            #if defined(DRFLAC_64BIT)
                "rev %w[out], %w[in]" : [out]"=r"(r) : [in]"r"(n)   
            #else
                "rev %[out], %[in]" : [out]"=r"(r) : [in]"r"(n)
            #endif
            );
            return r;
        #else
            return __builtin_bswap32(n);
        #endif
    #elif defined(__WATCOMC__) && defined(__386__)
        return _watcom_bswap32(n);
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

static DRFLAC_INLINE drflac_uint64 drflac__swap_endian_uint64(drflac_uint64 n)
{
#ifdef DRFLAC_HAS_BYTESWAP64_INTRINSIC
    #if defined(_MSC_VER) && !defined(__clang__)
        return _byteswap_uint64(n);
    #elif defined(__GNUC__) || defined(__clang__)
        return __builtin_bswap64(n);
    #elif defined(__WATCOMC__) && defined(__386__)
        return _watcom_bswap64(n);
    #else
        #error "This compiler does not support the byte swap intrinsic."
    #endif
#else
    
    return ((n & ((drflac_uint64)0xFF000000 << 32)) >> 56) |
           ((n & ((drflac_uint64)0x00FF0000 << 32)) >> 40) |
           ((n & ((drflac_uint64)0x0000FF00 << 32)) >> 24) |
           ((n & ((drflac_uint64)0x000000FF << 32)) >>  8) |
           ((n & ((drflac_uint64)0xFF000000      )) <<  8) |
           ((n & ((drflac_uint64)0x00FF0000      )) << 24) |
           ((n & ((drflac_uint64)0x0000FF00      )) << 40) |
           ((n & ((drflac_uint64)0x000000FF      )) << 56);
#endif
}


static DRFLAC_INLINE drflac_uint16 drflac__be2host_16(drflac_uint16 n)
{
    if (drflac__is_little_endian()) {
        return drflac__swap_endian_uint16(n);
    }

    return n;
}

static DRFLAC_INLINE drflac_uint32 drflac__be2host_32(drflac_uint32 n)
{
    if (drflac__is_little_endian()) {
        return drflac__swap_endian_uint32(n);
    }

    return n;
}

static DRFLAC_INLINE drflac_uint32 drflac__be2host_32_ptr_unaligned(const void* pData)
{
    const drflac_uint8* pData8 = (const drflac_uint8*)pData;

    return
        (drflac_uint32)pData8[0] << 24 |
        (drflac_uint32)pData8[1] << 16 |
        (drflac_uint32)pData8[2] <<  8 |
        (drflac_uint32)pData8[3] <<  0;
}

static DRFLAC_INLINE drflac_uint64 drflac__be2host_64(drflac_uint64 n)
{
    if (drflac__is_little_endian()) {
        return drflac__swap_endian_uint64(n);
    }

    return n;
}


static DRFLAC_INLINE drflac_uint32 drflac__le2host_32(drflac_uint32 n)
{
    if (!drflac__is_little_endian()) {
        return drflac__swap_endian_uint32(n);
    }

    return n;
}

static DRFLAC_INLINE drflac_uint32 drflac__le2host_32_ptr_unaligned(const void* pData)
{
    const drflac_uint8* pData8 = (const drflac_uint8*)pData;

    return
        (drflac_uint32)pData8[0] <<  0 |
        (drflac_uint32)pData8[1] <<  8 |
        (drflac_uint32)pData8[2] << 16 |
        (drflac_uint32)pData8[3] << 24;
}


static DRFLAC_INLINE drflac_uint32 drflac__unsynchsafe_32(drflac_uint32 n)
{
    drflac_uint32 result = 0;
    result |= (n & 0x7F000000) >> 3;
    result |= (n & 0x007F0000) >> 2;
    result |= (n & 0x00007F00) >> 1;
    result |= (n & 0x0000007F) >> 0;

    return result;
}




static drflac_uint8 drflac__crc8_table[] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
    0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, 0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
    0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
    0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2, 0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
    0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
    0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42, 0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
    0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
    0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
    0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
    0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B, 0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
    0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};

static drflac_uint16 drflac__crc16_table[] = {
    0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
    0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
    0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
    0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
    0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
    0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
    0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
    0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
    0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
    0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
    0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
    0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
    0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
    0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
    0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
    0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
    0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
    0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
    0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
    0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
    0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
    0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
    0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
    0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
    0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
    0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
    0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
    0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
    0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
    0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
    0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
    0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
};

static DRFLAC_INLINE drflac_uint8 drflac_crc8_byte(drflac_uint8 crc, drflac_uint8 data)
{
    return drflac__crc8_table[crc ^ data];
}

static DRFLAC_INLINE drflac_uint8 drflac_crc8(drflac_uint8 crc, drflac_uint32 data, drflac_uint32 count)
{
#ifdef DR_FLAC_NO_CRC
    (void)crc;
    (void)data;
    (void)count;
    return 0;
#else
#if 0
    
    drflac_uint8 p = 0x07;
    for (int i = count-1; i >= 0; --i) {
        drflac_uint8 bit = (data & (1 << i)) >> i;
        if (crc & 0x80) {
            crc = ((crc << 1) | bit) ^ p;
        } else {
            crc = ((crc << 1) | bit);
        }
    }
    return crc;
#else
    drflac_uint32 wholeBytes;
    drflac_uint32 leftoverBits;
    drflac_uint64 leftoverDataMask;

    static drflac_uint64 leftoverDataMaskTable[8] = {
        0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F
    };

    DRFLAC_ASSERT(count <= 32);

    wholeBytes = count >> 3;
    leftoverBits = count - (wholeBytes*8);
    leftoverDataMask = leftoverDataMaskTable[leftoverBits];

    switch (wholeBytes) {
        case 4: crc = drflac_crc8_byte(crc, (drflac_uint8)((data & (0xFF000000UL << leftoverBits)) >> (24 + leftoverBits)));
        case 3: crc = drflac_crc8_byte(crc, (drflac_uint8)((data & (0x00FF0000UL << leftoverBits)) >> (16 + leftoverBits)));
        case 2: crc = drflac_crc8_byte(crc, (drflac_uint8)((data & (0x0000FF00UL << leftoverBits)) >> ( 8 + leftoverBits)));
        case 1: crc = drflac_crc8_byte(crc, (drflac_uint8)((data & (0x000000FFUL << leftoverBits)) >> ( 0 + leftoverBits)));
        case 0: if (leftoverBits > 0) crc = (drflac_uint8)((crc << leftoverBits) ^ drflac__crc8_table[(crc >> (8 - leftoverBits)) ^ (data & leftoverDataMask)]);
    }
    return crc;
#endif
#endif
}

static DRFLAC_INLINE drflac_uint16 drflac_crc16_byte(drflac_uint16 crc, drflac_uint8 data)
{
    return (crc << 8) ^ drflac__crc16_table[(drflac_uint8)(crc >> 8) ^ data];
}

static DRFLAC_INLINE drflac_uint16 drflac_crc16_cache(drflac_uint16 crc, drflac_cache_t data)
{
#ifdef DRFLAC_64BIT
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 56) & 0xFF));
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 48) & 0xFF));
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 40) & 0xFF));
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 32) & 0xFF));
#endif
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 24) & 0xFF));
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 16) & 0xFF));
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data >>  8) & 0xFF));
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data >>  0) & 0xFF));

    return crc;
}

static DRFLAC_INLINE drflac_uint16 drflac_crc16_bytes(drflac_uint16 crc, drflac_cache_t data, drflac_uint32 byteCount)
{
    switch (byteCount)
    {
#ifdef DRFLAC_64BIT
    case 8: crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 56) & 0xFF));
    case 7: crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 48) & 0xFF));
    case 6: crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 40) & 0xFF));
    case 5: crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 32) & 0xFF));
#endif
    case 4: crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 24) & 0xFF));
    case 3: crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 16) & 0xFF));
    case 2: crc = drflac_crc16_byte(crc, (drflac_uint8)((data >>  8) & 0xFF));
    case 1: crc = drflac_crc16_byte(crc, (drflac_uint8)((data >>  0) & 0xFF));
    }

    return crc;
}

#if 0
static DRFLAC_INLINE drflac_uint16 drflac_crc16__32bit(drflac_uint16 crc, drflac_uint32 data, drflac_uint32 count)
{
#ifdef DR_FLAC_NO_CRC
    (void)crc;
    (void)data;
    (void)count;
    return 0;
#else
#if 0
    
    drflac_uint16 p = 0x8005;
    for (int i = count-1; i >= 0; --i) {
        drflac_uint16 bit = (data & (1ULL << i)) >> i;
        if (r & 0x8000) {
            r = ((r << 1) | bit) ^ p;
        } else {
            r = ((r << 1) | bit);
        }
    }

    return crc;
#else
    drflac_uint32 wholeBytes;
    drflac_uint32 leftoverBits;
    drflac_uint64 leftoverDataMask;

    static drflac_uint64 leftoverDataMaskTable[8] = {
        0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F
    };

    DRFLAC_ASSERT(count <= 64);

    wholeBytes = count >> 3;
    leftoverBits = count & 7;
    leftoverDataMask = leftoverDataMaskTable[leftoverBits];

    switch (wholeBytes) {
        default:
        case 4: crc = drflac_crc16_byte(crc, (drflac_uint8)((data & (0xFF000000UL << leftoverBits)) >> (24 + leftoverBits)));
        case 3: crc = drflac_crc16_byte(crc, (drflac_uint8)((data & (0x00FF0000UL << leftoverBits)) >> (16 + leftoverBits)));
        case 2: crc = drflac_crc16_byte(crc, (drflac_uint8)((data & (0x0000FF00UL << leftoverBits)) >> ( 8 + leftoverBits)));
        case 1: crc = drflac_crc16_byte(crc, (drflac_uint8)((data & (0x000000FFUL << leftoverBits)) >> ( 0 + leftoverBits)));
        case 0: if (leftoverBits > 0) crc = (crc << leftoverBits) ^ drflac__crc16_table[(crc >> (16 - leftoverBits)) ^ (data & leftoverDataMask)];
    }
    return crc;
#endif
#endif
}

static DRFLAC_INLINE drflac_uint16 drflac_crc16__64bit(drflac_uint16 crc, drflac_uint64 data, drflac_uint32 count)
{
#ifdef DR_FLAC_NO_CRC
    (void)crc;
    (void)data;
    (void)count;
    return 0;
#else
    drflac_uint32 wholeBytes;
    drflac_uint32 leftoverBits;
    drflac_uint64 leftoverDataMask;

    static drflac_uint64 leftoverDataMaskTable[8] = {
        0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F
    };

    DRFLAC_ASSERT(count <= 64);

    wholeBytes = count >> 3;
    leftoverBits = count & 7;
    leftoverDataMask = leftoverDataMaskTable[leftoverBits];

    switch (wholeBytes) {
        default:
        case 8: crc = drflac_crc16_byte(crc, (drflac_uint8)((data & (((drflac_uint64)0xFF000000 << 32) << leftoverBits)) >> (56 + leftoverBits)));    
        case 7: crc = drflac_crc16_byte(crc, (drflac_uint8)((data & (((drflac_uint64)0x00FF0000 << 32) << leftoverBits)) >> (48 + leftoverBits)));
        case 6: crc = drflac_crc16_byte(crc, (drflac_uint8)((data & (((drflac_uint64)0x0000FF00 << 32) << leftoverBits)) >> (40 + leftoverBits)));
        case 5: crc = drflac_crc16_byte(crc, (drflac_uint8)((data & (((drflac_uint64)0x000000FF << 32) << leftoverBits)) >> (32 + leftoverBits)));
        case 4: crc = drflac_crc16_byte(crc, (drflac_uint8)((data & (((drflac_uint64)0xFF000000      ) << leftoverBits)) >> (24 + leftoverBits)));
        case 3: crc = drflac_crc16_byte(crc, (drflac_uint8)((data & (((drflac_uint64)0x00FF0000      ) << leftoverBits)) >> (16 + leftoverBits)));
        case 2: crc = drflac_crc16_byte(crc, (drflac_uint8)((data & (((drflac_uint64)0x0000FF00      ) << leftoverBits)) >> ( 8 + leftoverBits)));
        case 1: crc = drflac_crc16_byte(crc, (drflac_uint8)((data & (((drflac_uint64)0x000000FF      ) << leftoverBits)) >> ( 0 + leftoverBits)));
        case 0: if (leftoverBits > 0) crc = (crc << leftoverBits) ^ drflac__crc16_table[(crc >> (16 - leftoverBits)) ^ (data & leftoverDataMask)];
    }
    return crc;
#endif
}


static DRFLAC_INLINE drflac_uint16 drflac_crc16(drflac_uint16 crc, drflac_cache_t data, drflac_uint32 count)
{
#ifdef DRFLAC_64BIT
    return drflac_crc16__64bit(crc, data, count);
#else
    return drflac_crc16__32bit(crc, data, count);
#endif
}
#endif


#ifdef DRFLAC_64BIT
#define drflac__be2host__cache_line drflac__be2host_64
#else
#define drflac__be2host__cache_line drflac__be2host_32
#endif










#define DRFLAC_CACHE_L1_SIZE_BYTES(bs)                      (sizeof((bs)->cache))
#define DRFLAC_CACHE_L1_SIZE_BITS(bs)                       (sizeof((bs)->cache)*8)
#define DRFLAC_CACHE_L1_BITS_REMAINING(bs)                  (DRFLAC_CACHE_L1_SIZE_BITS(bs) - (bs)->consumedBits)
#define DRFLAC_CACHE_L1_SELECTION_MASK(_bitCount)           (~((~(drflac_cache_t)0) >> (_bitCount)))
#define DRFLAC_CACHE_L1_SELECTION_SHIFT(bs, _bitCount)      (DRFLAC_CACHE_L1_SIZE_BITS(bs) - (_bitCount))
#define DRFLAC_CACHE_L1_SELECT(bs, _bitCount)               (((bs)->cache) & DRFLAC_CACHE_L1_SELECTION_MASK(_bitCount))
#define DRFLAC_CACHE_L1_SELECT_AND_SHIFT(bs, _bitCount)     (DRFLAC_CACHE_L1_SELECT((bs), (_bitCount)) >>  DRFLAC_CACHE_L1_SELECTION_SHIFT((bs), (_bitCount)))
#define DRFLAC_CACHE_L1_SELECT_AND_SHIFT_SAFE(bs, _bitCount)(DRFLAC_CACHE_L1_SELECT((bs), (_bitCount)) >> (DRFLAC_CACHE_L1_SELECTION_SHIFT((bs), (_bitCount)) & (DRFLAC_CACHE_L1_SIZE_BITS(bs)-1)))
#define DRFLAC_CACHE_L2_SIZE_BYTES(bs)                      (sizeof((bs)->cacheL2))
#define DRFLAC_CACHE_L2_LINE_COUNT(bs)                      (DRFLAC_CACHE_L2_SIZE_BYTES(bs) / sizeof((bs)->cacheL2[0]))
#define DRFLAC_CACHE_L2_LINES_REMAINING(bs)                 (DRFLAC_CACHE_L2_LINE_COUNT(bs) - (bs)->nextL2Line)


#ifndef DR_FLAC_NO_CRC
static DRFLAC_INLINE void drflac__reset_crc16(drflac_bs* bs)
{
    bs->crc16 = 0;
    bs->crc16CacheIgnoredBytes = bs->consumedBits >> 3;
}

static DRFLAC_INLINE void drflac__update_crc16(drflac_bs* bs)
{
    if (bs->crc16CacheIgnoredBytes == 0) {
        bs->crc16 = drflac_crc16_cache(bs->crc16, bs->crc16Cache);
    } else {
        bs->crc16 = drflac_crc16_bytes(bs->crc16, bs->crc16Cache, DRFLAC_CACHE_L1_SIZE_BYTES(bs) - bs->crc16CacheIgnoredBytes);
        bs->crc16CacheIgnoredBytes = 0;
    }
}

static DRFLAC_INLINE drflac_uint16 drflac__flush_crc16(drflac_bs* bs)
{
    
    DRFLAC_ASSERT((DRFLAC_CACHE_L1_BITS_REMAINING(bs) & 7) == 0);

    



    if (DRFLAC_CACHE_L1_BITS_REMAINING(bs) == 0) {
        drflac__update_crc16(bs);
    } else {
        
        bs->crc16 = drflac_crc16_bytes(bs->crc16, bs->crc16Cache >> DRFLAC_CACHE_L1_BITS_REMAINING(bs), (bs->consumedBits >> 3) - bs->crc16CacheIgnoredBytes);

        



        bs->crc16CacheIgnoredBytes = bs->consumedBits >> 3;
    }

    return bs->crc16;
}
#endif

static DRFLAC_INLINE drflac_bool32 drflac__reload_l1_cache_from_l2(drflac_bs* bs)
{
    size_t bytesRead;
    size_t alignedL1LineCount;

    
    if (bs->nextL2Line < DRFLAC_CACHE_L2_LINE_COUNT(bs)) {
        bs->cache = bs->cacheL2[bs->nextL2Line++];
        return DRFLAC_TRUE;
    }

    



    if (bs->unalignedByteCount > 0) {
        return DRFLAC_FALSE;   
    }

    bytesRead = bs->onRead(bs->pUserData, bs->cacheL2, DRFLAC_CACHE_L2_SIZE_BYTES(bs));

    bs->nextL2Line = 0;
    if (bytesRead == DRFLAC_CACHE_L2_SIZE_BYTES(bs)) {
        bs->cache = bs->cacheL2[bs->nextL2Line++];
        return DRFLAC_TRUE;
    }


    





    alignedL1LineCount = bytesRead / DRFLAC_CACHE_L1_SIZE_BYTES(bs);

    
    bs->unalignedByteCount = bytesRead - (alignedL1LineCount * DRFLAC_CACHE_L1_SIZE_BYTES(bs));
    if (bs->unalignedByteCount > 0) {
        bs->unalignedCache = bs->cacheL2[alignedL1LineCount];
    }

    if (alignedL1LineCount > 0) {
        size_t offset = DRFLAC_CACHE_L2_LINE_COUNT(bs) - alignedL1LineCount;
        size_t i;
        for (i = alignedL1LineCount; i > 0; --i) {
            bs->cacheL2[i-1 + offset] = bs->cacheL2[i-1];
        }

        bs->nextL2Line = (drflac_uint32)offset;
        bs->cache = bs->cacheL2[bs->nextL2Line++];
        return DRFLAC_TRUE;
    } else {
        
        bs->nextL2Line = DRFLAC_CACHE_L2_LINE_COUNT(bs);
        return DRFLAC_FALSE;
    }
}

static drflac_bool32 drflac__reload_cache(drflac_bs* bs)
{
    size_t bytesRead;

#ifndef DR_FLAC_NO_CRC
    drflac__update_crc16(bs);
#endif

    
    if (drflac__reload_l1_cache_from_l2(bs)) {
        bs->cache = drflac__be2host__cache_line(bs->cache);
        bs->consumedBits = 0;
#ifndef DR_FLAC_NO_CRC
        bs->crc16Cache = bs->cache;
#endif
        return DRFLAC_TRUE;
    }

    

    




    bytesRead = bs->unalignedByteCount;
    if (bytesRead == 0) {
        bs->consumedBits = DRFLAC_CACHE_L1_SIZE_BITS(bs);   
        return DRFLAC_FALSE;
    }

    DRFLAC_ASSERT(bytesRead < DRFLAC_CACHE_L1_SIZE_BYTES(bs));
    bs->consumedBits = (drflac_uint32)(DRFLAC_CACHE_L1_SIZE_BYTES(bs) - bytesRead) * 8;

    bs->cache = drflac__be2host__cache_line(bs->unalignedCache);
    bs->cache &= DRFLAC_CACHE_L1_SELECTION_MASK(DRFLAC_CACHE_L1_BITS_REMAINING(bs));    
    bs->unalignedByteCount = 0;     

#ifndef DR_FLAC_NO_CRC
    bs->crc16Cache = bs->cache >> bs->consumedBits;
    bs->crc16CacheIgnoredBytes = bs->consumedBits >> 3;
#endif
    return DRFLAC_TRUE;
}

static void drflac__reset_cache(drflac_bs* bs)
{
    bs->nextL2Line   = DRFLAC_CACHE_L2_LINE_COUNT(bs);  
    bs->consumedBits = DRFLAC_CACHE_L1_SIZE_BITS(bs);   
    bs->cache = 0;
    bs->unalignedByteCount = 0;                         
    bs->unalignedCache = 0;

#ifndef DR_FLAC_NO_CRC
    bs->crc16Cache = 0;
    bs->crc16CacheIgnoredBytes = 0;
#endif
}


static DRFLAC_INLINE drflac_bool32 drflac__read_uint32(drflac_bs* bs, unsigned int bitCount, drflac_uint32* pResultOut)
{
    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(pResultOut != NULL);
    DRFLAC_ASSERT(bitCount > 0);
    DRFLAC_ASSERT(bitCount <= 32);

    if (bs->consumedBits == DRFLAC_CACHE_L1_SIZE_BITS(bs)) {
        if (!drflac__reload_cache(bs)) {
            return DRFLAC_FALSE;
        }
    }

    if (bitCount <= DRFLAC_CACHE_L1_BITS_REMAINING(bs)) {
        




#ifdef DRFLAC_64BIT
        *pResultOut = (drflac_uint32)DRFLAC_CACHE_L1_SELECT_AND_SHIFT(bs, bitCount);
        bs->consumedBits += bitCount;
        bs->cache <<= bitCount;
#else
        if (bitCount < DRFLAC_CACHE_L1_SIZE_BITS(bs)) {
            *pResultOut = (drflac_uint32)DRFLAC_CACHE_L1_SELECT_AND_SHIFT(bs, bitCount);
            bs->consumedBits += bitCount;
            bs->cache <<= bitCount;
        } else {
            
            *pResultOut = (drflac_uint32)bs->cache;
            bs->consumedBits = DRFLAC_CACHE_L1_SIZE_BITS(bs);
            bs->cache = 0;
        }
#endif

        return DRFLAC_TRUE;
    } else {
        
        drflac_uint32 bitCountHi = DRFLAC_CACHE_L1_BITS_REMAINING(bs);
        drflac_uint32 bitCountLo = bitCount - bitCountHi;
        drflac_uint32 resultHi;

        DRFLAC_ASSERT(bitCountHi > 0);
        DRFLAC_ASSERT(bitCountHi < 32);
        resultHi = (drflac_uint32)DRFLAC_CACHE_L1_SELECT_AND_SHIFT(bs, bitCountHi);

        if (!drflac__reload_cache(bs)) {
            return DRFLAC_FALSE;
        }
        if (bitCountLo > DRFLAC_CACHE_L1_BITS_REMAINING(bs)) {
            
            return DRFLAC_FALSE;
        }

        *pResultOut = (resultHi << bitCountLo) | (drflac_uint32)DRFLAC_CACHE_L1_SELECT_AND_SHIFT(bs, bitCountLo);
        bs->consumedBits += bitCountLo;
        bs->cache <<= bitCountLo;
        return DRFLAC_TRUE;
    }
}

static drflac_bool32 drflac__read_int32(drflac_bs* bs, unsigned int bitCount, drflac_int32* pResult)
{
    drflac_uint32 result;

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(pResult != NULL);
    DRFLAC_ASSERT(bitCount > 0);
    DRFLAC_ASSERT(bitCount <= 32);

    if (!drflac__read_uint32(bs, bitCount, &result)) {
        return DRFLAC_FALSE;
    }

    
    if (bitCount < 32) {
        drflac_uint32 signbit;
        signbit = ((result >> (bitCount-1)) & 0x01);
        result |= (~signbit + 1) << bitCount;
    }

    *pResult = (drflac_int32)result;
    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__read_uint64(drflac_bs* bs, unsigned int bitCount, drflac_uint64* pResultOut)
{
    drflac_uint32 resultHi = 0;
    drflac_uint32 resultLo;

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(pResultOut != NULL);
    DRFLAC_ASSERT(bitCount > 0);
    DRFLAC_ASSERT(bitCount <= 64);

    if (bitCount > 32) {
        if (!drflac__read_uint32(bs, bitCount - 32, &resultHi)) {
            return DRFLAC_FALSE;
        }

        bitCount = 32;
    }

    if (!drflac__read_uint32(bs, bitCount, &resultLo)) {
        return DRFLAC_FALSE;
    }

    *pResultOut = (((drflac_uint64)resultHi) << 32) | ((drflac_uint64)resultLo);
    return DRFLAC_TRUE;
}

static DRFLAC_INLINE drflac_bool32 drflac__read_int64(drflac_bs* bs, unsigned int bitCount, drflac_int64* pResultOut)
{
    drflac_uint64 result;

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(pResultOut != NULL);
    DRFLAC_ASSERT(bitCount > 0);
    DRFLAC_ASSERT(bitCount <= 64);

    if (!drflac__read_uint64(bs, bitCount, &result)) {
        return DRFLAC_FALSE;
    }

    
    if (bitCount < 64) {
        drflac_uint64 signbit;
        signbit = ((result >> (bitCount-1)) & 0x01);
        result |= (~signbit + 1) << bitCount;
    }

    *pResultOut = (drflac_int64)result;
    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__read_uint16(drflac_bs* bs, unsigned int bitCount, drflac_uint16* pResult)
{
    drflac_uint32 result;

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(pResult != NULL);
    DRFLAC_ASSERT(bitCount > 0);
    DRFLAC_ASSERT(bitCount <= 16);

    if (!drflac__read_uint32(bs, bitCount, &result)) {
        return DRFLAC_FALSE;
    }

    *pResult = (drflac_uint16)result;
    return DRFLAC_TRUE;
}

#if 0
static drflac_bool32 drflac__read_int16(drflac_bs* bs, unsigned int bitCount, drflac_int16* pResult)
{
    drflac_int32 result;

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(pResult != NULL);
    DRFLAC_ASSERT(bitCount > 0);
    DRFLAC_ASSERT(bitCount <= 16);

    if (!drflac__read_int32(bs, bitCount, &result)) {
        return DRFLAC_FALSE;
    }

    *pResult = (drflac_int16)result;
    return DRFLAC_TRUE;
}
#endif

static drflac_bool32 drflac__read_uint8(drflac_bs* bs, unsigned int bitCount, drflac_uint8* pResult)
{
    drflac_uint32 result;

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(pResult != NULL);
    DRFLAC_ASSERT(bitCount > 0);
    DRFLAC_ASSERT(bitCount <= 8);

    if (!drflac__read_uint32(bs, bitCount, &result)) {
        return DRFLAC_FALSE;
    }

    *pResult = (drflac_uint8)result;
    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__read_int8(drflac_bs* bs, unsigned int bitCount, drflac_int8* pResult)
{
    drflac_int32 result;

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(pResult != NULL);
    DRFLAC_ASSERT(bitCount > 0);
    DRFLAC_ASSERT(bitCount <= 8);

    if (!drflac__read_int32(bs, bitCount, &result)) {
        return DRFLAC_FALSE;
    }

    *pResult = (drflac_int8)result;
    return DRFLAC_TRUE;
}


static drflac_bool32 drflac__seek_bits(drflac_bs* bs, size_t bitsToSeek)
{
    if (bitsToSeek <= DRFLAC_CACHE_L1_BITS_REMAINING(bs)) {
        bs->consumedBits += (drflac_uint32)bitsToSeek;
        bs->cache <<= bitsToSeek;
        return DRFLAC_TRUE;
    } else {
        
        bitsToSeek       -= DRFLAC_CACHE_L1_BITS_REMAINING(bs);
        bs->consumedBits += DRFLAC_CACHE_L1_BITS_REMAINING(bs);
        bs->cache         = 0;

        
#ifdef DRFLAC_64BIT
        while (bitsToSeek >= DRFLAC_CACHE_L1_SIZE_BITS(bs)) {
            drflac_uint64 bin;
            if (!drflac__read_uint64(bs, DRFLAC_CACHE_L1_SIZE_BITS(bs), &bin)) {
                return DRFLAC_FALSE;
            }
            bitsToSeek -= DRFLAC_CACHE_L1_SIZE_BITS(bs);
        }
#else
        while (bitsToSeek >= DRFLAC_CACHE_L1_SIZE_BITS(bs)) {
            drflac_uint32 bin;
            if (!drflac__read_uint32(bs, DRFLAC_CACHE_L1_SIZE_BITS(bs), &bin)) {
                return DRFLAC_FALSE;
            }
            bitsToSeek -= DRFLAC_CACHE_L1_SIZE_BITS(bs);
        }
#endif

        
        while (bitsToSeek >= 8) {
            drflac_uint8 bin;
            if (!drflac__read_uint8(bs, 8, &bin)) {
                return DRFLAC_FALSE;
            }
            bitsToSeek -= 8;
        }

        
        if (bitsToSeek > 0) {
            drflac_uint8 bin;
            if (!drflac__read_uint8(bs, (drflac_uint32)bitsToSeek, &bin)) {
                return DRFLAC_FALSE;
            }
            bitsToSeek = 0; 
        }

        DRFLAC_ASSERT(bitsToSeek == 0);
        return DRFLAC_TRUE;
    }
}



static drflac_bool32 drflac__find_and_seek_to_next_sync_code(drflac_bs* bs)
{
    DRFLAC_ASSERT(bs != NULL);

    



    if (!drflac__seek_bits(bs, DRFLAC_CACHE_L1_BITS_REMAINING(bs) & 7)) {
        return DRFLAC_FALSE;
    }

    for (;;) {
        drflac_uint8 hi;

#ifndef DR_FLAC_NO_CRC
        drflac__reset_crc16(bs);
#endif

        if (!drflac__read_uint8(bs, 8, &hi)) {
            return DRFLAC_FALSE;
        }

        while (hi == 0xFF) {
            drflac_uint8 lo;
            drflac_uint8 trailingBits;
            if (!drflac__read_uint8(bs, 6, &lo)) {
                return DRFLAC_FALSE;
            }

            if (lo == 0x3E) {
                return DRFLAC_TRUE;
            }

            if (!drflac__read_uint8(bs, 2, &trailingBits)) {
                return DRFLAC_FALSE;
            }

            hi = (drflac_uint8)((lo << 2) | trailingBits);
            if (hi == 0xFF) {
                
                #ifndef DR_FLAC_NO_CRC
                {
                    drflac__reset_crc16(bs);
                    bs->crc16 = drflac_crc16_byte(0, hi);
                }
                #endif
            }
        }
    }

    
    
}


#if defined(DRFLAC_HAS_LZCNT_INTRINSIC)
#define DRFLAC_IMPLEMENT_CLZ_LZCNT
#endif
#if  defined(_MSC_VER) && _MSC_VER >= 1400 && (defined(DRFLAC_X64) || defined(DRFLAC_X86)) && !defined(__clang__)
#define DRFLAC_IMPLEMENT_CLZ_MSVC
#endif
#if  defined(__WATCOMC__) && defined(__386__)
#define DRFLAC_IMPLEMENT_CLZ_WATCOM
#endif
#ifdef __MRC__
#include <intrinsics.h>
#define DRFLAC_IMPLEMENT_CLZ_MRC
#endif

static DRFLAC_INLINE drflac_uint32 drflac__clz_software(drflac_cache_t x)
{
    drflac_uint32 n;
    static drflac_uint32 clz_table_4[] = {
        0,
        4,
        3, 3,
        2, 2, 2, 2,
        1, 1, 1, 1, 1, 1, 1, 1
    };

    if (x == 0) {
        return sizeof(x)*8;
    }

    n = clz_table_4[x >> (sizeof(x)*8 - 4)];
    if (n == 0) {
#ifdef DRFLAC_64BIT
        if ((x & ((drflac_uint64)0xFFFFFFFF << 32)) == 0) { n  = 32; x <<= 32; }
        if ((x & ((drflac_uint64)0xFFFF0000 << 32)) == 0) { n += 16; x <<= 16; }
        if ((x & ((drflac_uint64)0xFF000000 << 32)) == 0) { n += 8;  x <<= 8;  }
        if ((x & ((drflac_uint64)0xF0000000 << 32)) == 0) { n += 4;  x <<= 4;  }
#else
        if ((x & 0xFFFF0000) == 0) { n  = 16; x <<= 16; }
        if ((x & 0xFF000000) == 0) { n += 8;  x <<= 8;  }
        if ((x & 0xF0000000) == 0) { n += 4;  x <<= 4;  }
#endif
        n += clz_table_4[x >> (sizeof(x)*8 - 4)];
    }

    return n - 1;
}

#ifdef DRFLAC_IMPLEMENT_CLZ_LZCNT
static DRFLAC_INLINE drflac_bool32 drflac__is_lzcnt_supported(void)
{
    
#if defined(DRFLAC_HAS_LZCNT_INTRINSIC) && defined(DRFLAC_ARM) && (defined(__ARM_ARCH) && __ARM_ARCH >= 5)
    return DRFLAC_TRUE;
#elif defined(__MRC__)
    return DRFLAC_TRUE;
#else
    
    #ifdef DRFLAC_HAS_LZCNT_INTRINSIC
        return drflac__gIsLZCNTSupported;
    #else
        return DRFLAC_FALSE;
    #endif
#endif
}

static DRFLAC_INLINE drflac_uint32 drflac__clz_lzcnt(drflac_cache_t x)
{
    

















#if defined(_MSC_VER)     
    #ifdef DRFLAC_64BIT
        return (drflac_uint32)__lzcnt64(x);
    #else
        return (drflac_uint32)__lzcnt(x);
    #endif
#else
    #if defined(__GNUC__) || defined(__clang__)
        #if defined(DRFLAC_X64)
            {
                






                drflac_uint64 r;
                __asm__ __volatile__ (
                    "rep; bsr{q %1, %0| %0, %1}" : "=r"(r) : "r"(x) : "cc"
                    
                );

                return (drflac_uint32)r;
            }
        #elif defined(DRFLAC_X86)
            {
                drflac_uint32 r;
                __asm__ __volatile__ (
                    "rep; bsr{l %1, %0| %0, %1}" : "=r"(r) : "r"(x) : "cc"
                    
                );

                return r;
            }
        #elif defined(DRFLAC_ARM) && (defined(__ARM_ARCH) && __ARM_ARCH >= 5) && !defined(__ARM_ARCH_6M__) && !(defined(__thumb__) && !defined(__thumb2__)) && !defined(DRFLAC_64BIT)   
            {
                unsigned int r;
                __asm__ __volatile__ (
                #if defined(DRFLAC_64BIT)
                    "clz %w[out], %w[in]" : [out]"=r"(r) : [in]"r"(x)   
                #else
                    "clz %[out], %[in]" : [out]"=r"(r) : [in]"r"(x)
                #endif
                );

                return r;
            }
        #else
            if (x == 0) {
                return sizeof(x)*8;
            }
            #ifdef DRFLAC_64BIT
                return (drflac_uint32)__builtin_clzll((drflac_uint64)x);
            #else
                return (drflac_uint32)__builtin_clzl((drflac_uint32)x);
            #endif
        #endif
    #else
        
        #error "This compiler does not support the lzcnt intrinsic."
    #endif
#endif
}
#endif

#ifdef DRFLAC_IMPLEMENT_CLZ_MSVC
#include <intrin.h> 

static DRFLAC_INLINE drflac_uint32 drflac__clz_msvc(drflac_cache_t x)
{
    drflac_uint32 n;

    if (x == 0) {
        return sizeof(x)*8;
    }

#ifdef DRFLAC_64BIT
    _BitScanReverse64((unsigned long*)&n, x);
#else
    _BitScanReverse((unsigned long*)&n, x);
#endif
    return sizeof(x)*8 - n - 1;
}
#endif

#ifdef DRFLAC_IMPLEMENT_CLZ_WATCOM
static __inline drflac_uint32 drflac__clz_watcom (drflac_uint32);
#ifdef DRFLAC_IMPLEMENT_CLZ_WATCOM_LZCNT

#pragma aux drflac__clz_watcom_lzcnt = \
    "db 0F3h, 0Fh, 0BDh, 0C0h"  \
    parm [eax] \
    value [eax] \
    modify nomemory;
#else

#pragma aux drflac__clz_watcom = \
    "bsr eax, eax" \
    "xor eax, 31" \
    parm [eax] nomemory \
    value [eax] \
    modify exact [eax] nomemory;
#endif
#endif

static DRFLAC_INLINE drflac_uint32 drflac__clz(drflac_cache_t x)
{
#ifdef DRFLAC_IMPLEMENT_CLZ_LZCNT
    if (drflac__is_lzcnt_supported()) {
        return drflac__clz_lzcnt(x);
    } else
#endif
    {
#ifdef DRFLAC_IMPLEMENT_CLZ_MSVC
        return drflac__clz_msvc(x);
#elif defined(DRFLAC_IMPLEMENT_CLZ_WATCOM_LZCNT)
        return drflac__clz_watcom_lzcnt(x);
#elif defined(DRFLAC_IMPLEMENT_CLZ_WATCOM)
        return (x == 0) ? sizeof(x)*8 : drflac__clz_watcom(x);
#elif defined(__MRC__)
        return __cntlzw(x);
#else
        return drflac__clz_software(x);
#endif
    }
}


static DRFLAC_INLINE drflac_bool32 drflac__seek_past_next_set_bit(drflac_bs* bs, unsigned int* pOffsetOut)
{
    drflac_uint32 zeroCounter = 0;
    drflac_uint32 setBitOffsetPlus1;

    while (bs->cache == 0) {
        zeroCounter += (drflac_uint32)DRFLAC_CACHE_L1_BITS_REMAINING(bs);
        if (!drflac__reload_cache(bs)) {
            return DRFLAC_FALSE;
        }
    }

    if (bs->cache == 1) {
        
        *pOffsetOut = zeroCounter + (drflac_uint32)DRFLAC_CACHE_L1_BITS_REMAINING(bs) - 1;
        if (!drflac__reload_cache(bs)) {
            return DRFLAC_FALSE;
        }

        return DRFLAC_TRUE;
    }

    setBitOffsetPlus1 = drflac__clz(bs->cache);
    setBitOffsetPlus1 += 1;

    if (setBitOffsetPlus1 > DRFLAC_CACHE_L1_BITS_REMAINING(bs)) {
        
        return DRFLAC_FALSE;
    }

    bs->consumedBits += setBitOffsetPlus1;
    bs->cache <<= setBitOffsetPlus1;

    *pOffsetOut = zeroCounter + setBitOffsetPlus1 - 1;
    return DRFLAC_TRUE;
}



static drflac_bool32 drflac__seek_to_byte(drflac_bs* bs, drflac_uint64 offsetFromStart)
{
    DRFLAC_ASSERT(bs != NULL);

    




    if (offsetFromStart > 0x7FFFFFFF) {
        drflac_uint64 bytesRemaining = offsetFromStart;
        if (!bs->onSeek(bs->pUserData, 0x7FFFFFFF, DRFLAC_SEEK_SET)) {
            return DRFLAC_FALSE;
        }
        bytesRemaining -= 0x7FFFFFFF;

        while (bytesRemaining > 0x7FFFFFFF) {
            if (!bs->onSeek(bs->pUserData, 0x7FFFFFFF, DRFLAC_SEEK_CUR)) {
                return DRFLAC_FALSE;
            }
            bytesRemaining -= 0x7FFFFFFF;
        }

        if (bytesRemaining > 0) {
            if (!bs->onSeek(bs->pUserData, (int)bytesRemaining, DRFLAC_SEEK_CUR)) {
                return DRFLAC_FALSE;
            }
        }
    } else {
        if (!bs->onSeek(bs->pUserData, (int)offsetFromStart, DRFLAC_SEEK_SET)) {
            return DRFLAC_FALSE;
        }
    }

    
    drflac__reset_cache(bs);
    return DRFLAC_TRUE;
}


static drflac_result drflac__read_utf8_coded_number(drflac_bs* bs, drflac_uint64* pNumberOut, drflac_uint8* pCRCOut)
{
    drflac_uint8 crc;
    drflac_uint64 result;
    drflac_uint8 utf8[7] = {0};
    int byteCount;
    int i;

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(pNumberOut != NULL);
    DRFLAC_ASSERT(pCRCOut != NULL);

    crc = *pCRCOut;

    if (!drflac__read_uint8(bs, 8, utf8)) {
        *pNumberOut = 0;
        return DRFLAC_AT_END;
    }
    crc = drflac_crc8(crc, utf8[0], 8);

    if ((utf8[0] & 0x80) == 0) {
        *pNumberOut = utf8[0];
        *pCRCOut = crc;
        return DRFLAC_SUCCESS;
    }

    
    if ((utf8[0] & 0xE0) == 0xC0) {
        byteCount = 2;
    } else if ((utf8[0] & 0xF0) == 0xE0) {
        byteCount = 3;
    } else if ((utf8[0] & 0xF8) == 0xF0) {
        byteCount = 4;
    } else if ((utf8[0] & 0xFC) == 0xF8) {
        byteCount = 5;
    } else if ((utf8[0] & 0xFE) == 0xFC) {
        byteCount = 6;
    } else if ((utf8[0] & 0xFF) == 0xFE) {
        byteCount = 7;
    } else {
        *pNumberOut = 0;
        return DRFLAC_CRC_MISMATCH;     
    }

    
    DRFLAC_ASSERT(byteCount > 1);

    result = (drflac_uint64)(utf8[0] & (0xFF >> (byteCount + 1)));
    for (i = 1; i < byteCount; ++i) {
        if (!drflac__read_uint8(bs, 8, utf8 + i)) {
            *pNumberOut = 0;
            return DRFLAC_AT_END;
        }
        crc = drflac_crc8(crc, utf8[i], 8);

        result = (result << 6) | (utf8[i] & 0x3F);
    }

    *pNumberOut = result;
    *pCRCOut = crc;
    return DRFLAC_SUCCESS;
}


static DRFLAC_INLINE drflac_uint32 drflac__ilog2_u32(drflac_uint32 x)
{
#if 1   
    drflac_uint32 result = 0;
    while (x > 0) {
        result += 1;
        x >>= 1;
    }

    return result;
#endif
}

static DRFLAC_INLINE drflac_bool32 drflac__use_64_bit_prediction(drflac_uint32 bitsPerSample, drflac_uint32 order, drflac_uint32 precision)
{
    
    return bitsPerSample + precision + drflac__ilog2_u32(order) > 32;
}








#if defined(__clang__)
__attribute__((no_sanitize("signed-integer-overflow")))
#endif
static DRFLAC_INLINE drflac_int32 drflac__calculate_prediction_32_s32(drflac_uint32 order, drflac_int32 shift, const drflac_int32* coefficients, drflac_int32* pDecodedSamples)
{
    drflac_int32 prediction = 0;

    DRFLAC_ASSERT(order <= 32);

    

    
    switch (order)
    {
    case 32: prediction += coefficients[31] * pDecodedSamples[-32];
    case 31: prediction += coefficients[30] * pDecodedSamples[-31];
    case 30: prediction += coefficients[29] * pDecodedSamples[-30];
    case 29: prediction += coefficients[28] * pDecodedSamples[-29];
    case 28: prediction += coefficients[27] * pDecodedSamples[-28];
    case 27: prediction += coefficients[26] * pDecodedSamples[-27];
    case 26: prediction += coefficients[25] * pDecodedSamples[-26];
    case 25: prediction += coefficients[24] * pDecodedSamples[-25];
    case 24: prediction += coefficients[23] * pDecodedSamples[-24];
    case 23: prediction += coefficients[22] * pDecodedSamples[-23];
    case 22: prediction += coefficients[21] * pDecodedSamples[-22];
    case 21: prediction += coefficients[20] * pDecodedSamples[-21];
    case 20: prediction += coefficients[19] * pDecodedSamples[-20];
    case 19: prediction += coefficients[18] * pDecodedSamples[-19];
    case 18: prediction += coefficients[17] * pDecodedSamples[-18];
    case 17: prediction += coefficients[16] * pDecodedSamples[-17];
    case 16: prediction += coefficients[15] * pDecodedSamples[-16];
    case 15: prediction += coefficients[14] * pDecodedSamples[-15];
    case 14: prediction += coefficients[13] * pDecodedSamples[-14];
    case 13: prediction += coefficients[12] * pDecodedSamples[-13];
    case 12: prediction += coefficients[11] * pDecodedSamples[-12];
    case 11: prediction += coefficients[10] * pDecodedSamples[-11];
    case 10: prediction += coefficients[ 9] * pDecodedSamples[-10];
    case  9: prediction += coefficients[ 8] * pDecodedSamples[- 9];
    case  8: prediction += coefficients[ 7] * pDecodedSamples[- 8];
    case  7: prediction += coefficients[ 6] * pDecodedSamples[- 7];
    case  6: prediction += coefficients[ 5] * pDecodedSamples[- 6];
    case  5: prediction += coefficients[ 4] * pDecodedSamples[- 5];
    case  4: prediction += coefficients[ 3] * pDecodedSamples[- 4];
    case  3: prediction += coefficients[ 2] * pDecodedSamples[- 3];
    case  2: prediction += coefficients[ 1] * pDecodedSamples[- 2];
    case  1: prediction += coefficients[ 0] * pDecodedSamples[- 1];
    }

    return (drflac_int32)(prediction >> shift);
}

static DRFLAC_INLINE drflac_int32 drflac__calculate_prediction_64_s32(drflac_uint32 order, drflac_int32 shift, const drflac_int32* coefficients, drflac_int32* pDecodedSamples)
{
    drflac_int64 prediction;

    DRFLAC_ASSERT(order <= 32);

    

    
#ifndef DRFLAC_64BIT
    if (order == 8)
    {
        prediction  = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
        prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
        prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
        prediction += coefficients[3] * (drflac_int64)pDecodedSamples[-4];
        prediction += coefficients[4] * (drflac_int64)pDecodedSamples[-5];
        prediction += coefficients[5] * (drflac_int64)pDecodedSamples[-6];
        prediction += coefficients[6] * (drflac_int64)pDecodedSamples[-7];
        prediction += coefficients[7] * (drflac_int64)pDecodedSamples[-8];
    }
    else if (order == 7)
    {
        prediction  = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
        prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
        prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
        prediction += coefficients[3] * (drflac_int64)pDecodedSamples[-4];
        prediction += coefficients[4] * (drflac_int64)pDecodedSamples[-5];
        prediction += coefficients[5] * (drflac_int64)pDecodedSamples[-6];
        prediction += coefficients[6] * (drflac_int64)pDecodedSamples[-7];
    }
    else if (order == 3)
    {
        prediction  = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
        prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
        prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
    }
    else if (order == 6)
    {
        prediction  = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
        prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
        prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
        prediction += coefficients[3] * (drflac_int64)pDecodedSamples[-4];
        prediction += coefficients[4] * (drflac_int64)pDecodedSamples[-5];
        prediction += coefficients[5] * (drflac_int64)pDecodedSamples[-6];
    }
    else if (order == 5)
    {
        prediction  = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
        prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
        prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
        prediction += coefficients[3] * (drflac_int64)pDecodedSamples[-4];
        prediction += coefficients[4] * (drflac_int64)pDecodedSamples[-5];
    }
    else if (order == 4)
    {
        prediction  = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
        prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
        prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
        prediction += coefficients[3] * (drflac_int64)pDecodedSamples[-4];
    }
    else if (order == 12)
    {
        prediction  = coefficients[0]  * (drflac_int64)pDecodedSamples[-1];
        prediction += coefficients[1]  * (drflac_int64)pDecodedSamples[-2];
        prediction += coefficients[2]  * (drflac_int64)pDecodedSamples[-3];
        prediction += coefficients[3]  * (drflac_int64)pDecodedSamples[-4];
        prediction += coefficients[4]  * (drflac_int64)pDecodedSamples[-5];
        prediction += coefficients[5]  * (drflac_int64)pDecodedSamples[-6];
        prediction += coefficients[6]  * (drflac_int64)pDecodedSamples[-7];
        prediction += coefficients[7]  * (drflac_int64)pDecodedSamples[-8];
        prediction += coefficients[8]  * (drflac_int64)pDecodedSamples[-9];
        prediction += coefficients[9]  * (drflac_int64)pDecodedSamples[-10];
        prediction += coefficients[10] * (drflac_int64)pDecodedSamples[-11];
        prediction += coefficients[11] * (drflac_int64)pDecodedSamples[-12];
    }
    else if (order == 2)
    {
        prediction  = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
        prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
    }
    else if (order == 1)
    {
        prediction = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
    }
    else if (order == 10)
    {
        prediction  = coefficients[0]  * (drflac_int64)pDecodedSamples[-1];
        prediction += coefficients[1]  * (drflac_int64)pDecodedSamples[-2];
        prediction += coefficients[2]  * (drflac_int64)pDecodedSamples[-3];
        prediction += coefficients[3]  * (drflac_int64)pDecodedSamples[-4];
        prediction += coefficients[4]  * (drflac_int64)pDecodedSamples[-5];
        prediction += coefficients[5]  * (drflac_int64)pDecodedSamples[-6];
        prediction += coefficients[6]  * (drflac_int64)pDecodedSamples[-7];
        prediction += coefficients[7]  * (drflac_int64)pDecodedSamples[-8];
        prediction += coefficients[8]  * (drflac_int64)pDecodedSamples[-9];
        prediction += coefficients[9]  * (drflac_int64)pDecodedSamples[-10];
    }
    else if (order == 9)
    {
        prediction  = coefficients[0]  * (drflac_int64)pDecodedSamples[-1];
        prediction += coefficients[1]  * (drflac_int64)pDecodedSamples[-2];
        prediction += coefficients[2]  * (drflac_int64)pDecodedSamples[-3];
        prediction += coefficients[3]  * (drflac_int64)pDecodedSamples[-4];
        prediction += coefficients[4]  * (drflac_int64)pDecodedSamples[-5];
        prediction += coefficients[5]  * (drflac_int64)pDecodedSamples[-6];
        prediction += coefficients[6]  * (drflac_int64)pDecodedSamples[-7];
        prediction += coefficients[7]  * (drflac_int64)pDecodedSamples[-8];
        prediction += coefficients[8]  * (drflac_int64)pDecodedSamples[-9];
    }
    else if (order == 11)
    {
        prediction  = coefficients[0]  * (drflac_int64)pDecodedSamples[-1];
        prediction += coefficients[1]  * (drflac_int64)pDecodedSamples[-2];
        prediction += coefficients[2]  * (drflac_int64)pDecodedSamples[-3];
        prediction += coefficients[3]  * (drflac_int64)pDecodedSamples[-4];
        prediction += coefficients[4]  * (drflac_int64)pDecodedSamples[-5];
        prediction += coefficients[5]  * (drflac_int64)pDecodedSamples[-6];
        prediction += coefficients[6]  * (drflac_int64)pDecodedSamples[-7];
        prediction += coefficients[7]  * (drflac_int64)pDecodedSamples[-8];
        prediction += coefficients[8]  * (drflac_int64)pDecodedSamples[-9];
        prediction += coefficients[9]  * (drflac_int64)pDecodedSamples[-10];
        prediction += coefficients[10] * (drflac_int64)pDecodedSamples[-11];
    }
    else
    {
        int j;

        prediction = 0;
        for (j = 0; j < (int)order; ++j) {
            prediction += coefficients[j] * (drflac_int64)pDecodedSamples[-j-1];
        }
    }
#endif

    



#ifdef DRFLAC_64BIT
    prediction = 0;
    switch (order)
    {
    case 32: prediction += coefficients[31] * (drflac_int64)pDecodedSamples[-32];
    case 31: prediction += coefficients[30] * (drflac_int64)pDecodedSamples[-31];
    case 30: prediction += coefficients[29] * (drflac_int64)pDecodedSamples[-30];
    case 29: prediction += coefficients[28] * (drflac_int64)pDecodedSamples[-29];
    case 28: prediction += coefficients[27] * (drflac_int64)pDecodedSamples[-28];
    case 27: prediction += coefficients[26] * (drflac_int64)pDecodedSamples[-27];
    case 26: prediction += coefficients[25] * (drflac_int64)pDecodedSamples[-26];
    case 25: prediction += coefficients[24] * (drflac_int64)pDecodedSamples[-25];
    case 24: prediction += coefficients[23] * (drflac_int64)pDecodedSamples[-24];
    case 23: prediction += coefficients[22] * (drflac_int64)pDecodedSamples[-23];
    case 22: prediction += coefficients[21] * (drflac_int64)pDecodedSamples[-22];
    case 21: prediction += coefficients[20] * (drflac_int64)pDecodedSamples[-21];
    case 20: prediction += coefficients[19] * (drflac_int64)pDecodedSamples[-20];
    case 19: prediction += coefficients[18] * (drflac_int64)pDecodedSamples[-19];
    case 18: prediction += coefficients[17] * (drflac_int64)pDecodedSamples[-18];
    case 17: prediction += coefficients[16] * (drflac_int64)pDecodedSamples[-17];
    case 16: prediction += coefficients[15] * (drflac_int64)pDecodedSamples[-16];
    case 15: prediction += coefficients[14] * (drflac_int64)pDecodedSamples[-15];
    case 14: prediction += coefficients[13] * (drflac_int64)pDecodedSamples[-14];
    case 13: prediction += coefficients[12] * (drflac_int64)pDecodedSamples[-13];
    case 12: prediction += coefficients[11] * (drflac_int64)pDecodedSamples[-12];
    case 11: prediction += coefficients[10] * (drflac_int64)pDecodedSamples[-11];
    case 10: prediction += coefficients[ 9] * (drflac_int64)pDecodedSamples[-10];
    case  9: prediction += coefficients[ 8] * (drflac_int64)pDecodedSamples[- 9];
    case  8: prediction += coefficients[ 7] * (drflac_int64)pDecodedSamples[- 8];
    case  7: prediction += coefficients[ 6] * (drflac_int64)pDecodedSamples[- 7];
    case  6: prediction += coefficients[ 5] * (drflac_int64)pDecodedSamples[- 6];
    case  5: prediction += coefficients[ 4] * (drflac_int64)pDecodedSamples[- 5];
    case  4: prediction += coefficients[ 3] * (drflac_int64)pDecodedSamples[- 4];
    case  3: prediction += coefficients[ 2] * (drflac_int64)pDecodedSamples[- 3];
    case  2: prediction += coefficients[ 1] * (drflac_int64)pDecodedSamples[- 2];
    case  1: prediction += coefficients[ 0] * (drflac_int64)pDecodedSamples[- 1];
    }
#endif

    return (drflac_int32)(prediction >> shift);
}


#if 0




static drflac_bool32 drflac__decode_samples_with_residual__rice__reference(drflac_bs* bs, drflac_uint32 bitsPerSample, drflac_uint32 count, drflac_uint8 riceParam, drflac_uint32 lpcOrder, drflac_int32 lpcShift, drflac_uint32 lpcPrecision, const drflac_int32* coefficients, drflac_int32* pSamplesOut)
{
    drflac_uint32 i;

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(pSamplesOut != NULL);

    for (i = 0; i < count; ++i) {
        drflac_uint32 zeroCounter = 0;
        for (;;) {
            drflac_uint8 bit;
            if (!drflac__read_uint8(bs, 1, &bit)) {
                return DRFLAC_FALSE;
            }

            if (bit == 0) {
                zeroCounter += 1;
            } else {
                break;
            }
        }

        drflac_uint32 decodedRice;
        if (riceParam > 0) {
            if (!drflac__read_uint32(bs, riceParam, &decodedRice)) {
                return DRFLAC_FALSE;
            }
        } else {
            decodedRice = 0;
        }

        decodedRice |= (zeroCounter << riceParam);
        if ((decodedRice & 0x01)) {
            decodedRice = ~(decodedRice >> 1);
        } else {
            decodedRice =  (decodedRice >> 1);
        }


        if (drflac__use_64_bit_prediction(bitsPerSample, lpcOrder, lpcPrecision)) {
            pSamplesOut[i] = decodedRice + drflac__calculate_prediction_64_s32(lpcOrder, lpcShift, coefficients, pSamplesOut + i);
        } else {
            pSamplesOut[i] = decodedRice + drflac__calculate_prediction_32_s32(lpcOrder, lpcShift, coefficients, pSamplesOut + i);
        }
    }

    return DRFLAC_TRUE;
}
#endif

#if 0
static drflac_bool32 drflac__read_rice_parts__reference(drflac_bs* bs, drflac_uint8 riceParam, drflac_uint32* pZeroCounterOut, drflac_uint32* pRiceParamPartOut)
{
    drflac_uint32 zeroCounter = 0;
    drflac_uint32 decodedRice;

    for (;;) {
        drflac_uint8 bit;
        if (!drflac__read_uint8(bs, 1, &bit)) {
            return DRFLAC_FALSE;
        }

        if (bit == 0) {
            zeroCounter += 1;
        } else {
            break;
        }
    }

    if (riceParam > 0) {
        if (!drflac__read_uint32(bs, riceParam, &decodedRice)) {
            return DRFLAC_FALSE;
        }
    } else {
        decodedRice = 0;
    }

    *pZeroCounterOut = zeroCounter;
    *pRiceParamPartOut = decodedRice;
    return DRFLAC_TRUE;
}
#endif

#if 0
static DRFLAC_INLINE drflac_bool32 drflac__read_rice_parts(drflac_bs* bs, drflac_uint8 riceParam, drflac_uint32* pZeroCounterOut, drflac_uint32* pRiceParamPartOut)
{
    drflac_cache_t riceParamMask;
    drflac_uint32 zeroCounter;
    drflac_uint32 setBitOffsetPlus1;
    drflac_uint32 riceParamPart;
    drflac_uint32 riceLength;

    DRFLAC_ASSERT(riceParam > 0);   

    riceParamMask = DRFLAC_CACHE_L1_SELECTION_MASK(riceParam);

    zeroCounter = 0;
    while (bs->cache == 0) {
        zeroCounter += (drflac_uint32)DRFLAC_CACHE_L1_BITS_REMAINING(bs);
        if (!drflac__reload_cache(bs)) {
            return DRFLAC_FALSE;
        }
    }

    setBitOffsetPlus1 = drflac__clz(bs->cache);
    zeroCounter += setBitOffsetPlus1;
    setBitOffsetPlus1 += 1;

    riceLength = setBitOffsetPlus1 + riceParam;
    if (riceLength < DRFLAC_CACHE_L1_BITS_REMAINING(bs)) {
        riceParamPart = (drflac_uint32)((bs->cache & (riceParamMask >> setBitOffsetPlus1)) >> DRFLAC_CACHE_L1_SELECTION_SHIFT(bs, riceLength));

        bs->consumedBits += riceLength;
        bs->cache <<= riceLength;
    } else {
        drflac_uint32 bitCountLo;
        drflac_cache_t resultHi;

        bs->consumedBits += riceLength;
        bs->cache <<= setBitOffsetPlus1 & (DRFLAC_CACHE_L1_SIZE_BITS(bs)-1);    

        
        bitCountLo = bs->consumedBits - DRFLAC_CACHE_L1_SIZE_BITS(bs);
        resultHi = DRFLAC_CACHE_L1_SELECT_AND_SHIFT(bs, riceParam);  

        if (bs->nextL2Line < DRFLAC_CACHE_L2_LINE_COUNT(bs)) {
#ifndef DR_FLAC_NO_CRC
            drflac__update_crc16(bs);
#endif
            bs->cache = drflac__be2host__cache_line(bs->cacheL2[bs->nextL2Line++]);
            bs->consumedBits = 0;
#ifndef DR_FLAC_NO_CRC
            bs->crc16Cache = bs->cache;
#endif
        } else {
            
            if (!drflac__reload_cache(bs)) {
                return DRFLAC_FALSE;
            }
            if (bitCountLo > DRFLAC_CACHE_L1_BITS_REMAINING(bs)) {
                
                return DRFLAC_FALSE;
            }
        }

        riceParamPart = (drflac_uint32)(resultHi | DRFLAC_CACHE_L1_SELECT_AND_SHIFT_SAFE(bs, bitCountLo));

        bs->consumedBits += bitCountLo;
        bs->cache <<= bitCountLo;
    }

    pZeroCounterOut[0] = zeroCounter;
    pRiceParamPartOut[0] = riceParamPart;

    return DRFLAC_TRUE;
}
#endif

static DRFLAC_INLINE drflac_bool32 drflac__read_rice_parts_x1(drflac_bs* bs, drflac_uint8 riceParam, drflac_uint32* pZeroCounterOut, drflac_uint32* pRiceParamPartOut)
{
    drflac_uint32  riceParamPlus1 = riceParam + 1;
    
    drflac_uint32  riceParamPlus1Shift = DRFLAC_CACHE_L1_SELECTION_SHIFT(bs, riceParamPlus1);
    drflac_uint32  riceParamPlus1MaxConsumedBits = DRFLAC_CACHE_L1_SIZE_BITS(bs) - riceParamPlus1;

    



    drflac_cache_t bs_cache = bs->cache;
    drflac_uint32  bs_consumedBits = bs->consumedBits;

    
    drflac_uint32  lzcount = drflac__clz(bs_cache);
    if (lzcount < sizeof(bs_cache)*8) {
        pZeroCounterOut[0] = lzcount;

        




    extract_rice_param_part:
        bs_cache       <<= lzcount;
        bs_consumedBits += lzcount;

        if (bs_consumedBits <= riceParamPlus1MaxConsumedBits) {
            
            pRiceParamPartOut[0] = (drflac_uint32)(bs_cache >> riceParamPlus1Shift);
            bs_cache       <<= riceParamPlus1;
            bs_consumedBits += riceParamPlus1;
        } else {
            drflac_uint32 riceParamPartHi;
            drflac_uint32 riceParamPartLo;
            drflac_uint32 riceParamPartLoBitCount;

            




            
            riceParamPartHi = (drflac_uint32)(bs_cache >> riceParamPlus1Shift);

            
            riceParamPartLoBitCount = bs_consumedBits - riceParamPlus1MaxConsumedBits;
            DRFLAC_ASSERT(riceParamPartLoBitCount > 0 && riceParamPartLoBitCount < 32);

            
            if (bs->nextL2Line < DRFLAC_CACHE_L2_LINE_COUNT(bs)) {
            #ifndef DR_FLAC_NO_CRC
                drflac__update_crc16(bs);
            #endif
                bs_cache = drflac__be2host__cache_line(bs->cacheL2[bs->nextL2Line++]);
                bs_consumedBits = riceParamPartLoBitCount;
            #ifndef DR_FLAC_NO_CRC
                bs->crc16Cache = bs_cache;
            #endif
            } else {
                
                if (!drflac__reload_cache(bs)) {
                    return DRFLAC_FALSE;
                }
                if (riceParamPartLoBitCount > DRFLAC_CACHE_L1_BITS_REMAINING(bs)) {
                    
                    return DRFLAC_FALSE;
                }

                bs_cache = bs->cache;
                bs_consumedBits = bs->consumedBits + riceParamPartLoBitCount;
            }

            
            riceParamPartLo = (drflac_uint32)(bs_cache >> (DRFLAC_CACHE_L1_SELECTION_SHIFT(bs, riceParamPartLoBitCount)));
            pRiceParamPartOut[0] = riceParamPartHi | riceParamPartLo;

            bs_cache <<= riceParamPartLoBitCount;
        }
    } else {
        



        drflac_uint32 zeroCounter = (drflac_uint32)(DRFLAC_CACHE_L1_SIZE_BITS(bs) - bs_consumedBits);
        for (;;) {
            if (bs->nextL2Line < DRFLAC_CACHE_L2_LINE_COUNT(bs)) {
            #ifndef DR_FLAC_NO_CRC
                drflac__update_crc16(bs);
            #endif
                bs_cache = drflac__be2host__cache_line(bs->cacheL2[bs->nextL2Line++]);
                bs_consumedBits = 0;
            #ifndef DR_FLAC_NO_CRC
                bs->crc16Cache = bs_cache;
            #endif
            } else {
                
                if (!drflac__reload_cache(bs)) {
                    return DRFLAC_FALSE;
                }

                bs_cache = bs->cache;
                bs_consumedBits = bs->consumedBits;
            }

            lzcount = drflac__clz(bs_cache);
            zeroCounter += lzcount;

            if (lzcount < sizeof(bs_cache)*8) {
                break;
            }
        }

        pZeroCounterOut[0] = zeroCounter;
        goto extract_rice_param_part;
    }

    
    bs->cache = bs_cache;
    bs->consumedBits = bs_consumedBits;

    return DRFLAC_TRUE;
}

static DRFLAC_INLINE drflac_bool32 drflac__seek_rice_parts(drflac_bs* bs, drflac_uint8 riceParam)
{
    drflac_uint32  riceParamPlus1 = riceParam + 1;
    drflac_uint32  riceParamPlus1MaxConsumedBits = DRFLAC_CACHE_L1_SIZE_BITS(bs) - riceParamPlus1;

    



    drflac_cache_t bs_cache = bs->cache;
    drflac_uint32  bs_consumedBits = bs->consumedBits;

    
    drflac_uint32  lzcount = drflac__clz(bs_cache);
    if (lzcount < sizeof(bs_cache)*8) {
        




    extract_rice_param_part:
        bs_cache       <<= lzcount;
        bs_consumedBits += lzcount;

        if (bs_consumedBits <= riceParamPlus1MaxConsumedBits) {
            
            bs_cache       <<= riceParamPlus1;
            bs_consumedBits += riceParamPlus1;
        } else {
            




            
            drflac_uint32 riceParamPartLoBitCount = bs_consumedBits - riceParamPlus1MaxConsumedBits;
            DRFLAC_ASSERT(riceParamPartLoBitCount > 0 && riceParamPartLoBitCount < 32);

            
            if (bs->nextL2Line < DRFLAC_CACHE_L2_LINE_COUNT(bs)) {
            #ifndef DR_FLAC_NO_CRC
                drflac__update_crc16(bs);
            #endif
                bs_cache = drflac__be2host__cache_line(bs->cacheL2[bs->nextL2Line++]);
                bs_consumedBits = riceParamPartLoBitCount;
            #ifndef DR_FLAC_NO_CRC
                bs->crc16Cache = bs_cache;
            #endif
            } else {
                
                if (!drflac__reload_cache(bs)) {
                    return DRFLAC_FALSE;
                }

                if (riceParamPartLoBitCount > DRFLAC_CACHE_L1_BITS_REMAINING(bs)) {
                    
                    return DRFLAC_FALSE;
                }

                bs_cache = bs->cache;
                bs_consumedBits = bs->consumedBits + riceParamPartLoBitCount;
            }

            bs_cache <<= riceParamPartLoBitCount;
        }
    } else {
        



        for (;;) {
            if (bs->nextL2Line < DRFLAC_CACHE_L2_LINE_COUNT(bs)) {
            #ifndef DR_FLAC_NO_CRC
                drflac__update_crc16(bs);
            #endif
                bs_cache = drflac__be2host__cache_line(bs->cacheL2[bs->nextL2Line++]);
                bs_consumedBits = 0;
            #ifndef DR_FLAC_NO_CRC
                bs->crc16Cache = bs_cache;
            #endif
            } else {
                
                if (!drflac__reload_cache(bs)) {
                    return DRFLAC_FALSE;
                }

                bs_cache = bs->cache;
                bs_consumedBits = bs->consumedBits;
            }

            lzcount = drflac__clz(bs_cache);
            if (lzcount < sizeof(bs_cache)*8) {
                break;
            }
        }

        goto extract_rice_param_part;
    }

    
    bs->cache = bs_cache;
    bs->consumedBits = bs_consumedBits;

    return DRFLAC_TRUE;
}


static drflac_bool32 drflac__decode_samples_with_residual__rice__scalar_zeroorder(drflac_bs* bs, drflac_uint32 bitsPerSample, drflac_uint32 count, drflac_uint8 riceParam, drflac_uint32 order, drflac_int32 shift, const drflac_int32* coefficients, drflac_int32* pSamplesOut)
{
    drflac_uint32 t[2] = {0x00000000, 0xFFFFFFFF};
    drflac_uint32 zeroCountPart0;
    drflac_uint32 riceParamPart0;
    drflac_uint32 riceParamMask;
    drflac_uint32 i;

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(pSamplesOut != NULL);

    (void)bitsPerSample;
    (void)order;
    (void)shift;
    (void)coefficients;

    riceParamMask  = (drflac_uint32)~((~0UL) << riceParam);

    i = 0;
    while (i < count) {
        
        if (!drflac__read_rice_parts_x1(bs, riceParam, &zeroCountPart0, &riceParamPart0)) {
            return DRFLAC_FALSE;
        }

        
        riceParamPart0 &= riceParamMask;
        riceParamPart0 |= (zeroCountPart0 << riceParam);
        riceParamPart0  = (riceParamPart0 >> 1) ^ t[riceParamPart0 & 0x01];

        pSamplesOut[i] = riceParamPart0;

        i += 1;
    }

    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples_with_residual__rice__scalar(drflac_bs* bs, drflac_uint32 bitsPerSample, drflac_uint32 count, drflac_uint8 riceParam, drflac_uint32 lpcOrder, drflac_int32 lpcShift, drflac_uint32 lpcPrecision, const drflac_int32* coefficients, drflac_int32* pSamplesOut)
{
    drflac_uint32 t[2] = {0x00000000, 0xFFFFFFFF};
    drflac_uint32 zeroCountPart0 = 0;
    drflac_uint32 zeroCountPart1 = 0;
    drflac_uint32 zeroCountPart2 = 0;
    drflac_uint32 zeroCountPart3 = 0;
    drflac_uint32 riceParamPart0 = 0;
    drflac_uint32 riceParamPart1 = 0;
    drflac_uint32 riceParamPart2 = 0;
    drflac_uint32 riceParamPart3 = 0;
    drflac_uint32 riceParamMask;
    const drflac_int32* pSamplesOutEnd;
    drflac_uint32 i;

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(pSamplesOut != NULL);

    if (lpcOrder == 0) {
        return drflac__decode_samples_with_residual__rice__scalar_zeroorder(bs, bitsPerSample, count, riceParam, lpcOrder, lpcShift, coefficients, pSamplesOut);
    }

    riceParamMask  = (drflac_uint32)~((~0UL) << riceParam);
    pSamplesOutEnd = pSamplesOut + (count & ~3);

    if (drflac__use_64_bit_prediction(bitsPerSample, lpcOrder, lpcPrecision)) {
        while (pSamplesOut < pSamplesOutEnd) {
            



            if (!drflac__read_rice_parts_x1(bs, riceParam, &zeroCountPart0, &riceParamPart0) ||
                !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountPart1, &riceParamPart1) ||
                !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountPart2, &riceParamPart2) ||
                !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountPart3, &riceParamPart3)) {
                return DRFLAC_FALSE;
            }

            riceParamPart0 &= riceParamMask;
            riceParamPart1 &= riceParamMask;
            riceParamPart2 &= riceParamMask;
            riceParamPart3 &= riceParamMask;

            riceParamPart0 |= (zeroCountPart0 << riceParam);
            riceParamPart1 |= (zeroCountPart1 << riceParam);
            riceParamPart2 |= (zeroCountPart2 << riceParam);
            riceParamPart3 |= (zeroCountPart3 << riceParam);

            riceParamPart0  = (riceParamPart0 >> 1) ^ t[riceParamPart0 & 0x01];
            riceParamPart1  = (riceParamPart1 >> 1) ^ t[riceParamPart1 & 0x01];
            riceParamPart2  = (riceParamPart2 >> 1) ^ t[riceParamPart2 & 0x01];
            riceParamPart3  = (riceParamPart3 >> 1) ^ t[riceParamPart3 & 0x01];

            pSamplesOut[0] = riceParamPart0 + drflac__calculate_prediction_64_s32(lpcOrder, lpcShift, coefficients, pSamplesOut + 0);
            pSamplesOut[1] = riceParamPart1 + drflac__calculate_prediction_64_s32(lpcOrder, lpcShift, coefficients, pSamplesOut + 1);
            pSamplesOut[2] = riceParamPart2 + drflac__calculate_prediction_64_s32(lpcOrder, lpcShift, coefficients, pSamplesOut + 2);
            pSamplesOut[3] = riceParamPart3 + drflac__calculate_prediction_64_s32(lpcOrder, lpcShift, coefficients, pSamplesOut + 3);

            pSamplesOut += 4;
        }
    } else {
        while (pSamplesOut < pSamplesOutEnd) {
            if (!drflac__read_rice_parts_x1(bs, riceParam, &zeroCountPart0, &riceParamPart0) ||
                !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountPart1, &riceParamPart1) ||
                !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountPart2, &riceParamPart2) ||
                !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountPart3, &riceParamPart3)) {
                return DRFLAC_FALSE;
            }

            riceParamPart0 &= riceParamMask;
            riceParamPart1 &= riceParamMask;
            riceParamPart2 &= riceParamMask;
            riceParamPart3 &= riceParamMask;

            riceParamPart0 |= (zeroCountPart0 << riceParam);
            riceParamPart1 |= (zeroCountPart1 << riceParam);
            riceParamPart2 |= (zeroCountPart2 << riceParam);
            riceParamPart3 |= (zeroCountPart3 << riceParam);

            riceParamPart0  = (riceParamPart0 >> 1) ^ t[riceParamPart0 & 0x01];
            riceParamPart1  = (riceParamPart1 >> 1) ^ t[riceParamPart1 & 0x01];
            riceParamPart2  = (riceParamPart2 >> 1) ^ t[riceParamPart2 & 0x01];
            riceParamPart3  = (riceParamPart3 >> 1) ^ t[riceParamPart3 & 0x01];

            pSamplesOut[0] = riceParamPart0 + drflac__calculate_prediction_32_s32(lpcOrder, lpcShift, coefficients, pSamplesOut + 0);
            pSamplesOut[1] = riceParamPart1 + drflac__calculate_prediction_32_s32(lpcOrder, lpcShift, coefficients, pSamplesOut + 1);
            pSamplesOut[2] = riceParamPart2 + drflac__calculate_prediction_32_s32(lpcOrder, lpcShift, coefficients, pSamplesOut + 2);
            pSamplesOut[3] = riceParamPart3 + drflac__calculate_prediction_32_s32(lpcOrder, lpcShift, coefficients, pSamplesOut + 3);

            pSamplesOut += 4;
        }
    }

    i = (count & ~3);
    while (i < count) {
        
        if (!drflac__read_rice_parts_x1(bs, riceParam, &zeroCountPart0, &riceParamPart0)) {
            return DRFLAC_FALSE;
        }

        
        riceParamPart0 &= riceParamMask;
        riceParamPart0 |= (zeroCountPart0 << riceParam);
        riceParamPart0  = (riceParamPart0 >> 1) ^ t[riceParamPart0 & 0x01];
        

        
        if (drflac__use_64_bit_prediction(bitsPerSample, lpcOrder, lpcPrecision)) {
            pSamplesOut[0] = riceParamPart0 + drflac__calculate_prediction_64_s32(lpcOrder, lpcShift, coefficients, pSamplesOut + 0);
        } else {
            pSamplesOut[0] = riceParamPart0 + drflac__calculate_prediction_32_s32(lpcOrder, lpcShift, coefficients, pSamplesOut + 0);
        }

        i += 1;
        pSamplesOut += 1;
    }

    return DRFLAC_TRUE;
}

#if defined(DRFLAC_SUPPORT_SSE2)
static DRFLAC_INLINE __m128i drflac__mm_packs_interleaved_epi32(__m128i a, __m128i b)
{
    __m128i r;

    
    r = _mm_packs_epi32(a, b);

    
    r = _mm_shuffle_epi32(r, _MM_SHUFFLE(3, 1, 2, 0));

    
    r = _mm_shufflehi_epi16(r, _MM_SHUFFLE(3, 1, 2, 0));
    r = _mm_shufflelo_epi16(r, _MM_SHUFFLE(3, 1, 2, 0));

    return r;
}
#endif

#if defined(DRFLAC_SUPPORT_SSE41)
static DRFLAC_INLINE __m128i drflac__mm_not_si128(__m128i a)
{
    return _mm_xor_si128(a, _mm_cmpeq_epi32(_mm_setzero_si128(), _mm_setzero_si128()));
}

static DRFLAC_INLINE __m128i drflac__mm_hadd_epi32(__m128i x)
{
    __m128i x64 = _mm_add_epi32(x, _mm_shuffle_epi32(x, _MM_SHUFFLE(1, 0, 3, 2)));
    __m128i x32 = _mm_shufflelo_epi16(x64, _MM_SHUFFLE(1, 0, 3, 2));
    return _mm_add_epi32(x64, x32);
}

static DRFLAC_INLINE __m128i drflac__mm_hadd_epi64(__m128i x)
{
    return _mm_add_epi64(x, _mm_shuffle_epi32(x, _MM_SHUFFLE(1, 0, 3, 2)));
}

static DRFLAC_INLINE __m128i drflac__mm_srai_epi64(__m128i x, int count)
{
    



    __m128i lo = _mm_srli_epi64(x, count);
    __m128i hi = _mm_srai_epi32(x, count);

    hi = _mm_and_si128(hi, _mm_set_epi32(0xFFFFFFFF, 0, 0xFFFFFFFF, 0));    

    return _mm_or_si128(lo, hi);
}

static drflac_bool32 drflac__decode_samples_with_residual__rice__sse41_32(drflac_bs* bs, drflac_uint32 count, drflac_uint8 riceParam, drflac_uint32 order, drflac_int32 shift, const drflac_int32* coefficients, drflac_int32* pSamplesOut)
{
    int i;
    drflac_uint32 riceParamMask;
    drflac_int32* pDecodedSamples    = pSamplesOut;
    drflac_int32* pDecodedSamplesEnd = pSamplesOut + (count & ~3);
    drflac_uint32 zeroCountParts0 = 0;
    drflac_uint32 zeroCountParts1 = 0;
    drflac_uint32 zeroCountParts2 = 0;
    drflac_uint32 zeroCountParts3 = 0;
    drflac_uint32 riceParamParts0 = 0;
    drflac_uint32 riceParamParts1 = 0;
    drflac_uint32 riceParamParts2 = 0;
    drflac_uint32 riceParamParts3 = 0;
    __m128i coefficients128_0;
    __m128i coefficients128_4;
    __m128i coefficients128_8;
    __m128i samples128_0;
    __m128i samples128_4;
    __m128i samples128_8;
    __m128i riceParamMask128;

    const drflac_uint32 t[2] = {0x00000000, 0xFFFFFFFF};

    riceParamMask    = (drflac_uint32)~((~0UL) << riceParam);
    riceParamMask128 = _mm_set1_epi32(riceParamMask);

    
    coefficients128_0 = _mm_setzero_si128();
    coefficients128_4 = _mm_setzero_si128();
    coefficients128_8 = _mm_setzero_si128();

    samples128_0 = _mm_setzero_si128();
    samples128_4 = _mm_setzero_si128();
    samples128_8 = _mm_setzero_si128();

    





#if 1
    {
        int runningOrder = order;

        
        if (runningOrder >= 4) {
            coefficients128_0 = _mm_loadu_si128((const __m128i*)(coefficients + 0));
            samples128_0      = _mm_loadu_si128((const __m128i*)(pSamplesOut  - 4));
            runningOrder -= 4;
        } else {
            switch (runningOrder) {
                case 3: coefficients128_0 = _mm_set_epi32(0, coefficients[2], coefficients[1], coefficients[0]); samples128_0 = _mm_set_epi32(pSamplesOut[-1], pSamplesOut[-2], pSamplesOut[-3], 0); break;
                case 2: coefficients128_0 = _mm_set_epi32(0, 0,               coefficients[1], coefficients[0]); samples128_0 = _mm_set_epi32(pSamplesOut[-1], pSamplesOut[-2], 0,               0); break;
                case 1: coefficients128_0 = _mm_set_epi32(0, 0,               0,               coefficients[0]); samples128_0 = _mm_set_epi32(pSamplesOut[-1], 0,               0,               0); break;
            }
            runningOrder = 0;
        }

        
        if (runningOrder >= 4) {
            coefficients128_4 = _mm_loadu_si128((const __m128i*)(coefficients + 4));
            samples128_4      = _mm_loadu_si128((const __m128i*)(pSamplesOut  - 8));
            runningOrder -= 4;
        } else {
            switch (runningOrder) {
                case 3: coefficients128_4 = _mm_set_epi32(0, coefficients[6], coefficients[5], coefficients[4]); samples128_4 = _mm_set_epi32(pSamplesOut[-5], pSamplesOut[-6], pSamplesOut[-7], 0); break;
                case 2: coefficients128_4 = _mm_set_epi32(0, 0,               coefficients[5], coefficients[4]); samples128_4 = _mm_set_epi32(pSamplesOut[-5], pSamplesOut[-6], 0,               0); break;
                case 1: coefficients128_4 = _mm_set_epi32(0, 0,               0,               coefficients[4]); samples128_4 = _mm_set_epi32(pSamplesOut[-5], 0,               0,               0); break;
            }
            runningOrder = 0;
        }

        
        if (runningOrder == 4) {
            coefficients128_8 = _mm_loadu_si128((const __m128i*)(coefficients + 8));
            samples128_8      = _mm_loadu_si128((const __m128i*)(pSamplesOut  - 12));
            runningOrder -= 4;
        } else {
            switch (runningOrder) {
                case 3: coefficients128_8 = _mm_set_epi32(0, coefficients[10], coefficients[9], coefficients[8]); samples128_8 = _mm_set_epi32(pSamplesOut[-9], pSamplesOut[-10], pSamplesOut[-11], 0); break;
                case 2: coefficients128_8 = _mm_set_epi32(0, 0,                coefficients[9], coefficients[8]); samples128_8 = _mm_set_epi32(pSamplesOut[-9], pSamplesOut[-10], 0,                0); break;
                case 1: coefficients128_8 = _mm_set_epi32(0, 0,                0,               coefficients[8]); samples128_8 = _mm_set_epi32(pSamplesOut[-9], 0,                0,                0); break;
            }
            runningOrder = 0;
        }

        
        coefficients128_0 = _mm_shuffle_epi32(coefficients128_0, _MM_SHUFFLE(0, 1, 2, 3));
        coefficients128_4 = _mm_shuffle_epi32(coefficients128_4, _MM_SHUFFLE(0, 1, 2, 3));
        coefficients128_8 = _mm_shuffle_epi32(coefficients128_8, _MM_SHUFFLE(0, 1, 2, 3));
    }
#else
    
    switch (order)
    {
    case 12: ((drflac_int32*)&coefficients128_8)[0] = coefficients[11]; ((drflac_int32*)&samples128_8)[0] = pDecodedSamples[-12];
    case 11: ((drflac_int32*)&coefficients128_8)[1] = coefficients[10]; ((drflac_int32*)&samples128_8)[1] = pDecodedSamples[-11];
    case 10: ((drflac_int32*)&coefficients128_8)[2] = coefficients[ 9]; ((drflac_int32*)&samples128_8)[2] = pDecodedSamples[-10];
    case 9:  ((drflac_int32*)&coefficients128_8)[3] = coefficients[ 8]; ((drflac_int32*)&samples128_8)[3] = pDecodedSamples[- 9];
    case 8:  ((drflac_int32*)&coefficients128_4)[0] = coefficients[ 7]; ((drflac_int32*)&samples128_4)[0] = pDecodedSamples[- 8];
    case 7:  ((drflac_int32*)&coefficients128_4)[1] = coefficients[ 6]; ((drflac_int32*)&samples128_4)[1] = pDecodedSamples[- 7];
    case 6:  ((drflac_int32*)&coefficients128_4)[2] = coefficients[ 5]; ((drflac_int32*)&samples128_4)[2] = pDecodedSamples[- 6];
    case 5:  ((drflac_int32*)&coefficients128_4)[3] = coefficients[ 4]; ((drflac_int32*)&samples128_4)[3] = pDecodedSamples[- 5];
    case 4:  ((drflac_int32*)&coefficients128_0)[0] = coefficients[ 3]; ((drflac_int32*)&samples128_0)[0] = pDecodedSamples[- 4];
    case 3:  ((drflac_int32*)&coefficients128_0)[1] = coefficients[ 2]; ((drflac_int32*)&samples128_0)[1] = pDecodedSamples[- 3];
    case 2:  ((drflac_int32*)&coefficients128_0)[2] = coefficients[ 1]; ((drflac_int32*)&samples128_0)[2] = pDecodedSamples[- 2];
    case 1:  ((drflac_int32*)&coefficients128_0)[3] = coefficients[ 0]; ((drflac_int32*)&samples128_0)[3] = pDecodedSamples[- 1];
    }
#endif

    
    while (pDecodedSamples < pDecodedSamplesEnd) {
        __m128i prediction128;
        __m128i zeroCountPart128;
        __m128i riceParamPart128;

        if (!drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts0, &riceParamParts0) ||
            !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts1, &riceParamParts1) ||
            !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts2, &riceParamParts2) ||
            !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts3, &riceParamParts3)) {
            return DRFLAC_FALSE;
        }

        zeroCountPart128 = _mm_set_epi32(zeroCountParts3, zeroCountParts2, zeroCountParts1, zeroCountParts0);
        riceParamPart128 = _mm_set_epi32(riceParamParts3, riceParamParts2, riceParamParts1, riceParamParts0);

        riceParamPart128 = _mm_and_si128(riceParamPart128, riceParamMask128);
        riceParamPart128 = _mm_or_si128(riceParamPart128, _mm_slli_epi32(zeroCountPart128, riceParam));
        riceParamPart128 = _mm_xor_si128(_mm_srli_epi32(riceParamPart128, 1), _mm_add_epi32(drflac__mm_not_si128(_mm_and_si128(riceParamPart128, _mm_set1_epi32(0x01))), _mm_set1_epi32(0x01)));  
           

        if (order <= 4) {
            for (i = 0; i < 4; i += 1) {
                prediction128 = _mm_mullo_epi32(coefficients128_0, samples128_0);

                
                prediction128 = drflac__mm_hadd_epi32(prediction128);
                prediction128 = _mm_srai_epi32(prediction128, shift);
                prediction128 = _mm_add_epi32(riceParamPart128, prediction128);

                samples128_0 = _mm_alignr_epi8(prediction128, samples128_0, 4);
                riceParamPart128 = _mm_alignr_epi8(_mm_setzero_si128(), riceParamPart128, 4);
            }
        } else if (order <= 8) {
            for (i = 0; i < 4; i += 1) {
                prediction128 =                              _mm_mullo_epi32(coefficients128_4, samples128_4);
                prediction128 = _mm_add_epi32(prediction128, _mm_mullo_epi32(coefficients128_0, samples128_0));

                
                prediction128 = drflac__mm_hadd_epi32(prediction128);
                prediction128 = _mm_srai_epi32(prediction128, shift);
                prediction128 = _mm_add_epi32(riceParamPart128, prediction128);

                samples128_4 = _mm_alignr_epi8(samples128_0,  samples128_4, 4);
                samples128_0 = _mm_alignr_epi8(prediction128, samples128_0, 4);
                riceParamPart128 = _mm_alignr_epi8(_mm_setzero_si128(), riceParamPart128, 4);
            }
        } else {
            for (i = 0; i < 4; i += 1) {
                prediction128 =                              _mm_mullo_epi32(coefficients128_8, samples128_8);
                prediction128 = _mm_add_epi32(prediction128, _mm_mullo_epi32(coefficients128_4, samples128_4));
                prediction128 = _mm_add_epi32(prediction128, _mm_mullo_epi32(coefficients128_0, samples128_0));

                
                prediction128 = drflac__mm_hadd_epi32(prediction128);
                prediction128 = _mm_srai_epi32(prediction128, shift);
                prediction128 = _mm_add_epi32(riceParamPart128, prediction128);

                samples128_8 = _mm_alignr_epi8(samples128_4,  samples128_8, 4);
                samples128_4 = _mm_alignr_epi8(samples128_0,  samples128_4, 4);
                samples128_0 = _mm_alignr_epi8(prediction128, samples128_0, 4);
                riceParamPart128 = _mm_alignr_epi8(_mm_setzero_si128(), riceParamPart128, 4);
            }
        }

        
        _mm_storeu_si128((__m128i*)pDecodedSamples, samples128_0);
        pDecodedSamples += 4;
    }

    
    i = (count & ~3);
    while (i < (int)count) {
        
        if (!drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts0, &riceParamParts0)) {
            return DRFLAC_FALSE;
        }

        
        riceParamParts0 &= riceParamMask;
        riceParamParts0 |= (zeroCountParts0 << riceParam);
        riceParamParts0  = (riceParamParts0 >> 1) ^ t[riceParamParts0 & 0x01];

        
        pDecodedSamples[0] = riceParamParts0 + drflac__calculate_prediction_32_s32(order, shift, coefficients, pDecodedSamples);

        i += 1;
        pDecodedSamples += 1;
    }

    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples_with_residual__rice__sse41_64(drflac_bs* bs, drflac_uint32 count, drflac_uint8 riceParam, drflac_uint32 order, drflac_int32 shift, const drflac_int32* coefficients, drflac_int32* pSamplesOut)
{
    int i;
    drflac_uint32 riceParamMask;
    drflac_int32* pDecodedSamples    = pSamplesOut;
    drflac_int32* pDecodedSamplesEnd = pSamplesOut + (count & ~3);
    drflac_uint32 zeroCountParts0 = 0;
    drflac_uint32 zeroCountParts1 = 0;
    drflac_uint32 zeroCountParts2 = 0;
    drflac_uint32 zeroCountParts3 = 0;
    drflac_uint32 riceParamParts0 = 0;
    drflac_uint32 riceParamParts1 = 0;
    drflac_uint32 riceParamParts2 = 0;
    drflac_uint32 riceParamParts3 = 0;
    __m128i coefficients128_0;
    __m128i coefficients128_4;
    __m128i coefficients128_8;
    __m128i samples128_0;
    __m128i samples128_4;
    __m128i samples128_8;
    __m128i prediction128;
    __m128i riceParamMask128;

    const drflac_uint32 t[2] = {0x00000000, 0xFFFFFFFF};

    DRFLAC_ASSERT(order <= 12);

    riceParamMask    = (drflac_uint32)~((~0UL) << riceParam);
    riceParamMask128 = _mm_set1_epi32(riceParamMask);

    prediction128 = _mm_setzero_si128();

    
    coefficients128_0  = _mm_setzero_si128();
    coefficients128_4  = _mm_setzero_si128();
    coefficients128_8  = _mm_setzero_si128();

    samples128_0  = _mm_setzero_si128();
    samples128_4  = _mm_setzero_si128();
    samples128_8  = _mm_setzero_si128();

#if 1
    {
        int runningOrder = order;

        
        if (runningOrder >= 4) {
            coefficients128_0 = _mm_loadu_si128((const __m128i*)(coefficients + 0));
            samples128_0      = _mm_loadu_si128((const __m128i*)(pSamplesOut  - 4));
            runningOrder -= 4;
        } else {
            switch (runningOrder) {
                case 3: coefficients128_0 = _mm_set_epi32(0, coefficients[2], coefficients[1], coefficients[0]); samples128_0 = _mm_set_epi32(pSamplesOut[-1], pSamplesOut[-2], pSamplesOut[-3], 0); break;
                case 2: coefficients128_0 = _mm_set_epi32(0, 0,               coefficients[1], coefficients[0]); samples128_0 = _mm_set_epi32(pSamplesOut[-1], pSamplesOut[-2], 0,               0); break;
                case 1: coefficients128_0 = _mm_set_epi32(0, 0,               0,               coefficients[0]); samples128_0 = _mm_set_epi32(pSamplesOut[-1], 0,               0,               0); break;
            }
            runningOrder = 0;
        }

        
        if (runningOrder >= 4) {
            coefficients128_4 = _mm_loadu_si128((const __m128i*)(coefficients + 4));
            samples128_4      = _mm_loadu_si128((const __m128i*)(pSamplesOut  - 8));
            runningOrder -= 4;
        } else {
            switch (runningOrder) {
                case 3: coefficients128_4 = _mm_set_epi32(0, coefficients[6], coefficients[5], coefficients[4]); samples128_4 = _mm_set_epi32(pSamplesOut[-5], pSamplesOut[-6], pSamplesOut[-7], 0); break;
                case 2: coefficients128_4 = _mm_set_epi32(0, 0,               coefficients[5], coefficients[4]); samples128_4 = _mm_set_epi32(pSamplesOut[-5], pSamplesOut[-6], 0,               0); break;
                case 1: coefficients128_4 = _mm_set_epi32(0, 0,               0,               coefficients[4]); samples128_4 = _mm_set_epi32(pSamplesOut[-5], 0,               0,               0); break;
            }
            runningOrder = 0;
        }

        
        if (runningOrder == 4) {
            coefficients128_8 = _mm_loadu_si128((const __m128i*)(coefficients + 8));
            samples128_8      = _mm_loadu_si128((const __m128i*)(pSamplesOut  - 12));
            runningOrder -= 4;
        } else {
            switch (runningOrder) {
                case 3: coefficients128_8 = _mm_set_epi32(0, coefficients[10], coefficients[9], coefficients[8]); samples128_8 = _mm_set_epi32(pSamplesOut[-9], pSamplesOut[-10], pSamplesOut[-11], 0); break;
                case 2: coefficients128_8 = _mm_set_epi32(0, 0,                coefficients[9], coefficients[8]); samples128_8 = _mm_set_epi32(pSamplesOut[-9], pSamplesOut[-10], 0,                0); break;
                case 1: coefficients128_8 = _mm_set_epi32(0, 0,                0,               coefficients[8]); samples128_8 = _mm_set_epi32(pSamplesOut[-9], 0,                0,                0); break;
            }
            runningOrder = 0;
        }

        
        coefficients128_0 = _mm_shuffle_epi32(coefficients128_0, _MM_SHUFFLE(0, 1, 2, 3));
        coefficients128_4 = _mm_shuffle_epi32(coefficients128_4, _MM_SHUFFLE(0, 1, 2, 3));
        coefficients128_8 = _mm_shuffle_epi32(coefficients128_8, _MM_SHUFFLE(0, 1, 2, 3));
    }
#else
    switch (order)
    {
    case 12: ((drflac_int32*)&coefficients128_8)[0] = coefficients[11]; ((drflac_int32*)&samples128_8)[0] = pDecodedSamples[-12];
    case 11: ((drflac_int32*)&coefficients128_8)[1] = coefficients[10]; ((drflac_int32*)&samples128_8)[1] = pDecodedSamples[-11];
    case 10: ((drflac_int32*)&coefficients128_8)[2] = coefficients[ 9]; ((drflac_int32*)&samples128_8)[2] = pDecodedSamples[-10];
    case 9:  ((drflac_int32*)&coefficients128_8)[3] = coefficients[ 8]; ((drflac_int32*)&samples128_8)[3] = pDecodedSamples[- 9];
    case 8:  ((drflac_int32*)&coefficients128_4)[0] = coefficients[ 7]; ((drflac_int32*)&samples128_4)[0] = pDecodedSamples[- 8];
    case 7:  ((drflac_int32*)&coefficients128_4)[1] = coefficients[ 6]; ((drflac_int32*)&samples128_4)[1] = pDecodedSamples[- 7];
    case 6:  ((drflac_int32*)&coefficients128_4)[2] = coefficients[ 5]; ((drflac_int32*)&samples128_4)[2] = pDecodedSamples[- 6];
    case 5:  ((drflac_int32*)&coefficients128_4)[3] = coefficients[ 4]; ((drflac_int32*)&samples128_4)[3] = pDecodedSamples[- 5];
    case 4:  ((drflac_int32*)&coefficients128_0)[0] = coefficients[ 3]; ((drflac_int32*)&samples128_0)[0] = pDecodedSamples[- 4];
    case 3:  ((drflac_int32*)&coefficients128_0)[1] = coefficients[ 2]; ((drflac_int32*)&samples128_0)[1] = pDecodedSamples[- 3];
    case 2:  ((drflac_int32*)&coefficients128_0)[2] = coefficients[ 1]; ((drflac_int32*)&samples128_0)[2] = pDecodedSamples[- 2];
    case 1:  ((drflac_int32*)&coefficients128_0)[3] = coefficients[ 0]; ((drflac_int32*)&samples128_0)[3] = pDecodedSamples[- 1];
    }
#endif

    
    while (pDecodedSamples < pDecodedSamplesEnd) {
        __m128i zeroCountPart128;
        __m128i riceParamPart128;

        if (!drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts0, &riceParamParts0) ||
            !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts1, &riceParamParts1) ||
            !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts2, &riceParamParts2) ||
            !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts3, &riceParamParts3)) {
            return DRFLAC_FALSE;
        }

        zeroCountPart128 = _mm_set_epi32(zeroCountParts3, zeroCountParts2, zeroCountParts1, zeroCountParts0);
        riceParamPart128 = _mm_set_epi32(riceParamParts3, riceParamParts2, riceParamParts1, riceParamParts0);

        riceParamPart128 = _mm_and_si128(riceParamPart128, riceParamMask128);
        riceParamPart128 = _mm_or_si128(riceParamPart128, _mm_slli_epi32(zeroCountPart128, riceParam));
        riceParamPart128 = _mm_xor_si128(_mm_srli_epi32(riceParamPart128, 1), _mm_add_epi32(drflac__mm_not_si128(_mm_and_si128(riceParamPart128, _mm_set1_epi32(1))), _mm_set1_epi32(1)));

        for (i = 0; i < 4; i += 1) {
            prediction128 = _mm_xor_si128(prediction128, prediction128);    

            switch (order)
            {
            case 12:
            case 11: prediction128 = _mm_add_epi64(prediction128, _mm_mul_epi32(_mm_shuffle_epi32(coefficients128_8, _MM_SHUFFLE(1, 1, 0, 0)), _mm_shuffle_epi32(samples128_8, _MM_SHUFFLE(1, 1, 0, 0))));
            case 10:
            case  9: prediction128 = _mm_add_epi64(prediction128, _mm_mul_epi32(_mm_shuffle_epi32(coefficients128_8, _MM_SHUFFLE(3, 3, 2, 2)), _mm_shuffle_epi32(samples128_8, _MM_SHUFFLE(3, 3, 2, 2))));
            case  8:
            case  7: prediction128 = _mm_add_epi64(prediction128, _mm_mul_epi32(_mm_shuffle_epi32(coefficients128_4, _MM_SHUFFLE(1, 1, 0, 0)), _mm_shuffle_epi32(samples128_4, _MM_SHUFFLE(1, 1, 0, 0))));
            case  6:
            case  5: prediction128 = _mm_add_epi64(prediction128, _mm_mul_epi32(_mm_shuffle_epi32(coefficients128_4, _MM_SHUFFLE(3, 3, 2, 2)), _mm_shuffle_epi32(samples128_4, _MM_SHUFFLE(3, 3, 2, 2))));
            case  4:
            case  3: prediction128 = _mm_add_epi64(prediction128, _mm_mul_epi32(_mm_shuffle_epi32(coefficients128_0, _MM_SHUFFLE(1, 1, 0, 0)), _mm_shuffle_epi32(samples128_0, _MM_SHUFFLE(1, 1, 0, 0))));
            case  2:
            case  1: prediction128 = _mm_add_epi64(prediction128, _mm_mul_epi32(_mm_shuffle_epi32(coefficients128_0, _MM_SHUFFLE(3, 3, 2, 2)), _mm_shuffle_epi32(samples128_0, _MM_SHUFFLE(3, 3, 2, 2))));
            }

            
            prediction128 = drflac__mm_hadd_epi64(prediction128);
            prediction128 = drflac__mm_srai_epi64(prediction128, shift);
            prediction128 = _mm_add_epi32(riceParamPart128, prediction128);

            
            samples128_8 = _mm_alignr_epi8(samples128_4,  samples128_8, 4);
            samples128_4 = _mm_alignr_epi8(samples128_0,  samples128_4, 4);
            samples128_0 = _mm_alignr_epi8(prediction128, samples128_0, 4);

            
            riceParamPart128 = _mm_alignr_epi8(_mm_setzero_si128(), riceParamPart128, 4);
        }

        
        _mm_storeu_si128((__m128i*)pDecodedSamples, samples128_0);
        pDecodedSamples += 4;
    }

    
    i = (count & ~3);
    while (i < (int)count) {
        
        if (!drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts0, &riceParamParts0)) {
            return DRFLAC_FALSE;
        }

        
        riceParamParts0 &= riceParamMask;
        riceParamParts0 |= (zeroCountParts0 << riceParam);
        riceParamParts0  = (riceParamParts0 >> 1) ^ t[riceParamParts0 & 0x01];

        
        pDecodedSamples[0] = riceParamParts0 + drflac__calculate_prediction_64_s32(order, shift, coefficients, pDecodedSamples);

        i += 1;
        pDecodedSamples += 1;
    }

    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples_with_residual__rice__sse41(drflac_bs* bs, drflac_uint32 bitsPerSample, drflac_uint32 count, drflac_uint8 riceParam, drflac_uint32 lpcOrder, drflac_int32 lpcShift, drflac_uint32 lpcPrecision, const drflac_int32* coefficients, drflac_int32* pSamplesOut)
{
    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(pSamplesOut != NULL);

    
    if (lpcOrder > 0 && lpcOrder <= 12) {
        if (drflac__use_64_bit_prediction(bitsPerSample, lpcOrder, lpcPrecision)) {
            return drflac__decode_samples_with_residual__rice__sse41_64(bs, count, riceParam, lpcOrder, lpcShift, coefficients, pSamplesOut);
        } else {
            return drflac__decode_samples_with_residual__rice__sse41_32(bs, count, riceParam, lpcOrder, lpcShift, coefficients, pSamplesOut);
        }
    } else {
        return drflac__decode_samples_with_residual__rice__scalar(bs, bitsPerSample, count, riceParam, lpcOrder, lpcShift, lpcPrecision, coefficients, pSamplesOut);
    }
}
#endif

#if defined(DRFLAC_SUPPORT_NEON)
static DRFLAC_INLINE void drflac__vst2q_s32(drflac_int32* p, int32x4x2_t x)
{
    vst1q_s32(p+0, x.val[0]);
    vst1q_s32(p+4, x.val[1]);
}

static DRFLAC_INLINE void drflac__vst2q_u32(drflac_uint32* p, uint32x4x2_t x)
{
    vst1q_u32(p+0, x.val[0]);
    vst1q_u32(p+4, x.val[1]);
}

static DRFLAC_INLINE void drflac__vst2q_f32(float* p, float32x4x2_t x)
{
    vst1q_f32(p+0, x.val[0]);
    vst1q_f32(p+4, x.val[1]);
}

static DRFLAC_INLINE void drflac__vst2q_s16(drflac_int16* p, int16x4x2_t x)
{
    vst1q_s16(p, vcombine_s16(x.val[0], x.val[1]));
}

static DRFLAC_INLINE void drflac__vst2q_u16(drflac_uint16* p, uint16x4x2_t x)
{
    vst1q_u16(p, vcombine_u16(x.val[0], x.val[1]));
}

static DRFLAC_INLINE int32x4_t drflac__vdupq_n_s32x4(drflac_int32 x3, drflac_int32 x2, drflac_int32 x1, drflac_int32 x0)
{
    drflac_int32 x[4];
    x[3] = x3;
    x[2] = x2;
    x[1] = x1;
    x[0] = x0;
    return vld1q_s32(x);
}

static DRFLAC_INLINE int32x4_t drflac__valignrq_s32_1(int32x4_t a, int32x4_t b)
{
    

    
    






    return vextq_s32(b, a, 1);
}

static DRFLAC_INLINE uint32x4_t drflac__valignrq_u32_1(uint32x4_t a, uint32x4_t b)
{
    

    
    






    return vextq_u32(b, a, 1);
}

static DRFLAC_INLINE int32x2_t drflac__vhaddq_s32(int32x4_t x)
{
    

    
    






    int32x2_t r = vadd_s32(vget_high_s32(x), vget_low_s32(x));
    return vpadd_s32(r, r);
}

static DRFLAC_INLINE int64x1_t drflac__vhaddq_s64(int64x2_t x)
{
    return vadd_s64(vget_high_s64(x), vget_low_s64(x));
}

static DRFLAC_INLINE int32x4_t drflac__vrevq_s32(int32x4_t x)
{
    
    






    return vrev64q_s32(vcombine_s32(vget_high_s32(x), vget_low_s32(x)));
}

static DRFLAC_INLINE int32x4_t drflac__vnotq_s32(int32x4_t x)
{
    return veorq_s32(x, vdupq_n_s32(0xFFFFFFFF));
}

static DRFLAC_INLINE uint32x4_t drflac__vnotq_u32(uint32x4_t x)
{
    return veorq_u32(x, vdupq_n_u32(0xFFFFFFFF));
}

static drflac_bool32 drflac__decode_samples_with_residual__rice__neon_32(drflac_bs* bs, drflac_uint32 count, drflac_uint8 riceParam, drflac_uint32 order, drflac_int32 shift, const drflac_int32* coefficients, drflac_int32* pSamplesOut)
{
    int i;
    drflac_uint32 riceParamMask;
    drflac_int32* pDecodedSamples    = pSamplesOut;
    drflac_int32* pDecodedSamplesEnd = pSamplesOut + (count & ~3);
    drflac_uint32 zeroCountParts[4];
    drflac_uint32 riceParamParts[4];
    int32x4_t coefficients128_0;
    int32x4_t coefficients128_4;
    int32x4_t coefficients128_8;
    int32x4_t samples128_0;
    int32x4_t samples128_4;
    int32x4_t samples128_8;
    uint32x4_t riceParamMask128;
    int32x4_t riceParam128;
    int32x2_t shift64;
    uint32x4_t one128;

    const drflac_uint32 t[2] = {0x00000000, 0xFFFFFFFF};

    riceParamMask    = (drflac_uint32)~((~0UL) << riceParam);
    riceParamMask128 = vdupq_n_u32(riceParamMask);

    riceParam128 = vdupq_n_s32(riceParam);
    shift64 = vdup_n_s32(-shift); 
    one128 = vdupq_n_u32(1);

    





    {
        int runningOrder = order;
        drflac_int32 tempC[4] = {0, 0, 0, 0};
        drflac_int32 tempS[4] = {0, 0, 0, 0};

        
        if (runningOrder >= 4) {
            coefficients128_0 = vld1q_s32(coefficients + 0);
            samples128_0      = vld1q_s32(pSamplesOut  - 4);
            runningOrder -= 4;
        } else {
            switch (runningOrder) {
                case 3: tempC[2] = coefficients[2]; tempS[1] = pSamplesOut[-3]; 
                case 2: tempC[1] = coefficients[1]; tempS[2] = pSamplesOut[-2]; 
                case 1: tempC[0] = coefficients[0]; tempS[3] = pSamplesOut[-1]; 
            }

            coefficients128_0 = vld1q_s32(tempC);
            samples128_0      = vld1q_s32(tempS);
            runningOrder = 0;
        }

        
        if (runningOrder >= 4) {
            coefficients128_4 = vld1q_s32(coefficients + 4);
            samples128_4      = vld1q_s32(pSamplesOut  - 8);
            runningOrder -= 4;
        } else {
            switch (runningOrder) {
                case 3: tempC[2] = coefficients[6]; tempS[1] = pSamplesOut[-7]; 
                case 2: tempC[1] = coefficients[5]; tempS[2] = pSamplesOut[-6]; 
                case 1: tempC[0] = coefficients[4]; tempS[3] = pSamplesOut[-5]; 
            }

            coefficients128_4 = vld1q_s32(tempC);
            samples128_4      = vld1q_s32(tempS);
            runningOrder = 0;
        }

        
        if (runningOrder == 4) {
            coefficients128_8 = vld1q_s32(coefficients + 8);
            samples128_8      = vld1q_s32(pSamplesOut  - 12);
            runningOrder -= 4;
        } else {
            switch (runningOrder) {
                case 3: tempC[2] = coefficients[10]; tempS[1] = pSamplesOut[-11]; 
                case 2: tempC[1] = coefficients[ 9]; tempS[2] = pSamplesOut[-10]; 
                case 1: tempC[0] = coefficients[ 8]; tempS[3] = pSamplesOut[- 9]; 
            }

            coefficients128_8 = vld1q_s32(tempC);
            samples128_8      = vld1q_s32(tempS);
            runningOrder = 0;
        }

        
        coefficients128_0 = drflac__vrevq_s32(coefficients128_0);
        coefficients128_4 = drflac__vrevq_s32(coefficients128_4);
        coefficients128_8 = drflac__vrevq_s32(coefficients128_8);
    }

    
    while (pDecodedSamples < pDecodedSamplesEnd) {
        int32x4_t prediction128;
        int32x2_t prediction64;
        uint32x4_t zeroCountPart128;
        uint32x4_t riceParamPart128;

        if (!drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts[0], &riceParamParts[0]) ||
            !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts[1], &riceParamParts[1]) ||
            !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts[2], &riceParamParts[2]) ||
            !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts[3], &riceParamParts[3])) {
            return DRFLAC_FALSE;
        }

        zeroCountPart128 = vld1q_u32(zeroCountParts);
        riceParamPart128 = vld1q_u32(riceParamParts);

        riceParamPart128 = vandq_u32(riceParamPart128, riceParamMask128);
        riceParamPart128 = vorrq_u32(riceParamPart128, vshlq_u32(zeroCountPart128, riceParam128));
        riceParamPart128 = veorq_u32(vshrq_n_u32(riceParamPart128, 1), vaddq_u32(drflac__vnotq_u32(vandq_u32(riceParamPart128, one128)), one128));

        if (order <= 4) {
            for (i = 0; i < 4; i += 1) {
                prediction128 = vmulq_s32(coefficients128_0, samples128_0);

                
                prediction64 = drflac__vhaddq_s32(prediction128);
                prediction64 = vshl_s32(prediction64, shift64);
                prediction64 = vadd_s32(prediction64, vget_low_s32(vreinterpretq_s32_u32(riceParamPart128)));

                samples128_0 = drflac__valignrq_s32_1(vcombine_s32(prediction64, vdup_n_s32(0)), samples128_0);
                riceParamPart128 = drflac__valignrq_u32_1(vdupq_n_u32(0), riceParamPart128);
            }
        } else if (order <= 8) {
            for (i = 0; i < 4; i += 1) {
                prediction128 =                vmulq_s32(coefficients128_4, samples128_4);
                prediction128 = vmlaq_s32(prediction128, coefficients128_0, samples128_0);

                
                prediction64 = drflac__vhaddq_s32(prediction128);
                prediction64 = vshl_s32(prediction64, shift64);
                prediction64 = vadd_s32(prediction64, vget_low_s32(vreinterpretq_s32_u32(riceParamPart128)));

                samples128_4 = drflac__valignrq_s32_1(samples128_0, samples128_4);
                samples128_0 = drflac__valignrq_s32_1(vcombine_s32(prediction64, vdup_n_s32(0)), samples128_0);
                riceParamPart128 = drflac__valignrq_u32_1(vdupq_n_u32(0), riceParamPart128);
            }
        } else {
            for (i = 0; i < 4; i += 1) {
                prediction128 =                vmulq_s32(coefficients128_8, samples128_8);
                prediction128 = vmlaq_s32(prediction128, coefficients128_4, samples128_4);
                prediction128 = vmlaq_s32(prediction128, coefficients128_0, samples128_0);

                
                prediction64 = drflac__vhaddq_s32(prediction128);
                prediction64 = vshl_s32(prediction64, shift64);
                prediction64 = vadd_s32(prediction64, vget_low_s32(vreinterpretq_s32_u32(riceParamPart128)));

                samples128_8 = drflac__valignrq_s32_1(samples128_4, samples128_8);
                samples128_4 = drflac__valignrq_s32_1(samples128_0, samples128_4);
                samples128_0 = drflac__valignrq_s32_1(vcombine_s32(prediction64, vdup_n_s32(0)), samples128_0);
                riceParamPart128 = drflac__valignrq_u32_1(vdupq_n_u32(0), riceParamPart128);
            }
        }

        
        vst1q_s32(pDecodedSamples, samples128_0);
        pDecodedSamples += 4;
    }

    
    i = (count & ~3);
    while (i < (int)count) {
        
        if (!drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts[0], &riceParamParts[0])) {
            return DRFLAC_FALSE;
        }

        
        riceParamParts[0] &= riceParamMask;
        riceParamParts[0] |= (zeroCountParts[0] << riceParam);
        riceParamParts[0]  = (riceParamParts[0] >> 1) ^ t[riceParamParts[0] & 0x01];

        
        pDecodedSamples[0] = riceParamParts[0] + drflac__calculate_prediction_32_s32(order, shift, coefficients, pDecodedSamples);

        i += 1;
        pDecodedSamples += 1;
    }

    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples_with_residual__rice__neon_64(drflac_bs* bs, drflac_uint32 count, drflac_uint8 riceParam, drflac_uint32 order, drflac_int32 shift, const drflac_int32* coefficients, drflac_int32* pSamplesOut)
{
    int i;
    drflac_uint32 riceParamMask;
    drflac_int32* pDecodedSamples    = pSamplesOut;
    drflac_int32* pDecodedSamplesEnd = pSamplesOut + (count & ~3);
    drflac_uint32 zeroCountParts[4];
    drflac_uint32 riceParamParts[4];
    int32x4_t coefficients128_0;
    int32x4_t coefficients128_4;
    int32x4_t coefficients128_8;
    int32x4_t samples128_0;
    int32x4_t samples128_4;
    int32x4_t samples128_8;
    uint32x4_t riceParamMask128;
    int32x4_t riceParam128;
    int64x1_t shift64;
    uint32x4_t one128;
    int64x2_t prediction128;
    uint32x4_t zeroCountPart128;
    uint32x4_t riceParamPart128;

    const drflac_uint32 t[2] = {0x00000000, 0xFFFFFFFF};

    riceParamMask    = (drflac_uint32)~((~0UL) << riceParam);
    riceParamMask128 = vdupq_n_u32(riceParamMask);

    riceParam128 = vdupq_n_s32(riceParam);
    shift64 = vdup_n_s64(-shift); 
    one128 = vdupq_n_u32(1);

    





    {
        int runningOrder = order;
        drflac_int32 tempC[4] = {0, 0, 0, 0};
        drflac_int32 tempS[4] = {0, 0, 0, 0};

        
        if (runningOrder >= 4) {
            coefficients128_0 = vld1q_s32(coefficients + 0);
            samples128_0      = vld1q_s32(pSamplesOut  - 4);
            runningOrder -= 4;
        } else {
            switch (runningOrder) {
                case 3: tempC[2] = coefficients[2]; tempS[1] = pSamplesOut[-3]; 
                case 2: tempC[1] = coefficients[1]; tempS[2] = pSamplesOut[-2]; 
                case 1: tempC[0] = coefficients[0]; tempS[3] = pSamplesOut[-1]; 
            }

            coefficients128_0 = vld1q_s32(tempC);
            samples128_0      = vld1q_s32(tempS);
            runningOrder = 0;
        }

        
        if (runningOrder >= 4) {
            coefficients128_4 = vld1q_s32(coefficients + 4);
            samples128_4      = vld1q_s32(pSamplesOut  - 8);
            runningOrder -= 4;
        } else {
            switch (runningOrder) {
                case 3: tempC[2] = coefficients[6]; tempS[1] = pSamplesOut[-7]; 
                case 2: tempC[1] = coefficients[5]; tempS[2] = pSamplesOut[-6]; 
                case 1: tempC[0] = coefficients[4]; tempS[3] = pSamplesOut[-5]; 
            }

            coefficients128_4 = vld1q_s32(tempC);
            samples128_4      = vld1q_s32(tempS);
            runningOrder = 0;
        }

        
        if (runningOrder == 4) {
            coefficients128_8 = vld1q_s32(coefficients + 8);
            samples128_8      = vld1q_s32(pSamplesOut  - 12);
            runningOrder -= 4;
        } else {
            switch (runningOrder) {
                case 3: tempC[2] = coefficients[10]; tempS[1] = pSamplesOut[-11]; 
                case 2: tempC[1] = coefficients[ 9]; tempS[2] = pSamplesOut[-10]; 
                case 1: tempC[0] = coefficients[ 8]; tempS[3] = pSamplesOut[- 9]; 
            }

            coefficients128_8 = vld1q_s32(tempC);
            samples128_8      = vld1q_s32(tempS);
            runningOrder = 0;
        }

        
        coefficients128_0 = drflac__vrevq_s32(coefficients128_0);
        coefficients128_4 = drflac__vrevq_s32(coefficients128_4);
        coefficients128_8 = drflac__vrevq_s32(coefficients128_8);
    }

    
    while (pDecodedSamples < pDecodedSamplesEnd) {
        if (!drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts[0], &riceParamParts[0]) ||
            !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts[1], &riceParamParts[1]) ||
            !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts[2], &riceParamParts[2]) ||
            !drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts[3], &riceParamParts[3])) {
            return DRFLAC_FALSE;
        }

        zeroCountPart128 = vld1q_u32(zeroCountParts);
        riceParamPart128 = vld1q_u32(riceParamParts);

        riceParamPart128 = vandq_u32(riceParamPart128, riceParamMask128);
        riceParamPart128 = vorrq_u32(riceParamPart128, vshlq_u32(zeroCountPart128, riceParam128));
        riceParamPart128 = veorq_u32(vshrq_n_u32(riceParamPart128, 1), vaddq_u32(drflac__vnotq_u32(vandq_u32(riceParamPart128, one128)), one128));

        for (i = 0; i < 4; i += 1) {
            int64x1_t prediction64;

            prediction128 = vdupq_n_s64(0);
            switch (order)
            {
            case 12:
            case 11: prediction128 = vaddq_s64(prediction128, vmull_s32(vget_low_s32(coefficients128_8), vget_low_s32(samples128_8)));
            case 10:
            case  9: prediction128 = vaddq_s64(prediction128, vmull_s32(vget_high_s32(coefficients128_8), vget_high_s32(samples128_8)));
            case  8:
            case  7: prediction128 = vaddq_s64(prediction128, vmull_s32(vget_low_s32(coefficients128_4), vget_low_s32(samples128_4)));
            case  6:
            case  5: prediction128 = vaddq_s64(prediction128, vmull_s32(vget_high_s32(coefficients128_4), vget_high_s32(samples128_4)));
            case  4:
            case  3: prediction128 = vaddq_s64(prediction128, vmull_s32(vget_low_s32(coefficients128_0), vget_low_s32(samples128_0)));
            case  2:
            case  1: prediction128 = vaddq_s64(prediction128, vmull_s32(vget_high_s32(coefficients128_0), vget_high_s32(samples128_0)));
            }

            
            prediction64 = drflac__vhaddq_s64(prediction128);
            prediction64 = vshl_s64(prediction64, shift64);
            prediction64 = vadd_s64(prediction64, vdup_n_s64(vgetq_lane_u32(riceParamPart128, 0)));

            
            samples128_8 = drflac__valignrq_s32_1(samples128_4, samples128_8);
            samples128_4 = drflac__valignrq_s32_1(samples128_0, samples128_4);
            samples128_0 = drflac__valignrq_s32_1(vcombine_s32(vreinterpret_s32_s64(prediction64), vdup_n_s32(0)), samples128_0);

            
            riceParamPart128 = drflac__valignrq_u32_1(vdupq_n_u32(0), riceParamPart128);
        }

        
        vst1q_s32(pDecodedSamples, samples128_0);
        pDecodedSamples += 4;
    }

    
    i = (count & ~3);
    while (i < (int)count) {
        
        if (!drflac__read_rice_parts_x1(bs, riceParam, &zeroCountParts[0], &riceParamParts[0])) {
            return DRFLAC_FALSE;
        }

        
        riceParamParts[0] &= riceParamMask;
        riceParamParts[0] |= (zeroCountParts[0] << riceParam);
        riceParamParts[0]  = (riceParamParts[0] >> 1) ^ t[riceParamParts[0] & 0x01];

        
        pDecodedSamples[0] = riceParamParts[0] + drflac__calculate_prediction_64_s32(order, shift, coefficients, pDecodedSamples);

        i += 1;
        pDecodedSamples += 1;
    }

    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples_with_residual__rice__neon(drflac_bs* bs, drflac_uint32 bitsPerSample, drflac_uint32 count, drflac_uint8 riceParam, drflac_uint32 lpcOrder, drflac_int32 lpcShift, drflac_uint32 lpcPrecision, const drflac_int32* coefficients, drflac_int32* pSamplesOut)
{
    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(pSamplesOut != NULL);

    
    if (lpcOrder > 0 && lpcOrder <= 12) {
        if (drflac__use_64_bit_prediction(bitsPerSample, lpcOrder, lpcPrecision)) {
            return drflac__decode_samples_with_residual__rice__neon_64(bs, count, riceParam, lpcOrder, lpcShift, coefficients, pSamplesOut);
        } else {
            return drflac__decode_samples_with_residual__rice__neon_32(bs, count, riceParam, lpcOrder, lpcShift, coefficients, pSamplesOut);
        }
    } else {
        return drflac__decode_samples_with_residual__rice__scalar(bs, bitsPerSample, count, riceParam, lpcOrder, lpcShift, lpcPrecision, coefficients, pSamplesOut);
    }
}
#endif

static drflac_bool32 drflac__decode_samples_with_residual__rice(drflac_bs* bs, drflac_uint32 bitsPerSample, drflac_uint32 count, drflac_uint8 riceParam, drflac_uint32 lpcOrder, drflac_int32 lpcShift, drflac_uint32 lpcPrecision, const drflac_int32* coefficients, drflac_int32* pSamplesOut)
{
#if defined(DRFLAC_SUPPORT_SSE41)
    if (drflac__gIsSSE41Supported) {
        return drflac__decode_samples_with_residual__rice__sse41(bs, bitsPerSample, count, riceParam, lpcOrder, lpcShift, lpcPrecision, coefficients, pSamplesOut);
    } else
#elif defined(DRFLAC_SUPPORT_NEON)
    if (drflac__gIsNEONSupported) {
        return drflac__decode_samples_with_residual__rice__neon(bs, bitsPerSample, count, riceParam, lpcOrder, lpcShift, lpcPrecision, coefficients, pSamplesOut);
    } else
#endif
    {
        
    #if 0
        return drflac__decode_samples_with_residual__rice__reference(bs, bitsPerSample, count, riceParam, lpcOrder, lpcShift, lpcPrecision, coefficients, pSamplesOut);
    #else
        return drflac__decode_samples_with_residual__rice__scalar(bs, bitsPerSample, count, riceParam, lpcOrder, lpcShift, lpcPrecision, coefficients, pSamplesOut);
    #endif
    }
}


static drflac_bool32 drflac__read_and_seek_residual__rice(drflac_bs* bs, drflac_uint32 count, drflac_uint8 riceParam)
{
    drflac_uint32 i;

    DRFLAC_ASSERT(bs != NULL);

    for (i = 0; i < count; ++i) {
        if (!drflac__seek_rice_parts(bs, riceParam)) {
            return DRFLAC_FALSE;
        }
    }

    return DRFLAC_TRUE;
}

#if defined(__clang__)
__attribute__((no_sanitize("signed-integer-overflow")))
#endif
static drflac_bool32 drflac__decode_samples_with_residual__unencoded(drflac_bs* bs, drflac_uint32 bitsPerSample, drflac_uint32 count, drflac_uint8 unencodedBitsPerSample, drflac_uint32 lpcOrder, drflac_int32 lpcShift, drflac_uint32 lpcPrecision, const drflac_int32* coefficients, drflac_int32* pSamplesOut)
{
    drflac_uint32 i;

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(unencodedBitsPerSample <= 31);    
    DRFLAC_ASSERT(pSamplesOut != NULL);

    for (i = 0; i < count; ++i) {
        if (unencodedBitsPerSample > 0) {
            if (!drflac__read_int32(bs, unencodedBitsPerSample, pSamplesOut + i)) {
                return DRFLAC_FALSE;
            }
        } else {
            pSamplesOut[i] = 0;
        }

        if (drflac__use_64_bit_prediction(bitsPerSample, lpcOrder, lpcPrecision)) {
            pSamplesOut[i] += drflac__calculate_prediction_64_s32(lpcOrder, lpcShift, coefficients, pSamplesOut + i);
        } else {
            pSamplesOut[i] += drflac__calculate_prediction_32_s32(lpcOrder, lpcShift, coefficients, pSamplesOut + i);
        }
    }

    return DRFLAC_TRUE;
}


static DRFLAC_INLINE drflac_int64 drflac__calculate_prediction_64_s64(drflac_uint32 order, drflac_int32 shift, const drflac_int32* coefficients, const drflac_int64* pDecodedSamples)
{
    drflac_int64 prediction = 0;
    drflac_uint32 i;

    
    for (i = 0; i < order; ++i) {
        prediction += coefficients[i] * pDecodedSamples[-(drflac_int32)i - 1];
    }

    return prediction >> shift;
}

static drflac_bool32 drflac__decode_samples_with_residual__rice__s64(drflac_bs* bs, drflac_uint32 bitsPerSample, drflac_uint32 count, drflac_uint8 riceParam, drflac_uint32 lpcOrder, drflac_int32 lpcShift, const drflac_int32* coefficients, drflac_int64* pSamplesOut)
{
    drflac_uint32 riceParamMask;
    drflac_int64 minSample;
    drflac_int64 maxSample;
    drflac_uint32 i;

    DRFLAC_ASSERT(bitsPerSample > 0 && bitsPerSample <= 33);

    riceParamMask = (1U << riceParam) - 1;
    minSample = -((drflac_int64)1 << (bitsPerSample - 1));
    maxSample = -(minSample + 1);

    for (i = 0; i < count; ++i) {
        drflac_uint32 zeroCountPart;
        drflac_uint32 riceParamPart;
        drflac_int32 residual;
        drflac_int64 sample;

        if (!drflac__read_rice_parts_x1(bs, riceParam, &zeroCountPart, &riceParamPart)) {
            return DRFLAC_FALSE;
        }

        
        if (zeroCountPart > (0xFFFFFFFFU >> riceParam)) {
            return DRFLAC_FALSE;
        }

        riceParamPart = (zeroCountPart << riceParam) | (riceParamPart & riceParamMask);
        if (riceParamPart == 0xFFFFFFFFU) {
            return DRFLAC_FALSE;
        }

        residual = (drflac_int32)(riceParamPart >> 1);
        if ((riceParamPart & 1) != 0) {
            residual = -residual - 1;
        }

        sample = drflac__calculate_prediction_64_s64(lpcOrder, lpcShift, coefficients, pSamplesOut + i) + residual;
        if (sample < minSample || sample > maxSample) {
            return DRFLAC_FALSE;
        }

        pSamplesOut[i] = sample;
    }

    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples_with_residual__unencoded_s64(drflac_bs* bs, drflac_uint32 bitsPerSample, drflac_uint32 count, drflac_uint8 unencodedBitsPerSample, drflac_uint32 lpcOrder, drflac_int32 lpcShift, const drflac_int32* coefficients, drflac_int64* pSamplesOut)
{
    drflac_int64 minSample;
    drflac_int64 maxSample;
    drflac_uint32 i;

    DRFLAC_ASSERT(bitsPerSample > 0 && bitsPerSample <= 33);
    DRFLAC_ASSERT(unencodedBitsPerSample <= 31);

    minSample = -((drflac_int64)1 << (bitsPerSample - 1));
    maxSample = -(minSample + 1);

    for (i = 0; i < count; ++i) {
        drflac_int32 residual = 0;
        drflac_int64 sample;

        if (unencodedBitsPerSample > 0) {
            if (!drflac__read_int32(bs, unencodedBitsPerSample, &residual)) {
                return DRFLAC_FALSE;
            }
        }

        sample = drflac__calculate_prediction_64_s64(lpcOrder, lpcShift, coefficients, pSamplesOut + i) + residual;
        if (sample < minSample || sample > maxSample) {
            return DRFLAC_FALSE;
        }

        pSamplesOut[i] = sample;
    }

    return DRFLAC_TRUE;
}






static drflac_bool32 drflac__decode_samples_with_residual(drflac_bs* bs, drflac_uint32 bitsPerSample, drflac_uint32 blockSize, drflac_uint32 lpcOrder, drflac_int32 lpcShift, drflac_uint32 lpcPrecision, const drflac_int32* coefficients, drflac_int32* pDecodedSamplesS32, drflac_int64* pDecodedSamplesS64)
{
    drflac_uint8 residualMethod;
    drflac_uint8 partitionOrder;
    drflac_uint32 samplesInPartition;
    drflac_uint32 partitionsRemaining;

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(blockSize != 0);
    DRFLAC_ASSERT(pDecodedSamplesS32 != NULL || pDecodedSamplesS64 != NULL);
    DRFLAC_ASSERT(pDecodedSamplesS32 == NULL || pDecodedSamplesS64 == NULL);

    if (!drflac__read_uint8(bs, 2, &residualMethod)) {
        return DRFLAC_FALSE;
    }

    if (residualMethod != DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE && residualMethod != DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE2) {
        return DRFLAC_FALSE;    
    }

    if (!drflac__read_uint8(bs, 4, &partitionOrder)) {
        return DRFLAC_FALSE;
    }

    







    if ((blockSize & ((1U << partitionOrder) - 1)) != 0) {
        return DRFLAC_FALSE;
    }

    
    if ((blockSize / (1 << partitionOrder)) < lpcOrder) {
        return DRFLAC_FALSE;
    }

    
    if (pDecodedSamplesS32 != NULL) {
        pDecodedSamplesS32 += lpcOrder;
    } else {
        pDecodedSamplesS64 += lpcOrder;
    }

    samplesInPartition = (blockSize / (1 << partitionOrder)) - lpcOrder;
    partitionsRemaining = (1 << partitionOrder);
    for (;;) {
        drflac_uint8 riceParam = 0;
        if (residualMethod == DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE) {
            if (!drflac__read_uint8(bs, 4, &riceParam)) {
                return DRFLAC_FALSE;
            }
            if (riceParam == 15) {
                riceParam = 0xFF;
            }
        } else if (residualMethod == DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE2) {
            if (!drflac__read_uint8(bs, 5, &riceParam)) {
                return DRFLAC_FALSE;
            }
            if (riceParam == 31) {
                riceParam = 0xFF;
            }
        }

        if (riceParam != 0xFF) {
            if (pDecodedSamplesS32 != NULL) {
                if (!drflac__decode_samples_with_residual__rice(bs, bitsPerSample, samplesInPartition, riceParam, lpcOrder, lpcShift, lpcPrecision, coefficients, pDecodedSamplesS32)) {
                    return DRFLAC_FALSE;
                }
            } else {
                if (!drflac__decode_samples_with_residual__rice__s64(bs, bitsPerSample, samplesInPartition, riceParam, lpcOrder, lpcShift, coefficients, pDecodedSamplesS64)) {
                    return DRFLAC_FALSE;
                }
            }
        } else {
            drflac_uint8 unencodedBitsPerSample = 0;
            if (!drflac__read_uint8(bs, 5, &unencodedBitsPerSample)) {
                return DRFLAC_FALSE;
            }

            if (pDecodedSamplesS32 != NULL) {
                if (!drflac__decode_samples_with_residual__unencoded(bs, bitsPerSample, samplesInPartition, unencodedBitsPerSample, lpcOrder, lpcShift, lpcPrecision, coefficients, pDecodedSamplesS32)) {
                    return DRFLAC_FALSE;
                }
            } else {
                if (!drflac__decode_samples_with_residual__unencoded_s64(bs, bitsPerSample, samplesInPartition, unencodedBitsPerSample, lpcOrder, lpcShift, coefficients, pDecodedSamplesS64)) {
                    return DRFLAC_FALSE;
                }
            }
        }

        if (pDecodedSamplesS32 != NULL) {
            pDecodedSamplesS32 += samplesInPartition;
        } else {
            pDecodedSamplesS64 += samplesInPartition;
        }

        if (partitionsRemaining == 1) {
            break;
        }

        partitionsRemaining -= 1;

        if (partitionOrder != 0) {
            samplesInPartition = blockSize / (1 << partitionOrder);
        }
    }

    return DRFLAC_TRUE;
}






static drflac_bool32 drflac__read_and_seek_residual(drflac_bs* bs, drflac_uint32 blockSize, drflac_uint32 order)
{
    drflac_uint8 residualMethod;
    drflac_uint8 partitionOrder;
    drflac_uint32 samplesInPartition;
    drflac_uint32 partitionsRemaining;

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(blockSize != 0);

    if (!drflac__read_uint8(bs, 2, &residualMethod)) {
        return DRFLAC_FALSE;
    }

    if (residualMethod != DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE && residualMethod != DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE2) {
        return DRFLAC_FALSE;    
    }

    if (!drflac__read_uint8(bs, 4, &partitionOrder)) {
        return DRFLAC_FALSE;
    }

    







    if ((blockSize & ((1U << partitionOrder) - 1)) != 0) {
        return DRFLAC_FALSE;
    }

    
    if ((blockSize / (1 << partitionOrder)) < order) {
        return DRFLAC_FALSE;
    }

    samplesInPartition = (blockSize / (1 << partitionOrder)) - order;
    partitionsRemaining = (1 << partitionOrder);
    for (;;)
    {
        drflac_uint8 riceParam = 0;
        if (residualMethod == DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE) {
            if (!drflac__read_uint8(bs, 4, &riceParam)) {
                return DRFLAC_FALSE;
            }
            if (riceParam == 15) {
                riceParam = 0xFF;
            }
        } else if (residualMethod == DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE2) {
            if (!drflac__read_uint8(bs, 5, &riceParam)) {
                return DRFLAC_FALSE;
            }
            if (riceParam == 31) {
                riceParam = 0xFF;
            }
        }

        if (riceParam != 0xFF) {
            if (!drflac__read_and_seek_residual__rice(bs, samplesInPartition, riceParam)) {
                return DRFLAC_FALSE;
            }
        } else {
            drflac_uint8 unencodedBitsPerSample = 0;
            if (!drflac__read_uint8(bs, 5, &unencodedBitsPerSample)) {
                return DRFLAC_FALSE;
            }

            if (!drflac__seek_bits(bs, unencodedBitsPerSample * samplesInPartition)) {
                return DRFLAC_FALSE;
            }
        }


        if (partitionsRemaining == 1) {
            break;
        }

        partitionsRemaining -= 1;
        samplesInPartition = blockSize / (1 << partitionOrder);
    }

    return DRFLAC_TRUE;
}


static drflac_bool32 drflac__decode_samples__constant(drflac_bs* bs, drflac_uint32 blockSize, drflac_uint32 subframeBitsPerSample, drflac_int32* pDecodedSamplesS32, drflac_int64* pDecodedSamplesS64)
{
    drflac_uint32 i;

    DRFLAC_ASSERT(pDecodedSamplesS32 != NULL || pDecodedSamplesS64 != NULL);
    DRFLAC_ASSERT(pDecodedSamplesS32 == NULL || pDecodedSamplesS64 == NULL);

    



    if (pDecodedSamplesS32 != NULL) {
        drflac_int32 sample;
        if (!drflac__read_int32(bs, subframeBitsPerSample, &sample)) {
            return DRFLAC_FALSE;
        }

        for (i = 0; i < blockSize; ++i) {
            pDecodedSamplesS32[i] = sample;
        }
    } else {
        drflac_int64 sample;
        if (!drflac__read_int64(bs, subframeBitsPerSample, &sample)) {
            return DRFLAC_FALSE;
        }

        for (i = 0; i < blockSize; ++i) {
            pDecodedSamplesS64[i] = sample;
        }
    }

    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples__verbatim(drflac_bs* bs, drflac_uint32 blockSize, drflac_uint32 subframeBitsPerSample, drflac_int32* pDecodedSamplesS32, drflac_int64* pDecodedSamplesS64)
{
    drflac_uint32 i;

    DRFLAC_ASSERT(pDecodedSamplesS32 != NULL || pDecodedSamplesS64 != NULL);
    DRFLAC_ASSERT(pDecodedSamplesS32 == NULL || pDecodedSamplesS64 == NULL);

    if (pDecodedSamplesS32 != NULL) {
        for (i = 0; i < blockSize; ++i) {
            if (!drflac__read_int32(bs, subframeBitsPerSample, pDecodedSamplesS32 + i)) {
                return DRFLAC_FALSE;
            }
        }
    } else {
        for (i = 0; i < blockSize; ++i) {
            if (!drflac__read_int64(bs, subframeBitsPerSample, pDecodedSamplesS64 + i)) {
                return DRFLAC_FALSE;
            }
        }
    }

    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples__fixed(drflac_bs* bs, drflac_uint32 blockSize, drflac_uint32 subframeBitsPerSample, drflac_uint8 lpcOrder, drflac_int32* pDecodedSamplesS32, drflac_int64* pDecodedSamplesS64)
{
    drflac_uint32 i;

    static drflac_int32 lpcCoefficientsTable[5][4] = {
        {0,  0, 0,  0},
        {1,  0, 0,  0},
        {2, -1, 0,  0},
        {3, -3, 1,  0},
        {4, -6, 4, -1}
    };

    DRFLAC_ASSERT(pDecodedSamplesS32 != NULL || pDecodedSamplesS64 != NULL);
    DRFLAC_ASSERT(pDecodedSamplesS32 == NULL || pDecodedSamplesS64 == NULL);

    
    if (pDecodedSamplesS32 != NULL) {
        for (i = 0; i < lpcOrder; ++i) {
            if (!drflac__read_int32(bs, subframeBitsPerSample, pDecodedSamplesS32 + i)) {
                return DRFLAC_FALSE;
            }
        }
    } else {
        for (i = 0; i < lpcOrder; ++i) {
            if (!drflac__read_int64(bs, subframeBitsPerSample, pDecodedSamplesS64 + i)) {
                return DRFLAC_FALSE;
            }
        }
    }

    if (!drflac__decode_samples_with_residual(bs, subframeBitsPerSample, blockSize, lpcOrder, 0, 4, lpcCoefficientsTable[lpcOrder], pDecodedSamplesS32, pDecodedSamplesS64)) {
        return DRFLAC_FALSE;
    }

    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples__lpc(drflac_bs* bs, drflac_uint32 blockSize, drflac_uint32 bitsPerSample, drflac_uint8 lpcOrder, drflac_int32* pDecodedSamplesS32, drflac_int64* pDecodedSamplesS64)
{
    drflac_uint8 i;
    drflac_uint8 lpcPrecision;
    drflac_int8 lpcShift;
    drflac_int32 coefficients[32];

    DRFLAC_ASSERT(pDecodedSamplesS32 != NULL || pDecodedSamplesS64 != NULL);
    DRFLAC_ASSERT(pDecodedSamplesS32 == NULL || pDecodedSamplesS64 == NULL);

    
    if (pDecodedSamplesS32 != NULL) {
        for (i = 0; i < lpcOrder; ++i) {
            if (!drflac__read_int32(bs, bitsPerSample, pDecodedSamplesS32 + i)) {
                return DRFLAC_FALSE;
            }
        }
    } else {
        for (i = 0; i < lpcOrder; ++i) {
            if (!drflac__read_int64(bs, bitsPerSample, pDecodedSamplesS64 + i)) {
                return DRFLAC_FALSE;
            }
        }
    }

    if (!drflac__read_uint8(bs, 4, &lpcPrecision)) {
        return DRFLAC_FALSE;
    }
    if (lpcPrecision == 15) {
        return DRFLAC_FALSE;    
    }
    lpcPrecision += 1;

    if (!drflac__read_int8(bs, 5, &lpcShift)) {
        return DRFLAC_FALSE;
    }

    







    if (lpcShift < 0) {
        return DRFLAC_FALSE;
    }

    DRFLAC_ZERO_MEMORY(coefficients, sizeof(coefficients));
    for (i = 0; i < lpcOrder; ++i) {
        if (!drflac__read_int32(bs, lpcPrecision, coefficients + i)) {
            return DRFLAC_FALSE;
        }
    }

    if (!drflac__decode_samples_with_residual(bs, bitsPerSample, blockSize, lpcOrder, lpcShift, lpcPrecision, coefficients, pDecodedSamplesS32, pDecodedSamplesS64)) {
        return DRFLAC_FALSE;
    }

    return DRFLAC_TRUE;
}


static drflac_bool32 drflac__read_next_flac_frame_header(drflac_bs* bs, drflac_uint8 streaminfoBitsPerSample, drflac_frame_header* header)
{
    const drflac_uint32 sampleRateTable[12]  = {0, 88200, 176400, 192000, 8000, 16000, 22050, 24000, 32000, 44100, 48000, 96000};
    const drflac_uint8 bitsPerSampleTable[8] = {0, 8, 12, (drflac_uint8)-1, 16, 20, 24, 32};   

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(header != NULL);

    
    for (;;) {
        drflac_uint8 crc8 = 0xCE; 
        drflac_uint8 reserved = 0;
        drflac_uint8 blockingStrategy = 0;
        drflac_uint8 blockSize = 0;
        drflac_uint8 sampleRate = 0;
        drflac_uint8 channelAssignment = 0;
        drflac_uint8 bitsPerSample = 0;
        drflac_bool32 isVariableBlockSize;

        if (!drflac__find_and_seek_to_next_sync_code(bs)) {
            return DRFLAC_FALSE;
        }

        if (!drflac__read_uint8(bs, 1, &reserved)) {
            return DRFLAC_FALSE;
        }
        if (reserved == 1) {
            continue;
        }
        crc8 = drflac_crc8(crc8, reserved, 1);

        if (!drflac__read_uint8(bs, 1, &blockingStrategy)) {
            return DRFLAC_FALSE;
        }
        crc8 = drflac_crc8(crc8, blockingStrategy, 1);

        if (!drflac__read_uint8(bs, 4, &blockSize)) {
            return DRFLAC_FALSE;
        }
        if (blockSize == 0) {
            continue;
        }
        crc8 = drflac_crc8(crc8, blockSize, 4);

        if (!drflac__read_uint8(bs, 4, &sampleRate)) {
            return DRFLAC_FALSE;
        }
        crc8 = drflac_crc8(crc8, sampleRate, 4);

        if (!drflac__read_uint8(bs, 4, &channelAssignment)) {
            return DRFLAC_FALSE;
        }
        if (channelAssignment > 10) {
            continue;
        }
        crc8 = drflac_crc8(crc8, channelAssignment, 4);

        if (!drflac__read_uint8(bs, 3, &bitsPerSample)) {
            return DRFLAC_FALSE;
        }
        if (bitsPerSample == 3) {
            continue;
        }
        crc8 = drflac_crc8(crc8, bitsPerSample, 3);


        if (!drflac__read_uint8(bs, 1, &reserved)) {
            return DRFLAC_FALSE;
        }
        if (reserved == 1) {
            continue;
        }
        crc8 = drflac_crc8(crc8, reserved, 1);


        isVariableBlockSize = blockingStrategy == 1;
        if (isVariableBlockSize) {
            drflac_uint64 pcmFrameNumber;
            drflac_result result = drflac__read_utf8_coded_number(bs, &pcmFrameNumber, &crc8);
            if (result != DRFLAC_SUCCESS) {
                if (result == DRFLAC_AT_END) {
                    return DRFLAC_FALSE;
                } else {
                    continue;
                }
            }
            header->flacFrameNumber  = 0;
            header->pcmFrameNumber = pcmFrameNumber;
        } else {
            drflac_uint64 flacFrameNumber = 0;
            drflac_result result = drflac__read_utf8_coded_number(bs, &flacFrameNumber, &crc8);
            if (result != DRFLAC_SUCCESS) {
                if (result == DRFLAC_AT_END) {
                    return DRFLAC_FALSE;
                } else {
                    continue;
                }
            }
            header->flacFrameNumber  = (drflac_uint32)flacFrameNumber;   
            header->pcmFrameNumber = 0;
        }


        DRFLAC_ASSERT(blockSize > 0);
        if (blockSize == 1) {
            header->blockSizeInPCMFrames = 192;
        } else if (blockSize <= 5) {
            DRFLAC_ASSERT(blockSize >= 2);
            header->blockSizeInPCMFrames = 576 * (1 << (blockSize - 2));
        } else if (blockSize == 6) {
            if (!drflac__read_uint16(bs, 8, &header->blockSizeInPCMFrames)) {
                return DRFLAC_FALSE;
            }
            crc8 = drflac_crc8(crc8, header->blockSizeInPCMFrames, 8);
            header->blockSizeInPCMFrames += 1;
        } else if (blockSize == 7) {
            if (!drflac__read_uint16(bs, 16, &header->blockSizeInPCMFrames)) {
                return DRFLAC_FALSE;
            }
            crc8 = drflac_crc8(crc8, header->blockSizeInPCMFrames, 16);
            if (header->blockSizeInPCMFrames == 0xFFFF) {
                return DRFLAC_FALSE;    
            }
            header->blockSizeInPCMFrames += 1;
        } else {
            DRFLAC_ASSERT(blockSize >= 8);
            header->blockSizeInPCMFrames = 256 * (1 << (blockSize - 8));
        }


        if (sampleRate <= 11) {
            header->sampleRate = sampleRateTable[sampleRate];
        } else if (sampleRate == 12) {
            if (!drflac__read_uint32(bs, 8, &header->sampleRate)) {
                return DRFLAC_FALSE;
            }
            crc8 = drflac_crc8(crc8, header->sampleRate, 8);
            header->sampleRate *= 1000;
        } else if (sampleRate == 13) {
            if (!drflac__read_uint32(bs, 16, &header->sampleRate)) {
                return DRFLAC_FALSE;
            }
            crc8 = drflac_crc8(crc8, header->sampleRate, 16);
        } else if (sampleRate == 14) {
            if (!drflac__read_uint32(bs, 16, &header->sampleRate)) {
                return DRFLAC_FALSE;
            }
            crc8 = drflac_crc8(crc8, header->sampleRate, 16);
            header->sampleRate *= 10;
        } else {
            continue;  
        }


        header->channelAssignment = channelAssignment;

        header->bitsPerSample = bitsPerSampleTable[bitsPerSample];
        if (header->bitsPerSample == 0) {
            header->bitsPerSample = streaminfoBitsPerSample;
        }

        
        if (streaminfoBitsPerSample != 0) {
            if (streaminfoBitsPerSample < 32) {
                if (header->bitsPerSample == 32) {
                    return DRFLAC_FALSE;
                }
            } else {
                if (header->bitsPerSample != 32) {
                    return DRFLAC_FALSE;
                }
            }
        }

        if (!drflac__read_uint8(bs, 8, &header->crc8)) {
            return DRFLAC_FALSE;
        }

#ifndef DR_FLAC_NO_CRC
        if (header->crc8 != crc8) {
            continue;    
        }
#endif
        return DRFLAC_TRUE;
    }
}

static drflac_bool32 drflac__read_subframe_header(drflac_bs* bs, drflac_subframe* pSubframe)
{
    drflac_uint8 header;
    int type;

    if (!drflac__read_uint8(bs, 8, &header)) {
        return DRFLAC_FALSE;
    }

    
    if ((header & 0x80) != 0) {
        return DRFLAC_FALSE;
    }

    



    pSubframe->lpcOrder = 0;

    type = (header & 0x7E) >> 1;
    if (type == 0) {
        pSubframe->subframeType = DRFLAC_SUBFRAME_CONSTANT;
    } else if (type == 1) {
        pSubframe->subframeType = DRFLAC_SUBFRAME_VERBATIM;
    } else {
        if ((type & 0x20) != 0) {
            pSubframe->subframeType = DRFLAC_SUBFRAME_LPC;
            pSubframe->lpcOrder = (drflac_uint8)(type & 0x1F) + 1;
        } else if ((type & 0x08) != 0) {
            pSubframe->subframeType = DRFLAC_SUBFRAME_FIXED;
            pSubframe->lpcOrder = (drflac_uint8)(type & 0x07);
            if (pSubframe->lpcOrder > 4) {
                pSubframe->subframeType = DRFLAC_SUBFRAME_RESERVED;
                pSubframe->lpcOrder = 0;
            }
        } else {
            pSubframe->subframeType = DRFLAC_SUBFRAME_RESERVED;
        }
    }

    if (pSubframe->subframeType == DRFLAC_SUBFRAME_RESERVED) {
        return DRFLAC_FALSE;
    }

    
    pSubframe->wastedBitsPerSample = 0;
    if ((header & 0x01) == 1) {
        unsigned int wastedBitsPerSample;
        if (!drflac__seek_past_next_set_bit(bs, &wastedBitsPerSample)) {
            return DRFLAC_FALSE;
        }
        pSubframe->wastedBitsPerSample = (drflac_uint8)wastedBitsPerSample + 1;
    }

    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_subframe(drflac_bs* bs, drflac_frame* frame, int subframeIndex, drflac_int32* pDecodedSamplesOutS32, drflac_int64* pDecodedSamplesOutS64)
{
    drflac_subframe* pSubframe;
    drflac_uint32 subframeBitsPerSample;
    drflac_bool32 decodeResult;

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(frame != NULL);
    DRFLAC_ASSERT(pDecodedSamplesOutS32 != NULL || pDecodedSamplesOutS64 != NULL);
    DRFLAC_ASSERT(pDecodedSamplesOutS32 == NULL || pDecodedSamplesOutS64 == NULL);

    pSubframe = frame->subframes + subframeIndex;
    if (!drflac__read_subframe_header(bs, pSubframe)) {
        return DRFLAC_FALSE;
    }

    
    subframeBitsPerSample = frame->header.bitsPerSample;
    if ((frame->header.channelAssignment == DRFLAC_CHANNEL_ASSIGNMENT_LEFT_SIDE || frame->header.channelAssignment == DRFLAC_CHANNEL_ASSIGNMENT_MID_SIDE) && subframeIndex == 1) {
        subframeBitsPerSample += 1;
    } else if (frame->header.channelAssignment == DRFLAC_CHANNEL_ASSIGNMENT_RIGHT_SIDE && subframeIndex == 0) {
        subframeBitsPerSample += 1;
    }

    
    if (pSubframe->wastedBitsPerSample >= subframeBitsPerSample) {
        return DRFLAC_FALSE;
    }
    subframeBitsPerSample -= pSubframe->wastedBitsPerSample;

    
    if (pDecodedSamplesOutS32 != NULL) {
        if (subframeBitsPerSample > 32) {
            return DRFLAC_FALSE;
        }
    } else {
        if (subframeBitsPerSample > 33) {
            return DRFLAC_FALSE;
        }
    }

    




    if (frame->header.blockSizeInPCMFrames < pSubframe->lpcOrder) {
        return DRFLAC_FALSE;
    }

    switch (pSubframe->subframeType)
    {
        case DRFLAC_SUBFRAME_CONSTANT:
        {
            decodeResult = drflac__decode_samples__constant(bs, frame->header.blockSizeInPCMFrames, subframeBitsPerSample, pDecodedSamplesOutS32, pDecodedSamplesOutS64);
        } break;

        case DRFLAC_SUBFRAME_VERBATIM:
        {
            decodeResult = drflac__decode_samples__verbatim(bs, frame->header.blockSizeInPCMFrames, subframeBitsPerSample, pDecodedSamplesOutS32, pDecodedSamplesOutS64);
        } break;

        case DRFLAC_SUBFRAME_FIXED:
        {
            decodeResult = drflac__decode_samples__fixed(bs, frame->header.blockSizeInPCMFrames, subframeBitsPerSample, pSubframe->lpcOrder, pDecodedSamplesOutS32, pDecodedSamplesOutS64);
        } break;

        case DRFLAC_SUBFRAME_LPC:
        {
            decodeResult = drflac__decode_samples__lpc(bs, frame->header.blockSizeInPCMFrames, subframeBitsPerSample, pSubframe->lpcOrder, pDecodedSamplesOutS32, pDecodedSamplesOutS64);
        } break;

        default: decodeResult = DRFLAC_FALSE;
    }

    if (!decodeResult) {
        return DRFLAC_FALSE;
    }

    if (pDecodedSamplesOutS32 != NULL) {
        pSubframe->samples.pS32 = pDecodedSamplesOutS32;
    } else {
        pSubframe->samples.pS64 = pDecodedSamplesOutS64;
    }

    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__seek_subframe(drflac_bs* bs, drflac_frame* frame, int subframeIndex)
{
    drflac_subframe* pSubframe;
    drflac_uint32 subframeBitsPerSample;

    DRFLAC_ASSERT(bs != NULL);
    DRFLAC_ASSERT(frame != NULL);

    pSubframe = frame->subframes + subframeIndex;
    if (!drflac__read_subframe_header(bs, pSubframe)) {
        return DRFLAC_FALSE;
    }

    
    subframeBitsPerSample = frame->header.bitsPerSample;
    if ((frame->header.channelAssignment == DRFLAC_CHANNEL_ASSIGNMENT_LEFT_SIDE || frame->header.channelAssignment == DRFLAC_CHANNEL_ASSIGNMENT_MID_SIDE) && subframeIndex == 1) {
        subframeBitsPerSample += 1;
    } else if (frame->header.channelAssignment == DRFLAC_CHANNEL_ASSIGNMENT_RIGHT_SIDE && subframeIndex == 0) {
        subframeBitsPerSample += 1;
    }

    
    if (pSubframe->wastedBitsPerSample >= subframeBitsPerSample) {
        return DRFLAC_FALSE;
    }
    subframeBitsPerSample -= pSubframe->wastedBitsPerSample;

    DRFLAC_ZERO_MEMORY(&pSubframe->samples, sizeof(pSubframe->samples));

    switch (pSubframe->subframeType)
    {
        case DRFLAC_SUBFRAME_CONSTANT:
        {
            if (!drflac__seek_bits(bs, subframeBitsPerSample)) {
                return DRFLAC_FALSE;
            }
        } break;

        case DRFLAC_SUBFRAME_VERBATIM:
        {
            unsigned int bitsToSeek = frame->header.blockSizeInPCMFrames * subframeBitsPerSample;
            if (!drflac__seek_bits(bs, bitsToSeek)) {
                return DRFLAC_FALSE;
            }
        } break;

        case DRFLAC_SUBFRAME_FIXED:
        {
            unsigned int bitsToSeek = pSubframe->lpcOrder * subframeBitsPerSample;
            if (!drflac__seek_bits(bs, bitsToSeek)) {
                return DRFLAC_FALSE;
            }

            if (!drflac__read_and_seek_residual(bs, frame->header.blockSizeInPCMFrames, pSubframe->lpcOrder)) {
                return DRFLAC_FALSE;
            }
        } break;

        case DRFLAC_SUBFRAME_LPC:
        {
            drflac_uint8 lpcPrecision;

            unsigned int bitsToSeek = pSubframe->lpcOrder * subframeBitsPerSample;
            if (!drflac__seek_bits(bs, bitsToSeek)) {
                return DRFLAC_FALSE;
            }

            if (!drflac__read_uint8(bs, 4, &lpcPrecision)) {
                return DRFLAC_FALSE;
            }
            if (lpcPrecision == 15) {
                return DRFLAC_FALSE;    
            }
            lpcPrecision += 1;


            bitsToSeek = (pSubframe->lpcOrder * lpcPrecision) + 5;    
            if (!drflac__seek_bits(bs, bitsToSeek)) {
                return DRFLAC_FALSE;
            }

            if (!drflac__read_and_seek_residual(bs, frame->header.blockSizeInPCMFrames, pSubframe->lpcOrder)) {
                return DRFLAC_FALSE;
            }
        } break;

        default: return DRFLAC_FALSE;
    }

    return DRFLAC_TRUE;
}


static DRFLAC_INLINE drflac_uint8 drflac__get_channel_count_from_channel_assignment(drflac_int8 channelAssignment)
{
    drflac_uint8 lookup[] = {1, 2, 3, 4, 5, 6, 7, 8, 2, 2, 2};

    DRFLAC_ASSERT(channelAssignment <= 10);
    return lookup[channelAssignment];
}

static drflac_bool32 drflac__resize_decoded_samples(drflac* pFlac, drflac_uint16 blockSizeInPCMFrames);

static drflac_result drflac__decode_flac_frame(drflac* pFlac)
{
    int channelCount;
    int i;
    drflac_uint8 paddingSizeInBits;
    drflac_uint16 desiredCRC16;
#ifndef DR_FLAC_NO_CRC
    drflac_uint16 actualCRC16;
#endif

    
    pFlac->currentFLACFrame.pcmFramesRemaining = 0;
    DRFLAC_ZERO_MEMORY(pFlac->currentFLACFrame.subframes, sizeof(pFlac->currentFLACFrame.subframes));

    
    channelCount = drflac__get_channel_count_from_channel_assignment(pFlac->currentFLACFrame.header.channelAssignment);
    if (channelCount != (int)pFlac->channels) {
        return DRFLAC_ERROR;
    }

    



    if (pFlac->currentFLACFrame.header.blockSizeInPCMFrames > pFlac->decodedSamplesCapacityInPCMFrames) {
        if (!drflac__resize_decoded_samples(pFlac, pFlac->currentFLACFrame.header.blockSizeInPCMFrames)) {
            return DRFLAC_OUT_OF_MEMORY;
        }
    }

    if (pFlac->bitsPerSample != 32) {
        for (i = 0; i < channelCount; ++i) {
            if (!drflac__decode_subframe(&pFlac->bs, &pFlac->currentFLACFrame, i, pFlac->decodedSamples.pS32 + (pFlac->currentFLACFrame.header.blockSizeInPCMFrames * i), NULL)) {
                return DRFLAC_ERROR;
            }
        }
    } else {
        for (i = 0; i < channelCount; ++i) {
            if (!drflac__decode_subframe(&pFlac->bs, &pFlac->currentFLACFrame, i, NULL, pFlac->decodedSamples.pS64 + (pFlac->currentFLACFrame.header.blockSizeInPCMFrames * i))) {
                return DRFLAC_ERROR;
            }
        }
    }

    paddingSizeInBits = (drflac_uint8)(DRFLAC_CACHE_L1_BITS_REMAINING(&pFlac->bs) & 7);
    if (paddingSizeInBits > 0) {
        drflac_uint8 padding = 0;
        if (!drflac__read_uint8(&pFlac->bs, paddingSizeInBits, &padding)) {
            return DRFLAC_AT_END;
        }
    }

#ifndef DR_FLAC_NO_CRC
    actualCRC16 = drflac__flush_crc16(&pFlac->bs);
#endif
    if (!drflac__read_uint16(&pFlac->bs, 16, &desiredCRC16)) {
        return DRFLAC_AT_END;
    }

#ifndef DR_FLAC_NO_CRC
    if (actualCRC16 != desiredCRC16) {
        return DRFLAC_CRC_MISMATCH;    
    }
#endif

    pFlac->currentFLACFrame.pcmFramesRemaining = pFlac->currentFLACFrame.header.blockSizeInPCMFrames;

    return DRFLAC_SUCCESS;
}

static drflac_result drflac__seek_flac_frame(drflac* pFlac)
{
    drflac_result result;
    int channelCount;
    int i;
    drflac_uint16 desiredCRC16;
#ifndef DR_FLAC_NO_CRC
    drflac_uint16 actualCRC16;
#endif

    pFlac->currentFLACFrame.pcmFramesRemaining = 0;

    channelCount = drflac__get_channel_count_from_channel_assignment(pFlac->currentFLACFrame.header.channelAssignment);
    for (i = 0; i < channelCount; ++i) {
        if (!drflac__seek_subframe(&pFlac->bs, &pFlac->currentFLACFrame, i)) {
            result = DRFLAC_ERROR;
            goto error;
        }
    }

    
    if (!drflac__seek_bits(&pFlac->bs, DRFLAC_CACHE_L1_BITS_REMAINING(&pFlac->bs) & 7)) {
        result = DRFLAC_ERROR;
        goto error;
    }

    
#ifndef DR_FLAC_NO_CRC
    actualCRC16 = drflac__flush_crc16(&pFlac->bs);
#endif
    if (!drflac__read_uint16(&pFlac->bs, 16, &desiredCRC16)) {
        result = DRFLAC_AT_END;
        goto error;
    }

#ifndef DR_FLAC_NO_CRC
    if (actualCRC16 != desiredCRC16) {
        result = DRFLAC_CRC_MISMATCH;   
        goto error;
    }
#endif

    return DRFLAC_SUCCESS;

error:
    DRFLAC_ZERO_MEMORY(pFlac->currentFLACFrame.subframes, sizeof(pFlac->currentFLACFrame.subframes));
    return result;
}

static drflac_bool32 drflac__read_and_decode_next_flac_frame(drflac* pFlac)
{
    DRFLAC_ASSERT(pFlac != NULL);

    for (;;) {
        drflac_result result;

        if (!drflac__read_next_flac_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFLACFrame.header)) {
            DRFLAC_ZERO_MEMORY(pFlac->currentFLACFrame.subframes, sizeof(pFlac->currentFLACFrame.subframes));
            return DRFLAC_FALSE;
        }

        result = drflac__decode_flac_frame(pFlac);
        if (result != DRFLAC_SUCCESS) {
            DRFLAC_ZERO_MEMORY(pFlac->currentFLACFrame.subframes, sizeof(pFlac->currentFLACFrame.subframes));
            if (result == DRFLAC_CRC_MISMATCH) {
                continue;   
            } else {
                return DRFLAC_FALSE;
            }
        }

        return DRFLAC_TRUE;
    }
}

static void drflac__get_pcm_frame_range_of_current_flac_frame(drflac* pFlac, drflac_uint64* pFirstPCMFrame, drflac_uint64* pLastPCMFrame)
{
    drflac_uint64 firstPCMFrame;
    drflac_uint64 lastPCMFrame;

    DRFLAC_ASSERT(pFlac != NULL);

    firstPCMFrame = pFlac->currentFLACFrame.header.pcmFrameNumber;
    if (firstPCMFrame == 0) {
        firstPCMFrame = ((drflac_uint64)pFlac->currentFLACFrame.header.flacFrameNumber) * pFlac->maxBlockSizeInPCMFrames;
    }

    lastPCMFrame = firstPCMFrame + pFlac->currentFLACFrame.header.blockSizeInPCMFrames;
    if (lastPCMFrame > 0) {
        lastPCMFrame -= 1; 
    }

    if (pFirstPCMFrame) {
        *pFirstPCMFrame = firstPCMFrame;
    }
    if (pLastPCMFrame) {
        *pLastPCMFrame = lastPCMFrame;
    }
}

static drflac_bool32 drflac__seek_to_first_frame(drflac* pFlac)
{
    drflac_bool32 result;

    DRFLAC_ASSERT(pFlac != NULL);

    result = drflac__seek_to_byte(&pFlac->bs, pFlac->firstFLACFramePosInBytes);

    DRFLAC_ZERO_MEMORY(&pFlac->currentFLACFrame, sizeof(pFlac->currentFLACFrame));
    pFlac->currentPCMFrame = 0;

    return result;
}

static DRFLAC_INLINE drflac_result drflac__seek_to_next_flac_frame(drflac* pFlac)
{
    
    DRFLAC_ASSERT(pFlac != NULL);
    return drflac__seek_flac_frame(pFlac);
}


static drflac_uint64 drflac__seek_forward_by_pcm_frames(drflac* pFlac, drflac_uint64 pcmFramesToSeek)
{
    drflac_uint64 pcmFramesRead = 0;
    while (pcmFramesToSeek > 0) {
        if (pFlac->currentFLACFrame.pcmFramesRemaining == 0) {
            if (!drflac__read_and_decode_next_flac_frame(pFlac)) {
                break;  
            }
        } else {
            if (pFlac->currentFLACFrame.pcmFramesRemaining > pcmFramesToSeek) {
                pcmFramesRead   += pcmFramesToSeek;
                pFlac->currentFLACFrame.pcmFramesRemaining -= (drflac_uint32)pcmFramesToSeek;   
                pcmFramesToSeek  = 0;
            } else {
                pcmFramesRead   += pFlac->currentFLACFrame.pcmFramesRemaining;
                pcmFramesToSeek -= pFlac->currentFLACFrame.pcmFramesRemaining;
                pFlac->currentFLACFrame.pcmFramesRemaining = 0;
            }
        }
    }

    pFlac->currentPCMFrame += pcmFramesRead;
    return pcmFramesRead;
}


static drflac_bool32 drflac__seek_to_pcm_frame__brute_force(drflac* pFlac, drflac_uint64 pcmFrameIndex)
{
    drflac_bool32 isMidFrame = DRFLAC_FALSE;
    drflac_uint64 runningPCMFrameCount;

    DRFLAC_ASSERT(pFlac != NULL);

    
    if (pcmFrameIndex >= pFlac->currentPCMFrame) {
        
        runningPCMFrameCount = pFlac->currentPCMFrame;

        
        if (pFlac->currentPCMFrame == 0 && pFlac->currentFLACFrame.pcmFramesRemaining == 0) {
            if (!drflac__read_next_flac_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFLACFrame.header)) {
                return DRFLAC_FALSE;
            }
        } else {
            isMidFrame = DRFLAC_TRUE;
        }
    } else {
        
        runningPCMFrameCount = 0;

        
        if (!drflac__seek_to_first_frame(pFlac)) {
            return DRFLAC_FALSE;
        }

        
        if (!drflac__read_next_flac_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFLACFrame.header)) {
            return DRFLAC_FALSE;
        }
    }

    



    for (;;) {
        drflac_uint64 pcmFrameCountInThisFLACFrame;
        drflac_uint64 firstPCMFrameInFLACFrame = 0;
        drflac_uint64 lastPCMFrameInFLACFrame = 0;

        drflac__get_pcm_frame_range_of_current_flac_frame(pFlac, &firstPCMFrameInFLACFrame, &lastPCMFrameInFLACFrame);

        pcmFrameCountInThisFLACFrame = (lastPCMFrameInFLACFrame - firstPCMFrameInFLACFrame) + 1;
        if (pcmFrameIndex < (runningPCMFrameCount + pcmFrameCountInThisFLACFrame)) {
            



            drflac_uint64 pcmFramesToDecode = pcmFrameIndex - runningPCMFrameCount;

            if (!isMidFrame) {
                drflac_result result = drflac__decode_flac_frame(pFlac);
                if (result == DRFLAC_SUCCESS) {
                    
                    return drflac__seek_forward_by_pcm_frames(pFlac, pcmFramesToDecode) == pcmFramesToDecode;  
                } else {
                    if (result == DRFLAC_CRC_MISMATCH) {
                        goto next_iteration;   
                    } else {
                        return DRFLAC_FALSE;
                    }
                }
            } else {
                
                return drflac__seek_forward_by_pcm_frames(pFlac, pcmFramesToDecode) == pcmFramesToDecode;
            }
        } else {
            



            if (!isMidFrame) {
                drflac_result result = drflac__seek_to_next_flac_frame(pFlac);
                if (result == DRFLAC_SUCCESS) {
                    runningPCMFrameCount += pcmFrameCountInThisFLACFrame;
                } else {
                    if (result == DRFLAC_CRC_MISMATCH) {
                        goto next_iteration;   
                    } else {
                        return DRFLAC_FALSE;
                    }
                }
            } else {
                



                runningPCMFrameCount += pFlac->currentFLACFrame.pcmFramesRemaining;
                pFlac->currentFLACFrame.pcmFramesRemaining = 0;
                isMidFrame = DRFLAC_FALSE;
            }

            
            if (runningPCMFrameCount == pcmFrameIndex) {
                return DRFLAC_TRUE;
            }
        }

    next_iteration:
        
        if (!drflac__read_next_flac_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFLACFrame.header)) {
            return DRFLAC_FALSE;
        }
    }
}


#if !defined(DR_FLAC_NO_CRC)





#define DRFLAC_BINARY_SEARCH_APPROX_COMPRESSION_RATIO 0.6f

static drflac_bool32 drflac__read_and_decode_next_flac_frame_for_seek(drflac* pFlac)
{
    DRFLAC_ASSERT(pFlac != NULL);

    for (;;) {
        drflac_result result;

        if (!drflac__read_next_flac_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFLACFrame.header)) {
            return DRFLAC_FALSE;
        }

        result = drflac__decode_flac_frame(pFlac);
        if (result == DRFLAC_SUCCESS) {
            return DRFLAC_TRUE;
        }

        if (result == DRFLAC_AT_END) {
            return DRFLAC_FALSE;
        }
    }
}

static drflac_bool32 drflac__seek_to_approximate_flac_frame_to_byte(drflac* pFlac, drflac_uint64 targetByte, drflac_uint64 rangeLo, drflac_uint64 rangeHi, drflac_uint64* pLastSuccessfulSeekOffset)
{
    DRFLAC_ASSERT(pFlac != NULL);
    DRFLAC_ASSERT(pLastSuccessfulSeekOffset != NULL);
    DRFLAC_ASSERT(targetByte >= rangeLo);
    DRFLAC_ASSERT(targetByte <= rangeHi);

    *pLastSuccessfulSeekOffset = pFlac->firstFLACFramePosInBytes;

    for (;;) {
        
        if (!drflac__seek_to_byte(&pFlac->bs, targetByte)) {
            
            if (targetByte == 0) {
                drflac__seek_to_first_frame(pFlac); 
                return DRFLAC_FALSE;
            }

            rangeHi = targetByte;
            if (rangeHi <= rangeLo) {
                return DRFLAC_FALSE;
            }

            
            targetByte = rangeLo + ((rangeHi - rangeLo)/2);
        } else {
            

            
            DRFLAC_ZERO_MEMORY(&pFlac->currentFLACFrame, sizeof(pFlac->currentFLACFrame));

            




            #if 1
            {
                if (!drflac__read_and_decode_next_flac_frame_for_seek(pFlac)) {
                    rangeHi = targetByte;
                    if (rangeHi <= rangeLo) {
                        return DRFLAC_FALSE;
                    }

                    
                    targetByte = rangeLo + ((rangeHi - rangeLo)/2);
                } else {
                    break;
                }
            }
            #else
            {
                if (!drflac__read_next_flac_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFLACFrame.header)) {
                    
                    targetByte = rangeLo + ((rangeHi - rangeLo)/2);
                    rangeHi = targetByte;
                } else {
                    break;
                }
            }
            #endif
        }
    }

    
    drflac__get_pcm_frame_range_of_current_flac_frame(pFlac, &pFlac->currentPCMFrame, NULL);

    DRFLAC_ASSERT(targetByte <= rangeHi);

    *pLastSuccessfulSeekOffset = targetByte;
    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_flac_frame_and_seek_forward_by_pcm_frames(drflac* pFlac, drflac_uint64 offset)
{
    
#if 0
    if (drflac__decode_flac_frame(pFlac) != DRFLAC_SUCCESS) {
        
        if (drflac__read_and_decode_next_flac_frame(pFlac) == DRFLAC_FALSE) {
            return DRFLAC_FALSE;
        }
    }
#endif

    return drflac__seek_forward_by_pcm_frames(pFlac, offset) == offset;
}


static drflac_bool32 drflac__seek_to_pcm_frame__binary_search_internal(drflac* pFlac, drflac_uint64 pcmFrameIndex, drflac_uint64 byteRangeLo, drflac_uint64 byteRangeHi)
{
    

    drflac_uint64 targetByte;
    drflac_uint64 pcmRangeLo = pFlac->totalPCMFrameCount;
    drflac_uint64 pcmRangeHi = 0;
    drflac_uint64 lastSuccessfulSeekOffset = (drflac_uint64)-1;
    drflac_uint64 closestSeekOffsetBeforeTargetPCMFrame = byteRangeLo;
    drflac_uint32 seekForwardThreshold = (pFlac->maxBlockSizeInPCMFrames != 0) ? pFlac->maxBlockSizeInPCMFrames*2 : 4096;

    if (byteRangeLo > byteRangeHi || pFlac->currentPCMFrame > pcmFrameIndex) {
        return DRFLAC_FALSE;
    }

    
    targetByte = (drflac_uint64)(((drflac_int64)((pcmFrameIndex - pFlac->currentPCMFrame) * pFlac->channels * pFlac->bitsPerSample)/8.0f) * DRFLAC_BINARY_SEARCH_APPROX_COMPRESSION_RATIO);
    if (targetByte > byteRangeHi - byteRangeLo) {
        targetByte = byteRangeHi;
    } else {
        targetByte += byteRangeLo;
    }

    for (;;) {
        drflac_uint64 previousByteRangeLo;
        drflac_uint64 previousByteRangeHi;

        previousByteRangeLo = byteRangeLo;
        previousByteRangeHi = byteRangeHi;

        



        if ((byteRangeHi - byteRangeLo) <= 1) {
            if (!drflac__seek_to_approximate_flac_frame_to_byte(pFlac, closestSeekOffsetBeforeTargetPCMFrame, closestSeekOffsetBeforeTargetPCMFrame, byteRangeHi, &lastSuccessfulSeekOffset)) {
                break;
            }

            if (pFlac->currentPCMFrame <= pcmFrameIndex && drflac__decode_flac_frame_and_seek_forward_by_pcm_frames(pFlac, pcmFrameIndex - pFlac->currentPCMFrame)) {
                return DRFLAC_TRUE;
            }

            break;
        }

        if (drflac__seek_to_approximate_flac_frame_to_byte(pFlac, targetByte, byteRangeLo, byteRangeHi, &lastSuccessfulSeekOffset)) {
            
            drflac_uint64 newPCMRangeLo;
            drflac_uint64 newPCMRangeHi;
            drflac__get_pcm_frame_range_of_current_flac_frame(pFlac, &newPCMRangeLo, &newPCMRangeHi);

            
            if (pcmRangeLo == newPCMRangeLo) {
                if (!drflac__seek_to_approximate_flac_frame_to_byte(pFlac, closestSeekOffsetBeforeTargetPCMFrame, closestSeekOffsetBeforeTargetPCMFrame, byteRangeHi, &lastSuccessfulSeekOffset)) {
                    break;  
                }

                if (drflac__decode_flac_frame_and_seek_forward_by_pcm_frames(pFlac, pcmFrameIndex - pFlac->currentPCMFrame)) {
                    return DRFLAC_TRUE;
                } else {
                    break;  
                }
            }

            pcmRangeLo = newPCMRangeLo;
            pcmRangeHi = newPCMRangeHi;

            if (pcmRangeLo <= pcmFrameIndex && pcmRangeHi >= pcmFrameIndex) {
                
                if (drflac__decode_flac_frame_and_seek_forward_by_pcm_frames(pFlac, pcmFrameIndex - pFlac->currentPCMFrame) ) {
                    return DRFLAC_TRUE;
                } else {
                    break;  
                }
            } else {
                if (pcmRangeLo > pcmFrameIndex) {
                    
                    byteRangeHi = lastSuccessfulSeekOffset;
                    if (byteRangeLo > byteRangeHi) {
                        byteRangeLo = byteRangeHi;
                    }

                    targetByte = byteRangeLo + ((byteRangeHi - byteRangeLo) / 2);
                    if (targetByte < byteRangeLo) {
                        targetByte = byteRangeLo;
                    }
                } else  {
                    

                    
                    if ((pcmFrameIndex - pcmRangeLo) < seekForwardThreshold) {
                        if (drflac__decode_flac_frame_and_seek_forward_by_pcm_frames(pFlac, pcmFrameIndex - pFlac->currentPCMFrame)) {
                            return DRFLAC_TRUE;
                        } else {
                            break;  
                        }
                    } else {
                        byteRangeLo = lastSuccessfulSeekOffset;
                        if (byteRangeHi < byteRangeLo) {
                            byteRangeHi = byteRangeLo;
                        }

                        if (pcmRangeLo > 0) {
                            const double approxCompressionRatio = (drflac_int64)(lastSuccessfulSeekOffset - pFlac->firstFLACFramePosInBytes) / ((drflac_int64)(pcmRangeLo * pFlac->channels * pFlac->bitsPerSample)/8.0);

                            targetByte = lastSuccessfulSeekOffset + (drflac_uint64)(((drflac_int64)((pcmFrameIndex-pcmRangeLo) * pFlac->channels * pFlac->bitsPerSample)/8.0) * approxCompressionRatio);
                            if (targetByte > byteRangeHi) {
                                targetByte = byteRangeHi;
                            }
                        } else {
                            
                            targetByte = byteRangeLo + ((byteRangeHi - byteRangeLo) / 2);
                        }

                        if (closestSeekOffsetBeforeTargetPCMFrame < lastSuccessfulSeekOffset) {
                            closestSeekOffsetBeforeTargetPCMFrame = lastSuccessfulSeekOffset;
                        }
                    }
                }
            }

            if (byteRangeLo == previousByteRangeLo && byteRangeHi == previousByteRangeHi) {
                if (!drflac__seek_to_approximate_flac_frame_to_byte(pFlac, closestSeekOffsetBeforeTargetPCMFrame, closestSeekOffsetBeforeTargetPCMFrame, byteRangeHi, &lastSuccessfulSeekOffset)) {
                    break;
                }

                if (pFlac->currentPCMFrame <= pcmFrameIndex && drflac__decode_flac_frame_and_seek_forward_by_pcm_frames(pFlac, pcmFrameIndex - pFlac->currentPCMFrame)) {
                    return DRFLAC_TRUE;
                }

                break;
            }

            if ((byteRangeHi - byteRangeLo) > 1 && (targetByte <= byteRangeLo || targetByte >= byteRangeHi)) {
                targetByte = byteRangeLo + ((byteRangeHi - byteRangeLo) / 2);
            }
        } else {
            
            break;
        }
    }

    drflac__seek_to_first_frame(pFlac); 
    return DRFLAC_FALSE;
}

static drflac_bool32 drflac__seek_to_pcm_frame__binary_search(drflac* pFlac, drflac_uint64 pcmFrameIndex)
{
    drflac_uint64 byteRangeLo;
    drflac_uint64 byteRangeHi;
    drflac_uint32 seekForwardThreshold = (pFlac->maxBlockSizeInPCMFrames != 0) ? pFlac->maxBlockSizeInPCMFrames*2 : 4096;

    
    if (drflac__seek_to_first_frame(pFlac) == DRFLAC_FALSE) {
        return DRFLAC_FALSE;
    }

    
    if (pcmFrameIndex < seekForwardThreshold) {
        return drflac__seek_forward_by_pcm_frames(pFlac, pcmFrameIndex) == pcmFrameIndex;
    }

    



    byteRangeLo = pFlac->firstFLACFramePosInBytes;
    byteRangeHi = pFlac->firstFLACFramePosInBytes + (drflac_uint64)((drflac_int64)(pFlac->totalPCMFrameCount * pFlac->channels * pFlac->bitsPerSample)/8.0f);

    return drflac__seek_to_pcm_frame__binary_search_internal(pFlac, pcmFrameIndex, byteRangeLo, byteRangeHi);
}
#endif  

static drflac_bool32 drflac__seek_to_pcm_frame__seek_table(drflac* pFlac, drflac_uint64 pcmFrameIndex)
{
    drflac_uint32 iClosestSeekpoint = 0;
    drflac_bool32 isMidFrame = DRFLAC_FALSE;
    drflac_uint64 runningPCMFrameCount;
    drflac_uint32 iSeekpoint;


    DRFLAC_ASSERT(pFlac != NULL);

    if (pFlac->pSeekpoints == NULL || pFlac->seekpointCount == 0) {
        return DRFLAC_FALSE;
    }

    
    if (pFlac->pSeekpoints[0].firstPCMFrame > pcmFrameIndex) {
        return DRFLAC_FALSE;
    }

    for (iSeekpoint = 0; iSeekpoint < pFlac->seekpointCount; ++iSeekpoint) {
        if (pFlac->pSeekpoints[iSeekpoint].firstPCMFrame >= pcmFrameIndex) {
            break;
        }

        iClosestSeekpoint = iSeekpoint;
    }

    
    if (pFlac->pSeekpoints[iClosestSeekpoint].pcmFrameCount == 0 || pFlac->pSeekpoints[iClosestSeekpoint].pcmFrameCount > pFlac->maxBlockSizeInPCMFrames) {
        return DRFLAC_FALSE;
    }
    if (pFlac->pSeekpoints[iClosestSeekpoint].firstPCMFrame > pFlac->totalPCMFrameCount && pFlac->totalPCMFrameCount > 0) {
        return DRFLAC_FALSE;
    }

    if (pFlac->pSeekpoints[iClosestSeekpoint].flacFrameOffset > (drflac_uint64)-1 - pFlac->firstFLACFramePosInBytes) {
        return DRFLAC_FALSE;
    }

#if !defined(DR_FLAC_NO_CRC)
    
    if (pFlac->totalPCMFrameCount > 0) {
        drflac_uint64 byteRangeLo;
        drflac_uint64 byteRangeHi;

        byteRangeHi = pFlac->firstFLACFramePosInBytes + (drflac_uint64)((drflac_int64)(pFlac->totalPCMFrameCount * pFlac->channels * pFlac->bitsPerSample)/8.0f);
        byteRangeLo = pFlac->firstFLACFramePosInBytes + pFlac->pSeekpoints[iClosestSeekpoint].flacFrameOffset;

        






        if (iClosestSeekpoint < pFlac->seekpointCount-1) {
            drflac_uint32 iNextSeekpoint = iClosestSeekpoint + 1;

            if (pFlac->pSeekpoints[iNextSeekpoint].firstPCMFrame != (((drflac_uint64)0xFFFFFFFF << 32) | 0xFFFFFFFF)) { 
                
                if (pFlac->pSeekpoints[iClosestSeekpoint].flacFrameOffset >= pFlac->pSeekpoints[iNextSeekpoint].flacFrameOffset || pFlac->pSeekpoints[iNextSeekpoint].pcmFrameCount == 0) {
                    return DRFLAC_FALSE;    
                }

                if (pFlac->pSeekpoints[iNextSeekpoint].flacFrameOffset > (drflac_uint64)-1 - pFlac->firstFLACFramePosInBytes) {
                    return DRFLAC_FALSE;
                }

                byteRangeHi = pFlac->firstFLACFramePosInBytes + pFlac->pSeekpoints[iNextSeekpoint].flacFrameOffset - 1; 
            }
        }

        if (byteRangeLo > byteRangeHi) {
            return DRFLAC_FALSE;
        }

        if (drflac__seek_to_byte(&pFlac->bs, pFlac->firstFLACFramePosInBytes + pFlac->pSeekpoints[iClosestSeekpoint].flacFrameOffset)) {
            
            DRFLAC_ZERO_MEMORY(&pFlac->currentFLACFrame, sizeof(pFlac->currentFLACFrame));

            if (drflac__read_next_flac_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFLACFrame.header)) {
                drflac__get_pcm_frame_range_of_current_flac_frame(pFlac, &pFlac->currentPCMFrame, NULL);

                if (pFlac->currentPCMFrame > pcmFrameIndex) {
                    return DRFLAC_FALSE;    
                }

                if (drflac__seek_to_pcm_frame__binary_search_internal(pFlac, pcmFrameIndex, byteRangeLo, byteRangeHi)) {
                    return DRFLAC_TRUE;
                }
            }
        }
    }
#endif  

    

    



    if (pcmFrameIndex >= pFlac->currentPCMFrame && pFlac->pSeekpoints[iClosestSeekpoint].firstPCMFrame <= pFlac->currentPCMFrame) {
        
        runningPCMFrameCount = pFlac->currentPCMFrame;

        
        if (pFlac->currentPCMFrame == 0 && pFlac->currentFLACFrame.pcmFramesRemaining == 0) {
            if (!drflac__read_next_flac_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFLACFrame.header)) {
                return DRFLAC_FALSE;
            }
        } else {
            isMidFrame = DRFLAC_TRUE;
        }
    } else {
        
        runningPCMFrameCount = pFlac->pSeekpoints[iClosestSeekpoint].firstPCMFrame;

        if (!drflac__seek_to_byte(&pFlac->bs, pFlac->firstFLACFramePosInBytes + pFlac->pSeekpoints[iClosestSeekpoint].flacFrameOffset)) {
            return DRFLAC_FALSE;
        }

        
        if (!drflac__read_next_flac_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFLACFrame.header)) {
            return DRFLAC_FALSE;
        }
    }

    for (;;) {
        drflac_uint64 pcmFrameCountInThisFLACFrame;
        drflac_uint64 firstPCMFrameInFLACFrame = 0;
        drflac_uint64 lastPCMFrameInFLACFrame = 0;

        drflac__get_pcm_frame_range_of_current_flac_frame(pFlac, &firstPCMFrameInFLACFrame, &lastPCMFrameInFLACFrame);

        pcmFrameCountInThisFLACFrame = (lastPCMFrameInFLACFrame - firstPCMFrameInFLACFrame) + 1;
        if (pcmFrameIndex < (runningPCMFrameCount + pcmFrameCountInThisFLACFrame)) {
            



            drflac_uint64 pcmFramesToDecode = pcmFrameIndex - runningPCMFrameCount;

            if (!isMidFrame) {
                drflac_result result = drflac__decode_flac_frame(pFlac);
                if (result == DRFLAC_SUCCESS) {
                    
                    return drflac__seek_forward_by_pcm_frames(pFlac, pcmFramesToDecode) == pcmFramesToDecode;  
                } else {
                    if (result == DRFLAC_CRC_MISMATCH) {
                        goto next_iteration;   
                    } else {
                        return DRFLAC_FALSE;
                    }
                }
            } else {
                
                return drflac__seek_forward_by_pcm_frames(pFlac, pcmFramesToDecode) == pcmFramesToDecode;
            }
        } else {
            



            if (!isMidFrame) {
                drflac_result result = drflac__seek_to_next_flac_frame(pFlac);
                if (result == DRFLAC_SUCCESS) {
                    runningPCMFrameCount += pcmFrameCountInThisFLACFrame;
                } else {
                    if (result == DRFLAC_CRC_MISMATCH) {
                        goto next_iteration;   
                    } else {
                        return DRFLAC_FALSE;
                    }
                }
            } else {
                



                runningPCMFrameCount += pFlac->currentFLACFrame.pcmFramesRemaining;
                pFlac->currentFLACFrame.pcmFramesRemaining = 0;
                isMidFrame = DRFLAC_FALSE;
            }

            
            if (runningPCMFrameCount == pcmFrameIndex) {
                return DRFLAC_TRUE;
            }
        }

    next_iteration:
        
        if (!drflac__read_next_flac_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFLACFrame.header)) {
            return DRFLAC_FALSE;
        }
    }
}


#ifndef DR_FLAC_NO_OGG
typedef struct
{
    drflac_uint8 capturePattern[4];  
    drflac_uint8 structureVersion;   
    drflac_uint8 headerType;
    drflac_uint64 granulePosition;
    drflac_uint32 serialNumber;
    drflac_uint32 sequenceNumber;
    drflac_uint32 checksum;
    drflac_uint8 segmentCount;
    drflac_uint8 segmentTable[255];
} drflac_ogg_page_header;
#endif

typedef struct
{
    drflac_read_proc onRead;
    drflac_seek_proc onSeek;
    drflac_tell_proc onTell;
    drflac_meta_proc onMeta;
    drflac_container container;
    void* pUserData;
    void* pUserDataMD;
    drflac_uint32 sampleRate;
    drflac_uint8  channels;
    drflac_uint8  bitsPerSample;
    drflac_uint64 totalPCMFrameCount;
    drflac_uint16 maxBlockSizeInPCMFrames;
    drflac_uint64 runningFilePos;
    drflac_uint64 firstFrameSearchPos;
    drflac_bool32 hasStreamInfoBlock;
    drflac_bool32 hasMetadataBlocks;
    drflac_bs bs;                           
    drflac_frame_header firstFrameHeader;   

#ifndef DR_FLAC_NO_OGG
    drflac_uint32 oggSerial;
    drflac_uint64 oggFirstBytePos;
    drflac_ogg_page_header oggBosHeader;
#endif
} drflac_init_info;

static DRFLAC_INLINE void drflac__decode_block_header(drflac_uint32 blockHeader, drflac_uint8* isLastBlock, drflac_uint8* blockType, drflac_uint32* blockSize)
{
    blockHeader = drflac__be2host_32(blockHeader);
    *isLastBlock = (drflac_uint8)((blockHeader & 0x80000000UL) >> 31);
    *blockType   = (drflac_uint8)((blockHeader & 0x7F000000UL) >> 24);
    *blockSize   =                (blockHeader & 0x00FFFFFFUL);
}

static DRFLAC_INLINE drflac_bool32 drflac__read_and_decode_block_header(drflac_read_proc onRead, void* pUserData, drflac_uint8* isLastBlock, drflac_uint8* blockType, drflac_uint32* blockSize)
{
    drflac_uint32 blockHeader;

    *blockSize = 0;
    if (onRead(pUserData, &blockHeader, 4) != 4) {
        return DRFLAC_FALSE;
    }

    drflac__decode_block_header(blockHeader, isLastBlock, blockType, blockSize);
    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__read_streaminfo(drflac_read_proc onRead, void* pUserData, drflac_streaminfo* pStreamInfo)
{
    drflac_uint32 blockSizes;
    drflac_uint64 frameSizes = 0;
    drflac_uint64 importantProps;
    drflac_uint8 md5[16];

    
    if (onRead(pUserData, &blockSizes, 4) != 4) {
        return DRFLAC_FALSE;
    }

    
    if (onRead(pUserData, &frameSizes, 6) != 6) {
        return DRFLAC_FALSE;
    }

    
    if (onRead(pUserData, &importantProps, 8) != 8) {
        return DRFLAC_FALSE;
    }

    
    if (onRead(pUserData, md5, sizeof(md5)) != sizeof(md5)) {
        return DRFLAC_FALSE;
    }

    blockSizes     = drflac__be2host_32(blockSizes);
    frameSizes     = drflac__be2host_64(frameSizes);
    importantProps = drflac__be2host_64(importantProps);

    pStreamInfo->minBlockSizeInPCMFrames = (drflac_uint16)((blockSizes & 0xFFFF0000) >> 16);
    pStreamInfo->maxBlockSizeInPCMFrames = (drflac_uint16) (blockSizes & 0x0000FFFF);
    pStreamInfo->minFrameSizeInPCMFrames = (drflac_uint32)((frameSizes     &  (((drflac_uint64)0x00FFFFFF << 16) << 24)) >> 40);
    pStreamInfo->maxFrameSizeInPCMFrames = (drflac_uint32)((frameSizes     &  (((drflac_uint64)0x00FFFFFF << 16) <<  0)) >> 16);
    pStreamInfo->sampleRate              = (drflac_uint32)((importantProps &  (((drflac_uint64)0x000FFFFF << 16) << 28)) >> 44);
    pStreamInfo->channels                = (drflac_uint8 )((importantProps &  (((drflac_uint64)0x0000000E << 16) << 24)) >> 41) + 1;
    pStreamInfo->bitsPerSample           = (drflac_uint8 )((importantProps &  (((drflac_uint64)0x0000001F << 16) << 20)) >> 36) + 1;
    pStreamInfo->totalPCMFrameCount      =                ((importantProps & ((((drflac_uint64)0x0000000F << 16) << 16) | 0xFFFFFFFF)));
    DRFLAC_COPY_MEMORY(pStreamInfo->md5, md5, sizeof(md5));

    return DRFLAC_TRUE;
}


static void* drflac__malloc_default(size_t sz, void* pUserData)
{
    (void)pUserData;
    return DRFLAC_MALLOC(sz);
}

static void* drflac__realloc_default(void* p, size_t sz, void* pUserData)
{
    (void)pUserData;
    return DRFLAC_REALLOC(p, sz);
}

static void drflac__free_default(void* p, void* pUserData)
{
    (void)pUserData;
    DRFLAC_FREE(p);
}


static void* drflac__malloc_from_callbacks(size_t sz, const drflac_allocation_callbacks* pAllocationCallbacks)
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

static void* drflac__realloc_from_callbacks(void* p, size_t szNew, size_t szOld, const drflac_allocation_callbacks* pAllocationCallbacks)
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
            DRFLAC_COPY_MEMORY(p2, p, DRFLAC_MIN(szNew, szOld));
            pAllocationCallbacks->onFree(p, pAllocationCallbacks->pUserData);
        }

        return p2;
    }

    return NULL;
}

static void drflac__free_from_callbacks(void* p, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    if (p == NULL || pAllocationCallbacks == NULL) {
        return;
    }

    if (pAllocationCallbacks->onFree != NULL) {
        pAllocationCallbacks->onFree(p, pAllocationCallbacks->pUserData);
    }
}


static drflac_bool32 drflac__resize_decoded_samples(drflac* pFlac, drflac_uint16 blockSizeInPCMFrames)
{
    size_t sampleSize;
    size_t allocationSize;
    void* pAllocation;
    void* pDecodedSamples;

    sampleSize = sizeof(drflac_int32);
    if (pFlac->bitsPerSample == 32) {
        sampleSize = sizeof(drflac_int64);
    }

    allocationSize = drflac_align(blockSizeInPCMFrames * sampleSize, DRFLAC_MAX_SIMD_VECTOR_SIZE) * pFlac->channels + DRFLAC_MAX_SIMD_VECTOR_SIZE;
    
    pAllocation = drflac__malloc_from_callbacks(allocationSize, &pFlac->allocationCallbacks);
    if (pAllocation == NULL) {
        return DRFLAC_FALSE;
    }

    
    drflac__free_from_callbacks(pFlac->pDecodedSamplesAllocation, &pFlac->allocationCallbacks);
    pFlac->pDecodedSamplesAllocation = pAllocation;
    pFlac->decodedSamplesCapacityInPCMFrames = blockSizeInPCMFrames;

    pDecodedSamples = (void*)drflac_align((size_t)pAllocation, DRFLAC_MAX_SIMD_VECTOR_SIZE);
    if (pFlac->bitsPerSample != 32) {
        pFlac->decodedSamples.pS32 = (drflac_int32*)pDecodedSamples;
    } else {
        pFlac->decodedSamples.pS64 = (drflac_int64*)pDecodedSamples;
    }

    return DRFLAC_TRUE;
}


static drflac_result drflac__read_and_decode_metadata(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, drflac_meta_proc onMeta, void* pUserData, void* pUserDataMD, drflac_uint64* pFirstFramePos, drflac_uint64* pSeektablePos, drflac_uint32* pSeekpointCount, drflac_allocation_callbacks* pAllocationCallbacks)
{
    



    drflac_uint64 runningFilePos   = 42;
    drflac_uint64 seektablePos     = 0;
    drflac_uint32 seektableSize    = 0;
    drflac_int64  fileSize         = 0;
    drflac_bool32 hasKnownFileSize = DRFLAC_FALSE;

    
    if (onTell != NULL && onSeek != NULL) {
        if (onSeek(pUserData, 0, DRFLAC_SEEK_END)) {
            if (onTell(pUserData, &fileSize)) {
                hasKnownFileSize = DRFLAC_TRUE;
            }

            if (!onSeek(pUserData, (int)runningFilePos, DRFLAC_SEEK_SET)) {    
                return DRFLAC_BAD_SEEK;
            }
        }
    }

    for (;;) {
        drflac_metadata metadata;
        drflac_uint8 isLastBlock = 0;
        drflac_uint8 blockType = 0;
        drflac_uint32 blockSize;
        if (drflac__read_and_decode_block_header(onRead, pUserData, &isLastBlock, &blockType, &blockSize) == DRFLAC_FALSE) {
            return DRFLAC_ERROR;
        }

        runningFilePos += 4;

        if (hasKnownFileSize && (runningFilePos > (drflac_uint64)fileSize || blockSize > (drflac_uint64)fileSize - runningFilePos)) {
            return DRFLAC_INVALID_DATA;    
        }

        metadata.type = blockType;
        metadata.rawDataSize = 0;
        metadata.rawDataOffset = runningFilePos;
        metadata.pRawData = NULL;

        switch (blockType)
        {
            case DRFLAC_METADATA_BLOCK_TYPE_APPLICATION:
            {
                if (blockSize < 4) {
                    return DRFLAC_INVALID_DATA;
                }

                if (onMeta) {
                    void* pRawData = drflac__malloc_from_callbacks(blockSize, pAllocationCallbacks);
                    if (pRawData == NULL) {
                        return DRFLAC_ERROR;
                    }

                    if (onRead(pUserData, pRawData, blockSize) != blockSize) {
                        drflac__free_from_callbacks(pRawData, pAllocationCallbacks);
                        return DRFLAC_ERROR;
                    }

                    metadata.pRawData = pRawData;
                    metadata.rawDataSize = blockSize;
                    metadata.data.application.id       = drflac__be2host_32(*(drflac_uint32*)pRawData);
                    metadata.data.application.pData    = (const void*)((drflac_uint8*)pRawData + sizeof(drflac_uint32));
                    metadata.data.application.dataSize = blockSize - sizeof(drflac_uint32);
                    onMeta(pUserDataMD, &metadata);

                    drflac__free_from_callbacks(pRawData, pAllocationCallbacks);
                }
            } break;

            case DRFLAC_METADATA_BLOCK_TYPE_SEEKTABLE:
            {
                if ((blockSize % DRFLAC_SEEKPOINT_SIZE_IN_BYTES) != 0) {
                    return DRFLAC_INVALID_DATA;
                }

                seektablePos  = runningFilePos;
                seektableSize = blockSize;

                if (onMeta) {
                    drflac_uint32 seekpointCount;
                    drflac_uint32 iSeekpoint;
                    void* pRawData;
                    size_t rawDataSize;

                    seekpointCount = blockSize/DRFLAC_SEEKPOINT_SIZE_IN_BYTES;
                    rawDataSize = seekpointCount * sizeof(drflac_seekpoint);

                    pRawData = drflac__malloc_from_callbacks(rawDataSize, pAllocationCallbacks);
                    if (pRawData == NULL) {
                        return DRFLAC_ERROR;
                    }

                    
                    for (iSeekpoint = 0; iSeekpoint < seekpointCount; ++iSeekpoint) {
                        drflac_seekpoint* pSeekpoint = (drflac_seekpoint*)pRawData + iSeekpoint;

                        if (onRead(pUserData, pSeekpoint, DRFLAC_SEEKPOINT_SIZE_IN_BYTES) != DRFLAC_SEEKPOINT_SIZE_IN_BYTES) {
                            drflac__free_from_callbacks(pRawData, pAllocationCallbacks);
                            return DRFLAC_ERROR;
                        }

                        
                        pSeekpoint->firstPCMFrame   = drflac__be2host_64(pSeekpoint->firstPCMFrame);
                        pSeekpoint->flacFrameOffset = drflac__be2host_64(pSeekpoint->flacFrameOffset);
                        pSeekpoint->pcmFrameCount   = drflac__be2host_16(pSeekpoint->pcmFrameCount);
                    }

                    metadata.pRawData = pRawData;
                    metadata.rawDataSize = (drflac_uint32)rawDataSize;
                    metadata.data.seektable.seekpointCount = seekpointCount;
                    metadata.data.seektable.pSeekpoints = (const drflac_seekpoint*)pRawData;

                    onMeta(pUserDataMD, &metadata);

                    drflac__free_from_callbacks(pRawData, pAllocationCallbacks);
                }
            } break;

            case DRFLAC_METADATA_BLOCK_TYPE_VORBIS_COMMENT:
            {
                if (blockSize < 8) {
                    
                    if (onMeta != NULL && blockSize > 0) {
                        drflac_uint8 unused[8];
                        if (onRead(pUserData, unused, blockSize) != blockSize) {
                            return DRFLAC_ERROR;
                        }
                    }

                    break;
                }

                if (onMeta) {
                    void* pRawData;
                    const char* pRunningData;
                    const char* pRunningDataEnd;
                    drflac_uint32 i;

                    pRawData = drflac__malloc_from_callbacks(blockSize, pAllocationCallbacks);
                    if (pRawData == NULL) {
                        return DRFLAC_ERROR;
                    }

                    if (onRead(pUserData, pRawData, blockSize) != blockSize) {
                        drflac__free_from_callbacks(pRawData, pAllocationCallbacks);
                        return DRFLAC_ERROR;
                    }

                    metadata.pRawData = pRawData;
                    metadata.rawDataSize = blockSize;

                    pRunningData    = (const char*)pRawData;
                    pRunningDataEnd = (const char*)pRawData + blockSize;

                    metadata.data.vorbis_comment.vendorLength = drflac__le2host_32_ptr_unaligned(pRunningData); pRunningData += 4;

                    
                    if ((pRunningDataEnd - pRunningData) - 4 < (drflac_int64)metadata.data.vorbis_comment.vendorLength) { 
                        goto vorbis_comment_done;
                    }
                    metadata.data.vorbis_comment.vendor       = pRunningData;                                   pRunningData += metadata.data.vorbis_comment.vendorLength;
                    metadata.data.vorbis_comment.commentCount = drflac__le2host_32_ptr_unaligned(pRunningData); pRunningData += 4;

                    
                    if ((pRunningDataEnd - pRunningData) / sizeof(drflac_uint32) < metadata.data.vorbis_comment.commentCount) { 
                        goto vorbis_comment_done;
                    }
                    metadata.data.vorbis_comment.pComments    = pRunningData;

                    
                    for (i = 0; i < metadata.data.vorbis_comment.commentCount; ++i) {
                        drflac_uint32 commentLength;

                        if (pRunningDataEnd - pRunningData < 4) {
                            goto vorbis_comment_done;
                        }

                        commentLength = drflac__le2host_32_ptr_unaligned(pRunningData); pRunningData += 4;
                        if (pRunningDataEnd - pRunningData < (drflac_int64)commentLength) { 
                            goto vorbis_comment_done;
                        }
                        pRunningData += commentLength;
                    }

                    onMeta(pUserDataMD, &metadata);

                vorbis_comment_done:
                    drflac__free_from_callbacks(pRawData, pAllocationCallbacks);
                }
            } break;

            case DRFLAC_METADATA_BLOCK_TYPE_CUESHEET:
            {
                if (blockSize < 396) {
                    return DRFLAC_INVALID_DATA;
                }

                if (onMeta) {
                    void* pRawData;
                    const char* pRunningData;
                    const char* pRunningDataEnd;
                    size_t bufferSize;
                    drflac_uint8 iTrack;
                    drflac_uint8 iIndex;
                    void* pTrackData;

                    



                    pRawData = drflac__malloc_from_callbacks(blockSize, pAllocationCallbacks);
                    if (pRawData == NULL) {
                        return DRFLAC_ERROR;
                    }

                    if (onRead(pUserData, pRawData, blockSize) != blockSize) {
                        drflac__free_from_callbacks(pRawData, pAllocationCallbacks);
                        return DRFLAC_ERROR;
                    }

                    metadata.pRawData = pRawData;
                    metadata.rawDataSize = blockSize;

                    pRunningData    = (const char*)pRawData;
                    pRunningDataEnd = (const char*)pRawData + blockSize;

                    DRFLAC_COPY_MEMORY(metadata.data.cuesheet.catalog, pRunningData, 128);                              pRunningData += 128;
                    metadata.data.cuesheet.leadInSampleCount = drflac__be2host_64(*(const drflac_uint64*)pRunningData); pRunningData += 8;
                    metadata.data.cuesheet.isCD              = (pRunningData[0] & 0x80) != 0;                           pRunningData += 259;
                    metadata.data.cuesheet.trackCount        = pRunningData[0];                                         pRunningData += 1;
                    metadata.data.cuesheet.pTrackData        = NULL;    

                    
                    {
                        const char* pRunningDataSaved = pRunningData;   

                        bufferSize = metadata.data.cuesheet.trackCount * DRFLAC_CUESHEET_TRACK_PADDED_SIZE_IN_BYTES;

                        for (iTrack = 0; iTrack < metadata.data.cuesheet.trackCount; ++iTrack) {
                            drflac_uint8 indexCount;
                            drflac_uint32 indexPointSize;

                            if (pRunningDataEnd - pRunningData < DRFLAC_CUESHEET_TRACK_SIZE_IN_BYTES) {
                                drflac__free_from_callbacks(pRawData, pAllocationCallbacks);
                                return DRFLAC_INVALID_DATA;
                            }

                            
                            pRunningData += 35;

                            indexCount = pRunningData[0];
                            pRunningData += 1;

                            bufferSize += indexCount * sizeof(drflac_cuesheet_track_index);

                            
                            indexPointSize = indexCount * DRFLAC_CUESHEET_TRACK_INDEX_SIZE_IN_BYTES;
                            if (pRunningDataEnd - pRunningData < (drflac_int64)indexPointSize) {
                                drflac__free_from_callbacks(pRawData, pAllocationCallbacks);
                                return DRFLAC_INVALID_DATA;
                            }

                            pRunningData += indexPointSize;
                        }

                        pRunningData = pRunningDataSaved;
                    }

                    
                    {
                        char* pRunningTrackData;

                        pTrackData = drflac__malloc_from_callbacks(bufferSize, pAllocationCallbacks);
                        if (pTrackData == NULL) {
                            drflac__free_from_callbacks(pRawData, pAllocationCallbacks);
                            return DRFLAC_ERROR;
                        }

                        pRunningTrackData = (char*)pTrackData;

                        for (iTrack = 0; iTrack < metadata.data.cuesheet.trackCount; ++iTrack) {
                            drflac_uint8 indexCount;

                            DRFLAC_COPY_MEMORY(pRunningTrackData, pRunningData, DRFLAC_CUESHEET_TRACK_SIZE_IN_BYTES);
                            pRunningData      += DRFLAC_CUESHEET_TRACK_SIZE_IN_BYTES-1; 
                            pRunningTrackData += DRFLAC_CUESHEET_TRACK_SIZE_IN_BYTES-1;

                            
                            indexCount = pRunningData[0];
                            pRunningData      += 1;
                            pRunningTrackData += 1 + (DRFLAC_CUESHEET_TRACK_PADDED_SIZE_IN_BYTES - DRFLAC_CUESHEET_TRACK_SIZE_IN_BYTES);

                            
                            for (iIndex = 0; iIndex < indexCount; ++iIndex) {
                                drflac_cuesheet_track_index* pTrackIndex = (drflac_cuesheet_track_index*)pRunningTrackData;

                                DRFLAC_COPY_MEMORY(pRunningTrackData, pRunningData, DRFLAC_CUESHEET_TRACK_INDEX_SIZE_IN_BYTES);
                                pRunningData      += DRFLAC_CUESHEET_TRACK_INDEX_SIZE_IN_BYTES;
                                pRunningTrackData += sizeof(drflac_cuesheet_track_index);

                                pTrackIndex->offset = drflac__be2host_64(pTrackIndex->offset);
                            }
                        }

                        metadata.data.cuesheet.pTrackData = pTrackData;
                    }

                    onMeta(pUserDataMD, &metadata);

                    drflac__free_from_callbacks(pRawData, pAllocationCallbacks);
                    pRawData = NULL;
                    drflac__free_from_callbacks(pTrackData, pAllocationCallbacks);
                    pTrackData = NULL;
                }
            } break;

            case DRFLAC_METADATA_BLOCK_TYPE_PICTURE:
            {
                if (blockSize < 32) {
                    return DRFLAC_INVALID_DATA;
                }

                if (onMeta) {
                    drflac_result result = DRFLAC_SUCCESS;
                    drflac_uint32 blockSizeRemaining = blockSize;
                    char* pMime = NULL;
                    char* pDescription = NULL;
                    void* pPictureData = NULL;

                    if (blockSizeRemaining < 4) {
                        result = DRFLAC_INVALID_DATA;
                        goto done_flac;
                    }

                    if (onRead(pUserData, &metadata.data.picture.type, 4) != 4) {
                        result = DRFLAC_ERROR;
                        goto done_flac;
                    }
                    blockSizeRemaining -= 4;
                    metadata.data.picture.type = drflac__be2host_32(metadata.data.picture.type);


                    if (blockSizeRemaining < 4) {
                        result = DRFLAC_INVALID_DATA;
                        goto done_flac;
                    }

                    if (onRead(pUserData, &metadata.data.picture.mimeLength, 4) != 4) {
                        result = DRFLAC_ERROR;
                        goto done_flac;
                    }
                    blockSizeRemaining -= 4;
                    metadata.data.picture.mimeLength = drflac__be2host_32(metadata.data.picture.mimeLength);

                    if (blockSizeRemaining < metadata.data.picture.mimeLength) {
                        result = DRFLAC_INVALID_DATA;
                        goto done_flac;
                    }

                    pMime = (char*)drflac__malloc_from_callbacks(metadata.data.picture.mimeLength + 1, pAllocationCallbacks); 
                    if (pMime == NULL) {
                        result = DRFLAC_ERROR;
                        goto done_flac;
                    }

                    if (onRead(pUserData, pMime, metadata.data.picture.mimeLength) != metadata.data.picture.mimeLength) {
                        result = DRFLAC_ERROR;
                        goto done_flac;
                    }
                    blockSizeRemaining -= metadata.data.picture.mimeLength;
                    pMime[metadata.data.picture.mimeLength] = '\0';  
                    metadata.data.picture.mime = (const char*)pMime;


                    if (blockSizeRemaining < 4) {
                        result = DRFLAC_INVALID_DATA;
                        goto done_flac;
                    }

                    if (onRead(pUserData, &metadata.data.picture.descriptionLength, 4) != 4) {
                        result = DRFLAC_ERROR;
                        goto done_flac;
                    }
                    blockSizeRemaining -= 4;
                    metadata.data.picture.descriptionLength = drflac__be2host_32(metadata.data.picture.descriptionLength);

                    if (blockSizeRemaining < metadata.data.picture.descriptionLength) {
                        result = DRFLAC_INVALID_DATA;
                        goto done_flac;
                    }

                    pDescription = (char*)drflac__malloc_from_callbacks(metadata.data.picture.descriptionLength + 1, pAllocationCallbacks); 
                    if (pDescription == NULL) {
                        result = DRFLAC_ERROR;
                        goto done_flac;
                    }

                    if (onRead(pUserData, pDescription, metadata.data.picture.descriptionLength) != metadata.data.picture.descriptionLength) {
                        result = DRFLAC_ERROR;
                        goto done_flac;
                    }
                    blockSizeRemaining -= metadata.data.picture.descriptionLength;
                    pDescription[metadata.data.picture.descriptionLength] = '\0';  
                    metadata.data.picture.description = (const char*)pDescription;


                    if (blockSizeRemaining < 4) {
                        result = DRFLAC_INVALID_DATA;
                        goto done_flac;
                    }

                    if (onRead(pUserData, &metadata.data.picture.width, 4) != 4) {
                        result = DRFLAC_ERROR;
                        goto done_flac;
                    }
                    blockSizeRemaining -= 4;
                    metadata.data.picture.width = drflac__be2host_32(metadata.data.picture.width);

                    if (blockSizeRemaining < 4) {
                        result = DRFLAC_INVALID_DATA;
                        goto done_flac;
                    }

                    if (onRead(pUserData, &metadata.data.picture.height, 4) != 4) {
                        result = DRFLAC_ERROR;
                        goto done_flac;
                    }
                    blockSizeRemaining -= 4;
                    metadata.data.picture.height = drflac__be2host_32(metadata.data.picture.height);

                    if (blockSizeRemaining < 4) {
                        result = DRFLAC_INVALID_DATA;
                        goto done_flac;
                    }

                    if (onRead(pUserData, &metadata.data.picture.colorDepth, 4) != 4) {
                        result = DRFLAC_ERROR;
                        goto done_flac;
                    }
                    blockSizeRemaining -= 4;
                    metadata.data.picture.colorDepth = drflac__be2host_32(metadata.data.picture.colorDepth);

                    if (blockSizeRemaining < 4) {
                        result = DRFLAC_INVALID_DATA;
                        goto done_flac;
                    }

                    if (onRead(pUserData, &metadata.data.picture.indexColorCount, 4) != 4) {
                        result = DRFLAC_ERROR;
                        goto done_flac;
                    }
                    blockSizeRemaining -= 4;
                    metadata.data.picture.indexColorCount = drflac__be2host_32(metadata.data.picture.indexColorCount);


                    
                    if (blockSizeRemaining < 4) {
                        result = DRFLAC_INVALID_DATA;
                        goto done_flac;
                    }

                    if (onRead(pUserData, &metadata.data.picture.pictureDataSize, 4) != 4) {
                        result = DRFLAC_ERROR;
                        goto done_flac;
                    }
                    blockSizeRemaining -= 4;
                    metadata.data.picture.pictureDataSize = drflac__be2host_32(metadata.data.picture.pictureDataSize);

                    if (blockSizeRemaining < metadata.data.picture.pictureDataSize) {
                        result = DRFLAC_INVALID_DATA;
                        goto done_flac;
                    }

                    
                    metadata.data.picture.pictureDataOffset = runningFilePos + (blockSize - blockSizeRemaining);

                    



                #ifndef DR_FLAC_NO_PICTURE_METADATA_MALLOC
                    pPictureData = drflac__malloc_from_callbacks(metadata.data.picture.pictureDataSize, pAllocationCallbacks);
                    if (pPictureData != NULL) {
                        if (onRead(pUserData, pPictureData, metadata.data.picture.pictureDataSize) != metadata.data.picture.pictureDataSize) {
                            result = DRFLAC_ERROR;
                            goto done_flac;
                        }
                    } else
                #endif
                    {
                        
                        if (!onSeek(pUserData, metadata.data.picture.pictureDataSize, DRFLAC_SEEK_CUR)) {
                            result = DRFLAC_ERROR;
                            goto done_flac;
                        }
                    }

                    blockSizeRemaining -= metadata.data.picture.pictureDataSize;
                    (void)blockSizeRemaining;

                    metadata.data.picture.pPictureData = (const drflac_uint8*)pPictureData;
                    

                    
                    if (metadata.data.picture.pictureDataOffset != 0 || metadata.data.picture.pPictureData != NULL) {
                        onMeta(pUserDataMD, &metadata);
                    } else {
                        
                    }

                done_flac:
                    drflac__free_from_callbacks(pMime,        pAllocationCallbacks);
                    drflac__free_from_callbacks(pDescription, pAllocationCallbacks);
                    drflac__free_from_callbacks(pPictureData, pAllocationCallbacks);

                    if (result != DRFLAC_SUCCESS) {
                        return result;
                    }
                }
            } break;

            case DRFLAC_METADATA_BLOCK_TYPE_PADDING:
            {
                if (onMeta) {
                    metadata.data.padding.unused = 0;

                    
                    if (!onSeek(pUserData, blockSize, DRFLAC_SEEK_CUR)) {
                        return DRFLAC_BAD_SEEK;
                    } else {
                        onMeta(pUserDataMD, &metadata);
                    }
                }
            } break;

            case DRFLAC_METADATA_BLOCK_TYPE_INVALID:
            {
                return DRFLAC_INVALID_DATA;
            } break;

            default:
            {
                



                if (onMeta) {
                    void* pRawData = drflac__malloc_from_callbacks(blockSize, pAllocationCallbacks);
                    if (pRawData != NULL) {
                        if (onRead(pUserData, pRawData, blockSize) != blockSize) {
                            drflac__free_from_callbacks(pRawData, pAllocationCallbacks);
                            return DRFLAC_ERROR;
                        }
                    } else {
                        
                        if (!onSeek(pUserData, blockSize, DRFLAC_SEEK_CUR)) {
                            return DRFLAC_ERROR;
                        }
                    }

                    metadata.pRawData = pRawData;
                    metadata.rawDataSize = blockSize;
                    onMeta(pUserDataMD, &metadata);

                    drflac__free_from_callbacks(pRawData, pAllocationCallbacks);
                }
            } break;
        }

        
        if (onMeta == NULL && blockSize > 0) {
            if (!onSeek(pUserData, blockSize, DRFLAC_SEEK_CUR)) {
                return DRFLAC_BAD_SEEK;
            }
        }

        runningFilePos += blockSize;
        if (isLastBlock) {
            break;
        }
    }

    *pSeektablePos   = seektablePos;
    *pSeekpointCount = seektableSize / DRFLAC_SEEKPOINT_SIZE_IN_BYTES;
    *pFirstFramePos  = runningFilePos;

    return DRFLAC_SUCCESS;
}

static drflac_bool32 drflac__init_private__native_without_streaminfo(drflac_init_info* pInit, const drflac_uint8* pInitialData, drflac_uint64 firstFrameSearchPos)
{
    drflac_uint32 initialData;

    pInit->container = drflac_container_native;
    pInit->hasStreamInfoBlock = DRFLAC_FALSE;
    pInit->hasMetadataBlocks = DRFLAC_FALSE;
    pInit->firstFrameSearchPos = firstFrameSearchPos;

    
    DRFLAC_COPY_MEMORY(&initialData, pInitialData, sizeof(initialData));
    initialData = drflac__be2host_32(initialData);

    pInit->bs.consumedBits = DRFLAC_CACHE_L1_SIZE_BITS(&pInit->bs) - 32;
    pInit->bs.cache        = (drflac_cache_t)initialData << pInit->bs.consumedBits;
    #ifndef DR_FLAC_NO_CRC
    {
        pInit->bs.crc16Cache = initialData;
        pInit->bs.crc16CacheIgnoredBytes = pInit->bs.consumedBits / 8;
    }
    #endif

    if (!drflac__read_next_flac_frame_header(&pInit->bs, 0, &pInit->firstFrameHeader)) {
        return DRFLAC_FALSE;
    }

    if (pInit->firstFrameHeader.bitsPerSample == 0 || pInit->firstFrameHeader.sampleRate == 0) {
        return DRFLAC_FALSE;    
    }

    pInit->sampleRate              = pInit->firstFrameHeader.sampleRate;
    pInit->channels                = drflac__get_channel_count_from_channel_assignment(pInit->firstFrameHeader.channelAssignment);
    pInit->bitsPerSample           = pInit->firstFrameHeader.bitsPerSample;
    pInit->maxBlockSizeInPCMFrames = 65535;   

    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__init_private__native(drflac_init_info* pInit, drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_meta_proc onMeta, void* pUserData, void* pUserDataMD, drflac_bool32 relaxed)
{
    

    drflac_uint8 isLastBlock;
    drflac_uint8 blockType;
    drflac_uint32 blockSize;
    drflac_uint32 blockHeader;

    (void)onSeek;

    pInit->container = drflac_container_native;

    
    if (onRead(pUserData, &blockHeader, 4) != 4) {
        return DRFLAC_FALSE;
    }

    drflac__decode_block_header(blockHeader, &isLastBlock, &blockType, &blockSize);

    if (blockType != DRFLAC_METADATA_BLOCK_TYPE_STREAMINFO || blockSize != 34) {
        if (!relaxed) {
            
            return DRFLAC_FALSE;
        } else {
            return drflac__init_private__native_without_streaminfo(pInit, (const drflac_uint8*)&blockHeader, pInit->runningFilePos);
        }
    } else {
        drflac_streaminfo streaminfo;
        if (!drflac__read_streaminfo(onRead, pUserData, &streaminfo)) {
            return DRFLAC_FALSE;
        }

        pInit->hasStreamInfoBlock      = DRFLAC_TRUE;
        pInit->sampleRate              = streaminfo.sampleRate;
        pInit->channels                = streaminfo.channels;
        pInit->bitsPerSample           = streaminfo.bitsPerSample;
        pInit->totalPCMFrameCount      = streaminfo.totalPCMFrameCount;
        pInit->maxBlockSizeInPCMFrames = streaminfo.maxBlockSizeInPCMFrames;    
        pInit->hasMetadataBlocks       = !isLastBlock;

        if (onMeta) {
            drflac_metadata metadata;
            metadata.type = DRFLAC_METADATA_BLOCK_TYPE_STREAMINFO;
            metadata.pRawData = NULL;
            metadata.rawDataSize = 0;
            metadata.data.streaminfo = streaminfo;
            onMeta(pUserDataMD, &metadata);
        }

        return DRFLAC_TRUE;
    }
}

#ifndef DR_FLAC_NO_OGG
#define DRFLAC_OGG_MAX_PAGE_SIZE            65307
#define DRFLAC_OGG_CAPTURE_PATTERN_CRC32    1605413199  

typedef enum
{
    drflac_ogg_recover_on_crc_mismatch,
    drflac_ogg_fail_on_crc_mismatch
} drflac_ogg_crc_mismatch_recovery;

#ifndef DR_FLAC_NO_CRC
static drflac_uint32 drflac__crc32_table[] = {
    0x00000000L, 0x04C11DB7L, 0x09823B6EL, 0x0D4326D9L,
    0x130476DCL, 0x17C56B6BL, 0x1A864DB2L, 0x1E475005L,
    0x2608EDB8L, 0x22C9F00FL, 0x2F8AD6D6L, 0x2B4BCB61L,
    0x350C9B64L, 0x31CD86D3L, 0x3C8EA00AL, 0x384FBDBDL,
    0x4C11DB70L, 0x48D0C6C7L, 0x4593E01EL, 0x4152FDA9L,
    0x5F15ADACL, 0x5BD4B01BL, 0x569796C2L, 0x52568B75L,
    0x6A1936C8L, 0x6ED82B7FL, 0x639B0DA6L, 0x675A1011L,
    0x791D4014L, 0x7DDC5DA3L, 0x709F7B7AL, 0x745E66CDL,
    0x9823B6E0L, 0x9CE2AB57L, 0x91A18D8EL, 0x95609039L,
    0x8B27C03CL, 0x8FE6DD8BL, 0x82A5FB52L, 0x8664E6E5L,
    0xBE2B5B58L, 0xBAEA46EFL, 0xB7A96036L, 0xB3687D81L,
    0xAD2F2D84L, 0xA9EE3033L, 0xA4AD16EAL, 0xA06C0B5DL,
    0xD4326D90L, 0xD0F37027L, 0xDDB056FEL, 0xD9714B49L,
    0xC7361B4CL, 0xC3F706FBL, 0xCEB42022L, 0xCA753D95L,
    0xF23A8028L, 0xF6FB9D9FL, 0xFBB8BB46L, 0xFF79A6F1L,
    0xE13EF6F4L, 0xE5FFEB43L, 0xE8BCCD9AL, 0xEC7DD02DL,
    0x34867077L, 0x30476DC0L, 0x3D044B19L, 0x39C556AEL,
    0x278206ABL, 0x23431B1CL, 0x2E003DC5L, 0x2AC12072L,
    0x128E9DCFL, 0x164F8078L, 0x1B0CA6A1L, 0x1FCDBB16L,
    0x018AEB13L, 0x054BF6A4L, 0x0808D07DL, 0x0CC9CDCAL,
    0x7897AB07L, 0x7C56B6B0L, 0x71159069L, 0x75D48DDEL,
    0x6B93DDDBL, 0x6F52C06CL, 0x6211E6B5L, 0x66D0FB02L,
    0x5E9F46BFL, 0x5A5E5B08L, 0x571D7DD1L, 0x53DC6066L,
    0x4D9B3063L, 0x495A2DD4L, 0x44190B0DL, 0x40D816BAL,
    0xACA5C697L, 0xA864DB20L, 0xA527FDF9L, 0xA1E6E04EL,
    0xBFA1B04BL, 0xBB60ADFCL, 0xB6238B25L, 0xB2E29692L,
    0x8AAD2B2FL, 0x8E6C3698L, 0x832F1041L, 0x87EE0DF6L,
    0x99A95DF3L, 0x9D684044L, 0x902B669DL, 0x94EA7B2AL,
    0xE0B41DE7L, 0xE4750050L, 0xE9362689L, 0xEDF73B3EL,
    0xF3B06B3BL, 0xF771768CL, 0xFA325055L, 0xFEF34DE2L,
    0xC6BCF05FL, 0xC27DEDE8L, 0xCF3ECB31L, 0xCBFFD686L,
    0xD5B88683L, 0xD1799B34L, 0xDC3ABDEDL, 0xD8FBA05AL,
    0x690CE0EEL, 0x6DCDFD59L, 0x608EDB80L, 0x644FC637L,
    0x7A089632L, 0x7EC98B85L, 0x738AAD5CL, 0x774BB0EBL,
    0x4F040D56L, 0x4BC510E1L, 0x46863638L, 0x42472B8FL,
    0x5C007B8AL, 0x58C1663DL, 0x558240E4L, 0x51435D53L,
    0x251D3B9EL, 0x21DC2629L, 0x2C9F00F0L, 0x285E1D47L,
    0x36194D42L, 0x32D850F5L, 0x3F9B762CL, 0x3B5A6B9BL,
    0x0315D626L, 0x07D4CB91L, 0x0A97ED48L, 0x0E56F0FFL,
    0x1011A0FAL, 0x14D0BD4DL, 0x19939B94L, 0x1D528623L,
    0xF12F560EL, 0xF5EE4BB9L, 0xF8AD6D60L, 0xFC6C70D7L,
    0xE22B20D2L, 0xE6EA3D65L, 0xEBA91BBCL, 0xEF68060BL,
    0xD727BBB6L, 0xD3E6A601L, 0xDEA580D8L, 0xDA649D6FL,
    0xC423CD6AL, 0xC0E2D0DDL, 0xCDA1F604L, 0xC960EBB3L,
    0xBD3E8D7EL, 0xB9FF90C9L, 0xB4BCB610L, 0xB07DABA7L,
    0xAE3AFBA2L, 0xAAFBE615L, 0xA7B8C0CCL, 0xA379DD7BL,
    0x9B3660C6L, 0x9FF77D71L, 0x92B45BA8L, 0x9675461FL,
    0x8832161AL, 0x8CF30BADL, 0x81B02D74L, 0x857130C3L,
    0x5D8A9099L, 0x594B8D2EL, 0x5408ABF7L, 0x50C9B640L,
    0x4E8EE645L, 0x4A4FFBF2L, 0x470CDD2BL, 0x43CDC09CL,
    0x7B827D21L, 0x7F436096L, 0x7200464FL, 0x76C15BF8L,
    0x68860BFDL, 0x6C47164AL, 0x61043093L, 0x65C52D24L,
    0x119B4BE9L, 0x155A565EL, 0x18197087L, 0x1CD86D30L,
    0x029F3D35L, 0x065E2082L, 0x0B1D065BL, 0x0FDC1BECL,
    0x3793A651L, 0x3352BBE6L, 0x3E119D3FL, 0x3AD08088L,
    0x2497D08DL, 0x2056CD3AL, 0x2D15EBE3L, 0x29D4F654L,
    0xC5A92679L, 0xC1683BCEL, 0xCC2B1D17L, 0xC8EA00A0L,
    0xD6AD50A5L, 0xD26C4D12L, 0xDF2F6BCBL, 0xDBEE767CL,
    0xE3A1CBC1L, 0xE760D676L, 0xEA23F0AFL, 0xEEE2ED18L,
    0xF0A5BD1DL, 0xF464A0AAL, 0xF9278673L, 0xFDE69BC4L,
    0x89B8FD09L, 0x8D79E0BEL, 0x803AC667L, 0x84FBDBD0L,
    0x9ABC8BD5L, 0x9E7D9662L, 0x933EB0BBL, 0x97FFAD0CL,
    0xAFB010B1L, 0xAB710D06L, 0xA6322BDFL, 0xA2F33668L,
    0xBCB4666DL, 0xB8757BDAL, 0xB5365D03L, 0xB1F740B4L
};
#endif

static DRFLAC_INLINE drflac_uint32 drflac_crc32_byte(drflac_uint32 crc32, drflac_uint8 data)
{
#ifndef DR_FLAC_NO_CRC
    return (crc32 << 8) ^ drflac__crc32_table[(drflac_uint8)((crc32 >> 24) & 0xFF) ^ data];
#else
    (void)data;
    return crc32;
#endif
}

#if 0
static DRFLAC_INLINE drflac_uint32 drflac_crc32_uint32(drflac_uint32 crc32, drflac_uint32 data)
{
    crc32 = drflac_crc32_byte(crc32, (drflac_uint8)((data >> 24) & 0xFF));
    crc32 = drflac_crc32_byte(crc32, (drflac_uint8)((data >> 16) & 0xFF));
    crc32 = drflac_crc32_byte(crc32, (drflac_uint8)((data >>  8) & 0xFF));
    crc32 = drflac_crc32_byte(crc32, (drflac_uint8)((data >>  0) & 0xFF));
    return crc32;
}

static DRFLAC_INLINE drflac_uint32 drflac_crc32_uint64(drflac_uint32 crc32, drflac_uint64 data)
{
    crc32 = drflac_crc32_uint32(crc32, (drflac_uint32)((data >> 32) & 0xFFFFFFFF));
    crc32 = drflac_crc32_uint32(crc32, (drflac_uint32)((data >>  0) & 0xFFFFFFFF));
    return crc32;
}
#endif

static DRFLAC_INLINE drflac_uint32 drflac_crc32_buffer(drflac_uint32 crc32, drflac_uint8* pData, drflac_uint32 dataSize)
{
    
    drflac_uint32 i;
    for (i = 0; i < dataSize; ++i) {
        crc32 = drflac_crc32_byte(crc32, pData[i]);
    }
    return crc32;
}


static DRFLAC_INLINE drflac_bool32 drflac_ogg__is_capture_pattern(drflac_uint8 pattern[4])
{
    return pattern[0] == 'O' && pattern[1] == 'g' && pattern[2] == 'g' && pattern[3] == 'S';
}

static DRFLAC_INLINE drflac_uint32 drflac_ogg__get_page_header_size(drflac_ogg_page_header* pHeader)
{
    return 27 + pHeader->segmentCount;
}

static DRFLAC_INLINE drflac_uint32 drflac_ogg__get_page_body_size(drflac_ogg_page_header* pHeader)
{
    drflac_uint32 pageBodySize = 0;
    int i;

    for (i = 0; i < pHeader->segmentCount; ++i) {
        pageBodySize += pHeader->segmentTable[i];
    }

    return pageBodySize;
}

static drflac_result drflac_ogg__read_page_header_after_capture_pattern(drflac_read_proc onRead, void* pUserData, drflac_ogg_page_header* pHeader, drflac_uint32* pBytesRead, drflac_uint32* pCRC32)
{
    drflac_uint8 data[23];
    drflac_uint32 i;

    DRFLAC_ASSERT(*pCRC32 == DRFLAC_OGG_CAPTURE_PATTERN_CRC32);

    if (onRead(pUserData, data, 23) != 23) {
        return DRFLAC_AT_END;
    }
    *pBytesRead += 23;

    




    pHeader->capturePattern[0] = 'O';
    pHeader->capturePattern[1] = 'g';
    pHeader->capturePattern[2] = 'g';
    pHeader->capturePattern[3] = 'S';

    pHeader->structureVersion = data[0];
    pHeader->headerType       = data[1];
    DRFLAC_COPY_MEMORY(&pHeader->granulePosition, &data[ 2], 8);
    DRFLAC_COPY_MEMORY(&pHeader->serialNumber,    &data[10], 4);
    DRFLAC_COPY_MEMORY(&pHeader->sequenceNumber,  &data[14], 4);
    DRFLAC_COPY_MEMORY(&pHeader->checksum,        &data[18], 4);
    pHeader->segmentCount     = data[22];

    
    data[18] = 0;
    data[19] = 0;
    data[20] = 0;
    data[21] = 0;

    for (i = 0; i < 23; ++i) {
        *pCRC32 = drflac_crc32_byte(*pCRC32, data[i]);
    }


    if (onRead(pUserData, pHeader->segmentTable, pHeader->segmentCount) != pHeader->segmentCount) {
        return DRFLAC_AT_END;
    }
    *pBytesRead += pHeader->segmentCount;

    for (i = 0; i < pHeader->segmentCount; ++i) {
        *pCRC32 = drflac_crc32_byte(*pCRC32, pHeader->segmentTable[i]);
    }

    return DRFLAC_SUCCESS;
}

static drflac_result drflac_ogg__read_page_header(drflac_read_proc onRead, void* pUserData, drflac_ogg_page_header* pHeader, drflac_uint32* pBytesRead, drflac_uint32* pCRC32)
{
    drflac_uint8 id[4];

    *pBytesRead = 0;

    if (onRead(pUserData, id, 4) != 4) {
        return DRFLAC_AT_END;
    }
    *pBytesRead += 4;

    
    for (;;) {
        if (drflac_ogg__is_capture_pattern(id)) {
            drflac_result result;

            *pCRC32 = DRFLAC_OGG_CAPTURE_PATTERN_CRC32;

            result = drflac_ogg__read_page_header_after_capture_pattern(onRead, pUserData, pHeader, pBytesRead, pCRC32);
            if (result == DRFLAC_SUCCESS) {
                return DRFLAC_SUCCESS;
            } else {
                if (result == DRFLAC_CRC_MISMATCH) {
                    continue;
                } else {
                    return result;
                }
            }
        } else {
            
            id[0] = id[1];
            id[1] = id[2];
            id[2] = id[3];
            if (onRead(pUserData, &id[3], 1) != 1) {
                return DRFLAC_AT_END;
            }
            *pBytesRead += 1;
        }
    }
}









typedef struct
{
    drflac_read_proc onRead;                
    drflac_seek_proc onSeek;                
    drflac_tell_proc onTell;                
    void* pUserData;                        
    drflac_uint64 currentBytePos;           
    drflac_uint64 firstBytePos;             
    drflac_uint32 serialNumber;             
    drflac_ogg_page_header bosPageHeader;   
    drflac_ogg_page_header currentPageHeader;
    drflac_uint32 bytesRemainingInPage;
    drflac_uint32 pageDataSize;
    drflac_uint8 pageData[DRFLAC_OGG_MAX_PAGE_SIZE];
} drflac_oggbs; 

static size_t drflac_oggbs__read_physical(drflac_oggbs* oggbs, void* bufferOut, size_t bytesToRead)
{
    size_t bytesActuallyRead = oggbs->onRead(oggbs->pUserData, bufferOut, bytesToRead);
    oggbs->currentBytePos += bytesActuallyRead;

    return bytesActuallyRead;
}

static drflac_bool32 drflac_oggbs__seek_physical(drflac_oggbs* oggbs, drflac_uint64 offset, drflac_seek_origin origin)
{
    if (origin == DRFLAC_SEEK_SET) {
        if (offset <= 0x7FFFFFFF) {
            if (!oggbs->onSeek(oggbs->pUserData, (int)offset, DRFLAC_SEEK_SET)) {
                return DRFLAC_FALSE;
            }
            oggbs->currentBytePos = offset;

            return DRFLAC_TRUE;
        } else {
            if (!oggbs->onSeek(oggbs->pUserData, 0x7FFFFFFF, DRFLAC_SEEK_SET)) {
                return DRFLAC_FALSE;
            }
            oggbs->currentBytePos = offset;

            return drflac_oggbs__seek_physical(oggbs, offset - 0x7FFFFFFF, DRFLAC_SEEK_CUR);
        }
    } else {
        while (offset > 0x7FFFFFFF) {
            if (!oggbs->onSeek(oggbs->pUserData, 0x7FFFFFFF, DRFLAC_SEEK_CUR)) {
                return DRFLAC_FALSE;
            }
            oggbs->currentBytePos += 0x7FFFFFFF;
            offset -= 0x7FFFFFFF;
        }

        if (!oggbs->onSeek(oggbs->pUserData, (int)offset, DRFLAC_SEEK_CUR)) {    
            return DRFLAC_FALSE;
        }
        oggbs->currentBytePos += offset;

        return DRFLAC_TRUE;
    }
}

static drflac_bool32 drflac_oggbs__goto_next_page(drflac_oggbs* oggbs, drflac_ogg_crc_mismatch_recovery recoveryMethod)
{
    drflac_ogg_page_header header;
    for (;;) {
        drflac_uint32 crc32 = 0;
        drflac_uint32 bytesRead;
        drflac_uint32 pageBodySize;
#ifndef DR_FLAC_NO_CRC
        drflac_uint32 actualCRC32;
#endif

        if (drflac_ogg__read_page_header(oggbs->onRead, oggbs->pUserData, &header, &bytesRead, &crc32) != DRFLAC_SUCCESS) {
            return DRFLAC_FALSE;
        }
        oggbs->currentBytePos += bytesRead;

        pageBodySize = drflac_ogg__get_page_body_size(&header);
        if (pageBodySize > DRFLAC_OGG_MAX_PAGE_SIZE) {
            continue;   
        }

        if (header.serialNumber != oggbs->serialNumber) {
            
            if (pageBodySize > 0 && !drflac_oggbs__seek_physical(oggbs, pageBodySize, DRFLAC_SEEK_CUR)) {
                return DRFLAC_FALSE;
            }
            continue;
        }


        
        if (drflac_oggbs__read_physical(oggbs, oggbs->pageData, pageBodySize) != pageBodySize) {
            return DRFLAC_FALSE;
        }
        oggbs->pageDataSize = pageBodySize;

#ifndef DR_FLAC_NO_CRC
        actualCRC32 = drflac_crc32_buffer(crc32, oggbs->pageData, oggbs->pageDataSize);
        if (actualCRC32 != header.checksum) {
            if (recoveryMethod == drflac_ogg_recover_on_crc_mismatch) {
                continue;   
            } else {
                




                drflac_oggbs__goto_next_page(oggbs, drflac_ogg_recover_on_crc_mismatch);
                return DRFLAC_FALSE;
            }
        }
#else
        (void)recoveryMethod;   
#endif

        oggbs->currentPageHeader = header;
        oggbs->bytesRemainingInPage = pageBodySize;
        return DRFLAC_TRUE;
    }
}


#if 0
static drflac_uint8 drflac_oggbs__get_current_segment_index(drflac_oggbs* oggbs, drflac_uint8* pBytesRemainingInSeg)
{
    drflac_uint32 bytesConsumedInPage = drflac_ogg__get_page_body_size(&oggbs->currentPageHeader) - oggbs->bytesRemainingInPage;
    drflac_uint8 iSeg = 0;
    drflac_uint32 iByte = 0;
    while (iByte < bytesConsumedInPage) {
        drflac_uint8 segmentSize = oggbs->currentPageHeader.segmentTable[iSeg];
        if (iByte + segmentSize > bytesConsumedInPage) {
            break;
        } else {
            iSeg += 1;
            iByte += segmentSize;
        }
    }

    *pBytesRemainingInSeg = oggbs->currentPageHeader.segmentTable[iSeg] - (drflac_uint8)(bytesConsumedInPage - iByte);
    return iSeg;
}

static drflac_bool32 drflac_oggbs__seek_to_next_packet(drflac_oggbs* oggbs)
{
    
    for (;;) {
        drflac_bool32 atEndOfPage = DRFLAC_FALSE;

        drflac_uint8 bytesRemainingInSeg;
        drflac_uint8 iFirstSeg = drflac_oggbs__get_current_segment_index(oggbs, &bytesRemainingInSeg);

        drflac_uint32 bytesToEndOfPacketOrPage = bytesRemainingInSeg;
        for (drflac_uint8 iSeg = iFirstSeg; iSeg < oggbs->currentPageHeader.segmentCount; ++iSeg) {
            drflac_uint8 segmentSize = oggbs->currentPageHeader.segmentTable[iSeg];
            if (segmentSize < 255) {
                if (iSeg == oggbs->currentPageHeader.segmentCount-1) {
                    atEndOfPage = DRFLAC_TRUE;
                }

                break;
            }

            bytesToEndOfPacketOrPage += segmentSize;
        }

        



        drflac_oggbs__seek_physical(oggbs, bytesToEndOfPacketOrPage, DRFLAC_SEEK_CUR);
        oggbs->bytesRemainingInPage -= bytesToEndOfPacketOrPage;

        if (atEndOfPage) {
            



            if (!drflac_oggbs__goto_next_page(oggbs)) {
                return DRFLAC_FALSE;
            }

            
            if ((oggbs->currentPageHeader.headerType & 0x01) == 0) {
                return DRFLAC_TRUE;
            }
        } else {
            
            return DRFLAC_TRUE;
        }
    }
}

static drflac_bool32 drflac_oggbs__seek_to_next_frame(drflac_oggbs* oggbs)
{
    

    
    return drflac_oggbs__seek_to_next_packet(oggbs);
}
#endif

static size_t drflac__on_read_ogg(void* pUserData, void* bufferOut, size_t bytesToRead)
{
    drflac_oggbs* oggbs = (drflac_oggbs*)pUserData;
    drflac_uint8* pRunningBufferOut = (drflac_uint8*)bufferOut;
    size_t bytesRead = 0;

    DRFLAC_ASSERT(oggbs != NULL);
    DRFLAC_ASSERT(pRunningBufferOut != NULL);

    
    while (bytesRead < bytesToRead) {
        size_t bytesRemainingToRead = bytesToRead - bytesRead;

        if (oggbs->bytesRemainingInPage >= bytesRemainingToRead) {
            DRFLAC_COPY_MEMORY(pRunningBufferOut, oggbs->pageData + (oggbs->pageDataSize - oggbs->bytesRemainingInPage), bytesRemainingToRead);
            bytesRead += bytesRemainingToRead;
            oggbs->bytesRemainingInPage -= (drflac_uint32)bytesRemainingToRead;
            break;
        }

        
        if (oggbs->bytesRemainingInPage > 0) {
            DRFLAC_COPY_MEMORY(pRunningBufferOut, oggbs->pageData + (oggbs->pageDataSize - oggbs->bytesRemainingInPage), oggbs->bytesRemainingInPage);
            bytesRead += oggbs->bytesRemainingInPage;
            pRunningBufferOut += oggbs->bytesRemainingInPage;
            oggbs->bytesRemainingInPage = 0;
        }

        DRFLAC_ASSERT(bytesRemainingToRead > 0);
        if (!drflac_oggbs__goto_next_page(oggbs, drflac_ogg_recover_on_crc_mismatch)) {
            break;  
        }
    }

    return bytesRead;
}

static drflac_bool32 drflac__on_seek_ogg(void* pUserData, int offset, drflac_seek_origin origin)
{
    drflac_oggbs* oggbs = (drflac_oggbs*)pUserData;
    int bytesSeeked = 0;

    DRFLAC_ASSERT(oggbs != NULL);
    DRFLAC_ASSERT(offset >= 0);  

    
    if (origin == DRFLAC_SEEK_SET) {
        if (!drflac_oggbs__seek_physical(oggbs, (int)oggbs->firstBytePos, DRFLAC_SEEK_SET)) {
            return DRFLAC_FALSE;
        }

        if (!drflac_oggbs__goto_next_page(oggbs, drflac_ogg_fail_on_crc_mismatch)) {
            return DRFLAC_FALSE;
        }

        return drflac__on_seek_ogg(pUserData, offset, DRFLAC_SEEK_CUR);
    } else if (origin == DRFLAC_SEEK_CUR) {
        while (bytesSeeked < offset) {
            int bytesRemainingToSeek = offset - bytesSeeked;
            DRFLAC_ASSERT(bytesRemainingToSeek >= 0);

            if (oggbs->bytesRemainingInPage >= (size_t)bytesRemainingToSeek) {
                bytesSeeked += bytesRemainingToSeek;
                (void)bytesSeeked;  
                oggbs->bytesRemainingInPage -= bytesRemainingToSeek;
                break;
            }

            
            if (oggbs->bytesRemainingInPage > 0) {
                bytesSeeked += (int)oggbs->bytesRemainingInPage;
                oggbs->bytesRemainingInPage = 0;
            }

            DRFLAC_ASSERT(bytesRemainingToSeek > 0);
            if (!drflac_oggbs__goto_next_page(oggbs, drflac_ogg_fail_on_crc_mismatch)) {
                
                return DRFLAC_FALSE;
            }
        }
    } else if (origin == DRFLAC_SEEK_END) {
        
        return DRFLAC_FALSE;
    }

    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__on_tell_ogg(void* pUserData, drflac_int64* pCursor)
{
    



    (void)pUserData;
    (void)pCursor;
    return DRFLAC_FALSE;
}


static drflac_bool32 drflac_ogg__seek_to_pcm_frame(drflac* pFlac, drflac_uint64 pcmFrameIndex)
{
    drflac_oggbs* oggbs = (drflac_oggbs*)pFlac->_oggbs;
    drflac_uint64 originalBytePos;
    drflac_uint64 runningGranulePosition;
    drflac_uint64 runningFrameBytePos = 0;
    drflac_uint64 runningPCMFrameCount = 0;
    drflac_bool32 hasFramePage = DRFLAC_FALSE;

    DRFLAC_ASSERT(oggbs != NULL);

    originalBytePos = oggbs->currentBytePos;   

    
    if (!drflac__seek_to_byte(&pFlac->bs, pFlac->firstFLACFramePosInBytes)) {
        return DRFLAC_FALSE;
    }
    oggbs->bytesRemainingInPage = 0;

    runningGranulePosition = 0;
    for (;;) {
        if (!drflac_oggbs__goto_next_page(oggbs, drflac_ogg_recover_on_crc_mismatch)) {
            drflac_oggbs__seek_physical(oggbs, originalBytePos, DRFLAC_SEEK_SET);
            return DRFLAC_FALSE;   
        }

        



        if ((oggbs->currentPageHeader.headerType & 0x01) == 0) {    
            if (oggbs->currentPageHeader.segmentTable[0] >= 2) {
                drflac_uint8 firstBytesInPage[2];
                firstBytesInPage[0] = oggbs->pageData[0];
                firstBytesInPage[1] = oggbs->pageData[1];

                if ((firstBytesInPage[0] == 0xFF) && (firstBytesInPage[1] & 0xFC) == 0xF8) {    
                    runningFrameBytePos = oggbs->currentBytePos - drflac_ogg__get_page_header_size(&oggbs->currentPageHeader) - oggbs->pageDataSize;
                    runningPCMFrameCount = runningGranulePosition;
                    hasFramePage = DRFLAC_TRUE;
                }
            }
        }

        
        if (oggbs->currentPageHeader.granulePosition != (drflac_uint64)-1) {
            if (hasFramePage && oggbs->currentPageHeader.granulePosition >= pcmFrameIndex) {
                break;
            }

            runningGranulePosition = oggbs->currentPageHeader.granulePosition;
        }
    }

    





    if (!drflac_oggbs__seek_physical(oggbs, runningFrameBytePos, DRFLAC_SEEK_SET)) {
        return DRFLAC_FALSE;
    }
    if (!drflac_oggbs__goto_next_page(oggbs, drflac_ogg_recover_on_crc_mismatch)) {
        return DRFLAC_FALSE;
    }

    



    for (;;) {
        




















        drflac_uint64 firstPCMFrameInFLACFrame = 0;
        drflac_uint64 lastPCMFrameInFLACFrame = 0;
        drflac_uint64 pcmFrameCountInThisFrame;

        if (!drflac__read_next_flac_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFLACFrame.header)) {
            return DRFLAC_FALSE;
        }

        drflac__get_pcm_frame_range_of_current_flac_frame(pFlac, &firstPCMFrameInFLACFrame, &lastPCMFrameInFLACFrame);

        pcmFrameCountInThisFrame = (lastPCMFrameInFLACFrame - firstPCMFrameInFLACFrame) + 1;

        
        if ((runningPCMFrameCount + pcmFrameCountInThisFrame) == pcmFrameIndex) {
            drflac_result result = drflac__decode_flac_frame(pFlac);
            if (result == DRFLAC_SUCCESS) {
                pFlac->currentPCMFrame = pcmFrameIndex;
                pFlac->currentFLACFrame.pcmFramesRemaining = 0;
                return DRFLAC_TRUE;
            } else {
                return DRFLAC_FALSE;
            }
        }

        if (pcmFrameIndex < (runningPCMFrameCount + pcmFrameCountInThisFrame)) {
            



            drflac_result result = drflac__decode_flac_frame(pFlac);
            if (result == DRFLAC_SUCCESS) {
                
                drflac_uint64 pcmFramesToDecode = (size_t)(pcmFrameIndex - runningPCMFrameCount);    
                if (pcmFramesToDecode == 0) {
                    return DRFLAC_TRUE;
                }

                pFlac->currentPCMFrame = runningPCMFrameCount;

                return drflac__seek_forward_by_pcm_frames(pFlac, pcmFramesToDecode) == pcmFramesToDecode;  
            } else {
                if (result == DRFLAC_CRC_MISMATCH) {
                    continue;   
                } else {
                    return DRFLAC_FALSE;
                }
            }
        } else {
            



            drflac_result result = drflac__seek_to_next_flac_frame(pFlac);
            if (result == DRFLAC_SUCCESS) {
                runningPCMFrameCount += pcmFrameCountInThisFrame;
            } else {
                if (result == DRFLAC_CRC_MISMATCH) {
                    continue;   
                } else {
                    return DRFLAC_FALSE;
                }
            }
        }
    }
}



static drflac_bool32 drflac__init_private__ogg(drflac_init_info* pInit, drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_meta_proc onMeta, void* pUserData, void* pUserDataMD, drflac_bool32 relaxed)
{
    drflac_ogg_page_header header;
    drflac_uint32 crc32 = DRFLAC_OGG_CAPTURE_PATTERN_CRC32;
    drflac_uint32 bytesRead = 0;

    
    (void)relaxed;

    pInit->container = drflac_container_ogg;
    pInit->oggFirstBytePos = 0;

    




    if (drflac_ogg__read_page_header_after_capture_pattern(onRead, pUserData, &header, &bytesRead, &crc32) != DRFLAC_SUCCESS) {
        return DRFLAC_FALSE;
    }
    pInit->runningFilePos += bytesRead;

    for (;;) {
        int pageBodySize;

        
        if ((header.headerType & 0x02) == 0) {
            return DRFLAC_FALSE;
        }

        
        pageBodySize = drflac_ogg__get_page_body_size(&header);
        if (pageBodySize == 51) {   
            
            drflac_uint32 bytesRemainingInPage = pageBodySize;
            drflac_uint8 packetType;

            if (onRead(pUserData, &packetType, 1) != 1) {
                return DRFLAC_FALSE;
            }

            bytesRemainingInPage -= 1;
            if (packetType == 0x7F) {
                
                drflac_uint8 sig[4];
                if (onRead(pUserData, sig, 4) != 4) {
                    return DRFLAC_FALSE;
                }

                bytesRemainingInPage -= 4;
                if (sig[0] == 'F' && sig[1] == 'L' && sig[2] == 'A' && sig[3] == 'C') {
                    
                    drflac_uint8 mappingVersion[2];
                    if (onRead(pUserData, mappingVersion, 2) != 2) {
                        return DRFLAC_FALSE;
                    }

                    if (mappingVersion[0] != 1) {
                        return DRFLAC_FALSE;   
                    }

                    



                    if (!onSeek(pUserData, 2, DRFLAC_SEEK_CUR)) {
                        return DRFLAC_FALSE;
                    }

                    
                    if (onRead(pUserData, sig, 4) != 4) {
                        return DRFLAC_FALSE;
                    }

                    if (sig[0] == 'f' && sig[1] == 'L' && sig[2] == 'a' && sig[3] == 'C') {
                        
                        drflac_streaminfo streaminfo;
                        drflac_uint8 isLastBlock;
                        drflac_uint8 blockType;
                        drflac_uint32 blockSize;
                        if (!drflac__read_and_decode_block_header(onRead, pUserData, &isLastBlock, &blockType, &blockSize)) {
                            return DRFLAC_FALSE;
                        }

                        if (blockType != DRFLAC_METADATA_BLOCK_TYPE_STREAMINFO || blockSize != 34) {
                            return DRFLAC_FALSE;    
                        }

                        if (drflac__read_streaminfo(onRead, pUserData, &streaminfo)) {
                            
                            pInit->hasStreamInfoBlock      = DRFLAC_TRUE;
                            pInit->sampleRate              = streaminfo.sampleRate;
                            pInit->channels                = streaminfo.channels;
                            pInit->bitsPerSample           = streaminfo.bitsPerSample;
                            pInit->totalPCMFrameCount      = streaminfo.totalPCMFrameCount;
                            pInit->maxBlockSizeInPCMFrames = streaminfo.maxBlockSizeInPCMFrames;
                            pInit->hasMetadataBlocks       = !isLastBlock;

                            if (onMeta) {
                                drflac_metadata metadata;
                                metadata.type = DRFLAC_METADATA_BLOCK_TYPE_STREAMINFO;
                                metadata.pRawData = NULL;
                                metadata.rawDataSize = 0;
                                metadata.data.streaminfo = streaminfo;
                                onMeta(pUserDataMD, &metadata);
                            }

                            pInit->runningFilePos  += pageBodySize;
                            pInit->oggFirstBytePos  = pInit->runningFilePos - 79;   
                            pInit->oggSerial        = header.serialNumber;
                            pInit->oggBosHeader     = header;
                            break;
                        } else {
                            
                            return DRFLAC_FALSE;
                        }
                    } else {
                        
                        return DRFLAC_FALSE;
                    }
                } else {
                    
                    if (!onSeek(pUserData, bytesRemainingInPage, DRFLAC_SEEK_CUR)) {
                        return DRFLAC_FALSE;
                    }
                }
            } else {
                
                if (!onSeek(pUserData, bytesRemainingInPage, DRFLAC_SEEK_CUR)) {
                    return DRFLAC_FALSE;
                }
            }
        } else {
            if (!onSeek(pUserData, pageBodySize, DRFLAC_SEEK_CUR)) {
                return DRFLAC_FALSE;
            }
        }

        pInit->runningFilePos += pageBodySize;


        
        if (drflac_ogg__read_page_header(onRead, pUserData, &header, &bytesRead, &crc32) != DRFLAC_SUCCESS) {
            return DRFLAC_FALSE;
        }
        pInit->runningFilePos += bytesRead;
    }

    




    pInit->hasMetadataBlocks = DRFLAC_TRUE;    
    return DRFLAC_TRUE;
}
#endif

static drflac_bool32 drflac__init_private(drflac_init_info* pInit, drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, drflac_meta_proc onMeta, drflac_container container, void* pUserData, void* pUserDataMD)
{
    drflac_bool32 relaxed;
    drflac_uint8 id[4];

    if (pInit == NULL || onRead == NULL || onSeek == NULL) {    
        return DRFLAC_FALSE;
    }

    DRFLAC_ZERO_MEMORY(pInit, sizeof(*pInit));
    pInit->onRead       = onRead;
    pInit->onSeek       = onSeek;
    pInit->onTell       = onTell;
    pInit->onMeta       = onMeta;
    pInit->container    = container;
    pInit->pUserData    = pUserData;
    pInit->pUserDataMD  = pUserDataMD;

    pInit->bs.onRead    = onRead;
    pInit->bs.onSeek    = onSeek;
    pInit->bs.onTell    = onTell;
    pInit->bs.pUserData = pUserData;
    drflac__reset_cache(&pInit->bs);


    
    relaxed = container != drflac_container_unknown;

    
    for (;;) {
        if (onRead(pUserData, id, 4) != 4) {
            return DRFLAC_FALSE;    
        }
        pInit->runningFilePos += 4;

        if (id[0] == 'I' && id[1] == 'D' && id[2] == '3') {
            drflac_uint8 header[6];
            drflac_uint8 flags;
            drflac_uint32 headerSize;

            if (onRead(pUserData, header, 6) != 6) {
                return DRFLAC_FALSE;    
            }
            pInit->runningFilePos += 6;

            flags = header[1];

            DRFLAC_COPY_MEMORY(&headerSize, header+2, 4);
            headerSize = drflac__unsynchsafe_32(drflac__be2host_32(headerSize));
            if (flags & 0x10) {
                headerSize += 10;
            }

            if (!onSeek(pUserData, headerSize, DRFLAC_SEEK_CUR)) {
                return DRFLAC_FALSE;    
            }
            pInit->runningFilePos += headerSize;
        } else {
            break;
        }
    }

    if (id[0] == 'f' && id[1] == 'L' && id[2] == 'a' && id[3] == 'C') {
        return drflac__init_private__native(pInit, onRead, onSeek, onMeta, pUserData, pUserDataMD, relaxed);
    }
#ifndef DR_FLAC_NO_OGG
    if (id[0] == 'O' && id[1] == 'g' && id[2] == 'g' && id[3] == 'S') {
        return drflac__init_private__ogg(pInit, onRead, onSeek, onMeta, pUserData, pUserDataMD, relaxed);
    }
#endif

    
    if (relaxed) {
        if (container == drflac_container_native) {
            return drflac__init_private__native_without_streaminfo(pInit, id, pInit->runningFilePos - 4);
        }
#ifndef DR_FLAC_NO_OGG
        if (container == drflac_container_ogg) {
            return drflac__init_private__ogg(pInit, onRead, onSeek, onMeta, pUserData, pUserDataMD, relaxed);
        }
#endif
    }

    
    return DRFLAC_FALSE;
}

static void drflac__init_from_info(drflac* pFlac, const drflac_init_info* pInit)
{
    DRFLAC_ASSERT(pFlac != NULL);
    DRFLAC_ASSERT(pInit != NULL);

    DRFLAC_ZERO_MEMORY(pFlac, sizeof(*pFlac));
    pFlac->bs                      = pInit->bs;
    pFlac->onMeta                  = pInit->onMeta;
    pFlac->pUserDataMD             = pInit->pUserDataMD;
    pFlac->maxBlockSizeInPCMFrames = pInit->maxBlockSizeInPCMFrames;
    pFlac->decodedSamplesCapacityInPCMFrames = pInit->maxBlockSizeInPCMFrames;
    pFlac->sampleRate              = pInit->sampleRate;
    pFlac->channels                = (drflac_uint8)pInit->channels;
    pFlac->bitsPerSample           = (drflac_uint8)pInit->bitsPerSample;
    pFlac->totalPCMFrameCount      = pInit->totalPCMFrameCount;
    pFlac->container               = pInit->container;
}


static drflac* drflac_open_with_metadata_private(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, drflac_meta_proc onMeta, drflac_container container, void* pUserData, void* pUserDataMD, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    drflac_init_info init;
    drflac_uint32 allocationSize;
    drflac_uint32 decodedSampleSize;
    drflac_uint32 wholeSIMDVectorCountPerChannel;
    drflac_uint32 decodedSamplesAllocationSize;
    drflac_uint8* pDecodedSamples;
#ifndef DR_FLAC_NO_OGG
    drflac_oggbs* pOggbs = NULL;
#endif
    drflac_uint64 firstFramePos;
    drflac_uint64 seektablePos;
    drflac_uint32 seekpointCount;
    drflac_bool32 recoverMetadata = DRFLAC_FALSE;
    drflac_allocation_callbacks allocationCallbacks;
    drflac* pFlac;

    
    drflac__init_cpu_caps();

    if (!drflac__init_private(&init, onRead, onSeek, onTell, onMeta, container, pUserData, pUserDataMD)) {
        return NULL;
    }

    if (pAllocationCallbacks != NULL) {
        allocationCallbacks = *pAllocationCallbacks;
        if (allocationCallbacks.onFree == NULL || (allocationCallbacks.onMalloc == NULL && allocationCallbacks.onRealloc == NULL)) {
            return NULL;    
        }
    } else {
        allocationCallbacks.pUserData = NULL;
        allocationCallbacks.onMalloc  = drflac__malloc_default;
        allocationCallbacks.onRealloc = drflac__realloc_default;
        allocationCallbacks.onFree    = drflac__free_default;
    }


    








    allocationSize = sizeof(drflac);

    




    decodedSampleSize = sizeof(drflac_int32);
    if (init.bitsPerSample == 32) {
        decodedSampleSize = sizeof(drflac_int64);
    }

    if ((init.maxBlockSizeInPCMFrames % (DRFLAC_MAX_SIMD_VECTOR_SIZE / decodedSampleSize)) == 0) {
        wholeSIMDVectorCountPerChannel = (init.maxBlockSizeInPCMFrames / (DRFLAC_MAX_SIMD_VECTOR_SIZE / decodedSampleSize));
    } else {
        wholeSIMDVectorCountPerChannel = (init.maxBlockSizeInPCMFrames / (DRFLAC_MAX_SIMD_VECTOR_SIZE / decodedSampleSize)) + 1;
    }

    decodedSamplesAllocationSize = wholeSIMDVectorCountPerChannel * DRFLAC_MAX_SIMD_VECTOR_SIZE * init.channels;

    allocationSize += decodedSamplesAllocationSize;
    allocationSize += DRFLAC_MAX_SIMD_VECTOR_SIZE;  

#ifndef DR_FLAC_NO_OGG
    
    if (init.container == drflac_container_ogg) {
        allocationSize += sizeof(drflac_oggbs);

        pOggbs = (drflac_oggbs*)drflac__malloc_from_callbacks(sizeof(*pOggbs), &allocationCallbacks);
        if (pOggbs == NULL) {
            return NULL; 
        }

        DRFLAC_ZERO_MEMORY(pOggbs, sizeof(*pOggbs));
        pOggbs->onRead = onRead;
        pOggbs->onSeek = onSeek;
        pOggbs->onTell = onTell;
        pOggbs->pUserData = pUserData;
        pOggbs->currentBytePos = init.oggFirstBytePos;
        pOggbs->firstBytePos = init.oggFirstBytePos;
        pOggbs->serialNumber = init.oggSerial;
        pOggbs->bosPageHeader = init.oggBosHeader;
        pOggbs->bytesRemainingInPage = 0;
    }
#endif

    




    firstFramePos  = 42;   
    if (!init.hasStreamInfoBlock) {
        firstFramePos = init.firstFrameSearchPos;
    }

    seektablePos   = 0;
    seekpointCount = 0;
    if (init.hasMetadataBlocks) {
        drflac_result metadataResult;
        drflac_read_proc onReadOverride = onRead;
        drflac_seek_proc onSeekOverride = onSeek;
        drflac_tell_proc onTellOverride = onTell;
        void* pUserDataOverride = pUserData;

#ifndef DR_FLAC_NO_OGG
        if (init.container == drflac_container_ogg) {
            onReadOverride = drflac__on_read_ogg;
            onSeekOverride = drflac__on_seek_ogg;
            onTellOverride = drflac__on_tell_ogg;
            pUserDataOverride = (void*)pOggbs;
        }
#endif

        metadataResult = drflac__read_and_decode_metadata(onReadOverride, onSeekOverride, onTellOverride, onMeta, pUserDataOverride, pUserDataMD, &firstFramePos, &seektablePos, &seekpointCount, &allocationCallbacks);
        if (metadataResult != DRFLAC_SUCCESS) {
            if (metadataResult == DRFLAC_INVALID_DATA && init.container == drflac_container_native && onSeek != NULL) {
                
                firstFramePos   = 42;
                seektablePos    = 0;
                seekpointCount  = 0;
                recoverMetadata = DRFLAC_TRUE;
            } else {
        #ifndef DR_FLAC_NO_OGG
                drflac__free_from_callbacks(pOggbs, &allocationCallbacks);
        #endif
                return NULL;
            }
        }

        if ((0xFFFFFFFF - (seekpointCount * sizeof(drflac_seekpoint))) < allocationSize) {
        #ifndef DR_FLAC_NO_OGG
            drflac__free_from_callbacks(pOggbs, &allocationCallbacks);
        #endif
            return NULL;
        }

        allocationSize += seekpointCount * sizeof(drflac_seekpoint);
    }

    pFlac = (drflac*)drflac__malloc_from_callbacks((size_t)allocationSize, &allocationCallbacks);
    if (pFlac == NULL) {
    #ifndef DR_FLAC_NO_OGG
        drflac__free_from_callbacks(pOggbs, &allocationCallbacks);
    #endif
        return NULL;
    }

    drflac__init_from_info(pFlac, &init);
    pFlac->allocationCallbacks = allocationCallbacks;

    pDecodedSamples = (drflac_uint8*)drflac_align((size_t)pFlac->pExtraData, DRFLAC_MAX_SIMD_VECTOR_SIZE);
    if (init.bitsPerSample != 32) {
        pFlac->decodedSamples.pS32 = (drflac_int32*)pDecodedSamples;
    } else {
        pFlac->decodedSamples.pS64 = (drflac_int64*)pDecodedSamples;
    }

#ifndef DR_FLAC_NO_OGG
    if (init.container == drflac_container_ogg) {
        drflac_oggbs* pInternalOggbs = (drflac_oggbs*)(pDecodedSamples + decodedSamplesAllocationSize + (seekpointCount * sizeof(drflac_seekpoint)));
        DRFLAC_COPY_MEMORY(pInternalOggbs, pOggbs, sizeof(*pOggbs));

        
        drflac__free_from_callbacks(pOggbs, &allocationCallbacks);
        pOggbs = NULL;

        
        pFlac->bs.onRead = drflac__on_read_ogg;
        pFlac->bs.onSeek = drflac__on_seek_ogg;
        pFlac->bs.onTell = drflac__on_tell_ogg;
        pFlac->bs.pUserData = (void*)pInternalOggbs;
        pFlac->_oggbs = (void*)pInternalOggbs;
    }
#endif

    pFlac->firstFLACFramePosInBytes = firstFramePos;

    
#ifndef DR_FLAC_NO_OGG
    if (init.container == drflac_container_ogg)
    {
        pFlac->pSeekpoints = NULL;
        pFlac->seekpointCount = 0;
    }
    else
#endif
    {
        
        if (seektablePos != 0) {
            pFlac->seekpointCount = seekpointCount;
            pFlac->pSeekpoints = (drflac_seekpoint*)(pDecodedSamples + decodedSamplesAllocationSize);

            DRFLAC_ASSERT(pFlac->bs.onSeek != NULL);
            DRFLAC_ASSERT(pFlac->bs.onRead != NULL);

            
            if (pFlac->bs.onSeek(pFlac->bs.pUserData, (int)seektablePos, DRFLAC_SEEK_SET)) {
                drflac_uint32 iSeekpoint;

                for (iSeekpoint = 0; iSeekpoint < seekpointCount; iSeekpoint += 1) {
                    if (pFlac->bs.onRead(pFlac->bs.pUserData, pFlac->pSeekpoints + iSeekpoint, DRFLAC_SEEKPOINT_SIZE_IN_BYTES) == DRFLAC_SEEKPOINT_SIZE_IN_BYTES) {
                        
                        pFlac->pSeekpoints[iSeekpoint].firstPCMFrame   = drflac__be2host_64(pFlac->pSeekpoints[iSeekpoint].firstPCMFrame);
                        pFlac->pSeekpoints[iSeekpoint].flacFrameOffset = drflac__be2host_64(pFlac->pSeekpoints[iSeekpoint].flacFrameOffset);
                        pFlac->pSeekpoints[iSeekpoint].pcmFrameCount   = drflac__be2host_16(pFlac->pSeekpoints[iSeekpoint].pcmFrameCount);
                    } else {
                        
                        pFlac->pSeekpoints = NULL;
                        pFlac->seekpointCount = 0;
                        break;
                    }
                }

                
                if (!pFlac->bs.onSeek(pFlac->bs.pUserData, (int)pFlac->firstFLACFramePosInBytes, DRFLAC_SEEK_SET)) {
                    drflac__free_from_callbacks(pFlac->pDecodedSamplesAllocation, &allocationCallbacks);
                    drflac__free_from_callbacks(pFlac, &allocationCallbacks);
                    return NULL;
                }
            } else {
                
                pFlac->pSeekpoints = NULL;
                pFlac->seekpointCount = 0;
            }
        }
    }


    


    if (recoverMetadata) {
        
        if (!drflac__seek_to_byte(&pFlac->bs, firstFramePos) || !drflac__read_and_decode_next_flac_frame(pFlac)) {
            drflac__free_from_callbacks(pFlac->pDecodedSamplesAllocation, &allocationCallbacks);
            drflac__free_from_callbacks(pFlac, &allocationCallbacks);
            return NULL;
        }
    } else if (!init.hasStreamInfoBlock) {
        pFlac->currentFLACFrame.header = init.firstFrameHeader;
        for (;;) {
            drflac_result result = drflac__decode_flac_frame(pFlac);
            if (result == DRFLAC_SUCCESS) {
                break;
            } else {
                if (result == DRFLAC_CRC_MISMATCH) {
                    if (!drflac__read_next_flac_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFLACFrame.header)) {
                        drflac__free_from_callbacks(pFlac->pDecodedSamplesAllocation, &allocationCallbacks);
                        drflac__free_from_callbacks(pFlac, &allocationCallbacks);
                        return NULL;
                    }
                    continue;
                } else {
                    drflac__free_from_callbacks(pFlac->pDecodedSamplesAllocation, &allocationCallbacks);
                    drflac__free_from_callbacks(pFlac, &allocationCallbacks);
                    return NULL;
                }
            }
        }
    }

    return pFlac;
}



#ifndef DR_FLAC_NO_STDIO
#include <stdio.h>
#ifndef DR_FLAC_NO_WCHAR
#include <wchar.h>      
#endif



#include <errno.h>
static drflac_result drflac_result_from_errno(int e)
{
    switch (e)
    {
        case 0: return DRFLAC_SUCCESS;
    #ifdef EPERM
        case EPERM: return DRFLAC_INVALID_OPERATION;
    #endif
    #ifdef ENOENT
        case ENOENT: return DRFLAC_DOES_NOT_EXIST;
    #endif
    #ifdef ESRCH
        case ESRCH: return DRFLAC_DOES_NOT_EXIST;
    #endif
    #ifdef EINTR
        case EINTR: return DRFLAC_INTERRUPT;
    #endif
    #ifdef EIO
        case EIO: return DRFLAC_IO_ERROR;
    #endif
    #ifdef ENXIO
        case ENXIO: return DRFLAC_DOES_NOT_EXIST;
    #endif
    #ifdef E2BIG
        case E2BIG: return DRFLAC_INVALID_ARGS;
    #endif
    #ifdef ENOEXEC
        case ENOEXEC: return DRFLAC_INVALID_FILE;
    #endif
    #ifdef EBADF
        case EBADF: return DRFLAC_INVALID_FILE;
    #endif
    #ifdef ECHILD
        case ECHILD: return DRFLAC_ERROR;
    #endif
    #ifdef EAGAIN
        case EAGAIN: return DRFLAC_UNAVAILABLE;
    #endif
    #ifdef ENOMEM
        case ENOMEM: return DRFLAC_OUT_OF_MEMORY;
    #endif
    #ifdef EACCES
        case EACCES: return DRFLAC_ACCESS_DENIED;
    #endif
    #ifdef EFAULT
        case EFAULT: return DRFLAC_BAD_ADDRESS;
    #endif
    #ifdef ENOTBLK
        case ENOTBLK: return DRFLAC_ERROR;
    #endif
    #ifdef EBUSY
        case EBUSY: return DRFLAC_BUSY;
    #endif
    #ifdef EEXIST
        case EEXIST: return DRFLAC_ALREADY_EXISTS;
    #endif
    #ifdef EXDEV
        case EXDEV: return DRFLAC_ERROR;
    #endif
    #ifdef ENODEV
        case ENODEV: return DRFLAC_DOES_NOT_EXIST;
    #endif
    #ifdef ENOTDIR
        case ENOTDIR: return DRFLAC_NOT_DIRECTORY;
    #endif
    #ifdef EISDIR
        case EISDIR: return DRFLAC_IS_DIRECTORY;
    #endif
    #ifdef EINVAL
        case EINVAL: return DRFLAC_INVALID_ARGS;
    #endif
    #ifdef ENFILE
        case ENFILE: return DRFLAC_TOO_MANY_OPEN_FILES;
    #endif
    #ifdef EMFILE
        case EMFILE: return DRFLAC_TOO_MANY_OPEN_FILES;
    #endif
    #ifdef ENOTTY
        case ENOTTY: return DRFLAC_INVALID_OPERATION;
    #endif
    #ifdef ETXTBSY
        case ETXTBSY: return DRFLAC_BUSY;
    #endif
    #ifdef EFBIG
        case EFBIG: return DRFLAC_TOO_BIG;
    #endif
    #ifdef ENOSPC
        case ENOSPC: return DRFLAC_NO_SPACE;
    #endif
    #ifdef ESPIPE
        case ESPIPE: return DRFLAC_BAD_SEEK;
    #endif
    #ifdef EROFS
        case EROFS: return DRFLAC_ACCESS_DENIED;
    #endif
    #ifdef EMLINK
        case EMLINK: return DRFLAC_TOO_MANY_LINKS;
    #endif
    #ifdef EPIPE
        case EPIPE: return DRFLAC_BAD_PIPE;
    #endif
    #ifdef EDOM
        case EDOM: return DRFLAC_OUT_OF_RANGE;
    #endif
    #ifdef ERANGE
        case ERANGE: return DRFLAC_OUT_OF_RANGE;
    #endif
    #ifdef EDEADLK
        case EDEADLK: return DRFLAC_DEADLOCK;
    #endif
    #ifdef ENAMETOOLONG
        case ENAMETOOLONG: return DRFLAC_PATH_TOO_LONG;
    #endif
    #ifdef ENOLCK
        case ENOLCK: return DRFLAC_ERROR;
    #endif
    #ifdef ENOSYS
        case ENOSYS: return DRFLAC_NOT_IMPLEMENTED;
    #endif
    #if defined(ENOTEMPTY) && ENOTEMPTY != EEXIST   
        case ENOTEMPTY: return DRFLAC_DIRECTORY_NOT_EMPTY;
    #endif
    #ifdef ELOOP
        case ELOOP: return DRFLAC_TOO_MANY_LINKS;
    #endif
    #ifdef ENOMSG
        case ENOMSG: return DRFLAC_NO_MESSAGE;
    #endif
    #ifdef EIDRM
        case EIDRM: return DRFLAC_ERROR;
    #endif
    #ifdef ECHRNG
        case ECHRNG: return DRFLAC_ERROR;
    #endif
    #ifdef EL2NSYNC
        case EL2NSYNC: return DRFLAC_ERROR;
    #endif
    #ifdef EL3HLT
        case EL3HLT: return DRFLAC_ERROR;
    #endif
    #ifdef EL3RST
        case EL3RST: return DRFLAC_ERROR;
    #endif
    #ifdef ELNRNG
        case ELNRNG: return DRFLAC_OUT_OF_RANGE;
    #endif
    #ifdef EUNATCH
        case EUNATCH: return DRFLAC_ERROR;
    #endif
    #ifdef ENOCSI
        case ENOCSI: return DRFLAC_ERROR;
    #endif
    #ifdef EL2HLT
        case EL2HLT: return DRFLAC_ERROR;
    #endif
    #ifdef EBADE
        case EBADE: return DRFLAC_ERROR;
    #endif
    #ifdef EBADR
        case EBADR: return DRFLAC_ERROR;
    #endif
    #ifdef EXFULL
        case EXFULL: return DRFLAC_ERROR;
    #endif
    #ifdef ENOANO
        case ENOANO: return DRFLAC_ERROR;
    #endif
    #ifdef EBADRQC
        case EBADRQC: return DRFLAC_ERROR;
    #endif
    #ifdef EBADSLT
        case EBADSLT: return DRFLAC_ERROR;
    #endif
    #ifdef EBFONT
        case EBFONT: return DRFLAC_INVALID_FILE;
    #endif
    #ifdef ENOSTR
        case ENOSTR: return DRFLAC_ERROR;
    #endif
    #ifdef ENODATA
        case ENODATA: return DRFLAC_NO_DATA_AVAILABLE;
    #endif
    #ifdef ETIME
        case ETIME: return DRFLAC_TIMEOUT;
    #endif
    #ifdef ENOSR
        case ENOSR: return DRFLAC_NO_DATA_AVAILABLE;
    #endif
    #ifdef ENONET
        case ENONET: return DRFLAC_NO_NETWORK;
    #endif
    #ifdef ENOPKG
        case ENOPKG: return DRFLAC_ERROR;
    #endif
    #ifdef EREMOTE
        case EREMOTE: return DRFLAC_ERROR;
    #endif
    #ifdef ENOLINK
        case ENOLINK: return DRFLAC_ERROR;
    #endif
    #ifdef EADV
        case EADV: return DRFLAC_ERROR;
    #endif
    #ifdef ESRMNT
        case ESRMNT: return DRFLAC_ERROR;
    #endif
    #ifdef ECOMM
        case ECOMM: return DRFLAC_ERROR;
    #endif
    #ifdef EPROTO
        case EPROTO: return DRFLAC_ERROR;
    #endif
    #ifdef EMULTIHOP
        case EMULTIHOP: return DRFLAC_ERROR;
    #endif
    #ifdef EDOTDOT
        case EDOTDOT: return DRFLAC_ERROR;
    #endif
    #ifdef EBADMSG
        case EBADMSG: return DRFLAC_BAD_MESSAGE;
    #endif
    #ifdef EOVERFLOW
        case EOVERFLOW: return DRFLAC_TOO_BIG;
    #endif
    #ifdef ENOTUNIQ
        case ENOTUNIQ: return DRFLAC_NOT_UNIQUE;
    #endif
    #ifdef EBADFD
        case EBADFD: return DRFLAC_ERROR;
    #endif
    #ifdef EREMCHG
        case EREMCHG: return DRFLAC_ERROR;
    #endif
    #ifdef ELIBACC
        case ELIBACC: return DRFLAC_ACCESS_DENIED;
    #endif
    #ifdef ELIBBAD
        case ELIBBAD: return DRFLAC_INVALID_FILE;
    #endif
    #ifdef ELIBSCN
        case ELIBSCN: return DRFLAC_INVALID_FILE;
    #endif
    #ifdef ELIBMAX
        case ELIBMAX: return DRFLAC_ERROR;
    #endif
    #ifdef ELIBEXEC
        case ELIBEXEC: return DRFLAC_ERROR;
    #endif
    #ifdef EILSEQ
        case EILSEQ: return DRFLAC_INVALID_DATA;
    #endif
    #ifdef ERESTART
        case ERESTART: return DRFLAC_ERROR;
    #endif
    #ifdef ESTRPIPE
        case ESTRPIPE: return DRFLAC_ERROR;
    #endif
    #ifdef EUSERS
        case EUSERS: return DRFLAC_ERROR;
    #endif
    #ifdef ENOTSOCK
        case ENOTSOCK: return DRFLAC_NOT_SOCKET;
    #endif
    #ifdef EDESTADDRREQ
        case EDESTADDRREQ: return DRFLAC_NO_ADDRESS;
    #endif
    #ifdef EMSGSIZE
        case EMSGSIZE: return DRFLAC_TOO_BIG;
    #endif
    #ifdef EPROTOTYPE
        case EPROTOTYPE: return DRFLAC_BAD_PROTOCOL;
    #endif
    #ifdef ENOPROTOOPT
        case ENOPROTOOPT: return DRFLAC_PROTOCOL_UNAVAILABLE;
    #endif
    #ifdef EPROTONOSUPPORT
        case EPROTONOSUPPORT: return DRFLAC_PROTOCOL_NOT_SUPPORTED;
    #endif
    #ifdef ESOCKTNOSUPPORT
        case ESOCKTNOSUPPORT: return DRFLAC_SOCKET_NOT_SUPPORTED;
    #endif
    #ifdef EOPNOTSUPP
        case EOPNOTSUPP: return DRFLAC_INVALID_OPERATION;
    #endif
    #ifdef EPFNOSUPPORT
        case EPFNOSUPPORT: return DRFLAC_PROTOCOL_FAMILY_NOT_SUPPORTED;
    #endif
    #ifdef EAFNOSUPPORT
        case EAFNOSUPPORT: return DRFLAC_ADDRESS_FAMILY_NOT_SUPPORTED;
    #endif
    #ifdef EADDRINUSE
        case EADDRINUSE: return DRFLAC_ALREADY_IN_USE;
    #endif
    #ifdef EADDRNOTAVAIL
        case EADDRNOTAVAIL: return DRFLAC_ERROR;
    #endif
    #ifdef ENETDOWN
        case ENETDOWN: return DRFLAC_NO_NETWORK;
    #endif
    #ifdef ENETUNREACH
        case ENETUNREACH: return DRFLAC_NO_NETWORK;
    #endif
    #ifdef ENETRESET
        case ENETRESET: return DRFLAC_NO_NETWORK;
    #endif
    #ifdef ECONNABORTED
        case ECONNABORTED: return DRFLAC_NO_NETWORK;
    #endif
    #ifdef ECONNRESET
        case ECONNRESET: return DRFLAC_CONNECTION_RESET;
    #endif
    #ifdef ENOBUFS
        case ENOBUFS: return DRFLAC_NO_SPACE;
    #endif
    #ifdef EISCONN
        case EISCONN: return DRFLAC_ALREADY_CONNECTED;
    #endif
    #ifdef ENOTCONN
        case ENOTCONN: return DRFLAC_NOT_CONNECTED;
    #endif
    #ifdef ESHUTDOWN
        case ESHUTDOWN: return DRFLAC_ERROR;
    #endif
    #ifdef ETOOMANYREFS
        case ETOOMANYREFS: return DRFLAC_ERROR;
    #endif
    #ifdef ETIMEDOUT
        case ETIMEDOUT: return DRFLAC_TIMEOUT;
    #endif
    #ifdef ECONNREFUSED
        case ECONNREFUSED: return DRFLAC_CONNECTION_REFUSED;
    #endif
    #ifdef EHOSTDOWN
        case EHOSTDOWN: return DRFLAC_NO_HOST;
    #endif
    #ifdef EHOSTUNREACH
        case EHOSTUNREACH: return DRFLAC_NO_HOST;
    #endif
    #ifdef EALREADY
        case EALREADY: return DRFLAC_IN_PROGRESS;
    #endif
    #ifdef EINPROGRESS
        case EINPROGRESS: return DRFLAC_IN_PROGRESS;
    #endif
    #ifdef ESTALE
        case ESTALE: return DRFLAC_INVALID_FILE;
    #endif
    #ifdef EUCLEAN
        case EUCLEAN: return DRFLAC_ERROR;
    #endif
    #ifdef ENOTNAM
        case ENOTNAM: return DRFLAC_ERROR;
    #endif
    #ifdef ENAVAIL
        case ENAVAIL: return DRFLAC_ERROR;
    #endif
    #ifdef EISNAM
        case EISNAM: return DRFLAC_ERROR;
    #endif
    #ifdef EREMOTEIO
        case EREMOTEIO: return DRFLAC_IO_ERROR;
    #endif
    #ifdef EDQUOT
        case EDQUOT: return DRFLAC_NO_SPACE;
    #endif
    #ifdef ENOMEDIUM
        case ENOMEDIUM: return DRFLAC_DOES_NOT_EXIST;
    #endif
    #ifdef EMEDIUMTYPE
        case EMEDIUMTYPE: return DRFLAC_ERROR;
    #endif
    #ifdef ECANCELED
        case ECANCELED: return DRFLAC_CANCELLED;
    #endif
    #ifdef ENOKEY
        case ENOKEY: return DRFLAC_ERROR;
    #endif
    #ifdef EKEYEXPIRED
        case EKEYEXPIRED: return DRFLAC_ERROR;
    #endif
    #ifdef EKEYREVOKED
        case EKEYREVOKED: return DRFLAC_ERROR;
    #endif
    #ifdef EKEYREJECTED
        case EKEYREJECTED: return DRFLAC_ERROR;
    #endif
    #ifdef EOWNERDEAD
        case EOWNERDEAD: return DRFLAC_ERROR;
    #endif
    #ifdef ENOTRECOVERABLE
        case ENOTRECOVERABLE: return DRFLAC_ERROR;
    #endif
    #ifdef ERFKILL
        case ERFKILL: return DRFLAC_ERROR;
    #endif
    #ifdef EHWPOISON
        case EHWPOISON: return DRFLAC_ERROR;
    #endif
        default: return DRFLAC_ERROR;
    }
}



static drflac_result drflac_fopen(FILE** ppFile, const char* pFilePath, const char* pOpenMode)
{
#if defined(_MSC_VER) && _MSC_VER >= 1400
    errno_t err;
#endif

    if (ppFile != NULL) {
        *ppFile = NULL;  
    }

    if (pFilePath == NULL || pOpenMode == NULL || ppFile == NULL) {
        return DRFLAC_INVALID_ARGS;
    }

#if defined(_MSC_VER) && _MSC_VER >= 1400
    err = fopen_s(ppFile, pFilePath, pOpenMode);
    if (err != 0) {
        return drflac_result_from_errno(err);
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
        drflac_result result = drflac_result_from_errno(errno);
        if (result == DRFLAC_SUCCESS) {
            result = DRFLAC_ERROR;   
        }

        return result;
    }
#endif

    return DRFLAC_SUCCESS;
}













#if defined(_WIN32)
    #if defined(_MSC_VER) || defined(__MINGW64__) || (!defined(__STRICT_ANSI__) && !defined(_NO_EXT_KEYS))
        #define DRFLAC_HAS_WFOPEN
    #endif
#endif

#ifndef DR_FLAC_NO_WCHAR
static drflac_result drflac_wfopen(FILE** ppFile, const wchar_t* pFilePath, const wchar_t* pOpenMode, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    if (ppFile != NULL) {
        *ppFile = NULL;  
    }

    if (pFilePath == NULL || pOpenMode == NULL || ppFile == NULL) {
        return DRFLAC_INVALID_ARGS;
    }

#if defined(DRFLAC_HAS_WFOPEN)
    {
        
    #if defined(_MSC_VER) && _MSC_VER >= 1400
        errno_t err = _wfopen_s(ppFile, pFilePath, pOpenMode);
        if (err != 0) {
            return drflac_result_from_errno(err);
        }
    #else
        *ppFile = _wfopen(pFilePath, pOpenMode);
        if (*ppFile == NULL) {
            return drflac_result_from_errno(errno);
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

        
        DRFLAC_ZERO_OBJECT(&mbs);
        lenMB = wcsrtombs(NULL, &pFilePathTemp, 0, &mbs);
        if (lenMB == (size_t)-1) {
            return drflac_result_from_errno(errno);
        }

        pFilePathMB = (char*)drflac__malloc_from_callbacks(lenMB + 1, pAllocationCallbacks);
        if (pFilePathMB == NULL) {
            return DRFLAC_OUT_OF_MEMORY;
        }

        pFilePathTemp = pFilePath;
        DRFLAC_ZERO_OBJECT(&mbs);
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

        drflac__free_from_callbacks(pFilePathMB, pAllocationCallbacks);
    }
	#endif

    if (*ppFile == NULL) {
        return DRFLAC_ERROR;
    }
#endif

    return DRFLAC_SUCCESS;
}
#endif


static size_t drflac__on_read_stdio(void* pUserData, void* bufferOut, size_t bytesToRead)
{
    return fread(bufferOut, 1, bytesToRead, (FILE*)pUserData);
}

static drflac_bool32 drflac__on_seek_stdio(void* pUserData, int offset, drflac_seek_origin origin)
{
    int whence = SEEK_SET;
    if (origin == DRFLAC_SEEK_CUR) {
        whence = SEEK_CUR;
    } else if (origin == DRFLAC_SEEK_END) {
        whence = SEEK_END;
    }

    return fseek((FILE*)pUserData, offset, whence) == 0;
}

static drflac_bool32 drflac__on_tell_stdio(void* pUserData, drflac_int64* pCursor)
{
    FILE* pFileStdio = (FILE*)pUserData;
    drflac_int64 result;

    
    DRFLAC_ASSERT(pFileStdio != NULL);
    DRFLAC_ASSERT(pCursor    != NULL);

#if defined(_WIN32) && !defined(NXDK)
    #if defined(_MSC_VER) && _MSC_VER > 1200
        result = _ftelli64(pFileStdio);
    #else
        result = ftell(pFileStdio);
    #endif
#else
    result = ftell(pFileStdio);
#endif

    *pCursor = result;

    return DRFLAC_TRUE;
}



DRFLAC_API drflac* drflac_open_file(const char* pFileName, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    drflac* pFlac;
    FILE* pFile;

    if (drflac_fopen(&pFile, pFileName, "rb") != DRFLAC_SUCCESS) {
        return NULL;
    }

    pFlac = drflac_open(drflac__on_read_stdio, drflac__on_seek_stdio, drflac__on_tell_stdio, (void*)pFile, pAllocationCallbacks);
    if (pFlac == NULL) {
        fclose(pFile);
        return NULL;
    }

    return pFlac;
}

#ifndef DR_FLAC_NO_WCHAR
DRFLAC_API drflac* drflac_open_file_w(const wchar_t* pFileName, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    drflac* pFlac;
    FILE* pFile;

    if (drflac_wfopen(&pFile, pFileName, L"rb", pAllocationCallbacks) != DRFLAC_SUCCESS) {
        return NULL;
    }

    pFlac = drflac_open(drflac__on_read_stdio, drflac__on_seek_stdio, drflac__on_tell_stdio, (void*)pFile, pAllocationCallbacks);
    if (pFlac == NULL) {
        fclose(pFile);
        return NULL;
    }

    return pFlac;
}
#endif

DRFLAC_API drflac* drflac_open_file_with_metadata(const char* pFileName, drflac_meta_proc onMeta, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    drflac* pFlac;
    FILE* pFile;

    if (drflac_fopen(&pFile, pFileName, "rb") != DRFLAC_SUCCESS) {
        return NULL;
    }

    pFlac = drflac_open_with_metadata_private(drflac__on_read_stdio, drflac__on_seek_stdio, drflac__on_tell_stdio, onMeta, drflac_container_unknown, (void*)pFile, pUserData, pAllocationCallbacks);
    if (pFlac == NULL) {
        fclose(pFile);
        return pFlac;
    }

    return pFlac;
}

#ifndef DR_FLAC_NO_WCHAR
DRFLAC_API drflac* drflac_open_file_with_metadata_w(const wchar_t* pFileName, drflac_meta_proc onMeta, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    drflac* pFlac;
    FILE* pFile;

    if (drflac_wfopen(&pFile, pFileName, L"rb", pAllocationCallbacks) != DRFLAC_SUCCESS) {
        return NULL;
    }

    pFlac = drflac_open_with_metadata_private(drflac__on_read_stdio, drflac__on_seek_stdio, drflac__on_tell_stdio, onMeta, drflac_container_unknown, (void*)pFile, pUserData, pAllocationCallbacks);
    if (pFlac == NULL) {
        fclose(pFile);
        return pFlac;
    }

    return pFlac;
}
#endif
#endif  

static size_t drflac__on_read_memory(void* pUserData, void* bufferOut, size_t bytesToRead)
{
    drflac__memory_stream* memoryStream = (drflac__memory_stream*)pUserData;
    size_t bytesRemaining;

    DRFLAC_ASSERT(memoryStream != NULL);
    DRFLAC_ASSERT(memoryStream->dataSize >= memoryStream->currentReadPos);

    bytesRemaining = memoryStream->dataSize - memoryStream->currentReadPos;
    if (bytesToRead > bytesRemaining) {
        bytesToRead = bytesRemaining;
    }

    if (bytesToRead > 0) {
        DRFLAC_COPY_MEMORY(bufferOut, memoryStream->data + memoryStream->currentReadPos, bytesToRead);
        memoryStream->currentReadPos += bytesToRead;
    }

    return bytesToRead;
}

static drflac_bool32 drflac__on_seek_memory(void* pUserData, int offset, drflac_seek_origin origin)
{
    drflac__memory_stream* memoryStream = (drflac__memory_stream*)pUserData;
    drflac_int64 newCursor;

    DRFLAC_ASSERT(memoryStream != NULL);

    if (origin == DRFLAC_SEEK_SET) {
        newCursor = 0;
    } else if (origin == DRFLAC_SEEK_CUR) {
        newCursor = (drflac_int64)memoryStream->currentReadPos;
    } else if (origin == DRFLAC_SEEK_END) {
        newCursor = (drflac_int64)memoryStream->dataSize;
    } else {
        DRFLAC_ASSERT(!"Invalid seek origin");
        return DRFLAC_FALSE;
    }

    newCursor += offset;

    if (newCursor < 0) {
        return DRFLAC_FALSE;  
    }
    if ((size_t)newCursor > memoryStream->dataSize) {
        return DRFLAC_FALSE;  
    }

    memoryStream->currentReadPos = (size_t)newCursor;

    return DRFLAC_TRUE;
}

static drflac_bool32 drflac__on_tell_memory(void* pUserData, drflac_int64* pCursor)
{
    drflac__memory_stream* memoryStream = (drflac__memory_stream*)pUserData;

    DRFLAC_ASSERT(memoryStream != NULL);
    DRFLAC_ASSERT(pCursor != NULL);

    *pCursor = (drflac_int64)memoryStream->currentReadPos;
    return DRFLAC_TRUE;
}

DRFLAC_API drflac* drflac_open_memory(const void* pData, size_t dataSize, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    drflac__memory_stream memoryStream;
    drflac* pFlac;

    memoryStream.data = (const drflac_uint8*)pData;
    memoryStream.dataSize = dataSize;
    memoryStream.currentReadPos = 0;
    pFlac = drflac_open(drflac__on_read_memory, drflac__on_seek_memory, drflac__on_tell_memory, &memoryStream, pAllocationCallbacks);
    if (pFlac == NULL) {
        return NULL;
    }

    pFlac->memoryStream = memoryStream;

    
#ifndef DR_FLAC_NO_OGG
    if (pFlac->container == drflac_container_ogg)
    {
        drflac_oggbs* oggbs = (drflac_oggbs*)pFlac->_oggbs;
        oggbs->pUserData = &pFlac->memoryStream;
    }
    else
#endif
    {
        pFlac->bs.pUserData = &pFlac->memoryStream;
    }

    return pFlac;
}

DRFLAC_API drflac* drflac_open_memory_with_metadata(const void* pData, size_t dataSize, drflac_meta_proc onMeta, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    drflac__memory_stream memoryStream;
    drflac* pFlac;

    memoryStream.data = (const drflac_uint8*)pData;
    memoryStream.dataSize = dataSize;
    memoryStream.currentReadPos = 0;
    pFlac = drflac_open_with_metadata_private(drflac__on_read_memory, drflac__on_seek_memory, drflac__on_tell_memory, onMeta, drflac_container_unknown, &memoryStream, pUserData, pAllocationCallbacks);
    if (pFlac == NULL) {
        return NULL;
    }

    pFlac->memoryStream = memoryStream;

    
#ifndef DR_FLAC_NO_OGG
    if (pFlac->container == drflac_container_ogg)
    {
        drflac_oggbs* oggbs = (drflac_oggbs*)pFlac->_oggbs;
        oggbs->pUserData = &pFlac->memoryStream;
    }
    else
#endif
    {
        pFlac->bs.pUserData = &pFlac->memoryStream;
    }

    return pFlac;
}



DRFLAC_API drflac* drflac_open(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    return drflac_open_with_metadata_private(onRead, onSeek, onTell, NULL, drflac_container_unknown, pUserData, pUserData, pAllocationCallbacks);
}
DRFLAC_API drflac* drflac_open_relaxed(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, drflac_container container, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    return drflac_open_with_metadata_private(onRead, onSeek, onTell, NULL, container, pUserData, pUserData, pAllocationCallbacks);
}

DRFLAC_API drflac* drflac_open_with_metadata(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, drflac_meta_proc onMeta, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    return drflac_open_with_metadata_private(onRead, onSeek, onTell, onMeta, drflac_container_unknown, pUserData, pUserData, pAllocationCallbacks);
}
DRFLAC_API drflac* drflac_open_with_metadata_relaxed(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, drflac_meta_proc onMeta, drflac_container container, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    return drflac_open_with_metadata_private(onRead, onSeek, onTell, onMeta, container, pUserData, pUserData, pAllocationCallbacks);
}

DRFLAC_API void drflac_close(drflac* pFlac)
{
    if (pFlac == NULL) {
        return;
    }

#ifndef DR_FLAC_NO_STDIO
    



    if (pFlac->bs.onRead == drflac__on_read_stdio) {
        fclose((FILE*)pFlac->bs.pUserData);
    }

#ifndef DR_FLAC_NO_OGG
    
    if (pFlac->container == drflac_container_ogg) {
        drflac_oggbs* oggbs = (drflac_oggbs*)pFlac->_oggbs;
        DRFLAC_ASSERT(pFlac->bs.onRead == drflac__on_read_ogg);

        if (oggbs->onRead == drflac__on_read_stdio) {
            fclose((FILE*)oggbs->pUserData);
        }
    }
#endif
#endif

    drflac__free_from_callbacks(pFlac->pDecodedSamplesAllocation, &pFlac->allocationCallbacks);
    drflac__free_from_callbacks(pFlac, &pFlac->allocationCallbacks);
}


#if 0
static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_left_side__reference(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    drflac_uint64 i;
    for (i = 0; i < frameCount; ++i) {
        drflac_uint32 left  = (drflac_uint32)pInputSamples0[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample);
        drflac_uint32 side  = (drflac_uint32)pInputSamples1[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample);
        drflac_uint32 right = left - side;

        pOutputSamples[i*2+0] = (drflac_int32)left;
        pOutputSamples[i*2+1] = (drflac_int32)right;
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_left_side__scalar(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

    for (i = 0; i < frameCount4; ++i) {
        drflac_uint32 left0 = pInputSamples0U32[i*4+0] << shift0;
        drflac_uint32 left1 = pInputSamples0U32[i*4+1] << shift0;
        drflac_uint32 left2 = pInputSamples0U32[i*4+2] << shift0;
        drflac_uint32 left3 = pInputSamples0U32[i*4+3] << shift0;

        drflac_uint32 side0 = pInputSamples1U32[i*4+0] << shift1;
        drflac_uint32 side1 = pInputSamples1U32[i*4+1] << shift1;
        drflac_uint32 side2 = pInputSamples1U32[i*4+2] << shift1;
        drflac_uint32 side3 = pInputSamples1U32[i*4+3] << shift1;

        drflac_uint32 right0 = left0 - side0;
        drflac_uint32 right1 = left1 - side1;
        drflac_uint32 right2 = left2 - side2;
        drflac_uint32 right3 = left3 - side3;

        pOutputSamples[i*8+0] = (drflac_int32)left0;
        pOutputSamples[i*8+1] = (drflac_int32)right0;
        pOutputSamples[i*8+2] = (drflac_int32)left1;
        pOutputSamples[i*8+3] = (drflac_int32)right1;
        pOutputSamples[i*8+4] = (drflac_int32)left2;
        pOutputSamples[i*8+5] = (drflac_int32)right2;
        pOutputSamples[i*8+6] = (drflac_int32)left3;
        pOutputSamples[i*8+7] = (drflac_int32)right3;
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 left  = pInputSamples0U32[i] << shift0;
        drflac_uint32 side  = pInputSamples1U32[i] << shift1;
        drflac_uint32 right = left - side;

        pOutputSamples[i*2+0] = (drflac_int32)left;
        pOutputSamples[i*2+1] = (drflac_int32)right;
    }
}

#if defined(DRFLAC_SUPPORT_SSE2)
static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_left_side__sse2(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    for (i = 0; i < frameCount4; ++i) {
        __m128i left  = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples0 + i), shift0);
        __m128i side  = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples1 + i), shift1);
        __m128i right = _mm_sub_epi32(left, side);

        _mm_storeu_si128((__m128i*)(pOutputSamples + i*8 + 0), _mm_unpacklo_epi32(left, right));
        _mm_storeu_si128((__m128i*)(pOutputSamples + i*8 + 4), _mm_unpackhi_epi32(left, right));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 left  = pInputSamples0U32[i] << shift0;
        drflac_uint32 side  = pInputSamples1U32[i] << shift1;
        drflac_uint32 right = left - side;

        pOutputSamples[i*2+0] = (drflac_int32)left;
        pOutputSamples[i*2+1] = (drflac_int32)right;
    }
}
#endif

#if defined(DRFLAC_SUPPORT_NEON)
static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_left_side__neon(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
    int32x4_t shift0_4;
    int32x4_t shift1_4;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    shift0_4 = vdupq_n_s32(shift0);
    shift1_4 = vdupq_n_s32(shift1);

    for (i = 0; i < frameCount4; ++i) {
        uint32x4_t left;
        uint32x4_t side;
        uint32x4_t right;

        left  = vshlq_u32(vld1q_u32(pInputSamples0U32 + i*4), shift0_4);
        side  = vshlq_u32(vld1q_u32(pInputSamples1U32 + i*4), shift1_4);
        right = vsubq_u32(left, side);

        drflac__vst2q_u32((drflac_uint32*)pOutputSamples + i*8, vzipq_u32(left, right));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 left  = pInputSamples0U32[i] << shift0;
        drflac_uint32 side  = pInputSamples1U32[i] << shift1;
        drflac_uint32 right = left - side;

        pOutputSamples[i*2+0] = (drflac_int32)left;
        pOutputSamples[i*2+1] = (drflac_int32)right;
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_duplicate_mono(drflac_uint64 frameCount, drflac_uint32 shift, const drflac_int32* pInputSamples, drflac_int32* pOutputSamples)
{
    drflac_uint64 i;
    const drflac_uint32* pInputSamplesU32 = (const drflac_uint32*)pInputSamples;

    DRFLAC_ASSERT(shift < 32);

    for (i = 0; i < frameCount; ++i) {
        drflac_int32 sample;

        sample = (drflac_int32)(pInputSamplesU32[i] << shift);
        pOutputSamples[i*2+0] = sample;
        pOutputSamples[i*2+1] = sample;
    }
}

static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_left_side(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    if (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample == 32) {
        drflac_read_pcm_frames_s32__decode_duplicate_mono(frameCount, unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample, pInputSamples0, pOutputSamples);
        return;
    }

#if defined(DRFLAC_SUPPORT_SSE2)
    if (drflac__gIsSSE2Supported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_s32__decode_left_side__sse2(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#elif defined(DRFLAC_SUPPORT_NEON)
    if (drflac__gIsNEONSupported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_s32__decode_left_side__neon(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#endif
    {
        
#if 0
        drflac_read_pcm_frames_s32__decode_left_side__reference(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#else
        drflac_read_pcm_frames_s32__decode_left_side__scalar(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#endif
    }
}


#if 0
static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_right_side__reference(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    drflac_uint64 i;
    for (i = 0; i < frameCount; ++i) {
        drflac_uint32 side  = (drflac_uint32)pInputSamples0[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample);
        drflac_uint32 right = (drflac_uint32)pInputSamples1[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample);
        drflac_uint32 left  = right + side;

        pOutputSamples[i*2+0] = (drflac_int32)left;
        pOutputSamples[i*2+1] = (drflac_int32)right;
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_right_side__scalar(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

    for (i = 0; i < frameCount4; ++i) {
        drflac_uint32 side0  = pInputSamples0U32[i*4+0] << shift0;
        drflac_uint32 side1  = pInputSamples0U32[i*4+1] << shift0;
        drflac_uint32 side2  = pInputSamples0U32[i*4+2] << shift0;
        drflac_uint32 side3  = pInputSamples0U32[i*4+3] << shift0;

        drflac_uint32 right0 = pInputSamples1U32[i*4+0] << shift1;
        drflac_uint32 right1 = pInputSamples1U32[i*4+1] << shift1;
        drflac_uint32 right2 = pInputSamples1U32[i*4+2] << shift1;
        drflac_uint32 right3 = pInputSamples1U32[i*4+3] << shift1;

        drflac_uint32 left0 = right0 + side0;
        drflac_uint32 left1 = right1 + side1;
        drflac_uint32 left2 = right2 + side2;
        drflac_uint32 left3 = right3 + side3;

        pOutputSamples[i*8+0] = (drflac_int32)left0;
        pOutputSamples[i*8+1] = (drflac_int32)right0;
        pOutputSamples[i*8+2] = (drflac_int32)left1;
        pOutputSamples[i*8+3] = (drflac_int32)right1;
        pOutputSamples[i*8+4] = (drflac_int32)left2;
        pOutputSamples[i*8+5] = (drflac_int32)right2;
        pOutputSamples[i*8+6] = (drflac_int32)left3;
        pOutputSamples[i*8+7] = (drflac_int32)right3;
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 side  = pInputSamples0U32[i] << shift0;
        drflac_uint32 right = pInputSamples1U32[i] << shift1;
        drflac_uint32 left  = right + side;

        pOutputSamples[i*2+0] = (drflac_int32)left;
        pOutputSamples[i*2+1] = (drflac_int32)right;
    }
}

#if defined(DRFLAC_SUPPORT_SSE2)
static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_right_side__sse2(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    for (i = 0; i < frameCount4; ++i) {
        __m128i side  = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples0 + i), shift0);
        __m128i right = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples1 + i), shift1);
        __m128i left  = _mm_add_epi32(right, side);

        _mm_storeu_si128((__m128i*)(pOutputSamples + i*8 + 0), _mm_unpacklo_epi32(left, right));
        _mm_storeu_si128((__m128i*)(pOutputSamples + i*8 + 4), _mm_unpackhi_epi32(left, right));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 side  = pInputSamples0U32[i] << shift0;
        drflac_uint32 right = pInputSamples1U32[i] << shift1;
        drflac_uint32 left  = right + side;

        pOutputSamples[i*2+0] = (drflac_int32)left;
        pOutputSamples[i*2+1] = (drflac_int32)right;
    }
}
#endif

#if defined(DRFLAC_SUPPORT_NEON)
static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_right_side__neon(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
    int32x4_t shift0_4;
    int32x4_t shift1_4;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    shift0_4 = vdupq_n_s32(shift0);
    shift1_4 = vdupq_n_s32(shift1);

    for (i = 0; i < frameCount4; ++i) {
        uint32x4_t side;
        uint32x4_t right;
        uint32x4_t left;

        side  = vshlq_u32(vld1q_u32(pInputSamples0U32 + i*4), shift0_4);
        right = vshlq_u32(vld1q_u32(pInputSamples1U32 + i*4), shift1_4);
        left  = vaddq_u32(right, side);

        drflac__vst2q_u32((drflac_uint32*)pOutputSamples + i*8, vzipq_u32(left, right));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 side  = pInputSamples0U32[i] << shift0;
        drflac_uint32 right = pInputSamples1U32[i] << shift1;
        drflac_uint32 left  = right + side;

        pOutputSamples[i*2+0] = (drflac_int32)left;
        pOutputSamples[i*2+1] = (drflac_int32)right;
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_right_side(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    if (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample == 32) {
        drflac_read_pcm_frames_s32__decode_duplicate_mono(frameCount, unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample, pInputSamples1, pOutputSamples);
        return;
    }

#if defined(DRFLAC_SUPPORT_SSE2)
    if (drflac__gIsSSE2Supported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_s32__decode_right_side__sse2(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#elif defined(DRFLAC_SUPPORT_NEON)
    if (drflac__gIsNEONSupported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_s32__decode_right_side__neon(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#endif
    {
        
#if 0
        drflac_read_pcm_frames_s32__decode_right_side__reference(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#else
        drflac_read_pcm_frames_s32__decode_right_side__scalar(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#endif
    }
}


#if 0
static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_mid_side__reference(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    for (drflac_uint64 i = 0; i < frameCount; ++i) {
        drflac_uint32 mid  = pInputSamples0U32[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
        drflac_uint32 side = pInputSamples1U32[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

        mid = (mid << 1) | (side & 0x01);

        pOutputSamples[i*2+0] = (drflac_int32)((drflac_uint32)((drflac_int32)(mid + side) >> 1) << unusedBitsPerSample);
        pOutputSamples[i*2+1] = (drflac_int32)((drflac_uint32)((drflac_int32)(mid - side) >> 1) << unusedBitsPerSample);
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_mid_side__scalar(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_int32 shift = unusedBitsPerSample;

    if (shift > 0) {
        for (i = 0; i < frameCount4; ++i) {
            drflac_uint32 temp0L;
            drflac_uint32 temp1L;
            drflac_uint32 temp2L;
            drflac_uint32 temp3L;
            drflac_uint32 temp0R;
            drflac_uint32 temp1R;
            drflac_uint32 temp2R;
            drflac_uint32 temp3R;

            drflac_uint32 mid0  = pInputSamples0U32[i*4+0] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 mid1  = pInputSamples0U32[i*4+1] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 mid2  = pInputSamples0U32[i*4+2] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 mid3  = pInputSamples0U32[i*4+3] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;

            drflac_uint32 side0 = pInputSamples1U32[i*4+0] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint32 side1 = pInputSamples1U32[i*4+1] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint32 side2 = pInputSamples1U32[i*4+2] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint32 side3 = pInputSamples1U32[i*4+3] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid0 = (mid0 << 1) | (side0 & 0x01);
            mid1 = (mid1 << 1) | (side1 & 0x01);
            mid2 = (mid2 << 1) | (side2 & 0x01);
            mid3 = (mid3 << 1) | (side3 & 0x01);

            temp0L = (mid0 + side0) << (shift - 1);
            temp1L = (mid1 + side1) << (shift - 1);
            temp2L = (mid2 + side2) << (shift - 1);
            temp3L = (mid3 + side3) << (shift - 1);

            temp0R = (mid0 - side0) << (shift - 1);
            temp1R = (mid1 - side1) << (shift - 1);
            temp2R = (mid2 - side2) << (shift - 1);
            temp3R = (mid3 - side3) << (shift - 1);

            pOutputSamples[i*8+0] = (drflac_int32)temp0L;
            pOutputSamples[i*8+1] = (drflac_int32)temp0R;
            pOutputSamples[i*8+2] = (drflac_int32)temp1L;
            pOutputSamples[i*8+3] = (drflac_int32)temp1R;
            pOutputSamples[i*8+4] = (drflac_int32)temp2L;
            pOutputSamples[i*8+5] = (drflac_int32)temp2R;
            pOutputSamples[i*8+6] = (drflac_int32)temp3L;
            pOutputSamples[i*8+7] = (drflac_int32)temp3R;
        }
    } else {
        
        for (i = 0; i < frameCount4; ++i) {
            drflac_uint32 temp0L;
            drflac_uint32 temp1L;
            drflac_uint32 temp2L;
            drflac_uint32 temp3L;
            drflac_uint32 temp0R;
            drflac_uint32 temp1R;
            drflac_uint32 temp2R;
            drflac_uint32 temp3R;

            drflac_uint64 mid0  = (drflac_uint64)(drflac_int64)pInputSamples0[i*4+0] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint64 mid1  = (drflac_uint64)(drflac_int64)pInputSamples0[i*4+1] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint64 mid2  = (drflac_uint64)(drflac_int64)pInputSamples0[i*4+2] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint64 mid3  = (drflac_uint64)(drflac_int64)pInputSamples0[i*4+3] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;

            drflac_uint64 side0 = (drflac_uint64)(drflac_int64)pInputSamples1[i*4+0] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint64 side1 = (drflac_uint64)(drflac_int64)pInputSamples1[i*4+1] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint64 side2 = (drflac_uint64)(drflac_int64)pInputSamples1[i*4+2] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint64 side3 = (drflac_uint64)(drflac_int64)pInputSamples1[i*4+3] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid0 = (mid0 << 1) | (side0 & 0x01);
            mid1 = (mid1 << 1) | (side1 & 0x01);
            mid2 = (mid2 << 1) | (side2 & 0x01);
            mid3 = (mid3 << 1) | (side3 & 0x01);

            temp0L = (drflac_uint32)((drflac_int64)(mid0 + side0) >> 1);
            temp1L = (drflac_uint32)((drflac_int64)(mid1 + side1) >> 1);
            temp2L = (drflac_uint32)((drflac_int64)(mid2 + side2) >> 1);
            temp3L = (drflac_uint32)((drflac_int64)(mid3 + side3) >> 1);

            temp0R = (drflac_uint32)((drflac_int64)(mid0 - side0) >> 1);
            temp1R = (drflac_uint32)((drflac_int64)(mid1 - side1) >> 1);
            temp2R = (drflac_uint32)((drflac_int64)(mid2 - side2) >> 1);
            temp3R = (drflac_uint32)((drflac_int64)(mid3 - side3) >> 1);

            pOutputSamples[i*8+0] = (drflac_int32)temp0L;
            pOutputSamples[i*8+1] = (drflac_int32)temp0R;
            pOutputSamples[i*8+2] = (drflac_int32)temp1L;
            pOutputSamples[i*8+3] = (drflac_int32)temp1R;
            pOutputSamples[i*8+4] = (drflac_int32)temp2L;
            pOutputSamples[i*8+5] = (drflac_int32)temp2R;
            pOutputSamples[i*8+6] = (drflac_int32)temp3L;
            pOutputSamples[i*8+7] = (drflac_int32)temp3R;
        }
    }

    if (shift > 0) {
        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint32 mid  = pInputSamples0U32[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 side = pInputSamples1U32[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = (drflac_int32)((drflac_uint32)((drflac_int32)(mid + side) >> 1) << unusedBitsPerSample);
            pOutputSamples[i*2+1] = (drflac_int32)((drflac_uint32)((drflac_int32)(mid - side) >> 1) << unusedBitsPerSample);
        }
    } else {
        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint64 mid;
            drflac_uint64 side;

            mid  = (drflac_uint64)(drflac_int64)pInputSamples0[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            side = (drflac_uint64)(drflac_int64)pInputSamples1[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = (drflac_int32)((drflac_int64)(mid + side) >> 1);
            pOutputSamples[i*2+1] = (drflac_int32)((drflac_int64)(mid - side) >> 1);
        }
    }
}

#if defined(DRFLAC_SUPPORT_SSE2)
static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_mid_side__sse2(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_int32 shift = unusedBitsPerSample;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    if (shift == 0) {
        for (i = 0; i < frameCount4; ++i) {
            __m128i mid;
            __m128i side;
            __m128i left;
            __m128i right;

            mid   = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples0 + i), pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample);
            side  = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples1 + i), pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample);

            mid   = _mm_or_si128(_mm_slli_epi32(mid, 1), _mm_and_si128(side, _mm_set1_epi32(0x01)));

            left  = _mm_srai_epi32(_mm_add_epi32(mid, side), 1);
            right = _mm_srai_epi32(_mm_sub_epi32(mid, side), 1);

            _mm_storeu_si128((__m128i*)(pOutputSamples + i*8 + 0), _mm_unpacklo_epi32(left, right));
            _mm_storeu_si128((__m128i*)(pOutputSamples + i*8 + 4), _mm_unpackhi_epi32(left, right));
        }

        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint32 mid  = pInputSamples0U32[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 side = pInputSamples1U32[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = (drflac_int32)(mid + side) >> 1;
            pOutputSamples[i*2+1] = (drflac_int32)(mid - side) >> 1;
        }
    } else {
        shift -= 1;
        for (i = 0; i < frameCount4; ++i) {
            __m128i mid;
            __m128i side;
            __m128i left;
            __m128i right;

            mid   = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples0 + i), pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample);
            side  = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples1 + i), pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample);

            mid   = _mm_or_si128(_mm_slli_epi32(mid, 1), _mm_and_si128(side, _mm_set1_epi32(0x01)));

            left  = _mm_slli_epi32(_mm_add_epi32(mid, side), shift);
            right = _mm_slli_epi32(_mm_sub_epi32(mid, side), shift);

            _mm_storeu_si128((__m128i*)(pOutputSamples + i*8 + 0), _mm_unpacklo_epi32(left, right));
            _mm_storeu_si128((__m128i*)(pOutputSamples + i*8 + 4), _mm_unpackhi_epi32(left, right));
        }

        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint32 mid  = pInputSamples0U32[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 side = pInputSamples1U32[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = (drflac_int32)((mid + side) << shift);
            pOutputSamples[i*2+1] = (drflac_int32)((mid - side) << shift);
        }
    }
}
#endif

#if defined(DRFLAC_SUPPORT_NEON)
static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_mid_side__neon(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_int32 shift = unusedBitsPerSample;
    int32x4_t  wbpsShift0_4; 
    int32x4_t  wbpsShift1_4; 
    uint32x4_t one4;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    wbpsShift0_4 = vdupq_n_s32(pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample);
    wbpsShift1_4 = vdupq_n_s32(pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample);
    one4         = vdupq_n_u32(1);

    if (shift == 0) {
        for (i = 0; i < frameCount4; ++i) {
            uint32x4_t mid;
            uint32x4_t side;
            int32x4_t left;
            int32x4_t right;

            mid   = vshlq_u32(vld1q_u32(pInputSamples0U32 + i*4), wbpsShift0_4);
            side  = vshlq_u32(vld1q_u32(pInputSamples1U32 + i*4), wbpsShift1_4);

            mid   = vorrq_u32(vshlq_n_u32(mid, 1), vandq_u32(side, one4));

            left  = vshrq_n_s32(vreinterpretq_s32_u32(vaddq_u32(mid, side)), 1);
            right = vshrq_n_s32(vreinterpretq_s32_u32(vsubq_u32(mid, side)), 1);

            drflac__vst2q_s32(pOutputSamples + i*8, vzipq_s32(left, right));
        }

        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint32 mid  = pInputSamples0U32[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 side = pInputSamples1U32[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = (drflac_int32)(mid + side) >> 1;
            pOutputSamples[i*2+1] = (drflac_int32)(mid - side) >> 1;
        }
    } else {
        int32x4_t shift4;

        shift -= 1;
        shift4 = vdupq_n_s32(shift);

        for (i = 0; i < frameCount4; ++i) {
            uint32x4_t mid;
            uint32x4_t side;
            int32x4_t left;
            int32x4_t right;

            mid   = vshlq_u32(vld1q_u32(pInputSamples0U32 + i*4), wbpsShift0_4);
            side  = vshlq_u32(vld1q_u32(pInputSamples1U32 + i*4), wbpsShift1_4);

            mid   = vorrq_u32(vshlq_n_u32(mid, 1), vandq_u32(side, one4));

            left  = vreinterpretq_s32_u32(vshlq_u32(vaddq_u32(mid, side), shift4));
            right = vreinterpretq_s32_u32(vshlq_u32(vsubq_u32(mid, side), shift4));

            drflac__vst2q_s32(pOutputSamples + i*8, vzipq_s32(left, right));
        }

        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint32 mid  = pInputSamples0U32[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 side = pInputSamples1U32[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = (drflac_int32)((mid + side) << shift);
            pOutputSamples[i*2+1] = (drflac_int32)((mid - side) << shift);
        }
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_mid_side(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
#if defined(DRFLAC_SUPPORT_SSE2)
    if (drflac__gIsSSE2Supported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_s32__decode_mid_side__sse2(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#elif defined(DRFLAC_SUPPORT_NEON)
    if (drflac__gIsNEONSupported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_s32__decode_mid_side__neon(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#endif
    {
        
#if 0
        drflac_read_pcm_frames_s32__decode_mid_side__reference(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#else
        drflac_read_pcm_frames_s32__decode_mid_side__scalar(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#endif
    }
}


#if 0
static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_independent_stereo__reference(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    for (drflac_uint64 i = 0; i < frameCount; ++i) {
        pOutputSamples[i*2+0] = (drflac_int32)((drflac_uint32)pInputSamples0[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample));
        pOutputSamples[i*2+1] = (drflac_int32)((drflac_uint32)pInputSamples1[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample));
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_independent_stereo__scalar(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

    for (i = 0; i < frameCount4; ++i) {
        drflac_uint32 tempL0 = pInputSamples0U32[i*4+0] << shift0;
        drflac_uint32 tempL1 = pInputSamples0U32[i*4+1] << shift0;
        drflac_uint32 tempL2 = pInputSamples0U32[i*4+2] << shift0;
        drflac_uint32 tempL3 = pInputSamples0U32[i*4+3] << shift0;

        drflac_uint32 tempR0 = pInputSamples1U32[i*4+0] << shift1;
        drflac_uint32 tempR1 = pInputSamples1U32[i*4+1] << shift1;
        drflac_uint32 tempR2 = pInputSamples1U32[i*4+2] << shift1;
        drflac_uint32 tempR3 = pInputSamples1U32[i*4+3] << shift1;

        pOutputSamples[i*8+0] = (drflac_int32)tempL0;
        pOutputSamples[i*8+1] = (drflac_int32)tempR0;
        pOutputSamples[i*8+2] = (drflac_int32)tempL1;
        pOutputSamples[i*8+3] = (drflac_int32)tempR1;
        pOutputSamples[i*8+4] = (drflac_int32)tempL2;
        pOutputSamples[i*8+5] = (drflac_int32)tempR2;
        pOutputSamples[i*8+6] = (drflac_int32)tempL3;
        pOutputSamples[i*8+7] = (drflac_int32)tempR3;
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        pOutputSamples[i*2+0] = (drflac_int32)(pInputSamples0U32[i] << shift0);
        pOutputSamples[i*2+1] = (drflac_int32)(pInputSamples1U32[i] << shift1);
    }
}

#if defined(DRFLAC_SUPPORT_SSE2)
static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_independent_stereo__sse2(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

    for (i = 0; i < frameCount4; ++i) {
        __m128i left  = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples0 + i), shift0);
        __m128i right = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples1 + i), shift1);

        _mm_storeu_si128((__m128i*)(pOutputSamples + i*8 + 0), _mm_unpacklo_epi32(left, right));
        _mm_storeu_si128((__m128i*)(pOutputSamples + i*8 + 4), _mm_unpackhi_epi32(left, right));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        pOutputSamples[i*2+0] = (drflac_int32)(pInputSamples0U32[i] << shift0);
        pOutputSamples[i*2+1] = (drflac_int32)(pInputSamples1U32[i] << shift1);
    }
}
#endif

#if defined(DRFLAC_SUPPORT_NEON)
static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_independent_stereo__neon(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

    int32x4_t shift4_0 = vdupq_n_s32(shift0);
    int32x4_t shift4_1 = vdupq_n_s32(shift1);

    for (i = 0; i < frameCount4; ++i) {
        int32x4_t left;
        int32x4_t right;

        left  = vreinterpretq_s32_u32(vshlq_u32(vld1q_u32(pInputSamples0U32 + i*4), shift4_0));
        right = vreinterpretq_s32_u32(vshlq_u32(vld1q_u32(pInputSamples1U32 + i*4), shift4_1));

        drflac__vst2q_s32(pOutputSamples + i*8, vzipq_s32(left, right));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        pOutputSamples[i*2+0] = (drflac_int32)(pInputSamples0U32[i] << shift0);
        pOutputSamples[i*2+1] = (drflac_int32)(pInputSamples1U32[i] << shift1);
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode_independent_stereo(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int32* pOutputSamples)
{
#if defined(DRFLAC_SUPPORT_SSE2)
    if (drflac__gIsSSE2Supported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_s32__decode_independent_stereo__sse2(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#elif defined(DRFLAC_SUPPORT_NEON)
    if (drflac__gIsNEONSupported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_s32__decode_independent_stereo__neon(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#endif
    {
        
#if 0
        drflac_read_pcm_frames_s32__decode_independent_stereo__reference(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#else
        drflac_read_pcm_frames_s32__decode_independent_stereo__scalar(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#endif
    }
}


static DRFLAC_INLINE void drflac__decode_stereo_sample_s64(const drflac_frame* pFrame, drflac_uint64 iPCMFrame, drflac_int32* pLeft, drflac_int32* pRight)
{
    drflac_uint64 sample0;
    drflac_uint64 sample1;

    sample0 = (drflac_uint64)pFrame->subframes[0].samples.pS64[iPCMFrame] << pFrame->subframes[0].wastedBitsPerSample;
    sample1 = (drflac_uint64)pFrame->subframes[1].samples.pS64[iPCMFrame] << pFrame->subframes[1].wastedBitsPerSample;

    switch (pFrame->header.channelAssignment)
    {
        case DRFLAC_CHANNEL_ASSIGNMENT_LEFT_SIDE:
        {
            *pLeft  = (drflac_int32)sample0;
            *pRight = (drflac_int32)(sample0 - sample1);
        } break;

        case DRFLAC_CHANNEL_ASSIGNMENT_RIGHT_SIDE:
        {
            *pLeft  = (drflac_int32)(sample0 + sample1);
            *pRight = (drflac_int32)sample1;
        } break;

        case DRFLAC_CHANNEL_ASSIGNMENT_MID_SIDE:
        {
            sample0 = (sample0 << 1) | (sample1 & 0x01);
            *pLeft  = (drflac_int32)((drflac_int64)(sample0 + sample1) >> 1);
            *pRight = (drflac_int32)((drflac_int64)(sample0 - sample1) >> 1);
        } break;

        case DRFLAC_CHANNEL_ASSIGNMENT_INDEPENDENT_STEREO:
        {
            *pLeft  = (drflac_int32)sample0;
            *pRight = (drflac_int32)sample1;
        } break;

        default:
        {
            DRFLAC_ASSERT(!"Invalid stereo channel assignment");
            
            *pLeft  = 0;
            *pRight = 0;
        } break;
    }
}

static void drflac_read_pcm_frames_s32__decode_s64(drflac* pFlac, drflac_uint64 frameCount, drflac_uint64 iFirstPCMFrame, drflac_int32* pOutputSamples)
{
    drflac_uint64 i;

    DRFLAC_ASSERT(pFlac->bitsPerSample == 32);

    if (pFlac->channels == 2) {
        for (i = 0; i < frameCount; ++i) {
            drflac_int32 left;
            drflac_int32 right;

            drflac__decode_stereo_sample_s64(&pFlac->currentFLACFrame, iFirstPCMFrame + i, &left, &right);

            pOutputSamples[i*2+0] = left;
            pOutputSamples[i*2+1] = right;
        }
    } else {
        for (i = 0; i < frameCount; ++i) {
            unsigned int j;
            for (j = 0; j < pFlac->channels; ++j) {
                drflac_int32 sample;

                sample = (drflac_int32)((drflac_uint64)pFlac->currentFLACFrame.subframes[j].samples.pS64[iFirstPCMFrame + i] << pFlac->currentFLACFrame.subframes[j].wastedBitsPerSample);
                pOutputSamples[i*pFlac->channels+j] = sample;
            }
        }
    }
}

static void drflac_read_pcm_frames_s16__decode_s64(drflac* pFlac, drflac_uint64 frameCount, drflac_uint64 iFirstPCMFrame, drflac_int16* pOutputSamples)
{
    drflac_uint64 i;

    DRFLAC_ASSERT(pFlac->bitsPerSample == 32);

    if (pFlac->channels == 2) {
        for (i = 0; i < frameCount; ++i) {
            drflac_int32 left;
            drflac_int32 right;

            drflac__decode_stereo_sample_s64(&pFlac->currentFLACFrame, iFirstPCMFrame + i, &left, &right);

            pOutputSamples[i*2+0] = (drflac_int16)(left  >> 16);
            pOutputSamples[i*2+1] = (drflac_int16)(right >> 16);
        }
    } else {
        for (i = 0; i < frameCount; ++i) {
            unsigned int j;
            for (j = 0; j < pFlac->channels; ++j) {
                drflac_int32 sample;

                sample = (drflac_int32)((drflac_uint64)pFlac->currentFLACFrame.subframes[j].samples.pS64[iFirstPCMFrame + i] << pFlac->currentFLACFrame.subframes[j].wastedBitsPerSample);
                pOutputSamples[i*pFlac->channels+j] = (drflac_int16)(sample >> 16);
            }
        }
    }
}

static void drflac_read_pcm_frames_f32__decode_s64(drflac* pFlac, drflac_uint64 frameCount, drflac_uint64 iFirstPCMFrame, float* pOutputSamples)
{
    drflac_uint64 i;

    DRFLAC_ASSERT(pFlac->bitsPerSample == 32);

    if (pFlac->channels == 2) {
        for (i = 0; i < frameCount; ++i) {
            drflac_int32 left;
            drflac_int32 right;

            drflac__decode_stereo_sample_s64(&pFlac->currentFLACFrame, iFirstPCMFrame + i, &left, &right);

            pOutputSamples[i*2+0] = left  * (1.0f / 2147483648.0f);
            pOutputSamples[i*2+1] = right * (1.0f / 2147483648.0f);
        }
    } else {
        for (i = 0; i < frameCount; ++i) {
            unsigned int j;
            for (j = 0; j < pFlac->channels; ++j) {
                drflac_int32 sample;

                sample = (drflac_int32)((drflac_uint64)pFlac->currentFLACFrame.subframes[j].samples.pS64[iFirstPCMFrame + i] << pFlac->currentFLACFrame.subframes[j].wastedBitsPerSample);
                pOutputSamples[i*pFlac->channels+j] = sample * (1.0f / 2147483648.0f);
            }
        }
    }
}

static drflac_bool32 drflac__is_current_flac_frame_valid(drflac* pFlac)
{
    drflac_uint32 iChannel;
    drflac_uint32 channelCount;

    if (pFlac->currentFLACFrame.header.blockSizeInPCMFrames > pFlac->decodedSamplesCapacityInPCMFrames || pFlac->currentFLACFrame.pcmFramesRemaining > pFlac->currentFLACFrame.header.blockSizeInPCMFrames) {
        return DRFLAC_FALSE;
    }

    
    channelCount = drflac__get_channel_count_from_channel_assignment(pFlac->currentFLACFrame.header.channelAssignment);
    if (channelCount != pFlac->channels) {
        return DRFLAC_FALSE;
    }

    if (pFlac->bitsPerSample == 32) {
        for (iChannel = 0; iChannel < channelCount; iChannel += 1) {
            if (pFlac->currentFLACFrame.subframes[iChannel].samples.pS64 == NULL) {
                return DRFLAC_FALSE;
            }
        }
    } else {
        for (iChannel = 0; iChannel < channelCount; iChannel += 1) {
            if (pFlac->currentFLACFrame.subframes[iChannel].samples.pS32 == NULL) {
                return DRFLAC_FALSE;
            }
        }
    }

    return DRFLAC_TRUE;
}

static DRFLAC_INLINE void drflac_read_pcm_frames_s32__decode(drflac* pFlac, drflac_uint64 frameCount, drflac_uint64 iFirstPCMFrame, drflac_int32* pOutputSamples)
{
    unsigned int channelCount;
    drflac_uint32 unusedBitsPerSample;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample < 32);

    channelCount = drflac__get_channel_count_from_channel_assignment(pFlac->currentFLACFrame.header.channelAssignment);
    unusedBitsPerSample = 32 - pFlac->currentFLACFrame.header.bitsPerSample;

    if (channelCount == 2) {
        const drflac_int32* pDecodedSamples0 = pFlac->currentFLACFrame.subframes[0].samples.pS32 + iFirstPCMFrame;
        const drflac_int32* pDecodedSamples1 = pFlac->currentFLACFrame.subframes[1].samples.pS32 + iFirstPCMFrame;

        switch (pFlac->currentFLACFrame.header.channelAssignment)
        {
            case DRFLAC_CHANNEL_ASSIGNMENT_LEFT_SIDE:
            {
                drflac_read_pcm_frames_s32__decode_left_side(pFlac, frameCount, unusedBitsPerSample, pDecodedSamples0, pDecodedSamples1, pOutputSamples);
            } break;

            case DRFLAC_CHANNEL_ASSIGNMENT_RIGHT_SIDE:
            {
                drflac_read_pcm_frames_s32__decode_right_side(pFlac, frameCount, unusedBitsPerSample, pDecodedSamples0, pDecodedSamples1, pOutputSamples);
            } break;

            case DRFLAC_CHANNEL_ASSIGNMENT_MID_SIDE:
            {
                drflac_read_pcm_frames_s32__decode_mid_side(pFlac, frameCount, unusedBitsPerSample, pDecodedSamples0, pDecodedSamples1, pOutputSamples);
            } break;

            case DRFLAC_CHANNEL_ASSIGNMENT_INDEPENDENT_STEREO:
            {
                drflac_read_pcm_frames_s32__decode_independent_stereo(pFlac, frameCount, unusedBitsPerSample, pDecodedSamples0, pDecodedSamples1, pOutputSamples);
            } break;

            default: DRFLAC_ASSERT(!"Invalid stereo channel assignment"); break;
        }
    } else {
        
        drflac_uint64 i;

        for (i = 0; i < frameCount; ++i) {
            unsigned int j;

            for (j = 0; j < channelCount; ++j) {
                pOutputSamples[(i*channelCount)+j] = (drflac_int32)((drflac_uint32)(pFlac->currentFLACFrame.subframes[j].samples.pS32[iFirstPCMFrame + i]) << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[j].wastedBitsPerSample));
            }
        }
    }
}

DRFLAC_API drflac_uint64 drflac_read_pcm_frames_s32(drflac* pFlac, drflac_uint64 framesToRead, drflac_int32* pBufferOut)
{
    drflac_uint64 framesRead;

    if (pFlac == NULL || framesToRead == 0) {
        return 0;
    }

    if (pBufferOut == NULL) {
        return drflac__seek_forward_by_pcm_frames(pFlac, framesToRead);
    }

    DRFLAC_ASSERT(pFlac->bitsPerSample <= 32);

    framesRead = 0;
    while (framesToRead > 0) {
        
        if (pFlac->currentFLACFrame.pcmFramesRemaining == 0) {
            if (!drflac__read_and_decode_next_flac_frame(pFlac)) {
                break;  
            }
        } else {
            unsigned int channelCount = drflac__get_channel_count_from_channel_assignment(pFlac->currentFLACFrame.header.channelAssignment);
            drflac_uint64 iFirstPCMFrame = pFlac->currentFLACFrame.header.blockSizeInPCMFrames - pFlac->currentFLACFrame.pcmFramesRemaining;
            drflac_uint64 frameCountThisIteration = framesToRead;

            if (frameCountThisIteration > pFlac->currentFLACFrame.pcmFramesRemaining) {
                frameCountThisIteration = pFlac->currentFLACFrame.pcmFramesRemaining;
            }

            if (pFlac->bitsPerSample != 32) {
                drflac_read_pcm_frames_s32__decode(pFlac, frameCountThisIteration, iFirstPCMFrame, pBufferOut);
            } else {
                drflac_read_pcm_frames_s32__decode_s64(pFlac, frameCountThisIteration, iFirstPCMFrame, pBufferOut);
            }

            framesRead             += frameCountThisIteration;
            pBufferOut             += frameCountThisIteration * channelCount;
            framesToRead           -= frameCountThisIteration;
            pFlac->currentPCMFrame += frameCountThisIteration;
            pFlac->currentFLACFrame.pcmFramesRemaining -= (drflac_uint32)frameCountThisIteration;
        }
    }

    return framesRead;
}


#if 0
static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_left_side__reference(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    drflac_uint64 i;
    for (i = 0; i < frameCount; ++i) {
        drflac_uint32 left  = (drflac_uint32)pInputSamples0[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample);
        drflac_uint32 side  = (drflac_uint32)pInputSamples1[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample);
        drflac_uint32 right = left - side;

        left  >>= 16;
        right >>= 16;

        pOutputSamples[i*2+0] = (drflac_int16)left;
        pOutputSamples[i*2+1] = (drflac_int16)right;
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_left_side__scalar(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

    for (i = 0; i < frameCount4; ++i) {
        drflac_uint32 left0 = pInputSamples0U32[i*4+0] << shift0;
        drflac_uint32 left1 = pInputSamples0U32[i*4+1] << shift0;
        drflac_uint32 left2 = pInputSamples0U32[i*4+2] << shift0;
        drflac_uint32 left3 = pInputSamples0U32[i*4+3] << shift0;

        drflac_uint32 side0 = pInputSamples1U32[i*4+0] << shift1;
        drflac_uint32 side1 = pInputSamples1U32[i*4+1] << shift1;
        drflac_uint32 side2 = pInputSamples1U32[i*4+2] << shift1;
        drflac_uint32 side3 = pInputSamples1U32[i*4+3] << shift1;

        drflac_uint32 right0 = left0 - side0;
        drflac_uint32 right1 = left1 - side1;
        drflac_uint32 right2 = left2 - side2;
        drflac_uint32 right3 = left3 - side3;

        left0  >>= 16;
        left1  >>= 16;
        left2  >>= 16;
        left3  >>= 16;

        right0 >>= 16;
        right1 >>= 16;
        right2 >>= 16;
        right3 >>= 16;

        pOutputSamples[i*8+0] = (drflac_int16)left0;
        pOutputSamples[i*8+1] = (drflac_int16)right0;
        pOutputSamples[i*8+2] = (drflac_int16)left1;
        pOutputSamples[i*8+3] = (drflac_int16)right1;
        pOutputSamples[i*8+4] = (drflac_int16)left2;
        pOutputSamples[i*8+5] = (drflac_int16)right2;
        pOutputSamples[i*8+6] = (drflac_int16)left3;
        pOutputSamples[i*8+7] = (drflac_int16)right3;
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 left  = pInputSamples0U32[i] << shift0;
        drflac_uint32 side  = pInputSamples1U32[i] << shift1;
        drflac_uint32 right = left - side;

        left  >>= 16;
        right >>= 16;

        pOutputSamples[i*2+0] = (drflac_int16)left;
        pOutputSamples[i*2+1] = (drflac_int16)right;
    }
}

#if defined(DRFLAC_SUPPORT_SSE2)
static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_left_side__sse2(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    for (i = 0; i < frameCount4; ++i) {
        __m128i left  = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples0 + i), shift0);
        __m128i side  = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples1 + i), shift1);
        __m128i right = _mm_sub_epi32(left, side);

        left  = _mm_srai_epi32(left,  16);
        right = _mm_srai_epi32(right, 16);

        _mm_storeu_si128((__m128i*)(pOutputSamples + i*8), drflac__mm_packs_interleaved_epi32(left, right));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 left  = pInputSamples0U32[i] << shift0;
        drflac_uint32 side  = pInputSamples1U32[i] << shift1;
        drflac_uint32 right = left - side;

        left  >>= 16;
        right >>= 16;

        pOutputSamples[i*2+0] = (drflac_int16)left;
        pOutputSamples[i*2+1] = (drflac_int16)right;
    }
}
#endif

#if defined(DRFLAC_SUPPORT_NEON)
static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_left_side__neon(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
    int32x4_t shift0_4;
    int32x4_t shift1_4;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    shift0_4 = vdupq_n_s32(shift0);
    shift1_4 = vdupq_n_s32(shift1);

    for (i = 0; i < frameCount4; ++i) {
        uint32x4_t left;
        uint32x4_t side;
        uint32x4_t right;

        left  = vshlq_u32(vld1q_u32(pInputSamples0U32 + i*4), shift0_4);
        side  = vshlq_u32(vld1q_u32(pInputSamples1U32 + i*4), shift1_4);
        right = vsubq_u32(left, side);

        left  = vshrq_n_u32(left,  16);
        right = vshrq_n_u32(right, 16);

        drflac__vst2q_u16((drflac_uint16*)pOutputSamples + i*8, vzip_u16(vmovn_u32(left), vmovn_u32(right)));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 left  = pInputSamples0U32[i] << shift0;
        drflac_uint32 side  = pInputSamples1U32[i] << shift1;
        drflac_uint32 right = left - side;

        left  >>= 16;
        right >>= 16;

        pOutputSamples[i*2+0] = (drflac_int16)left;
        pOutputSamples[i*2+1] = (drflac_int16)right;
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_duplicate_mono(drflac_uint64 frameCount, drflac_uint32 shift, const drflac_int32* pInputSamples, drflac_int16* pOutputSamples)
{
    drflac_uint64 i;
    const drflac_uint32* pInputSamplesU32 = (const drflac_uint32*)pInputSamples;

    DRFLAC_ASSERT(shift < 32);

    for (i = 0; i < frameCount; ++i) {
        drflac_int16 sample;

        sample = (drflac_int16)((pInputSamplesU32[i] << shift) >> 16);
        pOutputSamples[i*2+0] = sample;
        pOutputSamples[i*2+1] = sample;
    }
}

static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_left_side(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    if (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample == 32) {
        drflac_read_pcm_frames_s16__decode_duplicate_mono(frameCount, unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample, pInputSamples0, pOutputSamples);
        return;
    }

#if defined(DRFLAC_SUPPORT_SSE2)
    if (drflac__gIsSSE2Supported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_s16__decode_left_side__sse2(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#elif defined(DRFLAC_SUPPORT_NEON)
    if (drflac__gIsNEONSupported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_s16__decode_left_side__neon(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#endif
    {
        
#if 0
        drflac_read_pcm_frames_s16__decode_left_side__reference(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#else
        drflac_read_pcm_frames_s16__decode_left_side__scalar(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#endif
    }
}


#if 0
static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_right_side__reference(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    drflac_uint64 i;
    for (i = 0; i < frameCount; ++i) {
        drflac_uint32 side  = (drflac_uint32)pInputSamples0[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample);
        drflac_uint32 right = (drflac_uint32)pInputSamples1[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample);
        drflac_uint32 left  = right + side;

        left  >>= 16;
        right >>= 16;

        pOutputSamples[i*2+0] = (drflac_int16)left;
        pOutputSamples[i*2+1] = (drflac_int16)right;
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_right_side__scalar(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

    for (i = 0; i < frameCount4; ++i) {
        drflac_uint32 side0  = pInputSamples0U32[i*4+0] << shift0;
        drflac_uint32 side1  = pInputSamples0U32[i*4+1] << shift0;
        drflac_uint32 side2  = pInputSamples0U32[i*4+2] << shift0;
        drflac_uint32 side3  = pInputSamples0U32[i*4+3] << shift0;

        drflac_uint32 right0 = pInputSamples1U32[i*4+0] << shift1;
        drflac_uint32 right1 = pInputSamples1U32[i*4+1] << shift1;
        drflac_uint32 right2 = pInputSamples1U32[i*4+2] << shift1;
        drflac_uint32 right3 = pInputSamples1U32[i*4+3] << shift1;

        drflac_uint32 left0 = right0 + side0;
        drflac_uint32 left1 = right1 + side1;
        drflac_uint32 left2 = right2 + side2;
        drflac_uint32 left3 = right3 + side3;

        left0  >>= 16;
        left1  >>= 16;
        left2  >>= 16;
        left3  >>= 16;

        right0 >>= 16;
        right1 >>= 16;
        right2 >>= 16;
        right3 >>= 16;

        pOutputSamples[i*8+0] = (drflac_int16)left0;
        pOutputSamples[i*8+1] = (drflac_int16)right0;
        pOutputSamples[i*8+2] = (drflac_int16)left1;
        pOutputSamples[i*8+3] = (drflac_int16)right1;
        pOutputSamples[i*8+4] = (drflac_int16)left2;
        pOutputSamples[i*8+5] = (drflac_int16)right2;
        pOutputSamples[i*8+6] = (drflac_int16)left3;
        pOutputSamples[i*8+7] = (drflac_int16)right3;
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 side  = pInputSamples0U32[i] << shift0;
        drflac_uint32 right = pInputSamples1U32[i] << shift1;
        drflac_uint32 left  = right + side;

        left  >>= 16;
        right >>= 16;

        pOutputSamples[i*2+0] = (drflac_int16)left;
        pOutputSamples[i*2+1] = (drflac_int16)right;
    }
}

#if defined(DRFLAC_SUPPORT_SSE2)
static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_right_side__sse2(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    for (i = 0; i < frameCount4; ++i) {
        __m128i side  = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples0 + i), shift0);
        __m128i right = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples1 + i), shift1);
        __m128i left  = _mm_add_epi32(right, side);

        left  = _mm_srai_epi32(left,  16);
        right = _mm_srai_epi32(right, 16);

        _mm_storeu_si128((__m128i*)(pOutputSamples + i*8), drflac__mm_packs_interleaved_epi32(left, right));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 side  = pInputSamples0U32[i] << shift0;
        drflac_uint32 right = pInputSamples1U32[i] << shift1;
        drflac_uint32 left  = right + side;

        left  >>= 16;
        right >>= 16;

        pOutputSamples[i*2+0] = (drflac_int16)left;
        pOutputSamples[i*2+1] = (drflac_int16)right;
    }
}
#endif

#if defined(DRFLAC_SUPPORT_NEON)
static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_right_side__neon(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
    int32x4_t shift0_4;
    int32x4_t shift1_4;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    shift0_4 = vdupq_n_s32(shift0);
    shift1_4 = vdupq_n_s32(shift1);

    for (i = 0; i < frameCount4; ++i) {
        uint32x4_t side;
        uint32x4_t right;
        uint32x4_t left;

        side  = vshlq_u32(vld1q_u32(pInputSamples0U32 + i*4), shift0_4);
        right = vshlq_u32(vld1q_u32(pInputSamples1U32 + i*4), shift1_4);
        left  = vaddq_u32(right, side);

        left  = vshrq_n_u32(left,  16);
        right = vshrq_n_u32(right, 16);

        drflac__vst2q_u16((drflac_uint16*)pOutputSamples + i*8, vzip_u16(vmovn_u32(left), vmovn_u32(right)));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 side  = pInputSamples0U32[i] << shift0;
        drflac_uint32 right = pInputSamples1U32[i] << shift1;
        drflac_uint32 left  = right + side;

        left  >>= 16;
        right >>= 16;

        pOutputSamples[i*2+0] = (drflac_int16)left;
        pOutputSamples[i*2+1] = (drflac_int16)right;
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_right_side(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    if (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample == 32) {
        drflac_read_pcm_frames_s16__decode_duplicate_mono(frameCount, unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample, pInputSamples1, pOutputSamples);
        return;
    }

#if defined(DRFLAC_SUPPORT_SSE2)
    if (drflac__gIsSSE2Supported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_s16__decode_right_side__sse2(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#elif defined(DRFLAC_SUPPORT_NEON)
    if (drflac__gIsNEONSupported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_s16__decode_right_side__neon(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#endif
    {
        
#if 0
        drflac_read_pcm_frames_s16__decode_right_side__reference(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#else
        drflac_read_pcm_frames_s16__decode_right_side__scalar(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#endif
    }
}


#if 0
static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_mid_side__reference(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    for (drflac_uint64 i = 0; i < frameCount; ++i) {
        drflac_uint32 mid  = (drflac_uint32)pInputSamples0[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
        drflac_uint32 side = (drflac_uint32)pInputSamples1[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

        mid = (mid << 1) | (side & 0x01);

        pOutputSamples[i*2+0] = (drflac_int16)(((drflac_uint32)((drflac_int32)(mid + side) >> 1) << unusedBitsPerSample) >> 16);
        pOutputSamples[i*2+1] = (drflac_int16)(((drflac_uint32)((drflac_int32)(mid - side) >> 1) << unusedBitsPerSample) >> 16);
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_mid_side__scalar(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift = unusedBitsPerSample;

    if (shift > 0) {
        for (i = 0; i < frameCount4; ++i) {
            drflac_uint32 temp0L;
            drflac_uint32 temp1L;
            drflac_uint32 temp2L;
            drflac_uint32 temp3L;
            drflac_uint32 temp0R;
            drflac_uint32 temp1R;
            drflac_uint32 temp2R;
            drflac_uint32 temp3R;

            drflac_uint32 mid0  = pInputSamples0U32[i*4+0] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 mid1  = pInputSamples0U32[i*4+1] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 mid2  = pInputSamples0U32[i*4+2] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 mid3  = pInputSamples0U32[i*4+3] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;

            drflac_uint32 side0 = pInputSamples1U32[i*4+0] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint32 side1 = pInputSamples1U32[i*4+1] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint32 side2 = pInputSamples1U32[i*4+2] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint32 side3 = pInputSamples1U32[i*4+3] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid0 = (mid0 << 1) | (side0 & 0x01);
            mid1 = (mid1 << 1) | (side1 & 0x01);
            mid2 = (mid2 << 1) | (side2 & 0x01);
            mid3 = (mid3 << 1) | (side3 & 0x01);

            temp0L = (mid0 + side0) << (shift - 1);
            temp1L = (mid1 + side1) << (shift - 1);
            temp2L = (mid2 + side2) << (shift - 1);
            temp3L = (mid3 + side3) << (shift - 1);

            temp0R = (mid0 - side0) << (shift - 1);
            temp1R = (mid1 - side1) << (shift - 1);
            temp2R = (mid2 - side2) << (shift - 1);
            temp3R = (mid3 - side3) << (shift - 1);

            temp0L >>= 16;
            temp1L >>= 16;
            temp2L >>= 16;
            temp3L >>= 16;

            temp0R >>= 16;
            temp1R >>= 16;
            temp2R >>= 16;
            temp3R >>= 16;

            pOutputSamples[i*8+0] = (drflac_int16)temp0L;
            pOutputSamples[i*8+1] = (drflac_int16)temp0R;
            pOutputSamples[i*8+2] = (drflac_int16)temp1L;
            pOutputSamples[i*8+3] = (drflac_int16)temp1R;
            pOutputSamples[i*8+4] = (drflac_int16)temp2L;
            pOutputSamples[i*8+5] = (drflac_int16)temp2R;
            pOutputSamples[i*8+6] = (drflac_int16)temp3L;
            pOutputSamples[i*8+7] = (drflac_int16)temp3R;
        }
    } else {
        
        for (i = 0; i < frameCount4; ++i) {
            drflac_uint32 temp0L;
            drflac_uint32 temp1L;
            drflac_uint32 temp2L;
            drflac_uint32 temp3L;
            drflac_uint32 temp0R;
            drflac_uint32 temp1R;
            drflac_uint32 temp2R;
            drflac_uint32 temp3R;

            drflac_uint64 mid0  = (drflac_uint64)(drflac_int64)pInputSamples0[i*4+0] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint64 mid1  = (drflac_uint64)(drflac_int64)pInputSamples0[i*4+1] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint64 mid2  = (drflac_uint64)(drflac_int64)pInputSamples0[i*4+2] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint64 mid3  = (drflac_uint64)(drflac_int64)pInputSamples0[i*4+3] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;

            drflac_uint64 side0 = (drflac_uint64)(drflac_int64)pInputSamples1[i*4+0] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint64 side1 = (drflac_uint64)(drflac_int64)pInputSamples1[i*4+1] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint64 side2 = (drflac_uint64)(drflac_int64)pInputSamples1[i*4+2] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint64 side3 = (drflac_uint64)(drflac_int64)pInputSamples1[i*4+3] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid0 = (mid0 << 1) | (side0 & 0x01);
            mid1 = (mid1 << 1) | (side1 & 0x01);
            mid2 = (mid2 << 1) | (side2 & 0x01);
            mid3 = (mid3 << 1) | (side3 & 0x01);

            temp0L = (drflac_uint32)((drflac_int64)(mid0 + side0) >> 1);
            temp1L = (drflac_uint32)((drflac_int64)(mid1 + side1) >> 1);
            temp2L = (drflac_uint32)((drflac_int64)(mid2 + side2) >> 1);
            temp3L = (drflac_uint32)((drflac_int64)(mid3 + side3) >> 1);

            temp0R = (drflac_uint32)((drflac_int64)(mid0 - side0) >> 1);
            temp1R = (drflac_uint32)((drflac_int64)(mid1 - side1) >> 1);
            temp2R = (drflac_uint32)((drflac_int64)(mid2 - side2) >> 1);
            temp3R = (drflac_uint32)((drflac_int64)(mid3 - side3) >> 1);

            temp0L >>= 16;
            temp1L >>= 16;
            temp2L >>= 16;
            temp3L >>= 16;

            temp0R >>= 16;
            temp1R >>= 16;
            temp2R >>= 16;
            temp3R >>= 16;

            pOutputSamples[i*8+0] = (drflac_int16)temp0L;
            pOutputSamples[i*8+1] = (drflac_int16)temp0R;
            pOutputSamples[i*8+2] = (drflac_int16)temp1L;
            pOutputSamples[i*8+3] = (drflac_int16)temp1R;
            pOutputSamples[i*8+4] = (drflac_int16)temp2L;
            pOutputSamples[i*8+5] = (drflac_int16)temp2R;
            pOutputSamples[i*8+6] = (drflac_int16)temp3L;
            pOutputSamples[i*8+7] = (drflac_int16)temp3R;
        }
    }

    if (shift > 0) {
        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint32 mid  = pInputSamples0U32[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 side = pInputSamples1U32[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = (drflac_int16)(((drflac_uint32)((drflac_int32)(mid + side) >> 1) << unusedBitsPerSample) >> 16);
            pOutputSamples[i*2+1] = (drflac_int16)(((drflac_uint32)((drflac_int32)(mid - side) >> 1) << unusedBitsPerSample) >> 16);
        }
    } else {
        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint64 mid;
            drflac_uint64 side;

            mid  = (drflac_uint64)(drflac_int64)pInputSamples0[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            side = (drflac_uint64)(drflac_int64)pInputSamples1[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = (drflac_int16)((drflac_int64)(mid + side) >> 17);
            pOutputSamples[i*2+1] = (drflac_int16)((drflac_int64)(mid - side) >> 17);
        }
    }
}

#if defined(DRFLAC_SUPPORT_SSE2)
static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_mid_side__sse2(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift = unusedBitsPerSample;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    if (shift == 0) {
        for (i = 0; i < frameCount4; ++i) {
            __m128i mid;
            __m128i side;
            __m128i left;
            __m128i right;

            mid   = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples0 + i), pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample);
            side  = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples1 + i), pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample);

            mid   = _mm_or_si128(_mm_slli_epi32(mid, 1), _mm_and_si128(side, _mm_set1_epi32(0x01)));

            left  = _mm_srai_epi32(_mm_add_epi32(mid, side), 1);
            right = _mm_srai_epi32(_mm_sub_epi32(mid, side), 1);

            left  = _mm_srai_epi32(left,  16);
            right = _mm_srai_epi32(right, 16);

            _mm_storeu_si128((__m128i*)(pOutputSamples + i*8), drflac__mm_packs_interleaved_epi32(left, right));
        }

        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint32 mid  = pInputSamples0U32[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 side = pInputSamples1U32[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = (drflac_int16)(((drflac_int32)(mid + side) >> 1) >> 16);
            pOutputSamples[i*2+1] = (drflac_int16)(((drflac_int32)(mid - side) >> 1) >> 16);
        }
    } else {
        shift -= 1;
        for (i = 0; i < frameCount4; ++i) {
            __m128i mid;
            __m128i side;
            __m128i left;
            __m128i right;

            mid   = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples0 + i), pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample);
            side  = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples1 + i), pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample);

            mid   = _mm_or_si128(_mm_slli_epi32(mid, 1), _mm_and_si128(side, _mm_set1_epi32(0x01)));

            left  = _mm_slli_epi32(_mm_add_epi32(mid, side), shift);
            right = _mm_slli_epi32(_mm_sub_epi32(mid, side), shift);

            left  = _mm_srai_epi32(left,  16);
            right = _mm_srai_epi32(right, 16);

            _mm_storeu_si128((__m128i*)(pOutputSamples + i*8), drflac__mm_packs_interleaved_epi32(left, right));
        }

        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint32 mid  = pInputSamples0U32[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 side = pInputSamples1U32[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = (drflac_int16)(((mid + side) << shift) >> 16);
            pOutputSamples[i*2+1] = (drflac_int16)(((mid - side) << shift) >> 16);
        }
    }
}
#endif

#if defined(DRFLAC_SUPPORT_NEON)
static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_mid_side__neon(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift = unusedBitsPerSample;
    int32x4_t wbpsShift0_4; 
    int32x4_t wbpsShift1_4; 

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    wbpsShift0_4 = vdupq_n_s32(pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample);
    wbpsShift1_4 = vdupq_n_s32(pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample);

    if (shift == 0) {
        for (i = 0; i < frameCount4; ++i) {
            uint32x4_t mid;
            uint32x4_t side;
            int32x4_t left;
            int32x4_t right;

            mid   = vshlq_u32(vld1q_u32(pInputSamples0U32 + i*4), wbpsShift0_4);
            side  = vshlq_u32(vld1q_u32(pInputSamples1U32 + i*4), wbpsShift1_4);

            mid   = vorrq_u32(vshlq_n_u32(mid, 1), vandq_u32(side, vdupq_n_u32(1)));

            left  = vshrq_n_s32(vreinterpretq_s32_u32(vaddq_u32(mid, side)), 1);
            right = vshrq_n_s32(vreinterpretq_s32_u32(vsubq_u32(mid, side)), 1);

            left  = vshrq_n_s32(left,  16);
            right = vshrq_n_s32(right, 16);

            drflac__vst2q_s16(pOutputSamples + i*8, vzip_s16(vmovn_s32(left), vmovn_s32(right)));
        }

        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint32 mid  = pInputSamples0U32[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 side = pInputSamples1U32[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = (drflac_int16)(((drflac_int32)(mid + side) >> 1) >> 16);
            pOutputSamples[i*2+1] = (drflac_int16)(((drflac_int32)(mid - side) >> 1) >> 16);
        }
    } else {
        int32x4_t shift4;

        shift -= 1;
        shift4 = vdupq_n_s32(shift);

        for (i = 0; i < frameCount4; ++i) {
            uint32x4_t mid;
            uint32x4_t side;
            int32x4_t left;
            int32x4_t right;

            mid   = vshlq_u32(vld1q_u32(pInputSamples0U32 + i*4), wbpsShift0_4);
            side  = vshlq_u32(vld1q_u32(pInputSamples1U32 + i*4), wbpsShift1_4);

            mid   = vorrq_u32(vshlq_n_u32(mid, 1), vandq_u32(side, vdupq_n_u32(1)));

            left  = vreinterpretq_s32_u32(vshlq_u32(vaddq_u32(mid, side), shift4));
            right = vreinterpretq_s32_u32(vshlq_u32(vsubq_u32(mid, side), shift4));

            left  = vshrq_n_s32(left,  16);
            right = vshrq_n_s32(right, 16);

            drflac__vst2q_s16(pOutputSamples + i*8, vzip_s16(vmovn_s32(left), vmovn_s32(right)));
        }

        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint32 mid  = pInputSamples0U32[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 side = pInputSamples1U32[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = (drflac_int16)(((mid + side) << shift) >> 16);
            pOutputSamples[i*2+1] = (drflac_int16)(((mid - side) << shift) >> 16);
        }
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_mid_side(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
#if defined(DRFLAC_SUPPORT_SSE2)
    if (drflac__gIsSSE2Supported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_s16__decode_mid_side__sse2(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#elif defined(DRFLAC_SUPPORT_NEON)
    if (drflac__gIsNEONSupported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_s16__decode_mid_side__neon(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#endif
    {
        
#if 0
        drflac_read_pcm_frames_s16__decode_mid_side__reference(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#else
        drflac_read_pcm_frames_s16__decode_mid_side__scalar(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#endif
    }
}


#if 0
static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_independent_stereo__reference(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    for (drflac_uint64 i = 0; i < frameCount; ++i) {
        pOutputSamples[i*2+0] = (drflac_int16)((drflac_int32)((drflac_uint32)pInputSamples0[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample)) >> 16);
        pOutputSamples[i*2+1] = (drflac_int16)((drflac_int32)((drflac_uint32)pInputSamples1[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample)) >> 16);
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_independent_stereo__scalar(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

    for (i = 0; i < frameCount4; ++i) {
        drflac_uint32 tempL0 = pInputSamples0U32[i*4+0] << shift0;
        drflac_uint32 tempL1 = pInputSamples0U32[i*4+1] << shift0;
        drflac_uint32 tempL2 = pInputSamples0U32[i*4+2] << shift0;
        drflac_uint32 tempL3 = pInputSamples0U32[i*4+3] << shift0;

        drflac_uint32 tempR0 = pInputSamples1U32[i*4+0] << shift1;
        drflac_uint32 tempR1 = pInputSamples1U32[i*4+1] << shift1;
        drflac_uint32 tempR2 = pInputSamples1U32[i*4+2] << shift1;
        drflac_uint32 tempR3 = pInputSamples1U32[i*4+3] << shift1;

        tempL0 >>= 16;
        tempL1 >>= 16;
        tempL2 >>= 16;
        tempL3 >>= 16;

        tempR0 >>= 16;
        tempR1 >>= 16;
        tempR2 >>= 16;
        tempR3 >>= 16;

        pOutputSamples[i*8+0] = (drflac_int16)tempL0;
        pOutputSamples[i*8+1] = (drflac_int16)tempR0;
        pOutputSamples[i*8+2] = (drflac_int16)tempL1;
        pOutputSamples[i*8+3] = (drflac_int16)tempR1;
        pOutputSamples[i*8+4] = (drflac_int16)tempL2;
        pOutputSamples[i*8+5] = (drflac_int16)tempR2;
        pOutputSamples[i*8+6] = (drflac_int16)tempL3;
        pOutputSamples[i*8+7] = (drflac_int16)tempR3;
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        pOutputSamples[i*2+0] = (drflac_int16)((pInputSamples0U32[i] << shift0) >> 16);
        pOutputSamples[i*2+1] = (drflac_int16)((pInputSamples1U32[i] << shift1) >> 16);
    }
}

#if defined(DRFLAC_SUPPORT_SSE2)
static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_independent_stereo__sse2(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

    for (i = 0; i < frameCount4; ++i) {
        __m128i left  = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples0 + i), shift0);
        __m128i right = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples1 + i), shift1);

        left  = _mm_srai_epi32(left,  16);
        right = _mm_srai_epi32(right, 16);

        
        _mm_storeu_si128((__m128i*)(pOutputSamples + i*8), drflac__mm_packs_interleaved_epi32(left, right));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        pOutputSamples[i*2+0] = (drflac_int16)((pInputSamples0U32[i] << shift0) >> 16);
        pOutputSamples[i*2+1] = (drflac_int16)((pInputSamples1U32[i] << shift1) >> 16);
    }
}
#endif

#if defined(DRFLAC_SUPPORT_NEON)
static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_independent_stereo__neon(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

    int32x4_t shift0_4 = vdupq_n_s32(shift0);
    int32x4_t shift1_4 = vdupq_n_s32(shift1);

    for (i = 0; i < frameCount4; ++i) {
        int32x4_t left;
        int32x4_t right;

        left  = vreinterpretq_s32_u32(vshlq_u32(vld1q_u32(pInputSamples0U32 + i*4), shift0_4));
        right = vreinterpretq_s32_u32(vshlq_u32(vld1q_u32(pInputSamples1U32 + i*4), shift1_4));

        left  = vshrq_n_s32(left,  16);
        right = vshrq_n_s32(right, 16);

        drflac__vst2q_s16(pOutputSamples + i*8, vzip_s16(vmovn_s32(left), vmovn_s32(right)));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        pOutputSamples[i*2+0] = (drflac_int16)((pInputSamples0U32[i] << shift0) >> 16);
        pOutputSamples[i*2+1] = (drflac_int16)((pInputSamples1U32[i] << shift1) >> 16);
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode_independent_stereo(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, drflac_int16* pOutputSamples)
{
#if defined(DRFLAC_SUPPORT_SSE2)
    if (drflac__gIsSSE2Supported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_s16__decode_independent_stereo__sse2(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#elif defined(DRFLAC_SUPPORT_NEON)
    if (drflac__gIsNEONSupported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_s16__decode_independent_stereo__neon(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#endif
    {
        
#if 0
        drflac_read_pcm_frames_s16__decode_independent_stereo__reference(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#else
        drflac_read_pcm_frames_s16__decode_independent_stereo__scalar(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#endif
    }
}

static DRFLAC_INLINE void drflac_read_pcm_frames_s16__decode(drflac* pFlac, drflac_uint64 frameCount, drflac_uint64 iFirstPCMFrame, drflac_int16* pOutputSamples)
{
    unsigned int channelCount;
    drflac_uint32 unusedBitsPerSample;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample < 32);

    channelCount = drflac__get_channel_count_from_channel_assignment(pFlac->currentFLACFrame.header.channelAssignment);
    unusedBitsPerSample = 32 - pFlac->currentFLACFrame.header.bitsPerSample;

    if (channelCount == 2) {
        const drflac_int32* pDecodedSamples0 = pFlac->currentFLACFrame.subframes[0].samples.pS32 + iFirstPCMFrame;
        const drflac_int32* pDecodedSamples1 = pFlac->currentFLACFrame.subframes[1].samples.pS32 + iFirstPCMFrame;

        switch (pFlac->currentFLACFrame.header.channelAssignment)
        {
            case DRFLAC_CHANNEL_ASSIGNMENT_LEFT_SIDE:
            {
                drflac_read_pcm_frames_s16__decode_left_side(pFlac, frameCount, unusedBitsPerSample, pDecodedSamples0, pDecodedSamples1, pOutputSamples);
            } break;

            case DRFLAC_CHANNEL_ASSIGNMENT_RIGHT_SIDE:
            {
                drflac_read_pcm_frames_s16__decode_right_side(pFlac, frameCount, unusedBitsPerSample, pDecodedSamples0, pDecodedSamples1, pOutputSamples);
            } break;

            case DRFLAC_CHANNEL_ASSIGNMENT_MID_SIDE:
            {
                drflac_read_pcm_frames_s16__decode_mid_side(pFlac, frameCount, unusedBitsPerSample, pDecodedSamples0, pDecodedSamples1, pOutputSamples);
            } break;

            case DRFLAC_CHANNEL_ASSIGNMENT_INDEPENDENT_STEREO:
            {
                drflac_read_pcm_frames_s16__decode_independent_stereo(pFlac, frameCount, unusedBitsPerSample, pDecodedSamples0, pDecodedSamples1, pOutputSamples);
            } break;

            default: DRFLAC_ASSERT(!"Invalid stereo channel assignment"); break;
        }
    } else {
        
        drflac_uint64 i;

        for (i = 0; i < frameCount; ++i) {
            unsigned int j;

            for (j = 0; j < channelCount; ++j) {
                drflac_int32 sampleS32;
                
                sampleS32 = (drflac_int32)((drflac_uint32)(pFlac->currentFLACFrame.subframes[j].samples.pS32[iFirstPCMFrame + i]) << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[j].wastedBitsPerSample));
                pOutputSamples[(i*channelCount)+j] = (drflac_int16)(sampleS32 >> 16);
            }
        }
    }
}

DRFLAC_API drflac_uint64 drflac_read_pcm_frames_s16(drflac* pFlac, drflac_uint64 framesToRead, drflac_int16* pBufferOut)
{
    drflac_uint64 framesRead;

    if (pFlac == NULL || framesToRead == 0) {
        return 0;
    }

    if (pBufferOut == NULL) {
        return drflac__seek_forward_by_pcm_frames(pFlac, framesToRead);
    }

    DRFLAC_ASSERT(pFlac->bitsPerSample <= 32);

    framesRead = 0;
    while (framesToRead > 0) {
        
        if (pFlac->currentFLACFrame.pcmFramesRemaining == 0) {
            if (!drflac__read_and_decode_next_flac_frame(pFlac)) {
                break;  
            }
        } else {
            unsigned int channelCount = drflac__get_channel_count_from_channel_assignment(pFlac->currentFLACFrame.header.channelAssignment);
            drflac_uint64 iFirstPCMFrame = pFlac->currentFLACFrame.header.blockSizeInPCMFrames - pFlac->currentFLACFrame.pcmFramesRemaining;
            drflac_uint64 frameCountThisIteration = framesToRead;

            if (frameCountThisIteration > pFlac->currentFLACFrame.pcmFramesRemaining) {
                frameCountThisIteration = pFlac->currentFLACFrame.pcmFramesRemaining;
            }

            if (pFlac->bitsPerSample != 32) {
                drflac_read_pcm_frames_s16__decode(pFlac, frameCountThisIteration, iFirstPCMFrame, pBufferOut);
            } else {
                drflac_read_pcm_frames_s16__decode_s64(pFlac, frameCountThisIteration, iFirstPCMFrame, pBufferOut);
            }

            framesRead             += frameCountThisIteration;
            pBufferOut             += frameCountThisIteration * channelCount;
            framesToRead           -= frameCountThisIteration;
            pFlac->currentPCMFrame += frameCountThisIteration;
            pFlac->currentFLACFrame.pcmFramesRemaining -= (drflac_uint32)frameCountThisIteration;
        }
    }

    return framesRead;
}


#if 0
static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_left_side__reference(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    drflac_uint64 i;
    for (i = 0; i < frameCount; ++i) {
        drflac_uint32 left  = (drflac_uint32)pInputSamples0[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample);
        drflac_uint32 side  = (drflac_uint32)pInputSamples1[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample);
        drflac_uint32 right = left - side;

        pOutputSamples[i*2+0] = (float)((drflac_int32)left  / 2147483648.0);
        pOutputSamples[i*2+1] = (float)((drflac_int32)right / 2147483648.0);
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_left_side__scalar(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

    float factor = 1 / 2147483648.0;

    for (i = 0; i < frameCount4; ++i) {
        drflac_uint32 left0 = pInputSamples0U32[i*4+0] << shift0;
        drflac_uint32 left1 = pInputSamples0U32[i*4+1] << shift0;
        drflac_uint32 left2 = pInputSamples0U32[i*4+2] << shift0;
        drflac_uint32 left3 = pInputSamples0U32[i*4+3] << shift0;

        drflac_uint32 side0 = pInputSamples1U32[i*4+0] << shift1;
        drflac_uint32 side1 = pInputSamples1U32[i*4+1] << shift1;
        drflac_uint32 side2 = pInputSamples1U32[i*4+2] << shift1;
        drflac_uint32 side3 = pInputSamples1U32[i*4+3] << shift1;

        drflac_uint32 right0 = left0 - side0;
        drflac_uint32 right1 = left1 - side1;
        drflac_uint32 right2 = left2 - side2;
        drflac_uint32 right3 = left3 - side3;

        pOutputSamples[i*8+0] = (drflac_int32)left0  * factor;
        pOutputSamples[i*8+1] = (drflac_int32)right0 * factor;
        pOutputSamples[i*8+2] = (drflac_int32)left1  * factor;
        pOutputSamples[i*8+3] = (drflac_int32)right1 * factor;
        pOutputSamples[i*8+4] = (drflac_int32)left2  * factor;
        pOutputSamples[i*8+5] = (drflac_int32)right2 * factor;
        pOutputSamples[i*8+6] = (drflac_int32)left3  * factor;
        pOutputSamples[i*8+7] = (drflac_int32)right3 * factor;
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 left  = pInputSamples0U32[i] << shift0;
        drflac_uint32 side  = pInputSamples1U32[i] << shift1;
        drflac_uint32 right = left - side;

        pOutputSamples[i*2+0] = (drflac_int32)left  * factor;
        pOutputSamples[i*2+1] = (drflac_int32)right * factor;
    }
}

#if defined(DRFLAC_SUPPORT_SSE2)
static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_left_side__sse2(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample) - 8;
    drflac_uint32 shift1 = (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample) - 8;
    __m128 factor;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    factor = _mm_set1_ps(1.0f / 8388608.0f);

    for (i = 0; i < frameCount4; ++i) {
        __m128i left  = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples0 + i), shift0);
        __m128i side  = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples1 + i), shift1);
        __m128i right = _mm_sub_epi32(left, side);
        __m128 leftf  = _mm_mul_ps(_mm_cvtepi32_ps(left),  factor);
        __m128 rightf = _mm_mul_ps(_mm_cvtepi32_ps(right), factor);

        _mm_storeu_ps(pOutputSamples + i*8 + 0, _mm_unpacklo_ps(leftf, rightf));
        _mm_storeu_ps(pOutputSamples + i*8 + 4, _mm_unpackhi_ps(leftf, rightf));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 left  = pInputSamples0U32[i] << shift0;
        drflac_uint32 side  = pInputSamples1U32[i] << shift1;
        drflac_uint32 right = left - side;

        pOutputSamples[i*2+0] = (drflac_int32)left  / 8388608.0f;
        pOutputSamples[i*2+1] = (drflac_int32)right / 8388608.0f;
    }
}
#endif

#if defined(DRFLAC_SUPPORT_NEON)
static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_left_side__neon(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample) - 8;
    drflac_uint32 shift1 = (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample) - 8;
    float32x4_t factor4;
    int32x4_t shift0_4;
    int32x4_t shift1_4;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    factor4  = vdupq_n_f32(1.0f / 8388608.0f);
    shift0_4 = vdupq_n_s32(shift0);
    shift1_4 = vdupq_n_s32(shift1);

    for (i = 0; i < frameCount4; ++i) {
        uint32x4_t left;
        uint32x4_t side;
        uint32x4_t right;
        float32x4_t leftf;
        float32x4_t rightf;

        left   = vshlq_u32(vld1q_u32(pInputSamples0U32 + i*4), shift0_4);
        side   = vshlq_u32(vld1q_u32(pInputSamples1U32 + i*4), shift1_4);
        right  = vsubq_u32(left, side);
        leftf  = vmulq_f32(vcvtq_f32_s32(vreinterpretq_s32_u32(left)),  factor4);
        rightf = vmulq_f32(vcvtq_f32_s32(vreinterpretq_s32_u32(right)), factor4);

        drflac__vst2q_f32(pOutputSamples + i*8, vzipq_f32(leftf, rightf));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 left  = pInputSamples0U32[i] << shift0;
        drflac_uint32 side  = pInputSamples1U32[i] << shift1;
        drflac_uint32 right = left - side;

        pOutputSamples[i*2+0] = (drflac_int32)left  / 8388608.0f;
        pOutputSamples[i*2+1] = (drflac_int32)right / 8388608.0f;
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_duplicate_mono(drflac_uint64 frameCount, drflac_uint32 shift, const drflac_int32* pInputSamples, float* pOutputSamples)
{
    drflac_uint64 i;
    const drflac_uint32* pInputSamplesU32 = (const drflac_uint32*)pInputSamples;
    float factor;

    DRFLAC_ASSERT(shift < 32);

    factor = 1 / 2147483648.0f;

    for (i = 0; i < frameCount; ++i) {
        float sample;

        sample = (drflac_int32)(pInputSamplesU32[i] << shift) * factor;
        pOutputSamples[i*2+0] = sample;
        pOutputSamples[i*2+1] = sample;
    }
}

static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_left_side(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    if (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample == 32) {
        drflac_read_pcm_frames_f32__decode_duplicate_mono(frameCount, unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample, pInputSamples0, pOutputSamples);
        return;
    }

#if defined(DRFLAC_SUPPORT_SSE2)
    if (drflac__gIsSSE2Supported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_f32__decode_left_side__sse2(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#elif defined(DRFLAC_SUPPORT_NEON)
    if (drflac__gIsNEONSupported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_f32__decode_left_side__neon(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#endif
    {
        
#if 0
        drflac_read_pcm_frames_f32__decode_left_side__reference(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#else
        drflac_read_pcm_frames_f32__decode_left_side__scalar(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#endif
    }
}


#if 0
static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_right_side__reference(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    drflac_uint64 i;
    for (i = 0; i < frameCount; ++i) {
        drflac_uint32 side  = (drflac_uint32)pInputSamples0[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample);
        drflac_uint32 right = (drflac_uint32)pInputSamples1[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample);
        drflac_uint32 left  = right + side;

        pOutputSamples[i*2+0] = (float)((drflac_int32)left  / 2147483648.0);
        pOutputSamples[i*2+1] = (float)((drflac_int32)right / 2147483648.0);
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_right_side__scalar(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
    float factor = 1 / 2147483648.0;

    for (i = 0; i < frameCount4; ++i) {
        drflac_uint32 side0  = pInputSamples0U32[i*4+0] << shift0;
        drflac_uint32 side1  = pInputSamples0U32[i*4+1] << shift0;
        drflac_uint32 side2  = pInputSamples0U32[i*4+2] << shift0;
        drflac_uint32 side3  = pInputSamples0U32[i*4+3] << shift0;

        drflac_uint32 right0 = pInputSamples1U32[i*4+0] << shift1;
        drflac_uint32 right1 = pInputSamples1U32[i*4+1] << shift1;
        drflac_uint32 right2 = pInputSamples1U32[i*4+2] << shift1;
        drflac_uint32 right3 = pInputSamples1U32[i*4+3] << shift1;

        drflac_uint32 left0 = right0 + side0;
        drflac_uint32 left1 = right1 + side1;
        drflac_uint32 left2 = right2 + side2;
        drflac_uint32 left3 = right3 + side3;

        pOutputSamples[i*8+0] = (drflac_int32)left0  * factor;
        pOutputSamples[i*8+1] = (drflac_int32)right0 * factor;
        pOutputSamples[i*8+2] = (drflac_int32)left1  * factor;
        pOutputSamples[i*8+3] = (drflac_int32)right1 * factor;
        pOutputSamples[i*8+4] = (drflac_int32)left2  * factor;
        pOutputSamples[i*8+5] = (drflac_int32)right2 * factor;
        pOutputSamples[i*8+6] = (drflac_int32)left3  * factor;
        pOutputSamples[i*8+7] = (drflac_int32)right3 * factor;
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 side  = pInputSamples0U32[i] << shift0;
        drflac_uint32 right = pInputSamples1U32[i] << shift1;
        drflac_uint32 left  = right + side;

        pOutputSamples[i*2+0] = (drflac_int32)left  * factor;
        pOutputSamples[i*2+1] = (drflac_int32)right * factor;
    }
}

#if defined(DRFLAC_SUPPORT_SSE2)
static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_right_side__sse2(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample) - 8;
    drflac_uint32 shift1 = (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample) - 8;
    __m128 factor;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    factor = _mm_set1_ps(1.0f / 8388608.0f);

    for (i = 0; i < frameCount4; ++i) {
        __m128i side  = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples0 + i), shift0);
        __m128i right = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples1 + i), shift1);
        __m128i left  = _mm_add_epi32(right, side);
        __m128 leftf  = _mm_mul_ps(_mm_cvtepi32_ps(left),  factor);
        __m128 rightf = _mm_mul_ps(_mm_cvtepi32_ps(right), factor);

        _mm_storeu_ps(pOutputSamples + i*8 + 0, _mm_unpacklo_ps(leftf, rightf));
        _mm_storeu_ps(pOutputSamples + i*8 + 4, _mm_unpackhi_ps(leftf, rightf));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 side  = pInputSamples0U32[i] << shift0;
        drflac_uint32 right = pInputSamples1U32[i] << shift1;
        drflac_uint32 left  = right + side;

        pOutputSamples[i*2+0] = (drflac_int32)left  / 8388608.0f;
        pOutputSamples[i*2+1] = (drflac_int32)right / 8388608.0f;
    }
}
#endif

#if defined(DRFLAC_SUPPORT_NEON)
static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_right_side__neon(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample) - 8;
    drflac_uint32 shift1 = (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample) - 8;
    float32x4_t factor4;
    int32x4_t shift0_4;
    int32x4_t shift1_4;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    factor4  = vdupq_n_f32(1.0f / 8388608.0f);
    shift0_4 = vdupq_n_s32(shift0);
    shift1_4 = vdupq_n_s32(shift1);

    for (i = 0; i < frameCount4; ++i) {
        uint32x4_t side;
        uint32x4_t right;
        uint32x4_t left;
        float32x4_t leftf;
        float32x4_t rightf;

        side   = vshlq_u32(vld1q_u32(pInputSamples0U32 + i*4), shift0_4);
        right  = vshlq_u32(vld1q_u32(pInputSamples1U32 + i*4), shift1_4);
        left   = vaddq_u32(right, side);
        leftf  = vmulq_f32(vcvtq_f32_s32(vreinterpretq_s32_u32(left)),  factor4);
        rightf = vmulq_f32(vcvtq_f32_s32(vreinterpretq_s32_u32(right)), factor4);

        drflac__vst2q_f32(pOutputSamples + i*8, vzipq_f32(leftf, rightf));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        drflac_uint32 side  = pInputSamples0U32[i] << shift0;
        drflac_uint32 right = pInputSamples1U32[i] << shift1;
        drflac_uint32 left  = right + side;

        pOutputSamples[i*2+0] = (drflac_int32)left  / 8388608.0f;
        pOutputSamples[i*2+1] = (drflac_int32)right / 8388608.0f;
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_right_side(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    if (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample == 32) {
        drflac_read_pcm_frames_f32__decode_duplicate_mono(frameCount, unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample, pInputSamples1, pOutputSamples);
        return;
    }

#if defined(DRFLAC_SUPPORT_SSE2)
    if (drflac__gIsSSE2Supported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_f32__decode_right_side__sse2(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#elif defined(DRFLAC_SUPPORT_NEON)
    if (drflac__gIsNEONSupported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_f32__decode_right_side__neon(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#endif
    {
        
#if 0
        drflac_read_pcm_frames_f32__decode_right_side__reference(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#else
        drflac_read_pcm_frames_f32__decode_right_side__scalar(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#endif
    }
}


#if 0
static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_mid_side__reference(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    for (drflac_uint64 i = 0; i < frameCount; ++i) {
        drflac_uint32 mid  = (drflac_uint32)pInputSamples0[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
        drflac_uint32 side = (drflac_uint32)pInputSamples1[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

        mid = (mid << 1) | (side & 0x01);

        pOutputSamples[i*2+0] = (float)((((drflac_int32)(mid + side) >> 1) << (unusedBitsPerSample)) / 2147483648.0);
        pOutputSamples[i*2+1] = (float)((((drflac_int32)(mid - side) >> 1) << (unusedBitsPerSample)) / 2147483648.0);
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_mid_side__scalar(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift = unusedBitsPerSample;
    float factor = 1 / 2147483648.0;

    if (shift > 0) {
        for (i = 0; i < frameCount4; ++i) {
            drflac_uint32 temp0L;
            drflac_uint32 temp1L;
            drflac_uint32 temp2L;
            drflac_uint32 temp3L;
            drflac_uint32 temp0R;
            drflac_uint32 temp1R;
            drflac_uint32 temp2R;
            drflac_uint32 temp3R;

            drflac_uint32 mid0  = pInputSamples0U32[i*4+0] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 mid1  = pInputSamples0U32[i*4+1] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 mid2  = pInputSamples0U32[i*4+2] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 mid3  = pInputSamples0U32[i*4+3] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;

            drflac_uint32 side0 = pInputSamples1U32[i*4+0] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint32 side1 = pInputSamples1U32[i*4+1] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint32 side2 = pInputSamples1U32[i*4+2] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint32 side3 = pInputSamples1U32[i*4+3] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid0 = (mid0 << 1) | (side0 & 0x01);
            mid1 = (mid1 << 1) | (side1 & 0x01);
            mid2 = (mid2 << 1) | (side2 & 0x01);
            mid3 = (mid3 << 1) | (side3 & 0x01);

            temp0L = (mid0 + side0) << (shift - 1);
            temp1L = (mid1 + side1) << (shift - 1);
            temp2L = (mid2 + side2) << (shift - 1);
            temp3L = (mid3 + side3) << (shift - 1);

            temp0R = (mid0 - side0) << (shift - 1);
            temp1R = (mid1 - side1) << (shift - 1);
            temp2R = (mid2 - side2) << (shift - 1);
            temp3R = (mid3 - side3) << (shift - 1);

            pOutputSamples[i*8+0] = (drflac_int32)temp0L * factor;
            pOutputSamples[i*8+1] = (drflac_int32)temp0R * factor;
            pOutputSamples[i*8+2] = (drflac_int32)temp1L * factor;
            pOutputSamples[i*8+3] = (drflac_int32)temp1R * factor;
            pOutputSamples[i*8+4] = (drflac_int32)temp2L * factor;
            pOutputSamples[i*8+5] = (drflac_int32)temp2R * factor;
            pOutputSamples[i*8+6] = (drflac_int32)temp3L * factor;
            pOutputSamples[i*8+7] = (drflac_int32)temp3R * factor;
        }
    } else {
        
        for (i = 0; i < frameCount4; ++i) {
            drflac_uint32 temp0L;
            drflac_uint32 temp1L;
            drflac_uint32 temp2L;
            drflac_uint32 temp3L;
            drflac_uint32 temp0R;
            drflac_uint32 temp1R;
            drflac_uint32 temp2R;
            drflac_uint32 temp3R;

            drflac_uint64 mid0  = (drflac_uint64)(drflac_int64)pInputSamples0[i*4+0] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint64 mid1  = (drflac_uint64)(drflac_int64)pInputSamples0[i*4+1] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint64 mid2  = (drflac_uint64)(drflac_int64)pInputSamples0[i*4+2] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint64 mid3  = (drflac_uint64)(drflac_int64)pInputSamples0[i*4+3] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;

            drflac_uint64 side0 = (drflac_uint64)(drflac_int64)pInputSamples1[i*4+0] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint64 side1 = (drflac_uint64)(drflac_int64)pInputSamples1[i*4+1] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint64 side2 = (drflac_uint64)(drflac_int64)pInputSamples1[i*4+2] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
            drflac_uint64 side3 = (drflac_uint64)(drflac_int64)pInputSamples1[i*4+3] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid0 = (mid0 << 1) | (side0 & 0x01);
            mid1 = (mid1 << 1) | (side1 & 0x01);
            mid2 = (mid2 << 1) | (side2 & 0x01);
            mid3 = (mid3 << 1) | (side3 & 0x01);

            temp0L = (drflac_uint32)((drflac_int64)(mid0 + side0) >> 1);
            temp1L = (drflac_uint32)((drflac_int64)(mid1 + side1) >> 1);
            temp2L = (drflac_uint32)((drflac_int64)(mid2 + side2) >> 1);
            temp3L = (drflac_uint32)((drflac_int64)(mid3 + side3) >> 1);

            temp0R = (drflac_uint32)((drflac_int64)(mid0 - side0) >> 1);
            temp1R = (drflac_uint32)((drflac_int64)(mid1 - side1) >> 1);
            temp2R = (drflac_uint32)((drflac_int64)(mid2 - side2) >> 1);
            temp3R = (drflac_uint32)((drflac_int64)(mid3 - side3) >> 1);

            pOutputSamples[i*8+0] = (drflac_int32)temp0L * factor;
            pOutputSamples[i*8+1] = (drflac_int32)temp0R * factor;
            pOutputSamples[i*8+2] = (drflac_int32)temp1L * factor;
            pOutputSamples[i*8+3] = (drflac_int32)temp1R * factor;
            pOutputSamples[i*8+4] = (drflac_int32)temp2L * factor;
            pOutputSamples[i*8+5] = (drflac_int32)temp2R * factor;
            pOutputSamples[i*8+6] = (drflac_int32)temp3L * factor;
            pOutputSamples[i*8+7] = (drflac_int32)temp3R * factor;
        }
    }

    if (shift > 0) {
        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint32 mid  = pInputSamples0U32[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 side = pInputSamples1U32[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = (drflac_int32)((drflac_uint32)((drflac_int32)(mid + side) >> 1) << unusedBitsPerSample) * factor;
            pOutputSamples[i*2+1] = (drflac_int32)((drflac_uint32)((drflac_int32)(mid - side) >> 1) << unusedBitsPerSample) * factor;
        }
    } else {
        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint64 mid;
            drflac_uint64 side;

            mid  = (drflac_uint64)(drflac_int64)pInputSamples0[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            side = (drflac_uint64)(drflac_int64)pInputSamples1[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = (drflac_int32)((drflac_int64)(mid + side) >> 1) * factor;
            pOutputSamples[i*2+1] = (drflac_int32)((drflac_int64)(mid - side) >> 1) * factor;
        }
    }
}

#if defined(DRFLAC_SUPPORT_SSE2)
static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_mid_side__sse2(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift = unusedBitsPerSample - 8;
    float factor;
    __m128 factor128;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    factor = 1.0f / 8388608.0f;
    factor128 = _mm_set1_ps(factor);

    if (shift == 0) {
        for (i = 0; i < frameCount4; ++i) {
            __m128i mid;
            __m128i side;
            __m128i tempL;
            __m128i tempR;
            __m128  leftf;
            __m128  rightf;

            mid    = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples0 + i), pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample);
            side   = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples1 + i), pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample);

            mid    = _mm_or_si128(_mm_slli_epi32(mid, 1), _mm_and_si128(side, _mm_set1_epi32(0x01)));

            tempL  = _mm_srai_epi32(_mm_add_epi32(mid, side), 1);
            tempR  = _mm_srai_epi32(_mm_sub_epi32(mid, side), 1);

            leftf  = _mm_mul_ps(_mm_cvtepi32_ps(tempL), factor128);
            rightf = _mm_mul_ps(_mm_cvtepi32_ps(tempR), factor128);

            _mm_storeu_ps(pOutputSamples + i*8 + 0, _mm_unpacklo_ps(leftf, rightf));
            _mm_storeu_ps(pOutputSamples + i*8 + 4, _mm_unpackhi_ps(leftf, rightf));
        }

        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint32 mid  = pInputSamples0U32[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 side = pInputSamples1U32[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = ((drflac_int32)(mid + side) >> 1) * factor;
            pOutputSamples[i*2+1] = ((drflac_int32)(mid - side) >> 1) * factor;
        }
    } else {
        shift -= 1;
        for (i = 0; i < frameCount4; ++i) {
            __m128i mid;
            __m128i side;
            __m128i tempL;
            __m128i tempR;
            __m128 leftf;
            __m128 rightf;

            mid    = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples0 + i), pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample);
            side   = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples1 + i), pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample);

            mid    = _mm_or_si128(_mm_slli_epi32(mid, 1), _mm_and_si128(side, _mm_set1_epi32(0x01)));

            tempL  = _mm_slli_epi32(_mm_add_epi32(mid, side), shift);
            tempR  = _mm_slli_epi32(_mm_sub_epi32(mid, side), shift);

            leftf  = _mm_mul_ps(_mm_cvtepi32_ps(tempL), factor128);
            rightf = _mm_mul_ps(_mm_cvtepi32_ps(tempR), factor128);

            _mm_storeu_ps(pOutputSamples + i*8 + 0, _mm_unpacklo_ps(leftf, rightf));
            _mm_storeu_ps(pOutputSamples + i*8 + 4, _mm_unpackhi_ps(leftf, rightf));
        }

        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint32 mid  = pInputSamples0U32[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 side = pInputSamples1U32[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = (drflac_int32)((mid + side) << shift) * factor;
            pOutputSamples[i*2+1] = (drflac_int32)((mid - side) << shift) * factor;
        }
    }
}
#endif

#if defined(DRFLAC_SUPPORT_NEON)
static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_mid_side__neon(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift = unusedBitsPerSample - 8;
    float factor;
    float32x4_t factor4;
    int32x4_t shift4;
    int32x4_t wbps0_4;  
    int32x4_t wbps1_4;  

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample <= 24);

    factor  = 1.0f / 8388608.0f;
    factor4 = vdupq_n_f32(factor);
    wbps0_4 = vdupq_n_s32(pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample);
    wbps1_4 = vdupq_n_s32(pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample);

    if (shift == 0) {
        for (i = 0; i < frameCount4; ++i) {
            int32x4_t lefti;
            int32x4_t righti;
            float32x4_t leftf;
            float32x4_t rightf;

            uint32x4_t mid  = vshlq_u32(vld1q_u32(pInputSamples0U32 + i*4), wbps0_4);
            uint32x4_t side = vshlq_u32(vld1q_u32(pInputSamples1U32 + i*4), wbps1_4);

            mid    = vorrq_u32(vshlq_n_u32(mid, 1), vandq_u32(side, vdupq_n_u32(1)));

            lefti  = vshrq_n_s32(vreinterpretq_s32_u32(vaddq_u32(mid, side)), 1);
            righti = vshrq_n_s32(vreinterpretq_s32_u32(vsubq_u32(mid, side)), 1);

            leftf  = vmulq_f32(vcvtq_f32_s32(lefti),  factor4);
            rightf = vmulq_f32(vcvtq_f32_s32(righti), factor4);

            drflac__vst2q_f32(pOutputSamples + i*8, vzipq_f32(leftf, rightf));
        }

        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint32 mid  = pInputSamples0U32[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 side = pInputSamples1U32[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = ((drflac_int32)(mid + side) >> 1) * factor;
            pOutputSamples[i*2+1] = ((drflac_int32)(mid - side) >> 1) * factor;
        }
    } else {
        shift -= 1;
        shift4 = vdupq_n_s32(shift);
        for (i = 0; i < frameCount4; ++i) {
            uint32x4_t mid;
            uint32x4_t side;
            int32x4_t lefti;
            int32x4_t righti;
            float32x4_t leftf;
            float32x4_t rightf;

            mid    = vshlq_u32(vld1q_u32(pInputSamples0U32 + i*4), wbps0_4);
            side   = vshlq_u32(vld1q_u32(pInputSamples1U32 + i*4), wbps1_4);

            mid    = vorrq_u32(vshlq_n_u32(mid, 1), vandq_u32(side, vdupq_n_u32(1)));

            lefti  = vreinterpretq_s32_u32(vshlq_u32(vaddq_u32(mid, side), shift4));
            righti = vreinterpretq_s32_u32(vshlq_u32(vsubq_u32(mid, side), shift4));

            leftf  = vmulq_f32(vcvtq_f32_s32(lefti),  factor4);
            rightf = vmulq_f32(vcvtq_f32_s32(righti), factor4);

            drflac__vst2q_f32(pOutputSamples + i*8, vzipq_f32(leftf, rightf));
        }

        for (i = (frameCount4 << 2); i < frameCount; ++i) {
            drflac_uint32 mid  = pInputSamples0U32[i] << pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
            drflac_uint32 side = pInputSamples1U32[i] << pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;

            mid = (mid << 1) | (side & 0x01);

            pOutputSamples[i*2+0] = (drflac_int32)((mid + side) << shift) * factor;
            pOutputSamples[i*2+1] = (drflac_int32)((mid - side) << shift) * factor;
        }
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_mid_side(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
#if defined(DRFLAC_SUPPORT_SSE2)
    if (drflac__gIsSSE2Supported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_f32__decode_mid_side__sse2(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#elif defined(DRFLAC_SUPPORT_NEON)
    if (drflac__gIsNEONSupported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_f32__decode_mid_side__neon(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#endif
    {
        
#if 0
        drflac_read_pcm_frames_f32__decode_mid_side__reference(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#else
        drflac_read_pcm_frames_f32__decode_mid_side__scalar(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#endif
    }
}

#if 0
static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_independent_stereo__reference(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    for (drflac_uint64 i = 0; i < frameCount; ++i) {
        pOutputSamples[i*2+0] = (float)((drflac_int32)((drflac_uint32)pInputSamples0[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample)) / 2147483648.0);
        pOutputSamples[i*2+1] = (float)((drflac_int32)((drflac_uint32)pInputSamples1[i] << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample)) / 2147483648.0);
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_independent_stereo__scalar(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample;
    drflac_uint32 shift1 = unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample;
    float factor = 1 / 2147483648.0;

    for (i = 0; i < frameCount4; ++i) {
        drflac_uint32 tempL0 = pInputSamples0U32[i*4+0] << shift0;
        drflac_uint32 tempL1 = pInputSamples0U32[i*4+1] << shift0;
        drflac_uint32 tempL2 = pInputSamples0U32[i*4+2] << shift0;
        drflac_uint32 tempL3 = pInputSamples0U32[i*4+3] << shift0;

        drflac_uint32 tempR0 = pInputSamples1U32[i*4+0] << shift1;
        drflac_uint32 tempR1 = pInputSamples1U32[i*4+1] << shift1;
        drflac_uint32 tempR2 = pInputSamples1U32[i*4+2] << shift1;
        drflac_uint32 tempR3 = pInputSamples1U32[i*4+3] << shift1;

        pOutputSamples[i*8+0] = (drflac_int32)tempL0 * factor;
        pOutputSamples[i*8+1] = (drflac_int32)tempR0 * factor;
        pOutputSamples[i*8+2] = (drflac_int32)tempL1 * factor;
        pOutputSamples[i*8+3] = (drflac_int32)tempR1 * factor;
        pOutputSamples[i*8+4] = (drflac_int32)tempL2 * factor;
        pOutputSamples[i*8+5] = (drflac_int32)tempR2 * factor;
        pOutputSamples[i*8+6] = (drflac_int32)tempL3 * factor;
        pOutputSamples[i*8+7] = (drflac_int32)tempR3 * factor;
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        pOutputSamples[i*2+0] = (drflac_int32)(pInputSamples0U32[i] << shift0) * factor;
        pOutputSamples[i*2+1] = (drflac_int32)(pInputSamples1U32[i] << shift1) * factor;
    }
}

#if defined(DRFLAC_SUPPORT_SSE2)
static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_independent_stereo__sse2(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample) - 8;
    drflac_uint32 shift1 = (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample) - 8;

    float factor = 1.0f / 8388608.0f;
    __m128 factor128 = _mm_set1_ps(factor);

    for (i = 0; i < frameCount4; ++i) {
        __m128i lefti;
        __m128i righti;
        __m128 leftf;
        __m128 rightf;

        lefti  = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples0 + i), shift0);
        righti = _mm_slli_epi32(_mm_loadu_si128((const __m128i*)pInputSamples1 + i), shift1);

        leftf  = _mm_mul_ps(_mm_cvtepi32_ps(lefti),  factor128);
        rightf = _mm_mul_ps(_mm_cvtepi32_ps(righti), factor128);

        _mm_storeu_ps(pOutputSamples + i*8 + 0, _mm_unpacklo_ps(leftf, rightf));
        _mm_storeu_ps(pOutputSamples + i*8 + 4, _mm_unpackhi_ps(leftf, rightf));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        pOutputSamples[i*2+0] = (drflac_int32)(pInputSamples0U32[i] << shift0) * factor;
        pOutputSamples[i*2+1] = (drflac_int32)(pInputSamples1U32[i] << shift1) * factor;
    }
}
#endif

#if defined(DRFLAC_SUPPORT_NEON)
static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_independent_stereo__neon(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
    drflac_uint64 i;
    drflac_uint64 frameCount4 = frameCount >> 2;
    const drflac_uint32* pInputSamples0U32 = (const drflac_uint32*)pInputSamples0;
    const drflac_uint32* pInputSamples1U32 = (const drflac_uint32*)pInputSamples1;
    drflac_uint32 shift0 = (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[0].wastedBitsPerSample) - 8;
    drflac_uint32 shift1 = (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[1].wastedBitsPerSample) - 8;

    float factor = 1.0f / 8388608.0f;
    float32x4_t factor4 = vdupq_n_f32(factor);
    int32x4_t shift0_4  = vdupq_n_s32(shift0);
    int32x4_t shift1_4  = vdupq_n_s32(shift1);

    for (i = 0; i < frameCount4; ++i) {
        int32x4_t lefti;
        int32x4_t righti;
        float32x4_t leftf;
        float32x4_t rightf;

        lefti  = vreinterpretq_s32_u32(vshlq_u32(vld1q_u32(pInputSamples0U32 + i*4), shift0_4));
        righti = vreinterpretq_s32_u32(vshlq_u32(vld1q_u32(pInputSamples1U32 + i*4), shift1_4));

        leftf  = vmulq_f32(vcvtq_f32_s32(lefti),  factor4);
        rightf = vmulq_f32(vcvtq_f32_s32(righti), factor4);

        drflac__vst2q_f32(pOutputSamples + i*8, vzipq_f32(leftf, rightf));
    }

    for (i = (frameCount4 << 2); i < frameCount; ++i) {
        pOutputSamples[i*2+0] = (drflac_int32)(pInputSamples0U32[i] << shift0) * factor;
        pOutputSamples[i*2+1] = (drflac_int32)(pInputSamples1U32[i] << shift1) * factor;
    }
}
#endif

static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode_independent_stereo(drflac* pFlac, drflac_uint64 frameCount, drflac_uint32 unusedBitsPerSample, const drflac_int32* pInputSamples0, const drflac_int32* pInputSamples1, float* pOutputSamples)
{
#if defined(DRFLAC_SUPPORT_SSE2)
    if (drflac__gIsSSE2Supported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_f32__decode_independent_stereo__sse2(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#elif defined(DRFLAC_SUPPORT_NEON)
    if (drflac__gIsNEONSupported && pFlac->currentFLACFrame.header.bitsPerSample <= 24) {
        drflac_read_pcm_frames_f32__decode_independent_stereo__neon(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
    } else
#endif
    {
        
#if 0
        drflac_read_pcm_frames_f32__decode_independent_stereo__reference(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#else
        drflac_read_pcm_frames_f32__decode_independent_stereo__scalar(pFlac, frameCount, unusedBitsPerSample, pInputSamples0, pInputSamples1, pOutputSamples);
#endif
    }
}

static DRFLAC_INLINE void drflac_read_pcm_frames_f32__decode(drflac* pFlac, drflac_uint64 frameCount, drflac_uint64 iFirstPCMFrame, float* pOutputSamples)
{
    unsigned int channelCount;
    drflac_uint32 unusedBitsPerSample;

    DRFLAC_ASSERT(pFlac->currentFLACFrame.header.bitsPerSample < 32);

    channelCount = drflac__get_channel_count_from_channel_assignment(pFlac->currentFLACFrame.header.channelAssignment);
    unusedBitsPerSample = 32 - pFlac->currentFLACFrame.header.bitsPerSample;

    if (channelCount == 2) {
        const drflac_int32* pDecodedSamples0 = pFlac->currentFLACFrame.subframes[0].samples.pS32 + iFirstPCMFrame;
        const drflac_int32* pDecodedSamples1 = pFlac->currentFLACFrame.subframes[1].samples.pS32 + iFirstPCMFrame;

        switch (pFlac->currentFLACFrame.header.channelAssignment)
        {
            case DRFLAC_CHANNEL_ASSIGNMENT_LEFT_SIDE:
            {
                drflac_read_pcm_frames_f32__decode_left_side(pFlac, frameCount, unusedBitsPerSample, pDecodedSamples0, pDecodedSamples1, pOutputSamples);
            } break;

            case DRFLAC_CHANNEL_ASSIGNMENT_RIGHT_SIDE:
            {
                drflac_read_pcm_frames_f32__decode_right_side(pFlac, frameCount, unusedBitsPerSample, pDecodedSamples0, pDecodedSamples1, pOutputSamples);
            } break;

            case DRFLAC_CHANNEL_ASSIGNMENT_MID_SIDE:
            {
                drflac_read_pcm_frames_f32__decode_mid_side(pFlac, frameCount, unusedBitsPerSample, pDecodedSamples0, pDecodedSamples1, pOutputSamples);
            } break;

            case DRFLAC_CHANNEL_ASSIGNMENT_INDEPENDENT_STEREO:
            {
                drflac_read_pcm_frames_f32__decode_independent_stereo(pFlac, frameCount, unusedBitsPerSample, pDecodedSamples0, pDecodedSamples1, pOutputSamples);
            } break;

            default: DRFLAC_ASSERT(!"Invalid stereo channel assignment"); break;
        }
    } else {
        
        drflac_uint64 i;

        for (i = 0; i < frameCount; ++i) {
            unsigned int j;

            for (j = 0; j < channelCount; ++j) {
                drflac_int32 sampleS32;

                sampleS32 = (drflac_int32)((drflac_uint32)(pFlac->currentFLACFrame.subframes[j].samples.pS32[iFirstPCMFrame + i]) << (unusedBitsPerSample + pFlac->currentFLACFrame.subframes[j].wastedBitsPerSample));
                pOutputSamples[(i*channelCount)+j] = (float)(sampleS32 / 2147483648.0);
            }
        }
    }
}

DRFLAC_API drflac_uint64 drflac_read_pcm_frames_f32(drflac* pFlac, drflac_uint64 framesToRead, float* pBufferOut)
{
    drflac_uint64 framesRead;

    if (pFlac == NULL || framesToRead == 0) {
        return 0;
    }

    if (pBufferOut == NULL) {
        return drflac__seek_forward_by_pcm_frames(pFlac, framesToRead);
    }

    DRFLAC_ASSERT(pFlac->bitsPerSample <= 32);

    framesRead = 0;
    while (framesToRead > 0) {
        
        if (pFlac->currentFLACFrame.pcmFramesRemaining == 0) {
            if (!drflac__read_and_decode_next_flac_frame(pFlac)) {
                break;  
            }
        } else {
            unsigned int channelCount = drflac__get_channel_count_from_channel_assignment(pFlac->currentFLACFrame.header.channelAssignment);
            drflac_uint64 iFirstPCMFrame = pFlac->currentFLACFrame.header.blockSizeInPCMFrames - pFlac->currentFLACFrame.pcmFramesRemaining;
            drflac_uint64 frameCountThisIteration = framesToRead;

            if (frameCountThisIteration > pFlac->currentFLACFrame.pcmFramesRemaining) {
                frameCountThisIteration = pFlac->currentFLACFrame.pcmFramesRemaining;
            }

            if (pFlac->bitsPerSample != 32) {
                drflac_read_pcm_frames_f32__decode(pFlac, frameCountThisIteration, iFirstPCMFrame, pBufferOut);
            } else {
                drflac_read_pcm_frames_f32__decode_s64(pFlac, frameCountThisIteration, iFirstPCMFrame, pBufferOut);
            }

            framesRead             += frameCountThisIteration;
            pBufferOut             += frameCountThisIteration * channelCount;
            framesToRead           -= frameCountThisIteration;
            pFlac->currentPCMFrame += frameCountThisIteration;
            pFlac->currentFLACFrame.pcmFramesRemaining -= (unsigned int)frameCountThisIteration;
        }
    }

    return framesRead;
}


DRFLAC_API drflac_bool32 drflac_seek_to_pcm_frame(drflac* pFlac, drflac_uint64 pcmFrameIndex)
{
    if (pFlac == NULL) {
        return DRFLAC_FALSE;
    }

    
    if (pFlac->currentPCMFrame == pcmFrameIndex) {
        return DRFLAC_TRUE;
    }

    if (pcmFrameIndex == 0) {
        pFlac->currentPCMFrame = 0;
        return drflac__seek_to_first_frame(pFlac);
    } else {
        drflac_bool32 wasSuccessful = DRFLAC_FALSE;
        drflac_uint64 originalPCMFrame = pFlac->currentPCMFrame;

        
        if (pFlac->totalPCMFrameCount != 0 && pcmFrameIndex > pFlac->totalPCMFrameCount) {
            pcmFrameIndex = pFlac->totalPCMFrameCount;
        }

        
        if (drflac__is_current_flac_frame_valid(pFlac)) {
            if (pcmFrameIndex > pFlac->currentPCMFrame) {
                
                drflac_uint64 offset = pcmFrameIndex - pFlac->currentPCMFrame;
                if (pFlac->currentFLACFrame.pcmFramesRemaining > offset) {
                    pFlac->currentFLACFrame.pcmFramesRemaining -= (drflac_uint32)offset;
                    pFlac->currentPCMFrame = pcmFrameIndex;
                    return DRFLAC_TRUE;
                }
            } else {
                
                drflac_uint64 offsetAbs = pFlac->currentPCMFrame - pcmFrameIndex;
                drflac_uint32 currentFLACFramePCMFrameCount = pFlac->currentFLACFrame.header.blockSizeInPCMFrames;
                drflac_uint32 currentFLACFramePCMFramesConsumed = currentFLACFramePCMFrameCount - pFlac->currentFLACFrame.pcmFramesRemaining;
                if (currentFLACFramePCMFramesConsumed > offsetAbs) {
                    pFlac->currentFLACFrame.pcmFramesRemaining += (drflac_uint32)offsetAbs;
                    pFlac->currentPCMFrame = pcmFrameIndex;
                    return DRFLAC_TRUE;
                }
            }
        }

        



#ifndef DR_FLAC_NO_OGG
        if (pFlac->container == drflac_container_ogg)
        {
            wasSuccessful = drflac_ogg__seek_to_pcm_frame(pFlac, pcmFrameIndex);
        }
        else
#endif
        {
            
            if (!pFlac->_noSeekTableSeek) {
                wasSuccessful = drflac__seek_to_pcm_frame__seek_table(pFlac, pcmFrameIndex);
            }

#if !defined(DR_FLAC_NO_CRC)
            
            if (!wasSuccessful && !pFlac->_noBinarySearchSeek && pFlac->totalPCMFrameCount > 0) {
                wasSuccessful = drflac__seek_to_pcm_frame__binary_search(pFlac, pcmFrameIndex);
            }
#endif

            
            if (!wasSuccessful && !pFlac->_noBruteForceSeek) {
                wasSuccessful = drflac__seek_to_pcm_frame__brute_force(pFlac, pcmFrameIndex);
            }
        }

        if (wasSuccessful) {
            pFlac->currentPCMFrame = pcmFrameIndex;
        } else {
            
            drflac__seek_to_first_frame(pFlac);
            if (originalPCMFrame != 0 && drflac_seek_to_pcm_frame(pFlac, originalPCMFrame) == DRFLAC_FALSE) {
                
                drflac__seek_to_first_frame(pFlac);
            }
        }

        return wasSuccessful;
    }
}






#if defined(SIZE_MAX)
    #define DRFLAC_SIZE_MAX  SIZE_MAX
#else
    #if defined(DRFLAC_64BIT)
        #define DRFLAC_SIZE_MAX  ((drflac_uint64)0xFFFFFFFFFFFFFFFF)
    #else
        #define DRFLAC_SIZE_MAX  0xFFFFFFFF
    #endif
#endif




#define DRFLAC_DEFINE_FULL_READ_AND_CLOSE(extension, type) \
static type* drflac__full_read_and_close_ ## extension (drflac* pFlac, unsigned int* channelsOut, unsigned int* sampleRateOut, drflac_uint64* totalPCMFrameCountOut)\
{                                                                                                                                                                   \
    type* pSampleData = NULL;                                                                                                                                       \
    drflac_uint64 totalPCMFrameCount;                                                                                                                               \
    type buffer[4096];                                                                                                                                              \
    drflac_uint64 pcmFramesRead;                                                                                                                                    \
    size_t sampleDataBufferSize = sizeof(buffer);                                                                                                                   \
                                                                                                                                                                    \
    DRFLAC_ASSERT(pFlac != NULL);                                                                                                                                   \
                                                                                                                                                                    \
    totalPCMFrameCount = 0;                                                                                                                                         \
                                                                                                                                                                    \
    pSampleData = (type*)drflac__malloc_from_callbacks(sampleDataBufferSize, &pFlac->allocationCallbacks);                                                          \
    if (pSampleData == NULL) {                                                                                                                                      \
        goto on_error;                                                                                                                                              \
    }                                                                                                                                                               \
                                                                                                                                                                    \
    while ((pcmFramesRead = (drflac_uint64)drflac_read_pcm_frames_##extension(pFlac, sizeof(buffer)/sizeof(buffer[0])/pFlac->channels, buffer)) > 0) {              \
        if (((totalPCMFrameCount + pcmFramesRead) * pFlac->channels * sizeof(type)) > sampleDataBufferSize) {                                                       \
            type* pNewSampleData;                                                                                                                                   \
            size_t newSampleDataBufferSize;                                                                                                                         \
                                                                                                                                                                    \
            newSampleDataBufferSize = sampleDataBufferSize * 2;                                                                                                     \
            pNewSampleData = (type*)drflac__realloc_from_callbacks(pSampleData, newSampleDataBufferSize, sampleDataBufferSize, &pFlac->allocationCallbacks);        \
            if (pNewSampleData == NULL) {                                                                                                                           \
                drflac__free_from_callbacks(pSampleData, &pFlac->allocationCallbacks);                                                                              \
                goto on_error;                                                                                                                                      \
            }                                                                                                                                                       \
                                                                                                                                                                    \
            sampleDataBufferSize = newSampleDataBufferSize;                                                                                                         \
            pSampleData = pNewSampleData;                                                                                                                           \
        }                                                                                                                                                           \
                                                                                                                                                                    \
        DRFLAC_COPY_MEMORY(pSampleData + (totalPCMFrameCount*pFlac->channels), buffer, (size_t)(pcmFramesRead*pFlac->channels*sizeof(type)));                       \
        totalPCMFrameCount += pcmFramesRead;                                                                                                                        \
    }                                                                                                                                                               \
                                                                                                                                                                    \
    \
                                                                                                                     \
    DRFLAC_ZERO_MEMORY(pSampleData + (totalPCMFrameCount*pFlac->channels), (size_t)(sampleDataBufferSize - totalPCMFrameCount*pFlac->channels*sizeof(type)));       \
                                                                                                                                                                    \
    if (sampleRateOut) *sampleRateOut = pFlac->sampleRate;                                                                                                          \
    if (channelsOut) *channelsOut = pFlac->channels;                                                                                                                \
    if (totalPCMFrameCountOut) *totalPCMFrameCountOut = totalPCMFrameCount;                                                                                         \
                                                                                                                                                                    \
    drflac_close(pFlac);                                                                                                                                            \
    return pSampleData;                                                                                                                                             \
                                                                                                                                                                    \
on_error:                                                                                                                                                           \
    drflac_close(pFlac);                                                                                                                                            \
    return NULL;                                                                                                                                                    \
}

DRFLAC_DEFINE_FULL_READ_AND_CLOSE(s32, drflac_int32)
DRFLAC_DEFINE_FULL_READ_AND_CLOSE(s16, drflac_int16)
DRFLAC_DEFINE_FULL_READ_AND_CLOSE(f32, float)

DRFLAC_API drflac_int32* drflac_open_and_read_pcm_frames_s32(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, void* pUserData, unsigned int* channelsOut, unsigned int* sampleRateOut, drflac_uint64* totalPCMFrameCountOut, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    drflac* pFlac;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalPCMFrameCountOut) {
        *totalPCMFrameCountOut = 0;
    }

    pFlac = drflac_open(onRead, onSeek, onTell, pUserData, pAllocationCallbacks);
    if (pFlac == NULL) {
        return NULL;
    }

    return drflac__full_read_and_close_s32(pFlac, channelsOut, sampleRateOut, totalPCMFrameCountOut);
}

DRFLAC_API drflac_int16* drflac_open_and_read_pcm_frames_s16(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, void* pUserData, unsigned int* channelsOut, unsigned int* sampleRateOut, drflac_uint64* totalPCMFrameCountOut, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    drflac* pFlac;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalPCMFrameCountOut) {
        *totalPCMFrameCountOut = 0;
    }

    pFlac = drflac_open(onRead, onSeek, onTell, pUserData, pAllocationCallbacks);
    if (pFlac == NULL) {
        return NULL;
    }

    return drflac__full_read_and_close_s16(pFlac, channelsOut, sampleRateOut, totalPCMFrameCountOut);
}

DRFLAC_API float* drflac_open_and_read_pcm_frames_f32(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_tell_proc onTell, void* pUserData, unsigned int* channelsOut, unsigned int* sampleRateOut, drflac_uint64* totalPCMFrameCountOut, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    drflac* pFlac;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalPCMFrameCountOut) {
        *totalPCMFrameCountOut = 0;
    }

    pFlac = drflac_open(onRead, onSeek, onTell, pUserData, pAllocationCallbacks);
    if (pFlac == NULL) {
        return NULL;
    }

    return drflac__full_read_and_close_f32(pFlac, channelsOut, sampleRateOut, totalPCMFrameCountOut);
}

#ifndef DR_FLAC_NO_STDIO
DRFLAC_API drflac_int32* drflac_open_file_and_read_pcm_frames_s32(const char* filename, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    drflac* pFlac;

    if (sampleRate) {
        *sampleRate = 0;
    }
    if (channels) {
        *channels = 0;
    }
    if (totalPCMFrameCount) {
        *totalPCMFrameCount = 0;
    }

    pFlac = drflac_open_file(filename, pAllocationCallbacks);
    if (pFlac == NULL) {
        return NULL;
    }

    return drflac__full_read_and_close_s32(pFlac, channels, sampleRate, totalPCMFrameCount);
}

DRFLAC_API drflac_int16* drflac_open_file_and_read_pcm_frames_s16(const char* filename, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    drflac* pFlac;

    if (sampleRate) {
        *sampleRate = 0;
    }
    if (channels) {
        *channels = 0;
    }
    if (totalPCMFrameCount) {
        *totalPCMFrameCount = 0;
    }

    pFlac = drflac_open_file(filename, pAllocationCallbacks);
    if (pFlac == NULL) {
        return NULL;
    }

    return drflac__full_read_and_close_s16(pFlac, channels, sampleRate, totalPCMFrameCount);
}

DRFLAC_API float* drflac_open_file_and_read_pcm_frames_f32(const char* filename, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    drflac* pFlac;

    if (sampleRate) {
        *sampleRate = 0;
    }
    if (channels) {
        *channels = 0;
    }
    if (totalPCMFrameCount) {
        *totalPCMFrameCount = 0;
    }

    pFlac = drflac_open_file(filename, pAllocationCallbacks);
    if (pFlac == NULL) {
        return NULL;
    }

    return drflac__full_read_and_close_f32(pFlac, channels, sampleRate, totalPCMFrameCount);
}
#endif

DRFLAC_API drflac_int32* drflac_open_memory_and_read_pcm_frames_s32(const void* data, size_t dataSize, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    drflac* pFlac;

    if (sampleRate) {
        *sampleRate = 0;
    }
    if (channels) {
        *channels = 0;
    }
    if (totalPCMFrameCount) {
        *totalPCMFrameCount = 0;
    }

    pFlac = drflac_open_memory(data, dataSize, pAllocationCallbacks);
    if (pFlac == NULL) {
        return NULL;
    }

    return drflac__full_read_and_close_s32(pFlac, channels, sampleRate, totalPCMFrameCount);
}

DRFLAC_API drflac_int16* drflac_open_memory_and_read_pcm_frames_s16(const void* data, size_t dataSize, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    drflac* pFlac;

    if (sampleRate) {
        *sampleRate = 0;
    }
    if (channels) {
        *channels = 0;
    }
    if (totalPCMFrameCount) {
        *totalPCMFrameCount = 0;
    }

    pFlac = drflac_open_memory(data, dataSize, pAllocationCallbacks);
    if (pFlac == NULL) {
        return NULL;
    }

    return drflac__full_read_and_close_s16(pFlac, channels, sampleRate, totalPCMFrameCount);
}

DRFLAC_API float* drflac_open_memory_and_read_pcm_frames_f32(const void* data, size_t dataSize, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    drflac* pFlac;

    if (sampleRate) {
        *sampleRate = 0;
    }
    if (channels) {
        *channels = 0;
    }
    if (totalPCMFrameCount) {
        *totalPCMFrameCount = 0;
    }

    pFlac = drflac_open_memory(data, dataSize, pAllocationCallbacks);
    if (pFlac == NULL) {
        return NULL;
    }

    return drflac__full_read_and_close_f32(pFlac, channels, sampleRate, totalPCMFrameCount);
}


DRFLAC_API void drflac_free(void* p, const drflac_allocation_callbacks* pAllocationCallbacks)
{
    if (pAllocationCallbacks != NULL) {
        drflac__free_from_callbacks(p, pAllocationCallbacks);
    } else {
        drflac__free_default(p, NULL);
    }
}




DRFLAC_API void drflac_init_vorbis_comment_iterator(drflac_vorbis_comment_iterator* pIter, drflac_uint32 commentCount, const void* pComments)
{
    if (pIter == NULL) {
        return;
    }

    pIter->countRemaining = commentCount;
    pIter->pRunningData   = (const char*)pComments;
}

DRFLAC_API const char* drflac_next_vorbis_comment(drflac_vorbis_comment_iterator* pIter, drflac_uint32* pCommentLengthOut)
{
    drflac_int32 length;
    const char* pComment;

    
    if (pCommentLengthOut) {
        *pCommentLengthOut = 0;
    }

    if (pIter == NULL || pIter->countRemaining == 0 || pIter->pRunningData == NULL) {
        return NULL;
    }

    length = drflac__le2host_32_ptr_unaligned(pIter->pRunningData);
    pIter->pRunningData += 4;

    pComment = pIter->pRunningData;
    pIter->pRunningData += length;
    pIter->countRemaining -= 1;

    if (pCommentLengthOut) {
        *pCommentLengthOut = length;
    }

    return pComment;
}




DRFLAC_API void drflac_init_cuesheet_track_iterator(drflac_cuesheet_track_iterator* pIter, drflac_uint32 trackCount, const void* pTrackData)
{
    if (pIter == NULL) {
        return;
    }

    pIter->countRemaining = trackCount;
    pIter->pRunningData   = (const char*)pTrackData;
}

DRFLAC_API drflac_bool32 drflac_next_cuesheet_track(drflac_cuesheet_track_iterator* pIter, drflac_cuesheet_track* pCuesheetTrack)
{
    drflac_cuesheet_track cuesheetTrack;
    const char* pRunningData;
    drflac_uint64 offsetHi;
    drflac_uint64 offsetLo;

    if (pIter == NULL || pIter->countRemaining == 0 || pIter->pRunningData == NULL) {
        return DRFLAC_FALSE;
    }

    pRunningData = pIter->pRunningData;

    offsetHi                   = drflac__be2host_32(*(const drflac_uint32*)pRunningData); pRunningData += 4;
    offsetLo                   = drflac__be2host_32(*(const drflac_uint32*)pRunningData); pRunningData += 4;
    cuesheetTrack.offset       = offsetLo | (offsetHi << 32);
    cuesheetTrack.trackNumber  = pRunningData[0];                                         pRunningData += 1;
    DRFLAC_COPY_MEMORY(cuesheetTrack.ISRC, pRunningData, sizeof(cuesheetTrack.ISRC));     pRunningData += 12;
    cuesheetTrack.isAudio      = (pRunningData[0] & 0x80) != 0;
    cuesheetTrack.preEmphasis  = (pRunningData[0] & 0x40) != 0;                           pRunningData += 14;
    cuesheetTrack.indexCount   = pRunningData[0];                                         pRunningData += 1 + (DRFLAC_CUESHEET_TRACK_PADDED_SIZE_IN_BYTES - DRFLAC_CUESHEET_TRACK_SIZE_IN_BYTES);
    cuesheetTrack.pIndexPoints = (const drflac_cuesheet_track_index*)pRunningData;        pRunningData += cuesheetTrack.indexCount * sizeof(drflac_cuesheet_track_index);

    pIter->pRunningData = pRunningData;
    pIter->countRemaining -= 1;

    if (pCuesheetTrack) {
        *pCuesheetTrack = cuesheetTrack;
    }

    return DRFLAC_TRUE;
}

#if defined(__clang__) || (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))
    #pragma GCC diagnostic pop
#endif
#endif  
#endif  


















































