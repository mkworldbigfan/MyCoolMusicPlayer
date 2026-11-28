#ifndef M4A_DEMUX_H
#define M4A_DEMUX_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    uint32_t count;
    uint32_t delta;
} M4ASttsEntry;

typedef struct {
    FILE* file;
    uint32_t timescale;
    uint64_t durationUnits;
    float durationSec;
    uint32_t channels;
    uint32_t sampleRate;

    uint8_t asc[64];
    size_t ascLen;

    uint32_t sampleCount;
    uint32_t currentSample;
    uint32_t* sampleOffsets;
    uint32_t* sampleSizes;

    M4ASttsEntry* stts;
    uint32_t sttsCount;

    char title[256];
    char artist[256];
    char album[256];
    float bpm;
    bool hasBpm;

    uint8_t* coverData;
    size_t coverSize;
} M4ADemuxer;

//m4a thing new om g
bool m4a_open(M4ADemuxer* d, const char* path);

void m4a_close(M4ADemuxer* d);

uint32_t m4a_read_sample(M4ADemuxer* d, uint8_t* dst, size_t maxBytes);

float m4a_seek(M4ADemuxer* d, float seconds);

bool m4a_read_metadata(const char* path,
                       char* title, size_t titleSz,
                       char* artist, size_t artistSz,
                       char* album, size_t albumSz,
                       float* outBpm, bool* outHasBpm,
                       uint8_t** outCover, size_t* outCoverSize);

#endif
