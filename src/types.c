#include <asampl-ffi/ffi.h>

#include <string.h>


AsaValueContainer* asa_alloc_container() {
    AsaValueContainer* container = asa_alloc(sizeof(AsaValueContainer));
    container->type = ASA_UNDEFINED;
    container->timestamp = 0.0;
    container->data = NULL;
    return container;
}

void asa_new_double(double value, AsaValueContainer* container) {
    container->type = ASA_DOUBLE;
    container->data = asa_alloc(sizeof(AsaDouble));
    ((AsaDouble*)container->data)->value = value;
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

void asa_new_video_frame_take(uint8_t* data, uint32_t width, uint32_t height, AsaValueContainer* container) {
    container->type = ASA_VIDEO_FRAME;

    AsaVideoFrame* frame = asa_alloc(sizeof(AsaVideoFrame));
    container->data = frame;

    frame->width = width;
    frame->height = height;
    frame->data = data;
}

void asa_new_video_frame_copy(const uint8_t* data, uint32_t width, uint32_t height, AsaValueContainer* container) {
    uint8_t* buf = asa_alloc(asa_video_frame_size(width, height));
    memcpy(buf, data, asa_video_frame_size(width, height));

    asa_new_video_frame_take(buf, width, height, container);
}

uint32_t asa_video_frame_size(uint32_t width, uint32_t height) {
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

void asa_clone(const AsaValueContainer* src, AsaValueContainer* dst) {
    dst->type = src->type;
    dst->timestamp = src->timestamp;

    switch (src->type) {
        case ASA_DOUBLE:
            asa_new_double(((AsaDouble*)src->data)->value, dst);
            break;
        case ASA_STRING:
        case ASA_BYTES:
            {
                AsaString* string = src->data;
                asa_new_string_copy(string->data, string->size, dst);
            }
            break;

        case ASA_VIDEO_FRAME:
            {
                AsaVideoFrame* frame = src->data;
                asa_new_video_frame_copy(frame->data, frame->width, frame->height, dst);
            }
            break;

        case ASA_ARRAY:
            {
                AsaArray* array = src->data;
                asa_new_array_copy(array->items, array->size, dst);
            }
            break;

        default:
            break;
    }
}

void asa_deinit_container(AsaValueContainer* container) {
    switch (container->type) {
        case ASA_DOUBLE:
            break;
        case ASA_STRING:
        case ASA_BYTES:
            asa_free(((AsaString*)container->data)->data);
            break;
        case ASA_VIDEO_FRAME:
            asa_free(((AsaVideoFrame*)container->data)->data);
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
            
        default:
            return;
    }

    asa_free(container->data);
    container->data = NULL;
    container->type = ASA_UNDEFINED;
}
