#pragma once

#include "Any.h"
#include "Allocator.h"

extern "C" {
	YETCORELIBRARY_API Ptr yet_Allocator_raiseErrorF__U_U_U__N(EC* context, uintptr_t size, uintptr_t options, uintptr_t placeHint);
}

namespace Allocator {
	/// <summary>
	/// Generate an error object representing some occurred memory allocation failure.
	/// Assuming there is no free memory left in the system, the caller side
	/// shouldn't assume that returned error object will contain
	/// all the debug information available.
	/// <note>
	/// This function is intended for internal usage only.
	/// </note>
	/// </summary>
	/// <param name="context">Execution context which led to the allocation failure</param>
	/// <param name="size">The size of memory block requested during the failed allocation</param>
	/// <param name="options">The options used during the failed allocation</param>
	/// <param name="placeHint">The hint used during the failed allocation</param>
	/// <returns>An error object representing memory allocation failure</returns>
	inline Ptr raiseError(EC* context, uintptr_t size, uintptr_t options, uintptr_t placeHint) {
		return yet_Allocator_raiseErrorF__U_U_U__N(context, size, options, placeHint);
	}

	/// <summary>
	/// Allocate a contiguous block of dynamic memory of specified size or
	/// raise an error if the allocation is not possible.
	/// </summary>
	/// <param name="context">Current execution context</param>
	/// <param name="size">Required size of block (in bytes)</param>
	/// <param name="options">Allocation options. For available ones see <see cref="Options"/></param>
	/// <param name="result">The pointer to the place where the address of allocated memory block must be written.
	/// The place itself should either be initialized with zero or contain a hint for allocator</param>
	/// <returns><c>0</c> if the memory allocation was successful and
	/// an error object representing a failure otherwise</returns>
	inline Ptr allocateOrRaise(EC* context, uintptr_t size, uintptr_t options, Ptr* result) {
		auto place = allocate(size, options, *result);
		if (place != nullptr) {
			*result = Ptr(place);
			return 0;
		} else {
			return raiseError(context, size, options, *result);
		}
	}

	/// <summary>
	/// Allocate a contiguous block of dynamic memory of specified size or
	/// raise an error if the allocation is not possible.
	/// </summary>
	/// <param name="context">Current execution context</param>
	/// <param name="size">Required size of block (in bytes)</param>
	/// <param name="result">The pointer to the place where the address of allocated memory block must be written.
	/// The place itself should either be initialized with zero or contain a hint for allocator</param>
	/// <returns><c>0</c> if the memory allocation was successful and
	/// an error object representing a failure otherwise</returns>
	inline Ptr allocateOrRaise(EC* context, uintptr_t size, Ptr* result) {
		return allocateOrRaise(context, size, 0, result);
	}

	/// <summary>
	/// Allocate a contiguous block of dynamic memory of size enough for an object of specified type
	/// or raise an error if the allocation is not possible.
	/// </summary>
	/// <typeparam name="T">The type of object which the memory is being allocated for</typeparam>
	/// <param name="context">Current execution context</param>
	/// <param name="result">The pointer to the place where the address of allocated memory block must be written.
	/// The place itself should either be initialized with zero or contain a hint for allocator</param>
	/// <returns><c>0</c> if the memory allocation was successful and
	/// an error object representing a failure otherwise</returns>
	template<typename T>
	inline Ptr allocateOrRaise(EC* context, Ptr* result) {
		return allocateOrRaise(context, sizeof(T), result);
	}
}

