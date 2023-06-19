// main.cpp
#include "MyClass.h"
#include <vector>
extern "C" void function();

void function(void) {
    // Create an instance of MyClass
    std::vector<int> numbers;
    MyClass obj(42);

    numbers.push_back(obj.getNumber());
    numbers.push_back(obj.getNumber());
    numbers.push_back(obj.getNumber());

    int number = 0;
    for (const auto& element : numbers) {
        number += element;
    }
}
