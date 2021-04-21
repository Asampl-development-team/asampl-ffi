#include "asampl-ffi/types.h"
#include <asampl-ffi/ffi.h>

#include <string.h>


AsaValueContainer* asa_alloc_container() {
    AsaValueContainer* container = asa_alloc(sizeof(AsaValueContainer));
    container->type = ASA_UNDEFINED;
    container->timestamp = 0.0;
    container->data = NULL;
    return container;
}

void asa_new_number(double value, AsaValueContainer* container) {
    container->type = ASA_NUMBER;
    container->data = asa_alloc(sizeof(AsaNumber));
    ((AsaNumber*)container->data)->value = value;
}

void asa_new_string_take(char* value, uint32_t size, AsaValueContainer* container) {
    container->type = ASA_STRING;

    AsaString* string = asa_alloc(sizeof(AsaString));
    container->data = string;

    string->size = size;
    string->data = value;
}

void asa_new_string_copy(const char* value, uint32_t size, AsaValueContainer* container)
{
    char* buf = asa_alloc(size);
    memcpy(buf, value, size);

    asa_new_string_take(buf, size, container);
}

void asa_new_image_take(uint8_t* data, uint32_t width, uint32_t height, AsaValueContainer* container) {
    container->type = ASA_IMAGE;

    AsaImage* frame = asa_alloc(sizeof(AsaImage));
    container->data = frame;

    frame->width = width;
    frame->height = height;
    frame->data = data;
}

void asa_new_image_copy(const uint8_t* data, uint32_t width, uint32_t height, AsaValueContainer* container) {
    uint8_t* buf = asa_alloc(asa_image_size(width, height));
    memcpy(buf, data, asa_image_size(width, height));

    asa_new_image_take(buf, width, height, container);
}

uint32_t asa_image_size(uint32_t width, uint32_t height) {
    return width * height * ASAMPL_VIDEO_FRAME_PIXEL_SIZE;
}

void asa_new_array_take(AsaValueContainer items[], uint32_t size, AsaValueContainer* container) {
    container->type = ASA_ARRAY;

    AsaArray* array = asa_alloc(sizeof(AsaArray));
    container->data = array;

    array->size = size;
    array->items = items;
}

void asa_new_array_copy(const AsaValueContainer items[], uint32_t size, AsaValueContainer* container) {
    AsaValueContainer* buf = asa_alloc(sizeof(AsaValueContainer) * size);
    for (uint32_t i = 0; i < size; i++) {
        asa_clone(&items[i], &buf[i]);
    }

    asa_new_array_take(buf, size, container);
}

void asa_new_kv_list_take(AsaKV pairs[], uint32_t size, AsaValueContainer *container) {
    container->type = ASA_KV_LIST;

    AsaKVList* list = asa_alloc(sizeof(AsaKVList));
    container->data = list;

    list->size = size;
    list->pairs = pairs;
}

void asa_new_kv_list_copy(AsaKV pairs[], uint32_t size, AsaValueContainer *container) {
    AsaKV* buf = asa_alloc(sizeof(AsaKV) * size);
    for (uint32_t i = 0; i < size; i++) {
        asa_clone(&pairs[i].key, &buf[i].key);
        asa_clone(&pairs[i].value, &buf[i].value);
    }

    asa_new_kv_list_take(buf, size, container);
}

AsaValueContainer* asa_kv_list_find_skey(AsaKVList* list, const char* target_key) {
    const size_t key_len = strlen(target_key);

    for (uint32_t i = 0; i < list->size; i++) {
        AsaValueContainer* key = &list->pairs[i].key;
        if (key->type == ASA_STRING) {
            AsaString* string = key->data;
            if (key_len == string->size && strncmp(target_key, string->data, key_len) == 0) {
                return &list->pairs[i].value;
            }
        }
    }

    return NULL;
}

AsaString* asa_kv_list_find_skey_svalue(AsaKVList* list, const char* key) {
    AsaValueContainer* value = asa_kv_list_find_skey(list, key);
    if (value == NULL || value->type != ASA_STRING) {
        return NULL;
    }

    return value->data;
}

AsaNumber* asa_kv_list_find_skey_nvalue(AsaKVList* list, const char* key) {
    AsaValueContainer* value = asa_kv_list_find_skey(list, key);
    if (value == NULL || value->type != ASA_NUMBER) {
        return NULL;
    }

    return value->data;
}

void asa_clone(const AsaValueContainer* src, AsaValueContainer* dst) {
    dst->type = src->type;
    dst->timestamp = src->timestamp;

    switch (src->type) {
        case ASA_NUMBER:
            asa_new_number(((AsaNumber*)src->data)->value, dst);
            break;
        case ASA_STRING:
        case ASA_BYTES:
            {
                AsaString* string = src->data;
                asa_new_string_copy(string->data, string->size, dst);
            }
            break;

        case ASA_IMAGE:
            {
                AsaImage* frame = src->data;
                asa_new_image_copy(frame->data, frame->width, frame->height, dst);
            }
            break;

        case ASA_ARRAY:
            {
                AsaArray* array = src->data;
                asa_new_array_copy(array->items, array->size, dst);
            }
            break;

        case ASA_KV_LIST:
            {
                AsaKVList* list = src->data;
                asa_new_kv_list_copy(list->pairs, list->size, dst);
            }
            break;

        default:
            break;
    }
}

void asa_deinit_container(AsaValueContainer* container) {
    switch (container->type) {
        case ASA_NUMBER:
            break;
        case ASA_STRING:
        case ASA_BYTES:
            asa_free(((AsaString*)container->data)->data);
            break;
        case ASA_IMAGE:
            asa_free(((AsaImage*)container->data)->data);
            break;
        case ASA_ARRAY:
            {
                AsaArray* array = container->data;
                for (uint32_t i = 0; i < array->size; i++) {
                    asa_deinit_container(&array->items[i]);
                }
                asa_free(array->items);
            }
            break;
        case ASA_KV_LIST:
            {
                AsaKVList* list = container->data;
                for (uint32_t i = 0; i < list->size; i++) {
                    asa_deinit_container(&list->pairs[i].key);
                    asa_deinit_container(&list->pairs[i].value);
                }
                asa_free(list->pairs);
            }
            break;
            
        default:
            return;
    }

    asa_free(container->data);
    container->data = NULL;
    container->type = ASA_UNDEFINED;
}
