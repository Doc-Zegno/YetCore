#include "pch.h"
#include "Module.h"

#include <Windows.h>

#define BUFFER_SIZE 1024

static char buffer[BUFFER_SIZE];

ConstCharResult yet_Module_path__get__V__PC8(EC* context) {
    // TODO: error handling
    // TODO: not thread-safe
    // TODO: memoize the result
    auto length = GetModuleFileNameA(nullptr, buffer, BUFFER_SIZE);
    if (length > 0) {
        return okResult(buffer);
    } else {
        return errorConstCharResult(1);
    }
}
