#include "pch.h"
#include "PtrX.h"
#include "Allocator.h"

YETCORELIBRARY_API void yet_Ptr_retainR__s__V(Ptr object) {
	// TODO: check against tags when they will be introduced
	// TODO: check for null for the sake of robustness
	auto any = as<Any>(object);
	any->__manageable.__refCount++;
}

YETCORELIBRARY_API bool yet_Ptr_releaseR__s__B(Ptr object) {
	// TODO: check against tags when they will be introduced
	// TODO: check for null for the sake of robustness
	auto any = as<Any>(object);
	auto refCount = --any->__manageable.__refCount;
	if (refCount == 0) {
		auto deinit = any->__type->deinit;
		if (deinit != nullptr) {
			deinit(object);
		}
		if (getTag(object) <= PtrTags::LAST_TAG_WITH_DEALLOCATION) {
			Allocator::deallocate(any);
		}
		return true;
	} else {
		return false;
	}
}
