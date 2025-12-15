#include <arrow/api.h>
#include <iostream>

int main() {
    // 1. Initialize the Builder
    // We use a specific builder for 64-bit integers.
    arrow::Int64Builder builder;

    // 2. Pour data into the builder
    // Standard arrow functions return a "Status" to check for errors, 
    // but we will ignore that for this simple demo (using .ok() checks implicitly).
    builder.Append(1);
    builder.Append(2);
    builder.AppendNull(); // Explicitly adding a "null" value
    builder.Append(4);

    // 3. Finish the Array
    // This locks the memory and creates the immutable object.
    std::shared_ptr<arrow::Array> array;
    auto status = builder.Finish(&array);

    if (!status.ok()) {
        std::cerr << "Builder failed: " << status.ToString() << std::endl;
        return 1;
    }

    // 4. Verify the result
    // Arrow has a built-in ToString() method for debugging.
    std::cout << "Array created successfully!" << std::endl;
    std::cout << array->ToString() << std::endl;

    return 0;
}