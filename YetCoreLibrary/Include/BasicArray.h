#pragma once

#include <vector>

#include "Array.h"
#include "FatPtr.h"
#include "PtrGuard.h"
#include "AllocatorUtil.h"
#include "InvocationUtil.h"
#include "BasicArrayIterator.h"

template<typename E>
struct BasicArray {
	Any __base;
	std::vector<E> _elements;

	BasicArray() : __base(&__typeHolder.type) {}

	static Ptr iteratorF__get__s__1tIterator_t1(EC* context, Ptr self, Ptr* result) {
		auto basicArray = as<BasicArray<E>>(self);
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
		auto basicArray = as<BasicArray<E>>(self);
		auto& elements = basicArray->_elements;
		fragments->length = intptr_t(elements.size());
		fragments->start = const_cast<E*>(elements.data());
		return 1;
	}

	static Ptr countF__get__s__I(EC* context, Ptr self, intptr_t* result) {
		auto basicArray = as<BasicArray<E>>(self);
		*result = intptr_t(basicArray->_elements.size());
		return 0;
	}

	static Ptr addF__s_t1__V(EC* context, Ptr self, E element) {
		auto basicArray = as<BasicArray<E>>(self);
		basicArray->_elements.push_back(element);
		return 0;
	}

	static Ptr getF__operator__s_I__t1(EC* context, Ptr self, int index, YET_RES_TYPE(E)* result) {
		auto basicArray = as<BasicArray<E>>(self);
		auto& elements = basicArray->_elements;
		if (index >= 0 && index < int(elements.size())) {
			assign(result, elements[index]);
			return 0;
		} else {
			// TODO: proper out of range error
			return 1;
		}
	}

	static Ptr __init__s__V(EC* context, Ptr self) {
		new(as<void>(self)) BasicArray<E>;
		return 0;
	}

	static Ptr __new__V__s(EC* context, Ptr* result) {
		auto error = Allocator::allocateOrRaise<BasicArray<E>>(context, result);
		if (error) {
			return error;
		}
		error = __init__s__V(context, *result);
		if (error) {
			return error;
		}
		return 0;
	}

	static Ptr __new__V__0fs(EC* context, FatPtr* result) {
		auto error = __new__V__s(context, &result->ptr);
		if (error) {
			return error;
		}
		result->setTable(__typeHolder._arrayPtrs);
		return 0;
	}

	static void __deinit(Ptr self) {
		auto basicArray = as<BasicArray<E>>(self);
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

		FunctionPtr _arrayPtrs[5] = {
			nullptr,
			&iteratorF__get__s__1tIterator_t1,
			&decomposeR__s_1tPointer_ArrayFragment_I__I,
			&countF__get__s__I,
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
