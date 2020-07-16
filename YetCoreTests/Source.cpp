#include "pch.h"
#include "Allocator.h"

void* operator new(size_t size) {
    return yet_allocateR__U_B_B__PV((uintptr_t)size, false, false);
}

void operator delete(void* object) {
    yet_deallocateR__PV__V(object);
}
