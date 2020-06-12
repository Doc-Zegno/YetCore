#include <iostream>
#include <type_traits>

#include "Any.h"
#include "PtrX.h"
#include "Module.h"
#include "Allocator.h"
#include "BasicArray.h"

void* operator new(size_t size) {
    return yet_allocate__U_B_B__PV((uintptr_t)size, false, false);
}

void operator delete(void* object) {
    yet_deallocate__PV__B(object);
}

void demoModulePath() {
    auto result = yet_Module_path__get__V__PC8();
    if (result.error) {
        std::cerr << "Error ptr: " << result.error << std::endl;
    } else {
        std::cout << "Module path: '" << result.value << "'\n";
    }
}

void demoBasicArray() {
    auto result = BasicArray<int>::__new__V__s();
    auto ptr = result.value;
    std::cout << "Num allocated: " << yet_allocatedCount__get__V__I() << std::endl;
    BasicArray<int>::add__s_t1__V(ptr, 42);
    BasicArray<int>::add__s_t1__V(ptr, 137);
    std::cout << "Num allocated: " << yet_allocatedCount__get__V__I() << std::endl;
    std::cout << "Element #0: " << BasicArray<int>::get__operator__s_I__t1(ptr, 0).value << std::endl;
    std::cout << "Element #1: " << BasicArray<int>::get__operator__s_I__t1(ptr, 1).value << std::endl;
    std::cout << "Element #2: error? -> " << BasicArray<int>::get__operator__s_I__t1(ptr, 2).error << std::endl;

    auto table = findTableOf<Any>(ptr);
    if (table->type == &yet_Any__type) {
        std::cout << "Found Any table at address: " << table->type << std::endl;
    }
    release(ptr);
}

int main() {
    demoModulePath();
    demoBasicArray();
    std::cout << "Any name: " << typeOf<Any>()->name << std::endl;
    std::cout << "Array name: " << BasicArray<int>::__typeHolder.type.name << std::endl;
    std::cout << "Num allocated on exit: " << yet_allocatedCount__get__V__I() << std::endl;
}
