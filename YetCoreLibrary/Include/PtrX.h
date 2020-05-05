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
