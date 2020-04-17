#pragma once

#include <cstdint>

struct ConstCharResult {
	const char* value;
	uintptr_t error;
};

inline ConstCharResult okResult(const char* value) {
	return ConstCharResult{ value };
}

inline ConstCharResult errorConstCharResult(uintptr_t error) {
	return ConstCharResult{ nullptr, error };
}
