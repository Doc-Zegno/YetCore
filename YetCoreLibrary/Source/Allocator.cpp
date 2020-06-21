#include "pch.h"
#include "Allocator.h"

#include <cstdlib>

namespace {
	std::atomic<intptr_t> allocatedCount = 0;
}

void* yet_allocateR__U_B_B__PV(uintptr_t size, bool isManageable, bool canUseStack) {
	// TODO: implement path when stack placement is used
	// TODO: use custom memory manager
	auto p = calloc(1, (size_t)size);
	if (p) {
		if (isManageable) {
			auto manageable = (Manageable*)p;
			manageable->__refCount = 1;
		}
		allocatedCount++;
	}
	return p;
}

void* yet_allocateR__U__PV(uintptr_t size) {
	return yet_allocateR__U_B_B__PV(size, true, false);
}

void yet_deallocateR__PV__V(void* object) {
	// TODO: use custom memory manager
	if (object) {
		free(object);
		allocatedCount--;
	}
}

intptr_t yet_allocatedCountR__get__V__I() {
	return allocatedCount;
}
