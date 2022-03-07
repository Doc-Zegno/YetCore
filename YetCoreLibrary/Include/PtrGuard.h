#pragma once

#include <utility>

#include "PtrX.h"
#include "Allocator.h"
#include "StackBuffer.h"

/// <summary>
/// A thin wrapper around <c>Ptr</c> which enables automatic release on leaving its scope.
/// It's used in order to prevent memory leaks when dealing with function's
/// result values which reside on a heap.
/// <note type="caution">
/// Also keep in mind this guard guarantees that its <c>ptr</c> field will be zeroed
/// before a function invocation which is really crucial for functions that
/// try to allocate resulting objects locally. If you create a <c>Ptr</c>
/// on a stack manually, it will stay uninitialized thus leading to
/// subtle bugs and even crashes so use this guard instead!
/// </note>
/// </summary>
/// <example>
/// <code>
/// PtrGuard guard;
/// Ptr error = yet_YourFunction__V__S(&guard.ptr);
/// if (error) {
///     return error;  // No need to deallocate `guard.ptr` manually
/// }
/// *result = guard.unprotect();
/// </code>
/// </example>
struct PtrGuard {
	Ptr ptr;

	PtrGuard() : ptr(0) {}

	template<size_t SIZE>
	PtrGuard(StackBuffer<SIZE>* buffer) {
		ptr = Allocator::PlaceHint(buffer, SIZE).getValue();
	}

	PtrGuard(const PtrGuard& other) = delete;

	void operator=(const PtrGuard& other) = delete;

	Ptr unprotect() {
		auto result = ptr;
		ptr = 0;
		return result;
	}

	~PtrGuard() {
		if (ptr) {
			::release(ptr);
			ptr = 0;
		}
	}
};
