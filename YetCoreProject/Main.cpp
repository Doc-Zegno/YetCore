#include <iostream>
#include <type_traits>

#include "Module.h"
#include "BasicArray.h"

void demoModulePath() {
    auto result = yet_Module_path__get__V__PC8();
    if (result.error) {
        std::cerr << "Error ptr: " << result.error << std::endl;
    } else {
        std::cout << "Module path: '" << result.value << "'\n";
    }
}

void demoBasicArray() {
    auto basicArray = BasicArray<int>{};
    auto ptr = Ptr(&basicArray);
    BasicArray<int>::add__s_t1__V(ptr, 42);
    BasicArray<int>::add__s_t1__V(ptr, 137);
    std::cout << "Element #0: " << BasicArray<int>::get__operator__s_I__t1(ptr, 0).value << std::endl;
    std::cout << "Element #1: " << BasicArray<int>::get__operator__s_I__t1(ptr, 1).value << std::endl;
    std::cout << "Element #2: error? -> " << BasicArray<int>::get__operator__s_I__t1(ptr, 2).error << std::endl;
}

int main() {
    demoModulePath();
    demoBasicArray();
}
