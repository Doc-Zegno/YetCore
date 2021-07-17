#pragma once

#include <exception>

#include "PtrX.h"

/// <summary>
/// Extension of <c>Ptr</c> that allows either to speed up virtual table lookup
/// or to dynamically change the behavior of object referenced via <c>Ptr</c>.
/// Note that a caller side doesn't have to provide any virtual table at all,
/// i.e., passing <c>0</c> instead is absolutely valid option and means
/// that callee side have to find needed virtual table on its own.
/// On the other hand, if the virtual table is provided,
/// the callee side must use it no matter what
/// </summary>
/// <remarks>
/// <para>
/// API writers are encouraged to provide overloads of their functions
/// which accept <c>FatPtr</c>s instead of regular <c>Ptr</c>s
/// if they are expecting arguments of interface types,
/// as it may significantly increase performance
/// </para>
/// <para>
/// Optimizers are encouraged to replace adapters introduced by programmers
/// with the <c>FatPtr</c>s so there is no need to allocate
/// temporary objects on the heap anymore
/// </para>
/// <para>
/// If virtual table is not null, wrapped <c>Ptr</c> must not be assumed
/// to represent anything meaningful without it, i.e., it's not
/// even guaranteed that such a pointer actually refers
/// to any object allocated on the heap.
/// When needed, <c>getStandalonePtr()</c> must be used in order to
/// obtain a <c>Ptr</c> which can be used without
/// a corresponding virtual table
/// </para>
/// </remarks>
struct FatPtr {
	Ptr ptr;
	FunctionPtr* table;

	FatPtr(Ptr ptr, FunctionPtr* table = nullptr) : ptr(ptr), table(table) {}

	template<typename T>
	FunctionPtr* findTableOf() {
		if (table != nullptr) {
			return table;
		} else {
			return ::findTableOf<T>(ptr);
		}
	}

	/// <summary>
	/// Get an instance of <c>Ptr</c> which can be used independently without
	/// a corresponding virtual table. This operation can allocate
	/// additional objects on the heap so prefer to cache
	/// the result of its invocation
	/// </summary>
	Ptr getStandalonePtr() {
		if (table != nullptr) {
			auto boxPtr = Any::__Methods::__box(table);
			if (boxPtr != nullptr) {
				return boxPtr(ptr);
			}
		}
		return ptr;
	}
};
