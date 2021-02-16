#pragma once

#include "types.h"

struct AsaHandler;
typedef struct AsaHandler AsaHandler;

typedef enum {
    ASA_STATUS_NORMAL,
    ASA_STATUS_FATAL,
    ASA_STATUS_NOT_READY,
    ASA_STATUS_EOI,
} AsaHandlerResponseStatus;

typedef struct {
    AsaHandlerResponseStatus status;
    char* error;
    AsaValueContainer* value;
} AsaHandlerResponse;

void asa_new_response_normal(AsaValueContainer* value, AsaHandlerResponse* response);
void asa_new_response_fatal(const char* error, AsaHandlerResponse* response);
void asa_new_response_not_ready(AsaHandlerResponse* response);
void asa_new_response_eoi(AsaHandlerResponse* response);
void asa_deinit_response(AsaHandlerResponse* response);
