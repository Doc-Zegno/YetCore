#include <iostream>
#include <type_traits>

#include "Any.h"
#include "Ref.h"
#include "PtrX.h"
#include "Module.h"
#include "Allocator.h"
#include "BasicArray.h"
#include "StackFrame.h"

void* operator new(size_t size) {
    return yet_allocateR__U_B_B__PV((uintptr_t)size, false, false);
}

void operator delete(void* object) {
    yet_deallocateR__PV__V(object);
}

void demoModulePath() {
    auto result = yet_Module_pathF__get__V__PC8(nullptr);
    if (result.error) {
        std::cerr << "Error ptr: " << result.error << std::endl;
    } else {
        std::cout << "Module path: '" << result.value << "'\n";
    }
}

void demoBasicArray() {
    auto result = BasicArray<int>::__new__V__s(nullptr);
    auto ptr = result.value;
    auto ref = refOf(ptr);
    std::cout << "Num allocated: " << yet_allocatedCountR__get__V__I() << std::endl;
    BasicArray<int>::addG__s_t1__V(nullptr, ptr, 42);
    BasicArray<int>::addG__s_t1__V(nullptr, ptr, 137);
    std::cout << "Num allocated: " << yet_allocatedCountR__get__V__I() << std::endl;
    std::cout << "Element #0: " << BasicArray<int>::getG__operator__s_I__t1(nullptr, ptr, 0).value << std::endl;
    std::cout << "Element #1: " << BasicArray<int>::getG__operator__s_I__t1(nullptr, ptr, 1).value << std::endl;
    std::cout << "Element #2: error? -> " << BasicArray<int>::getG__operator__s_I__t1(nullptr, ptr, 2).error << std::endl;

    auto table = findTableOf<Any>(ptr);
    if (table->type == &yet_Any__type) {
        std::cout << "Found Any table at address: " << table->type << std::endl;
    }
}

void demoVector() {
    auto values = std::vector<int>();
    for (auto i = 0; i < 6; i++) {
        values.push_back(i);
    }
    std::cout << "Done\n";
}

void demoBasicArrayNested() {
    auto result = BasicArray<Ref>::__new__V__s(nullptr);
    auto ptr = result.value;
    auto ref = refOf(ptr);
    for (auto i = 0; i < 6; i++) {
        auto result = BasicArray<Ref>::__new__V__s(nullptr);
        auto nestedPtr = result.value;
        auto nestedRef = refOf(nestedPtr);
        BasicArray<Ref>::addG__s_t1__V(nullptr, ptr, nestedRef);
    }
    std::cout << "Num allocated: " << yet_allocatedCountR__get__V__I() << std::endl;
}

void printStackTrace(ExecutionContext* context) {
    std::cout << "Stack trace:\n";
    for (auto frame = context->current; frame; frame = frame->previous) {
        std::cout << "\tat " << frame->functionName << ":" << frame->location.firstLine << "\n";
    }
}

void demoNested2(ExecutionContext* context) {
    StackFrame frame(context, __FUNCTION__);
    frame.setLineAfter(__LINE__);
    printStackTrace(context);
}

void demoNested1(ExecutionContext* context) {
    StackFrame frame(context, __FUNCTION__);
    frame.setLineAfter(__LINE__);
    demoNested2(context);
}

int main() {
    ExecutionContext* context = nullptr;
    StackFrame frame(context, __FUNCTION__);
    demoModulePath();
    demoBasicArray();
    demoVector();
    demoBasicArrayNested();
    frame.setLineAfter(__LINE__);
    demoNested1(context);
    printStackTrace(context);
    std::cout << "Any name: " << typeOf<Any>()->name << std::endl;
    std::cout << "Array name: " << BasicArray<int>::__typeHolder.type.name << std::endl;
    std::cout << "Num allocated on exit: " << yet_allocatedCountR__get__V__I() << std::endl;
}
