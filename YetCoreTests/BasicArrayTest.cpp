#include "pch.h"
#include "CppUnitTest.h"

#include "PtrGuard.h"
#include "TestUtil.h"
#include "BasicArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace YetCoreTests {
	TEST_CLASS(BasicArrayTest) {
    private:
        template<typename TFunction>
        void runWithArray(const TFunction& function) {
            runWithMemoryCheck([&function] {
                PtrGuard guard;
                BasicArray<int>::__new__V__s(nullptr, &guard.ptr);
                function(guard.ptr);
            });
        }

	public:
		TEST_METHOD(BasicFunctionality) {
            runWithArray([](Ptr ptr) {
                BasicArray<int>::addF__s_t1__V(nullptr, ptr, 42);
                BasicArray<int>::addF__s_t1__V(nullptr, ptr, 137);
                int element0;
                BasicArray<int>::getF__operator__s_I__t1(nullptr, ptr, 0, &element0);
                Assert::AreEqual(42, element0);
                int element1;
                BasicArray<int>::getF__operator__s_I__t1(nullptr, ptr, 1, &element1);
                Assert::AreEqual(137, element1);
                int element2;
                auto error = BasicArray<int>::getF__operator__s_I__t1(nullptr, ptr, 2, &element2);
                Assert::IsTrue(error);
            });
		}

        TEST_METHOD(IterableImplementation) {
            runWithArray([](Ptr ptr) {
                BasicArray<int>::addF__s_t1__V(nullptr, ptr, 42);
                BasicArray<int>::addF__s_t1__V(nullptr, ptr, 137);
                auto iterableTable = findTableOf<Iterable<int>>(ptr);
                Assert::IsNotNull(iterableTable);
                auto iteratorGet = (Ptr(*)(EC*, Ptr, Ptr*))iterableTable[int(Iterable<int>::__Methods::iteratorF__get__s__1tIterator_t1)];
                Assert::IsNotNull((void*)iteratorGet);
                PtrGuard iteratorGuard;
                auto error1 = iteratorGet(nullptr, ptr, &iteratorGuard.ptr);
                Assert::IsTrue(iteratorGuard.ptr);
                Assert::IsFalse(error1);
                auto iteratorTable = findTableOf<Iterator<int>>(iteratorGuard.ptr);
                Assert::IsNotNull(iteratorTable);
                auto hasNextGet = (Ptr(*)(EC*, Ptr, bool*))iteratorTable[int(Iterator<int>::__Methods::hasNextF__get__s__B)];
                Assert::IsNotNull((void*)hasNextGet);
                auto nextGet = (Ptr(*)(EC*, Ptr, int*))iteratorTable[int(Iterator<int>::__Methods::nextF__get__s__t1)];
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
