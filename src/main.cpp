#include <iostream>
#include <arrow/api.h>
#include <arrow/csv/api.h>  // <--- The new header!
#include <arrow/io/api.h>
#include <parquet/arrow/writer.h>

int main() {
    std::string csv_filename = "../input.csv"; // Assuming running from build/
    
    std::cout << "Attempting to read CSV: " << csv_filename << std::endl;

    // 1. Open File
    auto input_file_result = arrow::io::ReadableFile::Open(csv_filename);
    if (!input_file_result.ok()) {
        std::cerr << "Error opening file: " << input_file_result.status().ToString() << std::endl;
        return 1;
    }
    std::shared_ptr<arrow::io::ReadableFile> input_file = *input_file_result;

    // 2. Configure CSV Reader (Auto-detect types)
    arrow::io::IOContext io_context = arrow::io::default_io_context();
    auto read_options = arrow::csv::ReadOptions::Defaults();
    auto parse_options = arrow::csv::ParseOptions::Defaults();
    auto convert_options = arrow::csv::ConvertOptions::Defaults();

    // 3. Create Reader
    auto reader_result = arrow::csv::TableReader::Make(
        io_context,
        input_file,
        read_options,
        parse_options,
        convert_options
    );
    
    if (!reader_result.ok()) {
        std::cerr << "Error creating reader: " << reader_result.status().ToString() << std::endl;
        return 1;
    }
    
    std::cout << "CSV Reader created successfully." << std::endl;

    return 0;
}