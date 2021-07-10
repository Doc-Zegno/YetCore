#pragma once

#include "Any.h"

/// <summary>
/// The abstraction of C-pointer to elements of C-array.
/// Iterator allows to sequentially access elements
/// of some container hiding the details
/// about its internal structure
/// </summary>
template<typename E>
struct Iterator {
	enum class __Methods {
		__box,
		hasNextF__get__s__B,
		nextF__get__s__t1,
	};
};

extern "C" {
	YETCORELIBRARY_API extern Type yet_Iterator__type;
}

template<typename E>
struct TypeOf<Iterator<E>> {
	Type* value = &yet_Iterator__type;
};
