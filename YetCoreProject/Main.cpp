#include <iostream>

#include "Module.h"

int main() {
    auto result = yet_Module_path__get__V__PC8();
    if (result.error) {
        std::cerr << "Error ptr: " << result.error << std::endl;
    } else {
        std::cout << "Module path: '" << result.value << "'\n";
    }
}
