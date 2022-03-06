#pragma once

#include "Api.h"
#include "Manageable.h"

namespace Allocator {
	struct Options {
		static const uintptr_t IS_ZEROING_DISABLED = 0x1;
		static const uintptr_t CAN_USE_EMERGENCY_POOL = 0x2;

		uintptr_t _bitSet = 0;
		uintptr_t _placeHint = 0;

		Options() {}

		/// <summary>
		/// Indicates that a new place should not be cleared after allocation,
		/// i.e., the returned memory block will contain random bytes.
		/// This might be useful in case of the allocation of large memory chunk
		/// which is claimed to be initialized by the caller side
		/// in order to get rid of unnecessary zeroing
		/// and thus increase a performance
		/// </summary>
		bool isZeroingDisabled() {
			return _bitSet & IS_ZEROING_DISABLED;
		}

		/// <summary>
		/// Indicates that a new place is allowed to be allocated inside a so-called emergency pool.
		/// Normally the allocator preserves some space for the sake of error handling
		/// which must be available to perform even if there is no
		/// free memory left in the regular pools
		/// </summary>
		bool canUseEmergencyPool() {
			return _bitSet & CAN_USE_EMERGENCY_POOL;
		}

		/// <summary>
		/// Get the hint about a stack location which is safe to use for a "fast" memory allocation.
		/// This might be useful in case of the allocation of so-called "local" objects
		/// which are semantically equivalent to automatic stack variables.
		/// If the hint equals to zero, it must be ignored
		/// </summary>
		uintptr_t getPlaceHint() {
			return _placeHint;
		}

		Options withZeroingDisabled() const {
			auto copy = *this;
			copy._bitSet |= IS_ZEROING_DISABLED;
			return copy;
		}

		Options withCanUseEmergencyPool() const {
			auto copy = *this;
			copy._bitSet |= CAN_USE_EMERGENCY_POOL;
			return copy;
		}

		Options withPlaceHint(uintptr_t placeHint) const {
			auto copy = *this;
			copy._placeHint = placeHint;
			return copy;
		}
	};
}

extern "C" {
	YETCORELIBRARY_API void* yet_Allocator_allocateR__U_2p1c_Options__PV(uintptr_t size, Allocator::Options* options);
	YETCORELIBRARY_API void yet_Allocator_deallocateR__PV__V(void* object);
	YETCORELIBRARY_API intptr_t yet_Allocator_allocatedCountR__get__V__I();
}

namespace Allocator {
	/// <summary>
	/// Allocate a contiguous block of dynamic memory of specified size.
	/// </summary>
	/// <param name="size">Required size of block (in bytes)</param>
	/// <param name="options">Allocation options. For available ones see <see cref="Options"/></param>
	/// <returns>The pointer to allocated memory block if there is enough free space
	/// and <c>null</c> otherwise</returns>
	inline void* allocate(uintptr_t size, Options* options = nullptr) {
		return yet_Allocator_allocateR__U_2p1c_Options__PV(size, options);
	}

	/// <summary>
	/// Allocate a contiguous block of dynamic memory of size enough for an object of specified type.
	/// </summary>
	/// <typeparam name="T">The type of object which the memory is being allocated for</typeparam>
	/// <returns>The pointer to allocated memory block if there is enough free space
	/// and <c>null</c> otherwise</returns>
	template<typename T>
	inline void* allocate() {
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
	inline void deallocate(void* object) {
		yet_Allocator_deallocateR__PV__V(object);
	}

	/// <summary>
	/// Get the total count of objects allocated (and not released yet) so far
	/// </summary>
	inline intptr_t getAllocatedCount() {
		return yet_Allocator_allocatedCountR__get__V__I();
	}
}
