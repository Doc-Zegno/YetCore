#pragma once

#include "Array.h"
#include "FatPtr.h"

template<typename E>
struct LocalArray {
	E* elements;
	intptr_t elementCount;

	LocalArray(E* elements, intptr_t elementCount) : elements(elements), elementCount(elementCount) {}

	template<int N>
	LocalArray(E(&elements)[N]) : elements(elements), elementCount(N) {}

	FatPtr toFatPtr() {
		return FatPtr(Ptr(this), __typeHolder._arrayPtrs, /* isLocal */ true);
	}

	static intptr_t decomposeR__s_1tPointer_ArrayFragment_I__I(Ptr self, ArrayFragment* fragments, intptr_t fragmentCount) {
		if (fragmentCount < 1) {
			return -1;
		}
		auto localArray = (LocalArray<E>*)self;
		fragments->start = localArray->elements;
		fragments->length = localArray->elementCount;
		return 1;
	}

	static Ptr countF__get__s__I(EC* context, Ptr self, intptr_t* result) {
		auto localArray = (LocalArray<E>*)self;
		*result = localArray->elementCount;
		return 0;
	}

	static Ptr getF__operator__s_I__t1(EC* context, Ptr self, int index, YET_RES_TYPE(E)* result) {
		auto localArray = (LocalArray<E>*)self;
		if (index < 0 || index >= localArray->elementCount) {
			return 1;  // TODO: proper out of range error
		}
		assign(result, localArray->elements[index]);
		return 0;
	}

	struct __TypeHolder {
		FunctionPtr _arrayPtrs[5] = {
			nullptr,  // TODO: &__box,
			nullptr,  // TODO: &iteratorF__get__s__1tIterator_t1,
			&decomposeR__s_1tPointer_ArrayFragment_I__I,
			&countF__get__s__I,
			&getF__operator__s_I__t1,
		};
	};

	static __TypeHolder __typeHolder;
};

template<typename E>
typename LocalArray<E>::__TypeHolder LocalArray<E>::__typeHolder{};
