#include "pch.h"
#include "Allocator.h"

void* operator new(size_t size) {
    return Allocator::allocate((uintptr_t)size, false, false);
}

void operator delete(void* object) {
    Allocator::deallocate(object);
}
