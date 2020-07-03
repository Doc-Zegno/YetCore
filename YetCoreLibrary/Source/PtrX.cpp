#include "pch.h"
#include "PtrX.h"
#include "Allocator.h"

YETCORELIBRARY_API void yet_Ptr_retainR__s__V(Ptr object) {
	auto any = (Any*)object;
	any->__manageable.__refCount++;
}

YETCORELIBRARY_API bool yet_Ptr_releaseR__s__V(Ptr object) {
	auto any = (Any*)object;
	auto refCount = --any->__manageable.__refCount;
	if (refCount == 0) {
		auto table = findTableOf<Any>(object);
		auto deinit = (void(*)(Ptr))table->ptrs[int(Any::__Methods::__deinit)];
		deinit(object);
		yet_deallocateR__PV__V(any);
		return true;
	} else {
		return false;
	}
}
