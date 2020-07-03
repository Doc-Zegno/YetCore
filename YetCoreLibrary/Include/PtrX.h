#pragma once

#include "Ptr.h"
#include "Any.h"

inline Type* getType(Ptr object) {
	auto any = (Any*)object;
	return any->__type;
}

template<typename T>
inline VirtualTable* findTableOf(Ptr object) {
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
