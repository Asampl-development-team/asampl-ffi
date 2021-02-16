#include <asampl-ffi/ffi.h>

#include <string.h>

void asa_new_response_normal(AsaValueContainer* value, AsaHandlerResponse* response) {
    response->status = ASA_STATUS_NORMAL;
    response->error = NULL;
    response->value = value;
}

void asa_new_response_fatal(const char* error, AsaHandlerResponse* response) {
    response->status = ASA_STATUS_FATAL;
    const size_t len = strlen(error);
    response->error = asa_alloc(len);
    memcpy(response->error, error, len);

    response->value = NULL;
}

void asa_new_response_not_ready(AsaHandlerResponse* response) {
    response->status = ASA_STATUS_NOT_READY;
    response->error = NULL;
    response->value = NULL;
}

void asa_new_response_eoi(AsaHandlerResponse* response) {
    response->status = ASA_STATUS_EOI;
    response->error = NULL;
    response->value = NULL;
}

void asa_deinit_response(AsaHandlerResponse* response) {
    if (response->error != NULL) {
        asa_free(response->error);
        response->error = NULL;
    }
    if (response->value != NULL) {
        asa_deinit_container(response->value);
        asa_free(response->value);
        response->value = NULL;
    }
}
