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
		auto deinit = any->__type->deinit;
		if (deinit != nullptr) {
			deinit(object);
		}
		Allocator::deallocate(any);
		return true;
	} else {
		return false;
	}
}
