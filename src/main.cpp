#include <iostream>
#include <vector>
#include <arrow/api.h>

int main() {
    // 1. GENERATE DATA (10 Days of Prices)
    arrow::DoubleBuilder price_builder;
    std::vector<double> raw_prices = {100.0, 102.0, 104.0, 103.0, 101.0, 105.0, 110.0, 108.0, 107.0, 110.0};
    
    for (double price : raw_prices) {
        if (!price_builder.Append(price).ok()) return 1;
    }

    std::shared_ptr<arrow::Array> price_array;
    if (!price_builder.Finish(&price_array).ok()) return 1; // Fixed the warning!

    std::cout << "--- Daily Prices ---" << std::endl;
    std::cout << price_array->ToString() << std::endl;
    std::cout << "--------------------" << std::endl;

    // 2. CALCULATE 3-DAY MOVING AVERAGE
    int window_size = 3;
    std::cout << "\n--- 3-Day Moving Averages ---" << std::endl;

    // We loop until we hit the end of the last window
    for (int64_t i = 0; i <= price_array->length() - window_size; ++i) {
        
        // A. CREATE A SLICE (Zero-Copy!)
        // "Give me a view starting at 'i' with length '3'"
        std::shared_ptr<arrow::Array> window_slice = price_array->Slice(i, window_size);

        // B. COMPUTE AVERAGE OF THE SLICE
        double sum = 0.0;
        
        // Cast the generic slice back to DoubleArray to read values
        auto double_slice = std::static_pointer_cast<arrow::DoubleArray>(window_slice);
        
        for (int64_t j = 0; j < double_slice->length(); ++j) {
            sum += double_slice->Value(j);
        }
        
        double avg = sum / window_size;

        // C. PRINT
        // We print 'Day i+3' because the average represents the end of the period
        std::cout << "Day " << (i + window_size) << ": " << avg << std::endl;
    }

    return 0;
}