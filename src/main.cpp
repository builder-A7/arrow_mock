#include <arrow/api.h>
#include <iostream>

int main() {
    std::cout << "Running with Apache Arrow version: " 
              << ARROW_VERSION_STRING << std::endl;
    return 0;
}