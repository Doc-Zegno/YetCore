#pragma once

#include "Ref.h"
#include "Iterator.h"
#include "AllocatorUtil.h"
#include "InvocationUtil.h"

template<typename E>
struct BasicArrayIterator {
	Any __base;
	Ref _array;
	E* _current;
	E* _end;

	BasicArrayIterator(Ptr array, E* start, E* end) : __base(&__typeHolder.type), _array(array, true), _current(start), _end(end) {}

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

	static Ptr __init__s_1tBasicArray_t1_Pt1_Pt1__V(EC* context, Ptr self, Ptr array, E* start, E* end) {
		new((void*)self) BasicArrayIterator<E>(array, start, end);
		return 0;
	}

	static Ptr __new__1tBasicArray_t1_Pt1_Pt1__s(EC* context, Ptr array, E* start, E* end, Ptr* result) {
		auto error = Allocator::allocateOrRaise<BasicArrayIterator<E>>(context, result);
		if (error) {
			return error;
		}
		error = __init__s_1tBasicArray_t1_Pt1_Pt1__V(context, *result, array, start, end);
		if (error) {
			return error;
		}
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
