#pragma once

#include "Any.h"

/// <summary>
/// The universal trait for all containers that can be iterated,
/// i.e., whose elements can be accessed sequentially one by one
/// </summary>
/// <remarks>
/// <para>
/// Programmers shouldn't put any particular semantics in the order
/// of element iteration. Such containers as maps and sets
/// don't have any meaningful iteration order obviously.
/// Even more, the same order is not guaranteed when iterating
/// over the same container again (in general case).
/// If you need stronger guarantees about the iteration order,
/// consider using more specialized traits
/// such as <c>Array</c>
/// </para>
/// <para>
/// The main and only difference between <c>Iterable</c> and <c>Iterator</c>
/// is that the former allows to iterate over its elements
/// an infinite number of times whilst the latter
/// allows to do it only once
/// </para>
/// <para>
/// It's assumed that sequential element access has either constant
/// or logarithmic (for tree-like structures) time complexity.
/// If a container doesn't meet this requirement,
/// it shouldn't be marked as <c>Iterable</c>
/// </para>
/// </remarks>
template<typename E>
struct Iterable {
	struct __Methods {
		struct Indices {
			enum {
				__box,
				iteratorF__get__s__1tIterator_t1,
			};
		};

		struct Signatures {
			using iteratorF__get__s__1tIterator_t1 = Ptr(*)(EC*, Ptr, Ptr*);
		};

		static typename Signatures::iteratorF__get__s__1tIterator_t1 iteratorF__get__s__1tIterator_t1(FunctionPtr* table) {
			return (typename Signatures::iteratorF__get__s__1tIterator_t1)table[Indices::iteratorF__get__s__1tIterator_t1];
		}
	};
};

extern "C" {
	YETCORELIBRARY_API extern Type yet_Iterable__type;
}

template<typename E>
struct TypeOf<Iterable<E>> {
	Type* value = &yet_Iterable__type;
};

