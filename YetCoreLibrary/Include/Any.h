#pragma once

#include "Api.h"
#include "Type.h"
#include "Manageable.h"
#include "StackFrame.h"

/// <summary>
/// A base for all classes conforming Yet virtual method invocation scheme
/// </summary>
struct Any {
	Manageable __manageable;  // NOTE: you can't derive from this without losing a standard layout
	Type* __type;

	Any(Type* type) : __type(type) {}

	struct __Methods {
		struct Indices {
			enum {
				__box,
			};
		};

		struct Signatures {
			using __box = Ptr(*)(Ptr);
		};

		static Signatures::__box __box(FunctionPtr* table) {
			return (Signatures::__box)table[Indices::__box];
		}
	};
};

extern "C" {
	YETCORELIBRARY_API extern Type yet_Any__type;
}

template<>
struct TypeOf<Any> {
	Type* value = &yet_Any__type;
};
