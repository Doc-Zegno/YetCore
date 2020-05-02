#pragma once

#include "Api.h"
#include "Manageable.h"

extern "C" {
	YETCORELIBRARY_API void* yet_allocate__U_B_B__PV(uintptr_t size, bool isManageable, bool canUseStack);
	YETCORELIBRARY_API void* yet_allocate__U__PV(uintptr_t size);
	YETCORELIBRARY_API bool yet_deallocate__PV__B(void* object);
	YETCORELIBRARY_API intptr_t yet_allocatedCount__get__V__I();
}
