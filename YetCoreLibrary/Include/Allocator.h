#pragma once

#include "Api.h"
#include "Manageable.h"

extern "C" {
	YETCORELIBRARY_API void* yet_allocateR__U_B_B__PV(uintptr_t size, bool isManageable, bool canUseStack);
	YETCORELIBRARY_API void* yet_allocateR__U__PV(uintptr_t size);
	YETCORELIBRARY_API void yet_deallocateR__PV__V(void* object);
	YETCORELIBRARY_API intptr_t yet_allocatedCountR__get__V__I();
}

struct Allocator {
	Allocator() = delete;

	// TODO: replace with specific versions for manageable and local objects
	static void* allocate(uintptr_t size, bool isManageable, bool canUseStack) {
		return yet_allocateR__U_B_B__PV(size, isManageable, canUseStack);
	}

	// TODO: make it incompatible with allocator for manageable objects
	/// <summary>
	/// Allocate a contiguous block of dynamic memory of specified size.
	/// <note type="caution">
	/// Keep in mind that this function are not allowed to return
	/// a null pointer in case it runs out of free memory.
	/// Instead, the program will be terminated
	/// </note>
	/// </summary>
	/// <param name="size">Required size of block</param>
	/// <returns>Not null pointer to allocated block</returns>
	static void* allocate(uintptr_t size) {
		return yet_allocateR__U__PV(size);
	}

	/// <summary>
	/// Release the specified block of dynamic memory.
	/// Its actual size is deduced by the means of runtime.
	/// Do nothing when argument is set to null.
	/// <note type="caution">
	/// Keep in mind that a program will be terminated
	/// in case it's not capable of releasing memory
	/// </note>
	/// </summary>
	/// <param name="object">Pointer to the block to be released</param>
	static void deallocate(void* object) {
		yet_deallocateR__PV__V(object);
	}

	/// <summary>
	/// Get the total count of objects allocated (and not released yet) so far
	/// </summary>
	static intptr_t getAllocatedCount() {
		return yet_allocatedCountR__get__V__I();
	}
};
