#pragma once

#include <stddef.h>

#include <asampl-ffi/types.h>

typedef struct {
    char* symbol_name;
    AsaString* function_name;
} AsaFunctionData;
