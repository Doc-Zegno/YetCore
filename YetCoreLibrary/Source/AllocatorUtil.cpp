#include "pch.h"
#include "AllocatorUtil.h"

namespace {
	const Ptr FALLBACK_ERROR = Ptr(1);  // TODO: a proper error object
}

Ptr yet_Allocator_raiseErrorF__U_U_U__N(EC* context, uintptr_t size, uintptr_t options, uintptr_t placeHint) {
	// TODO: instantiate a proper error object and fill debug data from provided context and arguments
	//auto place = Allocator::allocate(errorSize, Allocator::Options::CAN_USE_EMERGENCY_POOL);
	return FALLBACK_ERROR;
}
