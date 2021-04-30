#pragma once

#include <utility>

#include "PtrX.h"

/// <summary>
/// A thin wrapper around <c>Ptr</c> which enables automatic reference counting.
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

	/// <summary>
	/// Disable this automatic reference and return protected pointer.
	/// <note type="caution">
	/// From this moment, the reference is considered to be invalid
	/// and its destructor won't be called thus previously protected
	/// pointer won't be released
	/// </note>
	/// </summary>
	/// <remarks>
	/// The only justified usage for this method is to return
	/// protected pointer from a function without releasing it.
	/// Keep in mind, that this task can be accomplished by the means
	/// of <c>retain()</c> method as well (see example section)
	/// but <c>unprotect()</c> is significantly cheaper
	/// since it doesn't introduce any unnecessary calculations
	/// </remarks>
	/// <example>
	/// <code>
	/// Ptr value;
	/// auto error = yet_YourFunction__V__S(&value);
	/// auto s = protect(value);
	/// ...
	/// // The recommended approach:
	/// return okResult(s.unprotect());
	/// // Performs the same task but less effective and more verbose:
	/// s.retain();
	/// return okResult(s.get());
	/// </code>
	/// </example>
	/// <returns>Previously protected pointer</returns>
	Ptr unprotect() {
		auto ptr = _ptr;
		_ptr = 0;
		return ptr;
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
/// Ptr value;
/// auto error = yet_YourFunction__V__S(&value);
/// if (error) {
///     ...
/// }
/// auto s = protect(value);
/// </code>
/// </example>
inline Ref protect(Ptr object) {
	return Ref(object, false);
}
