#pragma once

#include "Any.h"
#include "Allocator.h"

extern "C" {
	YETCORELIBRARY_API Ptr yet_Allocator_raiseErrorF__U_U_U__PV(EC* context, uintptr_t size, uintptr_t options, uintptr_t placeHint);
}

namespace Allocator {
	inline Ptr raiseError(EC* context, uintptr_t size, uintptr_t options, uintptr_t placeHint) {
		return yet_Allocator_raiseErrorF__U_U_U__PV(context, size, options, placeHint);
	}

	inline Ptr allocateOrRaise(EC* context, uintptr_t size, uintptr_t options, Ptr* result) {
		auto place = allocate(size, options, *result);
		if (place != nullptr) {
			*result = Ptr(place);
			return 0;
		} else {
			return raiseError(context, size, options, *result);
		}
	}

	inline Ptr allocateOrRaise(EC* context, uintptr_t size, Ptr* result) {
		return allocateOrRaise(context, size, 0, result);
	}

	template<typename T>
	inline Ptr allocateOrRaise(EC* context, Ptr* result) {
		return allocateOrRaise(context, sizeof(T), result);
	}
}

