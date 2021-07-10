#pragma once

#include "Any.h"
#include "InvocationUtil.h"

/// <summary>
/// The abstraction of C-pointer to elements of C-array.
/// Iterator allows to sequentially access elements
/// of some container hiding the details
/// about its internal structure
/// </summary>
template<typename E>
struct Iterator {
	struct __Methods {
		struct Indices {
			enum {
				__box,
				hasNextF__get__s__B,
				nextF__get__s__t1,
			};
		};

		struct Signatures {
			using hasNextF__get__s__B = Ptr(*)(EC*, Ptr, bool*);
			using nextF__get__s__t1 = Ptr(*)(EC*, Ptr, YET_RES_TYPE(E)*);
		};

		static typename Signatures::hasNextF__get__s__B hasNextF__get__s__B(FunctionPtr* table) {
			return (typename Signatures::hasNextF__get__s__B)table[Indices::hasNextF__get__s__B];
		}

		static typename Signatures::nextF__get__s__t1 nextF__get__s__t1(FunctionPtr* table) {
			return (typename Signatures::nextF__get__s__t1)table[Indices::nextF__get__s__t1];
		}
	};
};

extern "C" {
	YETCORELIBRARY_API extern Type yet_Iterator__type;
}

template<typename E>
struct TypeOf<Iterator<E>> {
	Type* value = &yet_Iterator__type;
};
