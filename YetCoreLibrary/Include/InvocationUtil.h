#pragma once

#include <type_traits>

#include "GenericUtil.h"
#include "Optional.h"

#define YET_ARG_TYPE(T) typename ArgumentInfo<T>::Type
#define YET_RES_TYPE(T) typename ResultInfo<T>::Type

template<typename T>
struct ArgumentInfo {
	using Type = typename EnableIfElse<std::is_fundamental<T>::value, T, T*>::Type;

	static T* pass(T& arg) {
		return &arg;
	}
};

template<>
struct ArgumentInfo<Ref> {
	using Type = Ptr;

	static Ptr pass(Ref& arg) {
		return arg.get();
	}
};

template<>
struct ArgumentInfo<Nullable> {
	using Type = Ptr;

	static Ptr pass(Nullable& arg) {
		return arg.getValue().get();
	}
};

template<typename T>
struct ArgumentInfo<Optional<T>> {
	using Type = T*;

	static T* pass(Optional<T>& arg) {
		if (arg.isSome()) {
			return &arg._value;
		} else {
			return nullptr;
		}
	}
};

template<typename T>
struct ResultInfo {
	using Type = T;
};

template<>
struct ResultInfo<Ref> {
	using Type = Ptr;
};

template<>
struct ResultInfo<Nullable> {
	using Type = Ptr;
};

template<typename T>
struct AssignmentHelper {
	static void assign(typename ResultInfo<T>::Type* result, T& value) {
		*result = value;
	}
};

template<>
struct AssignmentHelper<Ref> {
	static void assign(Ptr* result, Ref& value) {
		value.retain();
		*result = value.get();
	}
};

template<>
struct AssignmentHelper<Nullable> {
	static void assign(Ptr* result, Nullable& value) {
		auto& ref = value._ref;
		ref.retain();
		*result = ref.get();
	}
};

/// <summary>
/// Specialization for arguments of primitive types
/// </summary>
template<typename T>
inline auto pass(T arg) -> typename std::enable_if<std::is_fundamental<T>::value, T>::type {
	return arg;
}

/// <summary>
/// Specialization for arguments of composite and non-trivial types
/// </summary>
template<typename T>
inline auto pass(T& arg) -> typename std::enable_if<!std::is_fundamental<T>::value, typename ArgumentInfo<T>::Type>::type {
	return ArgumentInfo<T>::pass(arg);
}

template<typename T>
inline void assign(typename ResultInfo<T>::Type* result, T& value) {
	AssignmentHelper<T>::assign(result, value);
}
