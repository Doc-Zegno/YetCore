#pragma once

#include <utility>

#include "PtrX.h"

/// <summary>
/// A thin wrapper around <code>Ptr</code> which enables automatic reference counting.
/// <note type="caution">
/// It must be considered not null in most cases. Null value is only allowed to indicate
/// it hasn't been initialized with a proper value yet
/// </note>
/// </summary>
struct Ref {
	Ptr _ptr;

	Ref() : _ptr(0) {}

	Ref(Ptr ptr, bool withRetain) : _ptr(ptr) {
		// TODO: Copy object to heap if it's local
		if (withRetain) {
			retain();
		}
	}

	Ref(const Ref& other) : _ptr(other._ptr) {
		retain();
	}

	Ref(Ref&& other) noexcept : _ptr(other._ptr) {
		other._ptr = 0;
	}

	void operator=(Ref other) {
		std::swap(_ptr, other._ptr);
	}

	Ptr get() {
		return _ptr;
	}

	void retain() {
		if (_ptr) {
			::retain(_ptr);
		}
	}

	void release() {
		if (_ptr) {
			::release(_ptr);
		}
	}

	~Ref() {
		if (_ptr) {
			::release(_ptr);
			_ptr = 0;
		}
	}
};

/// <summary>
/// Wrap an automatic reference counter around given object
/// without retaining it
/// </summary>
/// <remarks>
/// It's usually used in order to prevent memory leaks when
/// retrieving referential return values from functions
/// </remarks>
/// <example>
/// <code>
/// auto result = yet_YourFunction__V__S();
/// if (result.error) {
///     ...
/// }
/// auto s = refOf(result.value);
/// </code>
/// </example>
inline Ref refOf(Ptr object) {
	return Ref(object, false);
}
