#pragma once

#define ASAMPL_FFI_VERSION_MAJOR 0
#define ASAMPL_FFI_VERSION_MINOR 1

#include <stddef.h>

#include "types.h"
#include "library.h"
#include "handler.h"

void* asa_alloc(size_t bytes);
void asa_free(void* ptr);
void* asa_realloc(void* ptr, size_t new_size);
