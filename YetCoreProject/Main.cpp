#include <string>
#include <iostream>
#include <functional>
#include <type_traits>

#include "Any.h"
#include "Ref.h"
#include "PtrX.h"
#include "Module.h"
#include "PtrGuard.h"
#include "Optional.h"
#include "Allocator.h"
#include "BasicArray.h"
#include "StackFrame.h"
#include "InvocationUtil.h"

void* operator new(size_t size) {
    return Allocator::allocate((uintptr_t)size, false, false);
}

void operator delete(void* object) {
    Allocator::deallocate(object);
}

void runDemo(const char* name, std::function<void(void)> demo) {
    std::cout << "<" << name << ">\n";
    std::cout << "<demo>\n";
    demo();
    std::cout << "</demo>\n";
    std::cout << "Num allocated on exit: " << Allocator::getAllocatedCount() << "\n";
    std::cout << "</" << name << ">\n\n";
}

void demoModulePath() {
    runDemo("Module Path", [] {
        const char* path;
        auto error = yet_Module_pathF__get__V__PC8(nullptr, &path);
        if (error) {
            std::cerr << "Error ptr: " << error << std::endl;
        } else {
            std::cout << "Module path: '" << path << "'\n";
        }
    });
}

void demoBasicArray() {
    runDemo("Basic Array", [] {
        PtrGuard guard;
        BasicArray<int>::__new__V__s(nullptr, &guard.ptr);
        std::cout << "Num allocated: " << Allocator::getAllocatedCount() << std::endl;
        BasicArray<int>::addF__s_t1__V(nullptr, guard.ptr, 42);
        BasicArray<int>::addF__s_t1__V(nullptr, guard.ptr, 137);
        std::cout << "Num allocated: " << Allocator::getAllocatedCount() << std::endl;
        int value;
        BasicArray<int>::getF__operator__s_I__t1(nullptr, guard.ptr, 0, &value);
        std::cout << "Element #0: " << value << std::endl;
        BasicArray<int>::getF__operator__s_I__t1(nullptr, guard.ptr, 1, &value);
        std::cout << "Element #1: " << value << std::endl;
        auto error = BasicArray<int>::getF__operator__s_I__t1(nullptr, guard.ptr, 2, &value);
        std::cout << "Element #2: error? -> " << error << std::endl;
    });
}

void demoBasicArrayVirtualDispatch() {
    runDemo("Basic Array Virtual Dispatch", [] {
        PtrGuard guard;
        BasicArray<int>::__new__V__s(nullptr, &guard.ptr);
        std::cout << "Num allocated: " << Allocator::getAllocatedCount() << std::endl;
        BasicArray<int>::addF__s_t1__V(nullptr, guard.ptr, 42);
        BasicArray<int>::addF__s_t1__V(nullptr, guard.ptr, 137);
        std::cout << "Num allocated: " << Allocator::getAllocatedCount() << std::endl;

        auto iterableTable = findTableOf<Iterable<int>>(guard.ptr);
        if (iterableTable != nullptr) {
            std::cout << "Found Iterable<int> table at address: " << iterableTable << std::endl;
            auto iteratorPtr = Iterable<int>::__Methods::iteratorF__get__s__1tIterator_t1(iterableTable);
            std::cout << "Iterator's getter: " << iteratorPtr << std::endl;
            if (iteratorPtr != nullptr) {
                PtrGuard iteratorGuard;
                iteratorPtr(nullptr, guard.ptr, &iteratorGuard.ptr);
                std::cout << "Iterator: " << (void*)iteratorGuard.ptr << std::endl;
                std::cout << "Num allocated: " << Allocator::getAllocatedCount() << std::endl;
                auto iteratorTable = findTableOf<Iterator<int>>(iteratorGuard.ptr);
                if (iteratorTable != nullptr) {
                    auto hasNextPtr = Iterator<int>::__Methods::hasNextF__get__s__B(iteratorTable);
                    auto nextPtr = Iterator<int>::__Methods::nextF__get__s__t1(iteratorTable);
                    auto hasNext = false;
                    while (true) {
                        hasNextPtr(nullptr, iteratorGuard.ptr, &hasNext);
                        if (!hasNext) {
                            break;
                        }
                        auto next = 0;
                        nextPtr(nullptr, iteratorGuard.ptr, &next);
                        std::cout << "    next: " << next << std::endl;
                    }
                }
            }
        }

        auto arrayTable = findTableOf<Array<int>>(guard.ptr);
        if (arrayTable != nullptr) {
            std::cout << "Found Array<int> table at address: " << arrayTable << std::endl;
            auto boxPtr = Any::__Methods::__box(arrayTable);
            std::cout << "Boxing: " << boxPtr << std::endl;
            auto getPtr = Array<int>::__Methods::getF__operator__s_I__t1(arrayTable);
            std::cout << "Getter: " << getPtr << std::endl;
            if (getPtr != nullptr) {
                int value;
                Ptr error;
                for (int i = 0; i < 3; i++) {
                    error = getPtr(nullptr, guard.ptr, i, &value);
                    std::cout << "Element #" << i << ": " << value << std::endl;
                    std::cout << "    error: " << error << std::endl;
                }
            }
        }
    });
}

void demoVector() {
    runDemo("Vector", [] {
        auto values = std::vector<int>();
        for (auto i = 0; i < 6; i++) {
            values.push_back(i);
        }
    });
}

