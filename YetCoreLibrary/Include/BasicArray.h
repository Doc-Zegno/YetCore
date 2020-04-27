#pragma once

#include <vector>

#include "Result.h"

template<typename E>
struct BasicArray {
	std::vector<E> _elements;

	static VoidResult add__s_t1__V(Ptr self, E element) {
		auto basicArray = (BasicArray<E>*)self;
		basicArray->_elements.push_back(element);
		return okResult();
	}

	static Result<E> get__operator__s_I__t1(Ptr self, int index) {
		auto basicArray = (BasicArray<E>*)self;
		auto& elements = basicArray->_elements;
		if (index >= 0 && index < int(elements.size())) {
			return okGenericResult(elements[index]);
		} else {
			return errorGenericResult<E>(1);
		}
	}
};
