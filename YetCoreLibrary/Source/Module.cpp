#include "pch.h"
#include "Module.h"

#include <Windows.h>

#define BUFFER_SIZE 1024

static char buffer[BUFFER_SIZE];

const char* yet_Module_path__get() {
    // TODO: not thread-safe
    // TODO: memoize the result
    auto length = GetModuleFileNameA(nullptr, buffer, BUFFER_SIZE);
    if (length > 0) {
        return buffer;
    } else {
        return nullptr;
    }
}
