#pragma once

#include "Ptr.h"
#include "VirtualTable.h"

using DeinitPtr = void(*)(Ptr);

struct Type {
	DeinitPtr deinit;
	VirtualTable* tables;
	intptr_t tableCount;
	const char* name;

	Type(DeinitPtr deinit, VirtualTable* tables, intptr_t tableCount, const char* name) :
		deinit(deinit), tables(tables), tableCount(tableCount), name(name) {}

	template<intptr_t N>
	Type(DeinitPtr deinit, VirtualTable(&tables)[N], const char* name) :
		deinit(deinit), tables(tables), tableCount(N), name(name) {}

	FunctionPtr* findTableOf(Type* type) {
		for (auto i = 0; i < tableCount; i++) {
			auto table = &tables[i];
			if (table->type == type) {
				return table->ptrs;
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