void demoBasicArrayNested() {
    runDemo("Basic Array Nested", [] {
        PtrGuard guard;
        BasicArray<Ref>::__new__V__s(nullptr, &guard.ptr);
        for (auto i = 0; i < 6; i++) {
            Ptr nestedPtr = 0;
            BasicArray<Ref>::__new__V__s(nullptr, &nestedPtr);
            auto nestedRef = protect(nestedPtr);
            BasicArray<Ref>::addF__s_t1__V(nullptr, guard.ptr, nestedRef);
            PtrGuard elementGuard;
            BasicArray<Ref>::getF__operator__s_I__t1(nullptr, guard.ptr, i, &elementGuard.ptr);
            std::cout << "Num allocated at step #" << i << ": " << Allocator::getAllocatedCount() << std::endl;
        }
        std::cout << "Num allocated: " << Allocator::getAllocatedCount() << std::endl;
    });
}

void demoOptionalString() {
    runDemo("Optional String", [] {
        auto s = std::string("Sample Text");
        std::cout << "Num allocated at start: " << Allocator::getAllocatedCount() << "\n";
        auto optional = optionalOf(s);
        std::cout << "After one optional: " << Allocator::getAllocatedCount() << "\n";
        auto copy = optional;
        std::cout << "After copy: " << Allocator::getAllocatedCount() << "\n";
        std::cout << "Is optional some? -> " << optional.isSome() << "\n";
        std::cout << "Is copy some? -> " << copy.isSome() << "\n";
        std::cout << "Copy's value: " << copy.getValue() << "\n";
        auto move = std::move(copy);
        std::cout << "After move: " << Allocator::getAllocatedCount() << "\n";
        std::cout << "Is source some? -> " << copy.isSome() << "\n";
        std::cout << "Is destination some -> " << move.isSome() << "\n";
        std::cout << "Move's value: " << move.getValue() << "\n";
        copy = move;
        std::cout << "After copy assignment: " << Allocator::getAllocatedCount() << "\n";
        std::cout << "Is source some? -> " << move.isSome() << "\n";
        std::cout << "Is destination some -> " << copy.isSome() << "\n";
        std::cout << "Copy's value: " << copy.getValue() << "\n";
        move = std::move(copy);
        std::cout << "After move assignment: " << Allocator::getAllocatedCount() << "\n";
        std::cout << "Is source some? -> " << copy.isSome() << "\n";
        std::cout << "Is destination some -> " << move.isSome() << "\n";
        std::cout << "Move's value: " << move.getValue() << "\n";
    });
}

void demoOptionalRef() {
    runDemo("Optional Ref", [] {
        auto nullable = Nullable();
        {
            Ptr ptr = 0;
            BasicArray<int>::__new__V__s(nullptr, &ptr);
            auto ref = protect(ptr);
            for (auto i = 0; i < 10; i++) {
                BasicArray<int>::addF__s_t1__V(nullptr, ptr, i);
            }
            std::cout << "Num allocated at start: " << Allocator::getAllocatedCount() << "\n";
            nullable = Nullable(ref);
        }
        std::cout << "After nullable: " << Allocator::getAllocatedCount() << "\n";
        std::cout << "Is nullable some? -> " << nullable.isSome() << "\n";
    });
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
    runDemo("Nested Execution Context", [context, &frame] {
        frame.setLineAfter(__LINE__);
        demoNested2(context);
    });
}

template<typename TIn, typename TOut>
void testMacro(YET_ARG_TYPE(TIn) in, YET_RES_TYPE(TOut)* out) {
    // This function is not intended to do anything useful.
    // It's only purpose is to enable VS IntelliSense
    // in order to check that right function
    // specializations are used
}

template<typename E>
void testMacro(YET_ARG_TYPE(Optional<E>) optional) {
    if (!optional) {
        std::cout << "Optional is null\n";
    } else {
        std::cout << "Optional is not null\n";
    }
}

void demoInvocationUtil() {
    runDemo("Invocation Utilities", []() {
        int intArg = 137;
        bool boolResult;
        testMacro<int, bool>(pass(intArg), &boolResult);
        YET_RES_TYPE(Ref) refResult;
        testMacro<int, Ref>(pass(intArg), &refResult);
        Optional<int> optionalArg;
        YET_RES_TYPE(Optional<bool>) optionalResult;
        testMacro<Optional<int>, Optional<bool>>(pass(optionalArg), &optionalResult);
        testMacro<int>(pass(optionalArg));
        optionalArg = Optional<int>(intArg);
        testMacro<int>(pass(optionalArg));
    });
}

int main() {
    ExecutionContext* context = nullptr;
    StackFrame frame(context, __FUNCTION__);
    std::cout << std::boolalpha;
    demoModulePath();
    demoBasicArray();
    demoBasicArrayVirtualDispatch();
    demoVector();
    demoBasicArrayNested();
    demoOptionalString();
    demoOptionalRef();
    frame.setLineAfter(__LINE__);
    demoNested1(context);
    demoInvocationUtil();
    printStackTrace(context);
    std::cout << "Any name: " << typeOf<Any>()->name << std::endl;
    std::cout << "Array name: " << BasicArray<int>::__typeHolder.type.name << std::endl;
    std::cout << "Num allocated on exit: " << Allocator::getAllocatedCount() << std::endl;
}
