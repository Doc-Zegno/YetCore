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

		static PtrResult createIntArray(int first, int second) {
			auto result = BasicArray<int>::__new__V__s(nullptr);
			auto ref = protect(result.value);
			BasicArray<int>::addG__s_t1__V(nullptr, ref.get(), first);
			BasicArray<int>::addG__s_t1__V(nullptr, ref.get(), second);
			return okResult(ref.unprotect());
		}

	public:
		TEST_METHOD(DefaultCtor) {
			runWithMemoryCheck([] {
				auto ref = Ref();
				Assert::AreEqual(Ptr(), ref.get());
			});
		}

		TEST_METHOD(ValueCtor) {
			runWithArray([](Ptr ptr, intptr_t allocatedCount) {
				auto ref = protect(ptr);
				Assert::AreEqual(ptr, ref.get());
				assertAllocatedCount(allocatedCount);
			});
		}

		TEST_METHOD(CopyCtor) {
			runWithArray([](Ptr ptr, intptr_t allocatedCount) {
				auto ref = protect(ptr);
				auto copy = ref;
				Assert::AreEqual(ptr, ref.get());
				Assert::AreEqual(ptr, copy.get());
				assertAllocatedCount(allocatedCount);
			});
		}

		TEST_METHOD(MoveCtor) {
			runWithArray([](Ptr ptr, intptr_t allocatedCount) {
				auto ref = protect(ptr);
				auto move = std::move(ref);
				Assert::AreEqual(Ptr(), ref.get());
				Assert::AreEqual(ptr, move.get());
				assertAllocatedCount(allocatedCount);
			});
		}

		TEST_METHOD(CopyAssignment) {
			runWithArray([](Ptr ptr, intptr_t allocatedCount) {
				auto copy = Ref();
				{
					auto ref = protect(ptr);
					copy = ref;
					Assert::AreEqual(ptr, ref.get());
					assertAllocatedCount(allocatedCount);
				}
				Assert::AreEqual(ptr, copy.get());
				assertAllocatedCount(allocatedCount);
			});
		}

		TEST_METHOD(MoveAssignment) {
			runWithArray([](Ptr ptr, intptr_t allocatedCount) {
				auto move = Ref();
				{
					auto ref = protect(ptr);
					move = std::move(ref);
					Assert::AreEqual(Ptr(), ref.get());
					assertAllocatedCount(allocatedCount);
				}
				Assert::AreEqual(ptr, move.get());
				assertAllocatedCount(allocatedCount);
			});
		}

		TEST_METHOD(BasicArrayNested) {
			runWithMemoryCheck([] {
				auto result = BasicArray<Ref>::__new__V__s(nullptr);
				auto ptr = result.value;
				auto ref = protect(ptr);
				for (auto i = 0; i < 6; i++) {
					auto result = BasicArray<Ref>::__new__V__s(nullptr);
					auto nestedPtr = result.value;
					auto nestedRef = protect(nestedPtr);
					BasicArray<Ref>::addG__s_t1__V(nullptr, ptr, nestedRef);
				}
				logAllocatedCount(__FUNCTION__);
			});
		}

		TEST_METHOD(ProtectUnprotect) {
			runWithMemoryCheck([] {
				auto startCount = Allocator::getAllocatedCount();
				auto first = 137;
				auto second = 42;
				auto result = createIntArray(first, second);
				auto ref = protect(result.value);
				logAllocatedCount(__FUNCTION__);
				Assert::IsTrue(Allocator::getAllocatedCount() > startCount, L"Array has been released too early");
				Assert::AreEqual(first, BasicArray<int>::getG__operator__s_I__t1(nullptr, ref.get(), 0).value);
				Assert::AreEqual(second, BasicArray<int>::getG__operator__s_I__t1(nullptr, ref.get(), 1).value);
			});
		}
	};
}
