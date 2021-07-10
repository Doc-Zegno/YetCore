#pragma once

#include "Ref.h"
#include "Iterator.h"
#include "Allocator.h"
#include "InvocationUtil.h"

template<typename E>
struct BasicArrayIterator {
	Any __base;
	Ref _array;
	E* _current;
	E* _end;

	BasicArrayIterator(Ptr array, E* start, E* end) : _array(array, true), _current(start), _end(end) {
		__base.__type = &__typeHolder.type;
	}

	static Ptr hasNextF__get__s__B(EC* context, Ptr self, bool* result) {
		auto iterator = (BasicArrayIterator<E>*)self;
		*result = iterator->_current < iterator->_end;
		return 0;
	}

	static Ptr nextF__get__s__t1(EC* context, Ptr self, YET_RES_TYPE(E)* result) {
		auto iterator = (BasicArrayIterator<E>*)self;
		auto current = iterator->_current;
		if (current < iterator->_end) {
			assign(result, *current);
			iterator->_current++;
			return 0;
		} else {
			// TODO: return proper out of iterable error
			return 1;
		}
	}

	static Ptr __init__PV_1tBasicArray_t1_Pt1_Pt1__V(EC* context, void* object, Ptr array, E* start, E* end) {
		new(object) BasicArrayIterator<E>(array, start, end);
		return 0;
	}

	static Ptr __new__1tBasicArray_t1_Pt1_Pt1__s(EC* context, Ptr array, E* start, E* end, Ptr* result) {
		auto object = Allocator::allocate(sizeof(BasicArrayIterator<E>));
		auto error = __init__PV_1tBasicArray_t1_Pt1_Pt1__V(context, object, array, start, end);
		if (error) {
			return error;
		}
		*result = Ptr(object);
		return 0;
	}

	static void __deinit(Ptr self) {
		auto iterator = (BasicArrayIterator<E>*)self;
		iterator->~BasicArrayIterator<E>();
	}

	struct __TypeHolder {
		FunctionPtr _anyPtrs[1] = {
			nullptr,
		};

		FunctionPtr _iteratorPtrs[3] = {
			nullptr,
			&hasNextF__get__s__B,
			&nextF__get__s__t1,
		};

		VirtualTable _tables[2] = {
			createTableOf<Any>(_anyPtrs),
			createTableOf<Iterator<E>>(_iteratorPtrs),
		};

		Type type{
			&__deinit,
			_tables,
			"Builtin.BasicArrayIterator",
		};
	};

	static __TypeHolder __typeHolder;
};

template<typename E>
typename BasicArrayIterator<E>::__TypeHolder BasicArrayIterator<E>::__typeHolder{};

template<typename E>
struct TypeOf<BasicArrayIterator<E>> {
	Type* value = &BasicArrayIterator<E>::__typeHolder.type;
};
