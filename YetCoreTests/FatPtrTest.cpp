#include "pch.h"
#include "CppUnitTest.h"

#include <initializer_list>

#include "PtrGuard.h"
#include "TestUtil.h"
#include "ArrayUtil.h"
#include "BasicArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace YetCoreTests {
    TEST_CLASS(FatPtrTest) {
    private:
        template<typename TFunction>
        void runWithBasicArray(const TFunction& function) {
            runWithMemoryCheck([&function] {
                PtrGuard guard;
                BasicArray<int>::__new__V__s(nullptr, &guard.ptr);
                function(FatPtr(guard.ptr, BasicArray<int>::__typeHolder._arrayPtrs));
            });
        }

        template<typename TFunction>
        void runWithBasicArray(const std::initializer_list<int>& values, const TFunction& function) {
            runWithBasicArray([&values, &function](FatPtr fatPtr) {
                for (auto value : values) {
                    BasicArray<int>::addF__s_t1__V(nullptr, fatPtr.ptr, value);
                }
                function(fatPtr);
            });
        }

        template<typename TFunction>
        void runWithLocalArray(const std::initializer_list<int>& values, const TFunction& function) {
            auto elements = const_cast<int*>(values.begin());
            auto elementCount = intptr_t(values.size());
            LocalArray<int> localArray(elements, elementCount);
            function(localArray.toFatPtr());
        }

        static void checkEqual(const std::initializer_list<int>& expected, FatPtr fatPtr) {
            Assert::IsNotNull((void*)fatPtr.taggedTable);
            auto table = fatPtr.findTableOf<Array<int>>();
            Assert::IsNotNull(table);
            auto countGet = Array<int>::__Methods::countF__get__s__I(table);
            Assert::IsNotNull((void*)countGet);
            auto getOperator = Array<int>::__Methods::getF__operator__s_I__t1(table);
            Assert::IsNotNull((void*)getOperator);
            intptr_t countResult;
            Ptr error1 = countGet(nullptr, fatPtr.ptr, &countResult);
            Assert::IsFalse(error1);
            Assert::AreEqual(intptr_t(expected.size()), countResult);
            for (intptr_t i = 0; i < countResult; i++) {
                int getResult;
                Ptr error2 = getOperator(nullptr, fatPtr.ptr, int(i), &getResult);
                Assert::IsFalse(error2);
                Assert::AreEqual(expected.begin()[i], getResult);
            }
        }

    public:
        TEST_METHOD(WithBasicArray) {
            auto elements = { 1, 1, 2, 3, 5, 8, 13, 21, 34 };
            runWithBasicArray(elements, [&elements](FatPtr fatPtr) {
                checkEqual(elements, fatPtr);
            });
        }

        TEST_METHOD(WithLocalArray) {
            auto elements = { 1, 1, 2, 3, 5, 8, 13, 21, 34 };
            runWithLocalArray(elements, [&elements](FatPtr fatPtr) {
                checkEqual(elements, fatPtr);
            });
        }
    };
}
