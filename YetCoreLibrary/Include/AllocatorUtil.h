#pragma once

#include "Any.h"
#include "Allocator.h"

extern "C" {
	YETCORELIBRARY_API Ptr yet_Allocator_allocateOrRaiseF__U_2p1c_Options__R(EC* context, uintptr_t size, Allocator::Options* options, Ptr* result);
}

namespace Allocator {
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
	inline Ptr allocateOrRaise(EC* context, uintptr_t size, Options* options, Ptr* result) {
		return yet_Allocator_allocateOrRaiseF__U_2p1c_Options__R(context, size, options, result);
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
		return allocateOrRaise(context, size, nullptr, result);
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

