#pragma once

#include "FatPtr.h"

/// <summary>
/// A <c>PtrGuard</c> equivalent for a <c>FatPtr</c>.
/// This guard does not try to release objects
/// which are allocated on a stack,
/// so it is pretty universal
/// </summary>
struct FatPtrGuard {
	FatPtr fatPtr;

	FatPtrGuard() : fatPtr(0) {}

	FatPtrGuard(const FatPtrGuard& other) = delete;

	void operator=(const FatPtrGuard& other) = delete;

	FatPtr unprotect() {
		auto result = fatPtr;
		fatPtr.clear();
		return result;
	}

	~FatPtrGuard() {
		if (fatPtr.ptr && !fatPtr.isLocal()) {
			::release(fatPtr.ptr);
			fatPtr.clear();
		}
	}
};
