#pragma once

#include "Api.h"
#include "Manageable.h"

extern "C" {
	YETCORELIBRARY_API void* yet_allocateR__U_U_U__PV(uintptr_t size, uintptr_t options, uintptr_t placeHint);
	YETCORELIBRARY_API void yet_deallocateR__PV__V(void* object);
	YETCORELIBRARY_API intptr_t yet_allocatedCountR__get__V__I();
}

struct Allocator {
	struct Options {
		/// <summary>
		/// Indicates that a new place should not be cleared after allocation,
		/// i.e., the returned memory block will contain random bytes.
		/// This might be useful in case of the allocation of large memory chunk
		/// which is claimed to be initialized by the caller side
		/// in order to get rid of unnecessary zeroing
		/// and thus increase a performance
		/// </summary>
		static const uintptr_t WITHOUT_ZEROING = 0x1;

		/// <summary>
		/// Indicates that a new place can (and actually should) be allocated on the stack.
		/// The hint about a stack location which is safe to use for this purpose
		/// must be provided via the last argument of allocation method.
		/// If the hint is <c>null</c>, this option is ignored
		/// </summary>
		static const uintptr_t CAN_USE_STACK = 0x2;
	};

	Allocator() = delete;

	/// <summary>
	/// Allocate a contiguous block of dynamic memory of specified size.
	/// </summary>
	/// <param name="size">Required size of block (in bytes)</param>
	/// <param name="options">Allocation options. For available ones see <see cref="Options"/></param>
	/// <param name="placeHint">The hint about the place where the memory should be allocated.
	/// This hint is ignored when either it equals to zero or <see cref="CAN_USE_STACK"/>
	/// option is not specified in a previous argument</param>
	/// <returns>The pointer to allocated memory block if there is enough free space
	/// and <c>null</c> otherwise</returns>
	static void* allocate(uintptr_t size, uintptr_t options = 0, uintptr_t placeHint = 0) {
		return yet_allocateR__U_U_U__PV(size, options, placeHint);
	}

	template<typename T>
	static void* allocate() {
		return allocate(sizeof(T));
	}

	/// <summary>
	/// Release the specified block of dynamic memory.
	/// Its actual size is deduced by the means of runtime.
	/// Do nothing when argument is set to null.
	/// <note type="caution">
	/// Keep in mind that a behavior of program is not defined
	/// in case it's not capable of releasing memory, e.g.,
	/// if the memory was not obtained by the means of
	/// corresponding <see cref="allocate"/> call
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
