#include "pch.h"
#include "CppUnitTest.h"

#include "Ref.h"
#include "TestUtil.h"
#include "BasicArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace YetCoreTests {
	TEST_CLASS(RefTest) {
	private:
		template<typename TFunction>
		void runWithArray(const TFunction& function) {
			runWithMemoryCheck([&function] {
				auto result = BasicArray<Ref>::__new__V__s(nullptr);
				auto ptr = result.value;
				auto allocatedCount = Allocator::getAllocatedCount();
				function(ptr, allocatedCount);
			});
		}

	public:
		TEST_METHOD(DefaultCtor) {
			runWithMemoryCheck([] {
				auto ref = Ref();
				Assert::AreEqual(Ptr(), ref._ptr);
			});
		}

		TEST_METHOD(ValueCtor) {
			runWithArray([](Ptr ptr, intptr_t allocatedCount) {
				auto ref = refOf(ptr);
				Assert::AreEqual(ptr, ref._ptr);
				assertAllocatedCount(allocatedCount);
			});
		}

		TEST_METHOD(CopyCtor) {
			runWithArray([](Ptr ptr, intptr_t allocatedCount) {
				auto ref = refOf(ptr);
				auto copy = ref;
				Assert::AreEqual(ptr, ref._ptr);
				Assert::AreEqual(ptr, copy._ptr);
				assertAllocatedCount(allocatedCount);
			});
		}

		TEST_METHOD(MoveCtor) {
			runWithArray([](Ptr ptr, intptr_t allocatedCount) {
				auto ref = refOf(ptr);
				auto move = std::move(ref);
				Assert::AreEqual(Ptr(), ref._ptr);
				Assert::AreEqual(ptr, move._ptr);
				assertAllocatedCount(allocatedCount);
			});
		}

		TEST_METHOD(CopyAssignment) {
			runWithArray([](Ptr ptr, intptr_t allocatedCount) {
				auto copy = Ref();
				{
					auto ref = refOf(ptr);
					copy = ref;
					Assert::AreEqual(ptr, ref._ptr);
					assertAllocatedCount(allocatedCount);
				}
				Assert::AreEqual(ptr, copy._ptr);
				assertAllocatedCount(allocatedCount);
			});
		}

		TEST_METHOD(MoveAssignment) {
			runWithArray([](Ptr ptr, intptr_t allocatedCount) {
				auto move = Ref();
				{
					auto ref = refOf(ptr);
					move = std::move(ref);
					Assert::AreEqual(Ptr(), ref._ptr);
					assertAllocatedCount(allocatedCount);
				}
				Assert::AreEqual(ptr, move._ptr);
				assertAllocatedCount(allocatedCount);
			});
		}

		TEST_METHOD(BasicArrayNested) {
			runWithMemoryCheck([] {
				auto result = BasicArray<Ref>::__new__V__s(nullptr);
				auto ptr = result.value;
				auto ref = refOf(ptr);
				for (auto i = 0; i < 6; i++) {
					auto result = BasicArray<Ref>::__new__V__s(nullptr);
					auto nestedPtr = result.value;
					auto nestedRef = refOf(nestedPtr);
					BasicArray<Ref>::addG__s_t1__V(nullptr, ptr, nestedRef);
				}
				logAllocatedCount(__FUNCTION__);
			});
		}
	};
}
