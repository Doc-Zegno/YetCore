#pragma once

#include <atomic>
#include <cstdint>
#include <type_traits>

/// <summary>
/// A base for all classes compatible with Yet automatic references
/// </summary>
struct Manageable {
	std::atomic<intptr_t> __refCount;

	Manageable() : __refCount(1) {}
};
