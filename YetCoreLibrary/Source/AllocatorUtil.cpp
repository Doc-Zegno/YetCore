#include "pch.h"
#include "AllocatorUtil.h"

namespace Allocator {
	namespace {
		Options DEFAULT_OPTIONS = Options();
		const Ptr FALLBACK_ERROR = Ptr(1);  // TODO: a proper error object

		/// <summary>
		/// Generate an error object representing some occurred memory allocation failure.
		/// Assuming there is no free memory left in the system, the caller side
		/// shouldn't assume that returned error object will contain
		/// all the debug information available.
		/// </summary>
		/// <param name="context">Execution context which led to the allocation failure</param>
		/// <param name="size">The size of memory block requested during the failed allocation</param>
		/// <param name="options">The options used during the failed allocation</param>
		/// <returns>An error object representing memory allocation failure</returns>
		Ptr raiseError(EC* context, uintptr_t size, Options* options) {
			// TODO: instantiate a proper error object and fill debug data from provided context and arguments
			//auto place = Allocator::allocate(errorSize, Allocator::Options::CAN_USE_EMERGENCY_POOL);
			return FALLBACK_ERROR;
		}
	}
}

YETCORELIBRARY_API Ptr yet_Allocator_allocateOrRaiseF__U_2p1c_Options__R(EC* context, uintptr_t size, Allocator::Options* options, Ptr* result) {
	if (options == nullptr) {
		options = &Allocator::DEFAULT_OPTIONS;
	}
	auto optionsWithHint = options->withPlaceHint(*result);
	auto place = allocate(size, &optionsWithHint);
	if (place != nullptr) {
		*result = Ptr(place);
		return 0;
	} else {
		return Allocator::raiseError(context, size, &optionsWithHint);
	}
}
