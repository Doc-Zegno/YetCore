#include "pch.h"
#include "Module.h"

#include <Windows.h>

#define BUFFER_SIZE 1024

static char buffer[BUFFER_SIZE];

Ptr yet_Module_pathF__get__V__PC8(EC* context, const char** result) {
    // TODO: error handling
    // TODO: not thread-safe
    // TODO: memoize the result
    auto length = GetModuleFileNameA(nullptr, buffer, BUFFER_SIZE);
    if (length > 0) {
        *result = buffer;
        return 0;
    } else {
        return 1;
    }
}
