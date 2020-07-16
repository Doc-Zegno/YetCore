#include "pch.h"
#include "CppUnitTest.h"

#include <string>

#include "Ref.h"
#include "TestUtil.h"
#include "Optional.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace YetCoreTests {
	TEST_CLASS(OptionalTest) {
	public:
		TEST_METHOD(DefaultCtor) {
			runWithMemoryCheck([] {
				auto optional = Optional<std::string>();
				Assert::IsTrue(optional.isNone());
			});
		}

		TEST_METHOD(ValueCtorHeap) {
			runWithMemoryCheck([] {
                auto s = std::string("Sample Text");
                auto optional = optionalOf(s);
				Assert::IsTrue(optional.isSome());
				Assert::AreEqual(s, optional.getValue());
			});
		}

		TEST_METHOD(CopyCtorHeap) {
			runWithMemoryCheck([] {
				auto s = std::string("Sample Text");
				auto optional = optionalOf(s);
				auto copy = optional;
				Assert::IsTrue(optional.isSome());
				Assert::IsTrue(copy.isSome());
				Assert::AreEqual(s, optional.getValue());
				Assert::AreEqual(optional.getValue(), copy.getValue());
			});
		}

		TEST_METHOD(MoveCtorHeap) {
			runWithMemoryCheck([] {
				auto s = std::string("Sample Text");
				auto optional = optionalOf(s);
				auto move = std::move(optional);
				Assert::IsFalse(optional.isSome());
				Assert::IsTrue(move.isSome());
				Assert::AreEqual(s, move.getValue());
			});
		}

		TEST_METHOD(CopyAssignmentHeap) {
			runWithMemoryCheck([] {
				auto copy = Optional<std::string>();
				auto s = std::string("Sample Text");
				{
					auto optional = optionalOf(s);
					copy = optional;
					Assert::IsTrue(optional.isSome());
					Assert::AreEqual(s, optional.getValue());
				}
				Assert::IsTrue(copy.isSome());
				Assert::AreEqual(s, copy.getValue());
			});
		}

		TEST_METHOD(MoveAssignmentHeap) {
			runWithMemoryCheck([] {
				auto move = Optional<std::string>();
				auto s = std::string("Sample Text");
				{
					auto optional = optionalOf(s);
					move = std::move(optional);
					Assert::IsFalse(optional.isSome());
				}
				Assert::IsTrue(move.isSome());
				Assert::AreEqual(s, move.getValue());
			});
		}
	};
}