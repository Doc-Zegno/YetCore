#pragma once

#include "Ptr.h"
#include "Any.h"

namespace PtrTags {
	constexpr Ptr NO_TAG = 0;
	constexpr Ptr ON_STACK = 15;

	constexpr Ptr TAG_MASK = 0xf;  // Allocated objects must be aligned at the boundary of 16 bytes
	constexpr Ptr LAST_TAG_WITH_DEALLOCATION = NO_TAG;  // Inclusive
}

inline Ptr getTag(Ptr object) {
	return object & PtrTags::TAG_MASK;
}

inline Ptr removeTag(Ptr object) {
	return object & ~PtrTags::TAG_MASK;
}

inline Type* getType(Ptr object) {
	auto any = (Any*)removeTag(object);
	return any->__type;
}

template<typename T>
inline FunctionPtr* findTableOf(Ptr object) {
	auto type = getType(object);
	return type->findTableOf(typeOf<T>());
}

extern "C" {
	YETCORELIBRARY_API void yet_Ptr_retainR__s__V(Ptr object);
	YETCORELIBRARY_API bool yet_Ptr_releaseR__s__V(Ptr object);
}

/// <summary>
/// Increase reference count for given object (if it's on heap)
/// </summary>
/// <param name="object">(NOT NULL)</param>
inline void retain(Ptr object) {
	yet_Ptr_retainR__s__V(object);
}

/// <summary>
/// Decrease reference count for given object (if it's on heap)
/// </summary>
/// <param name="object">(NOT NULL)</param>
/// <returns><code>true</code> if object was deallocated and <code>false</code> otherwise</returns>
inline bool release(Ptr object) {
	return yet_Ptr_releaseR__s__V(object);
}
