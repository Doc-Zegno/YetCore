#pragma once

#include "Ref.h"

template<typename T>
struct Optional {
	struct __Empty {};

	bool _isSome;
	union {
		__Empty _empty;
		T _value;
	};

	Optional() : _isSome(false) {}

	explicit Optional(T value) : _isSome(true) {
		new(&_empty) T(value);
	}

	Optional(const Optional& other) : _isSome(other._isSome) {
		if (_isSome) {
			new(&_empty) T(other._value);
		}
	}

	Optional(Optional&& other) : _isSome(other._isSome) {
		if (_isSome) {
			new(&_empty) T(std::move(other._value));
			other._isSome = false;
			other._value.~T();
		}
	}

	void operator=(Optional other) {
		this->~Optional();
		if (other._isSome) {
			new(&_empty) T(std::move(other._value));
			other._isSome = false;
			other._value.~T();
			_isSome = true;
		}
	}

	bool isSome() {
		return _isSome;
	}

	bool isNone() {
		return !_isSome;
	}

	T getValue() {
		return _value;
	}

	~Optional() {
		if (_isSome) {
			_value.~T();
			_isSome = false;
		}
	}
};

template<>
struct Optional<Ref> {
	Ref _ref;

	Optional() = default;

	explicit Optional(Ref ref) : _ref(ref) {}

	bool isSome() {
		return _ref.get();
	}

	bool isNone() {
		return !_ref.get();
	}

	Ref getValue() {
		return _ref;
	}
};

using Nullable = Optional<Ref>;

template<typename T>
inline Optional<T> optionalOf(T value) {
	return Optional<T>(value);
}
