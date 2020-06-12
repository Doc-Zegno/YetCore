#pragma once

#include "Ptr.h"

struct VoidResult {
	Ptr error;
};

struct ConstCharResult {
	const char* value;
	Ptr error;
};

struct PtrResult {
	Ptr value;
	Ptr error;
};

template<typename V>
struct Result {
	V value;
	Ptr error;
};

inline VoidResult okResult() {
	return VoidResult{ 0 };
}

inline VoidResult errorVoidResult(Ptr error) {
	return VoidResult{ error };
}

inline ConstCharResult okResult(const char* value) {
	return ConstCharResult{ value, 0 };
}

inline ConstCharResult errorConstCharResult(Ptr error) {
	return ConstCharResult{ nullptr, error };
}

inline PtrResult okResult(Ptr value) {
	return PtrResult{ value, 0 };
}

inline PtrResult errorPtrResult(Ptr error) {
	return PtrResult{ 0, error };
}

template<typename V>
inline Result<V> okGenericResult(V value) {
	return Result<V>{ value, 0 };
}

template<typename V>
inline Result<V> errorGenericResult(Ptr error) {
	return Result<V>{ V(), error };
}
