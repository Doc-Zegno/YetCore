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
				Ptr ptr;
				BasicArray<Ref>::__new__V__s(nullptr, &ptr);
				auto allocatedCount = Allocator::getAllocatedCount();
				function(ptr, allocatedCount);
			});
		}

		static Ptr createIntArray(int first, int second) {
			Ptr ptr;
			BasicArray<int>::__new__V__s(nullptr, &ptr);
			auto ref = protect(ptr);
			BasicArray<int>::addF__s_t1__V(nullptr, ref.get(), first);
			BasicArray<int>::addF__s_t1__V(nullptr, ref.get(), second);
			return ref.unprotect();
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
				Ptr ptr;
				BasicArray<Ref>::__new__V__s(nullptr, &ptr);
				auto ref = protect(ptr);
				for (auto i = 0; i < 6; i++) {
					Ptr nestedPtr;
					auto result = BasicArray<Ref>::__new__V__s(nullptr, &nestedPtr);
					auto nestedRef = protect(nestedPtr);
					BasicArray<Ref>::addF__s_t1__V(nullptr, ptr, nestedRef);
				}
				logAllocatedCount(__FUNCTION__);
			});
		}

		TEST_METHOD(ProtectUnprotect) {
			runWithMemoryCheck([] {
				auto startCount = Allocator::getAllocatedCount();
				auto first = 137;
				auto second = 42;
				auto ptr = createIntArray(first, second);
				auto ref = protect(ptr);
				logAllocatedCount(__FUNCTION__);
				Assert::IsTrue(Allocator::getAllocatedCount() > startCount, L"Array has been released too early");
				int value;
				BasicArray<int>::getF__operator__s_I__t1(nullptr, ref.get(), 0, &value);
				Assert::AreEqual(first, value);
				BasicArray<int>::getF__operator__s_I__t1(nullptr, ref.get(), 1, &value);
				Assert::AreEqual(second, value);
			});
		}
	};
}
