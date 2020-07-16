#pragma once

#include <cstdio>
#include <cinttypes>
#include "CppUnitTest.h"

#include "Allocator.h"

namespace YetCoreTests {
	template<typename TFunction>
	inline void runWithMemoryCheck(const TFunction& function) {
		auto startCount = Allocator::getAllocatedCount();
		function();
		auto endCount = Allocator::getAllocatedCount();
		Microsoft::VisualStudio::CppUnitTestFramework::Assert::AreEqual(startCount, endCount, L"Memory load before and after test are not equal");
	}

	inline void logValue(const char* place, const char* name, intptr_t value) {
		char buffer[1024] = { 0 };
		sprintf_s(buffer, "  <%s> %s: %" PRIiPTR, place, name, value);
		Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(buffer);
	}

	inline void logAllocatedCount(const char* place) {
		logValue(place, "Allocated count", Allocator::getAllocatedCount());
	}

	inline void assertAllocatedCount(intptr_t expected) {
		Microsoft::VisualStudio::CppUnitTestFramework::Assert::AreEqual(expected, Allocator::getAllocatedCount());
	}
}
