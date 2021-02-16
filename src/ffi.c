#include <asampl-ffi/ffi.h>

#include <stdlib.h>


void* asa_alloc(size_t bytes) {
    return malloc(bytes);
}

void asa_free(void *ptr) {
    free(ptr);
}

void* asa_realloc(void* ptr, size_t new_size) {
    return realloc(ptr, new_size);
}
