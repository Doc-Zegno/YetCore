#pragma once

#include <vector>

#include "Array.h"
#include "PtrGuard.h"
#include "Allocator.h"
#include "InvocationUtil.h"
#include "BasicArrayIterator.h"

template<typename E>
struct BasicArray {
	Any __base;
	std::vector<E> _elements;

	static Ptr iteratorF__get__s__1tIterator_t1(EC* context, Ptr self, Ptr* result) {
		auto basicArray = (BasicArray<E>*)self;
		auto& elements = basicArray->_elements;
		E* start;
		E* end;
		if (elements.empty()) {
			start = nullptr;
			end = nullptr;
		} else {
			auto count = elements.size();
			start = &elements[0];
			end = &elements[count - 1] + 1;  // Prevent debugger's warnings in case of out of range &elements[count]
		}
		auto error = BasicArrayIterator<E>::__new__1tBasicArray_t1_Pt1_Pt1__s(context, self, start, end, result);
		return error;
	}

	static intptr_t decomposeR__s_1tPointer_ArrayFragment_I__I(Ptr self, ArrayFragment* fragments, intptr_t fragmentCount) {
		if (fragmentCount < 1) {
			return -1;
		}
		auto basicArray = (BasicArray<E>*)self;
		auto& elements = basicArray->_elements;
		fragments->length = intptr_t(elements.size());
		fragments->start = const_cast<E*>(elements.data());
		return 1;
	}

	static Ptr addF__s_t1__V(EC* context, Ptr self, E element) {
		auto basicArray = (BasicArray<E>*)self;
		basicArray->_elements.push_back(element);
		return 0;
	}

	static Ptr getF__operator__s_I__t1(EC* context, Ptr self, int index, YET_RES_TYPE(E)* result) {
		auto basicArray = (BasicArray<E>*)self;
		auto& elements = basicArray->_elements;
		if (index >= 0 && index < int(elements.size())) {
			assign(result, elements[index]);
			return 0;
		} else {
			// TODO: proper out of range error
			return 1;
		}
	}

	static Ptr __init__PV__V(EC* context, void* object) {
		auto self = new(object) BasicArray<E>;
		self->__base.__type = &__typeHolder.type;
		return 0;
	}

	static Ptr __new__V__s(EC* context, Ptr* result) {
		auto place = Allocator::allocate<BasicArray<E>>();
		auto error = __init__PV__V(context, place);
		if (error) {
			return error;
		}
		*result = Ptr(place);
		return 0;
	}

	static void __deinit(Ptr self) {
		auto basicArray = (BasicArray<E>*)self;
		basicArray->~BasicArray<E>();
	}

	struct __TypeHolder {
		// TODO: merge them into one
		FunctionPtr _anyPtrs[1] = {
			nullptr,
		};

		FunctionPtr _iterablePtrs[2] = {
			nullptr,
			&iteratorF__get__s__1tIterator_t1,
		};

		FunctionPtr _arrayPtrs[4] = {
			nullptr,
			&iteratorF__get__s__1tIterator_t1,
			&decomposeR__s_1tPointer_ArrayFragment_I__I,
			&getF__operator__s_I__t1,
		};

		VirtualTable _tables[3] = {
			createTableOf<Any>(_anyPtrs),
			createTableOf<Iterable<E>>(_iterablePtrs),
			createTableOf<Array<E>>(_arrayPtrs),
		};

		Type type{
			&__deinit,
			_tables,
			"Builtin.BasicArray",
		};
	};

	static __TypeHolder __typeHolder;
};

template<typename E>
typename BasicArray<E>::__TypeHolder BasicArray<E>::__typeHolder{};

template<typename E>
struct TypeOf<BasicArray<E>> {
	Type* value = &BasicArray<E>::__typeHolder.type;
};
