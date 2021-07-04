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

	enum class __Methods {
	};
};

extern "C" {
	YETCORELIBRARY_API extern Type yet_Any__type;
}

template<>
struct TypeOf<Any> {
	Type* value = &yet_Any__type;
};
