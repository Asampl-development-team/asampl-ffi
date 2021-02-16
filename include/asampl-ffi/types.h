#pragma once

#define ASAMPL_VIDEO_FRAME_PIXEL_SIZE 3

#include <stdint.h>

typedef enum {
    ASA_UNDEFINED = 0,
    ASA_DOUBLE,
    ASA_STRING,
    ASA_VIDEO_FRAME,
    ASA_ARRAY,
    ASA_BYTES,
} AsaType;

typedef struct {
    AsaType type;
    double timestamp;
    void* data;
} AsaValueContainer;

typedef struct {
    double value;
} AsaDouble;

typedef struct {
    uint32_t size;
    char* data;
} AsaString;

typedef struct {
    uint32_t size;
    uint8_t* data;
} AsaBytes;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t* data;
} AsaVideoFrame;

typedef struct {
    uint32_t size;
    AsaValueContainer* items;
} AsaArray;

AsaValueContainer* asa_alloc_container();

void asa_new_double(double value, AsaValueContainer* container);

void asa_new_string_take(char* value, uint32_t size, AsaValueContainer* container);
void asa_new_string_copy(const char* value, uint32_t size, AsaValueContainer* container);

void asa_new_video_frame_take(uint8_t* data, uint32_t width, uint32_t height, AsaValueContainer* container);
void asa_new_video_frame_copy(const uint8_t* data, uint32_t width, uint32_t height, AsaValueContainer* container);
uint32_t asa_video_frame_size(uint32_t width, uint32_t height);

void asa_new_array_take(AsaValueContainer items[], uint32_t size, AsaValueContainer* container);
void asa_new_array_copy(const AsaValueContainer items[], uint32_t size, AsaValueContainer* container);

void asa_clone(const AsaValueContainer* src, AsaValueContainer* dst);

void asa_deinit_container(AsaValueContainer* container);
