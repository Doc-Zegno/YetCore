#pragma once

#include "Ptr.h"
#include "Any.h"

namespace PtrTags {
	constexpr Ptr NO_TAG = 0;
	constexpr Ptr ON_STACK = 15;

	constexpr Ptr TAG_MASK = 0xf;  // Allocated objects must be aligned at the boundary of 16 bytes
	constexpr Ptr LAST_TAG_WITH_DEALLOCATION = NO_TAG;  // Inclusive
}

inline Ptr getTag(Ptr object) {
	return object & PtrTags::TAG_MASK;
}

inline Ptr removeTag(Ptr object) {
	return object & ~PtrTags::TAG_MASK;
}

/// <summary>
/// Get an address of allocated block which a given pointer refers to.
/// This method effectively removes the tag from the pointer
/// and interpreters what's left as a C-pointer.
/// <note type="caution">
/// This method mustn't be called on pointers which actually
/// don't refer to any allocated memory area, e.g.,
/// embedded strings, integers and so on.
/// </note>
/// </summary>
/// <typeparam name="T">The type of object a given pointer refers to</typeparam>
/// <param name="object">The (possibly tagged) pointer to the object of type <c>T</c></param>
/// <returns>A casted C-pointer to allocated object without any tags</returns>
template<typename T>
T* as(Ptr object) {
	return (T*)removeTag(object);
}

extern "C" {
	YETCORELIBRARY_API void yet_Ptr_retainR__s__V(Ptr object);
	YETCORELIBRARY_API bool yet_Ptr_releaseR__s__B(Ptr object);
	YETCORELIBRARY_API Type* yet_Ptr_typeR__get__s__1tPointer_Type(Ptr object);
	YETCORELIBRARY_API FunctionPtr* yet_Ptr_findTableOfR__s_Type__PPV(Ptr object, Type* type);
}

/// <summary>
/// Get the type of the object a given pointer refers to.
/// Must return correct type for the special cases
/// like embedded strings, integers and so on.
/// </summary>
/// <param name="object">The pointer to an object</param>
/// <returns>The type of object. Technically it's not guaranteed to be not <c>null</c>
/// since nothing stops 3rd party plugin writers from passing pointers
/// to objects with invalid memory layout. It's up to the caller
/// side to raise error in such a case</returns>
inline Type* getType(Ptr object) {
	return yet_Ptr_typeR__get__s__1tPointer_Type(object);
}

/// <summary>
/// Find the virtual table implementing a specified type from a given object.
/// Must return correct virtual table for the special cases
/// like embedded strings, integers and so on.
/// </summary>
/// <param name="object">The pointer to an object</param>
/// <param name="type">The type of virtual table</param>
/// <returns>The address of array of functions from a requested virtual table
/// if the given object implements the specified type,
/// and <c>null</c> otherwise</returns>
inline FunctionPtr* findTableOf(Ptr object, Type* type) {
	return yet_Ptr_findTableOfR__s_Type__PPV(object, type);
}

/// <summary>
/// Find the virtual table implementing a specified type from a given object.
/// Must return correct virtual table for the special cases
/// like embedded strings, integers and so on.
/// </summary>
/// <typeparam name="T">The type of virtual table</typeparam>
/// <param name="object">The pointer to an object</param>
/// <returns>The address of array of functions from a requested virtual table
/// if the given object implements the specified type,
/// and <c>null</c> otherwise</returns>
template<typename T>
inline FunctionPtr* findTableOf(Ptr object) {
	return findTableOf(object, typeOf<T>());
}

/// <summary>
/// Increase reference count for given object (if it's on heap)
/// </summary>
/// <param name="object">(NOT NULL)</param>
inline void retain(Ptr object) {
	yet_Ptr_retainR__s__V(object);
}

/// <summary>
/// Decrease reference count for given object (if it's on heap)
/// </summary>
/// <param name="object">(NOT NULL)</param>
/// <returns><code>true</code> if object was deallocated and <code>false</code> otherwise</returns>
inline bool release(Ptr object) {
	return yet_Ptr_releaseR__s__B(object);
}
