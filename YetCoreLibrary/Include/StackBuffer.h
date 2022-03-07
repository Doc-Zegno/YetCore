#pragma once

/// <summary>
/// An empty buffer of specified size, guaranteed to be zeroed on construction.
/// It is usually utilized for the "fast" memory allocations
/// of objects with semantics of automatic variables
/// on stack instead of heap in order to
/// increase a performance
/// </summary>
template<size_t SIZE>
struct alignas(16) StackBuffer {
	char bytes[SIZE] = { 0 };

	StackBuffer() {}
};
