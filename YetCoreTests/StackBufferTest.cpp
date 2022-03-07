#include "pch.h"
#include "CppUnitTest.h"

#include <initializer_list>

#include "PtrGuard.h"
#include "TestUtil.h"
#include "BasicArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace YetCoreTests {
    TEST_CLASS(StackBufferTest) {
    private:
        template<typename TFunction>
        void runWithArray(const TFunction& function) {
            runWithMemoryCheck([&function] {
                // Note the stack placement of array,
                // should work just fine since it is used as an automatic variable
                StackBuffer<64> buffer;
                PtrGuard guard(&buffer);
                BasicArray<int>::__new__V__s(nullptr, &guard.ptr);
                function(guard.ptr);
            });
        }

        template<typename TFunction>
        void runWithArray(const std::initializer_list<int>& values, const TFunction& function) {
            runWithArray([&values, &function](Ptr ptr) {
                for (auto value : values) {
                    BasicArray<int>::addF__s_t1__V(nullptr, ptr, value);
                }
                function(ptr);
            });
        }

    public:
        TEST_METHOD(AllocateArrayOnStack) {
            runWithMemoryCheck([] {
                // Check that a stack buffer really helps to save allocations on heap
                auto allocatedCountBefore = Allocator::getAllocatedCount();
                decltype(allocatedCountBefore) allocatedCountDelta = 0;
                {
                    // Firstly, find out how many allocations "regular" ctor requires
                    PtrGuard guard;
                    BasicArray<int>::__new__V__s(nullptr, &guard.ptr);
                    auto allocatedCountAfter = Allocator::getAllocatedCount();
                    allocatedCountDelta = allocatedCountAfter - allocatedCountBefore;
                }
                {
                    // Secondly, compare it with allocation on stack. Should save 1 allocation
                    StackBuffer<64> buffer;
                    PtrGuard guard(&buffer);
                    BasicArray<int>::__new__V__s(nullptr, &guard.ptr);
                    auto allocatedCountAfter = Allocator::getAllocatedCount();
                    Assert::AreEqual(allocatedCountDelta - 1, allocatedCountAfter - allocatedCountBefore);
                }
            });
        }

        TEST_METHOD(AllocateIteratorOnStack) {
            runWithArray({ 42, 137 }, [](Ptr ptr) {
                // On might say this test explores the limits
                // of the stack placement during a memory allocation,
                // i.e., whether it's possible to allocate array's iterator on stack
                // if the array itself has already been allocated on stack?
                // The theory says it must be possible to do so
                // if an iterator is used as local variable.
                // Apart from this, it is the copy of the corresponding test
                // of BasicArray's iterable capabilities
                auto iterableTable = findTableOf<Iterable<int>>(ptr);
                Assert::IsNotNull(iterableTable);
                auto iteratorGet = Iterable<int>::__Methods::iteratorF__get__s__1tIterator_t1(iterableTable);
                Assert::IsNotNull((void*)iteratorGet);

                // Allocate an iterator on stack.
                // The number of allocations on heap should stay unchanged
                auto allocatedCountBefore = Allocator::getAllocatedCount();
                StackBuffer<64> buffer;
                PtrGuard iteratorGuard(&buffer);
                auto error1 = iteratorGet(nullptr, ptr, &iteratorGuard.ptr);
                Assert::IsTrue(iteratorGuard.ptr);
                Assert::IsFalse(error1);
                auto allocatedCountAfter = Allocator::getAllocatedCount();
                Assert::AreEqual(allocatedCountBefore, allocatedCountAfter);

                // Check that obtained iterator is still functioning correctly
                auto iteratorTable = findTableOf<Iterator<int>>(iteratorGuard.ptr);
                Assert::IsNotNull(iteratorTable);
                auto hasNextGet = Iterator<int>::__Methods::hasNextF__get__s__B(iteratorTable);
                Assert::IsNotNull((void*)hasNextGet);
                auto nextGet = Iterator<int>::__Methods::nextF__get__s__t1(iteratorTable);
                Assert::IsNotNull((void*)nextGet);

                auto hasNext = false;
                auto error2 = hasNextGet(nullptr, iteratorGuard.ptr, &hasNext);
                Assert::IsTrue(hasNext);
                Assert::IsFalse(error2);
                auto next = 0;
                auto error3 = nextGet(nullptr, iteratorGuard.ptr, &next);
                Assert::IsFalse(error3);
                Assert::AreEqual(42, next);

                auto error4 = hasNextGet(nullptr, iteratorGuard.ptr, &hasNext);
                Assert::IsTrue(hasNext);
                Assert::IsFalse(error4);
                auto error5 = nextGet(nullptr, iteratorGuard.ptr, &next);
                Assert::IsFalse(error5);
                Assert::AreEqual(137, next);

                auto error6 = hasNextGet(nullptr, iteratorGuard.ptr, &hasNext);
                Assert::IsFalse(hasNext);
                Assert::IsFalse(error6);
                auto error7 = nextGet(nullptr, iteratorGuard.ptr, &next);
                Assert::IsTrue(error7);
            });
        }
    };
}