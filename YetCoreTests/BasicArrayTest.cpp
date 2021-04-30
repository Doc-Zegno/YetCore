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
                Ptr ptr;
                BasicArray<int>::__new__V__s(nullptr, &ptr);
                auto ref = protect(ptr);
                function(ref);
            });
        }

	public:
		TEST_METHOD(BasicFunctionality) {
            runWithArray([](Ref ref) {
                auto ptr = ref.get();
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
	};
}
