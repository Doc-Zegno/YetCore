#pragma once

#include "Api.h"
#include "Manageable.h"

extern "C" {
	YETCORELIBRARY_API void* yet_allocateR__U_B_B__PV(uintptr_t size, bool isManageable, bool canUseStack);
	YETCORELIBRARY_API void* yet_allocateR__U__PV(uintptr_t size);
	YETCORELIBRARY_API void yet_deallocateR__PV__V(void* object);
	YETCORELIBRARY_API intptr_t yet_allocatedCountR__get__V__I();
}
