#include "pch.h"
#include "Allocator.h"

#include <cstdlib>

namespace {
	std::atomic<intptr_t> allocatedCount = 0;
}

void* yet_allocate__U_B_B__PV(uintptr_t size, bool isManageable, bool canUseStack) {
	// TODO: implement path when stack placement is used
	// TODO: use custom memory manager
	auto p = malloc((size_t)size);
	if (p) {
		if (isManageable) {
			auto manageable = (Manageable*)p;
			manageable->__refCount = 1;
		}
		allocatedCount++;
	}
	return p;
}

void* yet_allocate__U__PV(uintptr_t size) {
	return yet_allocate__U_B_B__PV(size, true, false);
}

bool yet_deallocate__PV__B(void* object) {
	// TODO: use custom memory manager. Return `false` on failure
	if (object) {
		free(object);
		allocatedCount--;
	}
	return true;
}

intptr_t yet_allocatedCount__get__V__I() {
	return allocatedCount;
}
