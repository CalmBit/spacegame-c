#include "wav.h"

#include "memory.h"
#include "error.h"
#include "file.h"

#include <string.h>


wav_t* wav_load(const char* path) {
    file_t* file;
    wav_t* wav;
    uint32_t data_len;
    char tag_buffer[5];
    uint16_t short_buffer;
    uint32_t int_buffer;

    wav = memory_alloc(SPC_MU_SOUND, sizeof(wav_t));

    file = file_load(path, "r");
    
    file_read_string(file, tag_buffer, 4);

    if(strcmp("RIFF", tag_buffer)) {
        file_destroy(file);
        memory_free(wav);
        error("loaded wav file '%s' not a valid RIFF file, '%s'", path, tag_buffer);
    }

    // skip this
    fread(&int_buffer, 4, 1, file->handle);

    file_read_string(file, tag_buffer, 4);

    if(strcmp("WAVE", tag_buffer)) {
        file_destroy(file);
        memory_free(wav);
        error("loaded wav file '%s' not a valid WAVE file, '%s'", 
                path, tag_buffer);
    }

    file_read_string(file, tag_buffer, 4);

    if(strcmp("fmt ", tag_buffer)) {
        file_destroy(file);
        memory_free(wav);
        error("wav file '%s' malformed: 'fmt ' header corrupt, '%s'", 
                path,  tag_buffer);
    }

    fread(&int_buffer, 4, 1, file->handle);

    if(int_buffer != 16) {
        file_destroy(file);
        memory_free(wav);
        error("wav file '%s' malformed: subchunk1 size '%i' not '16'", 
                path,  int_buffer);
    }

    fread(&short_buffer, 2, 1, file->handle);

    if(short_buffer != 0x0001) {
        file_destroy(file);
        memory_free(wav);
        error("wav file '%s' malformed: audio format is '%i', not PCM (1)", 
                path,  short_buffer);
    }

    fread(&wav->num_channels, 2, 1, file->handle);
    fread(&wav->sample_rate, 4, 1, file->handle);
    fread(&wav->byte_rate, 4, 1, file->handle);
    fread(&wav->block_align, 2, 1, file->handle);
    fread(&wav->bits_per_sample, 2, 1, file->handle);

    file_read_string(file, tag_buffer, 4);

    if(strcmp("data", tag_buffer)) {
        file_destroy(file);
        memory_free(wav);
        error("wav file '%s' malformed: 'data' header corrupt, '%s'", 
                path,  tag_buffer);
    }

    fread(&data_len, 4, 1, file->handle);

    size_t actual_len = data_len;
    wav->data.buffer = memory_alloc(SPC_MU_SOUND, actual_len);
    wav->data.len = 0;
    wav->data.len = data_len;

    fread(wav->data.buffer, 1, data_len, file->handle);

    file_destroy(file);

    return wav;

}

void wav_destroy(wav_t* wav) {
    memory_free(wav->data.buffer);
    memory_free(wav);
}
