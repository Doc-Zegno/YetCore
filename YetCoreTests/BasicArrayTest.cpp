#include "pch.h"
#include "CppUnitTest.h"

#include <initializer_list>

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
            runWithArray({ 42, 137 }, [](Ptr ptr) {
                auto iterableTable = findTableOf<Iterable<int>>(ptr);
                Assert::IsNotNull(iterableTable);
                auto iteratorGet = Iterable<int>::__Methods::iteratorF__get__s__1tIterator_t1(iterableTable);
                Assert::IsNotNull((void*)iteratorGet);
                PtrGuard iteratorGuard;
                auto error1 = iteratorGet(nullptr, ptr, &iteratorGuard.ptr);
                Assert::IsTrue(iteratorGuard.ptr);
                Assert::IsFalse(error1);
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

        TEST_METHOD(ArrayImplementationDecompose) {
            runWithArray({ 42, 137 }, [](Ptr ptr) {
                auto table = findTableOf<Array<int>>(ptr);
                Assert::IsNotNull(table);
                auto decompose = Array<int>::__Methods::decomposeR__s_1tPointer_ArrayFragment_I__I(table);
                Assert::IsNotNull((void*)decompose);
                ArrayFragment fragments[3];
                auto fragmentCount = decompose(ptr, fragments, 3);
                Assert::AreEqual(intptr_t(1), fragmentCount);
                Assert::AreEqual(intptr_t(2), fragments[0].length);
                auto values = (int*)fragments[0].start;
                Assert::AreEqual(42, values[0]);
                Assert::AreEqual(137, values[1]);
            });
        }
	};
}
