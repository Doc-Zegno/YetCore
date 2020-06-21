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
	YETCORELIBRARY_API void yet_Ptr_releaseR__s__V(Ptr object);
}

inline void retain(Ptr object) {
	yet_Ptr_retainR__s__V(object);
}

inline void release(Ptr object) {
	yet_Ptr_releaseR__s__V(object);
}
