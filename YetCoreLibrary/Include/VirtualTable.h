#pragma once

#include <cstdint>

struct Type;

template<typename T>
struct TypeOf;

using FunctionPtr = void*;

struct VirtualTable {
	Type* type;
	FunctionPtr* ptrs;
	intptr_t ptrCount;

	VirtualTable(Type* type, FunctionPtr* ptrs, intptr_t ptrCount) :
		type(type), ptrs(ptrs), ptrCount(ptrCount) {}

	template<intptr_t N>
	VirtualTable(Type* type, FunctionPtr(&ptrs)[N]) :
		type(type), ptrs(ptrs), ptrCount(N) {}
};

template<typename T, intptr_t N>
inline VirtualTable createTableOf(FunctionPtr(&ptrs)[N]) {
	return VirtualTable(TypeOf<T>().value, ptrs);
}
