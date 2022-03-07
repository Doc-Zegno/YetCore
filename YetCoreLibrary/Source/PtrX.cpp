#include "pch.h"
#include "PtrX.h"
#include "Allocator.h"

namespace {
	Type* getTypeOfNotNull(Ptr object) {
		// TODO: add support for embedded objects when they are introduced
		auto any = as<Any>(object);
		return any->__type;
	}
}

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

YETCORELIBRARY_API Type* yet_Ptr_typeR__get__s__1tPointer_Type(Ptr object) {
	if (!object) {  // For the sake of robustness
		return nullptr;
	}
	return getTypeOfNotNull(object);
}

YETCORELIBRARY_API FunctionPtr* yet_Ptr_findTableOfR__s_Type__PPV(Ptr object, Type* type) {
	if (!object) {  // For the sake of robustness
		return nullptr;
	}
	auto objectType = getTypeOfNotNull(object);
	if (!objectType) {
		// Normally it shouldn't, otherwise it's a runtime error
		// (e.g., an object with invalid memory layout).
		// What to do with a not-null object which
		// has no type is a tricky question,
		// let's make the caller decide
		return nullptr;
	}
	return objectType->findTableOf(type);
}
