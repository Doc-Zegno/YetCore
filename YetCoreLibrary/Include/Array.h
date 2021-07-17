#pragma once

#include "Iterable.h"
#include "ArrayFragment.h"
#include "InvocationUtil.h"

/// <summary>
/// The trait for basic container with a random access getter.
/// Extends <c>Iterable</c>
/// </summary>
/// <remarks>
/// <para>
/// Random access means that its concrete implementations must provide
/// a getter which have to be adequately efficient no matter which index
/// is passed to it. Usually this implies either constant or, at worst case,
/// logarithmic time complexity (scaling with the array's size),
/// while a linear time one is absolutely forbidden.
/// </para>
/// <para>
/// Missing setter does not imply immutability of any instance implementing this trait.
/// In fact, declaring function's argument as an array simply means that
/// this function aren't going to mutate its contents.
/// So it might be beneficial to think of it as if it was the view of some
/// potentially mutable container with array semantics
/// </para>
/// <para>
/// Since any virtual call is usually pretty costy and arrays are so widely used,
/// there are a few dedicated specializations which are supposed to
/// decrease both virtual table lookup time and memory footprint
/// </para>
/// <para>
/// Optimizers are highly encouraged to introduce additional functions' overloads
/// with C-pointer to the C-array instead of canonical <c>Ptr</c>
/// </para>
/// </remarks>
template<typename E>
struct Array {
	struct __Methods {
		struct Indices {
			enum {
				// Iterable
				__box,
				iteratorF__get__s__1tIterator_t1,
				// Array
				decomposeR__s_1tPointer_ArrayFragment_I__I,
				countF__get__s__I,
				getF__operator__s_I__t1,
			};
		};

		struct Signatures {
			using decomposeR__s_1tPointer_ArrayFragment_I__I = intptr_t(*)(Ptr, ArrayFragment*, intptr_t);
			using countF__get__s__I = Ptr(*)(EC*, Ptr, intptr_t*);
			using getF__operator__s_I__t1 = Ptr(*)(EC*, Ptr, int, YET_RES_TYPE(E)*);
		};

		static typename Signatures::decomposeR__s_1tPointer_ArrayFragment_I__I decomposeR__s_1tPointer_ArrayFragment_I__I(FunctionPtr* table) {
			return (typename Signatures::decomposeR__s_1tPointer_ArrayFragment_I__I)table[Indices::decomposeR__s_1tPointer_ArrayFragment_I__I];
		}

		static typename Signatures::countF__get__s__I countF__get__s__I(FunctionPtr* table) {
			return (typename Signatures::countF__get__s__I)table[Indices::countF__get__s__I];
		}

		static typename Signatures::getF__operator__s_I__t1 getF__operator__s_I__t1(FunctionPtr* table) {
			return (typename Signatures::getF__operator__s_I__t1)table[Indices::getF__operator__s_I__t1];
		}
	};
};

extern "C" {
	YETCORELIBRARY_API extern Type yet_Array__type;
}

template<typename E>
struct TypeOf<Array<E>> {
	Type* value = &yet_Array__type;
};
