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

	VirtualTable* findTableOf(Type* type) {
		for (auto i = 0; i < tableCount; i++) {
			auto table = &tables[i];
			if (table->type == type) {
				return table;
			}
		}
		return nullptr;
	}
};

template<typename T>
struct TypeOf {
	// Specializations must provide `value` property
};

template<typename T>
inline Type* typeOf() {
	return TypeOf<T>().value;
}
