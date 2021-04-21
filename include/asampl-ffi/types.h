#pragma once

#define ASAMPL_VIDEO_FRAME_PIXEL_SIZE 3

#include <stdint.h>

typedef enum {
    ASA_UNDEFINED = 0,
    ASA_NUMBER,
    ASA_STRING,
    ASA_IMAGE,
    ASA_ARRAY,
    ASA_BYTES,
    ASA_KV_LIST,
} AsaType;

typedef struct {
    AsaType type;
    double timestamp;
    void* data;
} AsaValueContainer;

typedef struct {
    double value;
} AsaNumber;

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
} AsaImage;

typedef struct {
    uint32_t size;
    AsaValueContainer* items;
} AsaArray;

typedef struct {
    AsaValueContainer key;
    AsaValueContainer value;
} AsaKV;

typedef struct {
    uint32_t size;
    AsaKV* pairs;
} AsaKVList;

AsaValueContainer* asa_alloc_container();

void asa_new_number(double value, AsaValueContainer* container);

void asa_new_string_take(char* value, uint32_t size, AsaValueContainer* container);
void asa_new_string_copy(const char* value, uint32_t size, AsaValueContainer* container);

void asa_new_image_take(uint8_t* data, uint32_t width, uint32_t height, AsaValueContainer* container);
void asa_new_image_copy(const uint8_t* data, uint32_t width, uint32_t height, AsaValueContainer* container);
uint32_t asa_image_size(uint32_t width, uint32_t height);

void asa_new_array_take(AsaValueContainer items[], uint32_t size, AsaValueContainer* container);
void asa_new_array_copy(const AsaValueContainer items[], uint32_t size, AsaValueContainer* container);

void asa_new_kv_list_take(AsaKV pairs[], uint32_t size, AsaValueContainer* container);
void asa_new_kv_list_copy(AsaKV pairs[], uint32_t size, AsaValueContainer* container);
AsaValueContainer* asa_kv_list_find_skey(AsaKVList* list, const char* key);
AsaString* asa_kv_list_find_skey_svalue(AsaKVList* list, const char* key);
AsaNumber* asa_kv_list_find_skey_nvalue(AsaKVList* list, const char* key);

void asa_clone(const AsaValueContainer* src, AsaValueContainer* dst);

void asa_deinit_container(AsaValueContainer* container);
