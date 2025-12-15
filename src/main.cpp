#include <iostream>
#include <vector>
#include <random>
#include <chrono> // Added for timing
#include <arrow/api.h>

int main() {
    // ==========================================
    // 1. GENERATE DATA
    // ==========================================
    int64_t num_rows = 1000000; 
    std::cout << "1. Generating " << num_rows << " log entries..." << std::endl;

    arrow::Int64Builder timestamp_builder;
    arrow::StringBuilder level_builder;
    arrow::StringBuilder message_builder;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9); 

    for (int64_t i = 0; i < num_rows; ++i) {
        if (!timestamp_builder.Append(i).ok()) return 1;
        int roll = dis(gen);
        if (roll < 7) {
            if (!level_builder.Append("INFO").ok()) return 1;
            if (!message_builder.Append("User login successful").ok()) return 1;
        } else if (roll < 9) {
            if (!level_builder.Append("WARN").ok()) return 1;
            if (!message_builder.Append("High memory usage detected").ok()) return 1;
        } else {
            if (!level_builder.Append("ERROR").ok()) return 1; 
            if (!message_builder.Append("Database connection failed").ok()) return 1;
        }
    }

    std::shared_ptr<arrow::Array> timestamp_array;
    std::shared_ptr<arrow::Array> level_array;
    std::shared_ptr<arrow::Array> message_array;

    timestamp_builder.Finish(&timestamp_array);
    level_builder.Finish(&level_array);
    message_builder.Finish(&message_array);

    auto schema = arrow::schema({
        arrow::field("timestamp", arrow::int64()),
        arrow::field("level", arrow::utf8()),
        arrow::field("message", arrow::utf8())
    });

    auto table = arrow::Table::Make(schema, {timestamp_array, level_array, message_array});
    std::cout << "   Table created in memory." << std::endl;

    // ==========================================
    // 2. ANALYZE (Find Errors)
    // ==========================================
    std::cout << "2. Scanning for ERRORS..." << std::endl;

    // Start Timer
    auto start_time = std::chrono::high_resolution_clock::now();

    int64_t error_count = 0;
    auto level_column = table->column(1); // Column 1 is "level"

    // Iterate over chunks
    for (const auto& chunk : level_column->chunks()) {
        auto string_chunk = std::static_pointer_cast<arrow::StringArray>(chunk);
        
        // Iterate over rows in this chunk
        for (int64_t i = 0; i < string_chunk->length(); ++i) {
            // We use GetView(i) because it's slightly faster than GetString(i)
            // It returns a lightweight string_view instead of a full std::string copy.
            if (string_chunk->GetView(i) == "ERROR") {
                error_count++;
            }
        }
    }

    // Stop Timer
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end_time - start_time;

    std::cout << "   Analysis Complete!" << std::endl;
    std::cout << "   Found " << error_count << " ERRORs." << std::endl;
    std::cout << "   Time taken: " << diff.count() << " seconds." << std::endl;

    return 0;
}