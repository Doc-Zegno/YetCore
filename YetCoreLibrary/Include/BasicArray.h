#pragma once

#include <vector>

#include "Any.h"
#include "Result.h"
#include "Allocator.h"

template<typename E>
struct BasicArray {
	Any __base;
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

	static VoidResult __init__PV__V(void* object) {
		auto any = (Any*)object;
		any->__type = &__typeHolder.type;
		return okResult();
	}

	static PtrResult __new__V__s() {
		auto place = yet_allocate__U__PV(sizeof(BasicArray<E>));
		if (place) {
			auto result = __init__PV__V(place);
			if (!result.error) {
				return okResult(Ptr(place));
			} else {
				return errorPtrResult(result.error);
			}
		} else {
			// TODO: return out of memory error message
			return errorPtrResult(1);
		}
	}

	static VoidResult __deinit(Ptr self) {
		auto basicArray = (BasicArray<E>*)self;
		basicArray->~BasicArray<E>();
		return okResult();
	}

	struct __TypeHolder {
		FunctionPtr _ptrs[1] = {
			&__deinit,
		};

		VirtualTable _tables[1] = {
			VirtualTable{ &yet_Any__type, _ptrs, 1 },
		};

		Type type{
			_tables,
			1,
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
