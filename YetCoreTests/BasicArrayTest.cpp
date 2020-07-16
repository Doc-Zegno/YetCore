#include "pch.h"
#include "CppUnitTest.h"

#include "Ref.h"
#include "TestUtil.h"
#include "BasicArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace YetCoreTests {
	TEST_CLASS(BasicArrayTest) {
    private:
        template<typename TFunction>
        void runWithArray(const TFunction& function) {
            runWithMemoryCheck([&function] {
                auto result = BasicArray<int>::__new__V__s(nullptr);
                auto ptr = result.value;
                auto ref = protect(ptr);
                function(ref);
            });
        }

	public:
		TEST_METHOD(BasicFunctionality) {
            runWithArray([](Ref ref) {
                auto ptr = ref.get();
                BasicArray<int>::addG__s_t1__V(nullptr, ptr, 42);
                BasicArray<int>::addG__s_t1__V(nullptr, ptr, 137);
                auto element0 = BasicArray<int>::getG__operator__s_I__t1(nullptr, ptr, 0).value;
                Assert::AreEqual(42, element0);
                auto element1 = BasicArray<int>::getG__operator__s_I__t1(nullptr, ptr, 1).value;
                Assert::AreEqual(137, element1);
                auto error = BasicArray<int>::getG__operator__s_I__t1(nullptr, ptr, 2).error;
                Assert::IsTrue(error);
            });
		}
	};
}
