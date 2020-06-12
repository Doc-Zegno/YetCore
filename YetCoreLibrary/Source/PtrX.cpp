#include "pch.h"
#include "PtrX.h"
#include "Allocator.h"

YETCORELIBRARY_API void yet_Ptr_retain__s__V(Ptr object) {
	auto any = (Any*)object;
	any->__manageable.__refCount++;
}

YETCORELIBRARY_API void yet_Ptr_release__s__V(Ptr object) {
	auto any = (Any*)object;
	auto refCount = --any->__manageable.__refCount;
	if (refCount == 0) {
		auto table = findTableOf<Any>(object);
		auto deinit = (void(*)(Ptr))table->ptrs[int(Any::__Methods::__deinit)];
		deinit(object);
		yet_deallocate__PV__B(any);
	}
}
