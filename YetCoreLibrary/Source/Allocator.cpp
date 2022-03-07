#include "pch.h"
#include "Allocator.h"

#include <cstdlib>

namespace {
	std::atomic<intptr_t> allocatedCount = 0;
}

void* yet_Allocator_allocateR__U_2p1c_Options__PV(uintptr_t size, Allocator::Options* options) {
	// TODO: implement path when emergency memory pool is used
	// TODO: use custom memory manager

	// Fast path: try to use a stack placement
	if (options != nullptr) {
		auto placeHint = options->getPlaceHint();
		if (placeHint.getSize() >= size) {  // Returns 0 if no hint is available which is totally fine
			return placeHint.getPlace();
		}
	}

	// Slow path: allocate memory on a heap
	void* result;
	if (options != nullptr && options->isZeroingDisabled()) {
		result = malloc((size_t)size);
	} else {
		result = calloc(1, (size_t)size);
	}
	if (result) {
		allocatedCount++;  // TODO: a bottleneck. Consider using a per-thread counter
	}
	return result;
}

void yet_Allocator_deallocateR__PV__V(void* object) {
	// TODO: use custom memory manager
	if (object) {
		free(object);
		allocatedCount--;
	}
}

intptr_t yet_Allocator_allocatedCountR__get__V__I() {
	return allocatedCount;
}
