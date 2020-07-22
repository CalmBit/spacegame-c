#ifndef _SPC_WAV_H
#define _SPC_WAV_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct wav_data_t {
    char* buffer;
    uint32_t len;
} wav_data_t;

typedef struct wav_t {
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    wav_data_t data;
} wav_t;

wav_t* wav_load(const char* path);
void wav_destroy(wav_t* wav);

#endif
