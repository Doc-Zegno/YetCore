#pragma once

#include <cstdint>

struct Type;

using FunctionPtr = void*;

struct VirtualTable {
	Type* type;
	FunctionPtr* ptrs;
	intptr_t ptrCount;
};

struct Type {
	VirtualTable* tables;
	intptr_t tableCount;
	const char* name;
};

template<typename T>
struct TypeOf {
	// Specializations must provide `value` property
};

template<typename T>
inline Type* typeOf() {
	return TypeOf<T>().value;
}
