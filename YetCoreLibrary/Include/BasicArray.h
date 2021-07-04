#pragma once

#include <vector>

#include "Any.h"
#include "Allocator.h"
#include "InvocationUtil.h"

template<typename E>
struct BasicArray {
	Any __base;
	std::vector<E> _elements;

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
		auto place = Allocator::allocate(sizeof(BasicArray<E>));
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
		FunctionPtr _ptrs[1] = {
		};

		VirtualTable _tables[1] = {
			createTableOf<Any>(_ptrs),
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
