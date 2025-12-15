#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#include <iostream>

int main() {
    std::cout << "Attempting to read 'output.parquet'..." << std::endl;

    // 1. Open the file for reading
    // We use ReadableFile instead of FileOutputStream
    auto infile_result = arrow::io::ReadableFile::Open("output.parquet");
    if (!infile_result.ok()) {
        std::cerr << "Error opening file: " << infile_result.status().ToString() << std::endl;
        return 1;
    }
    std::shared_ptr<arrow::io::ReadableFile> infile = *infile_result;

    // 2. Create the Parquet Reader
    // We explicitly cast our specific file to the generic "RandomAccessFile" type
    std::shared_ptr<arrow::io::RandomAccessFile> input_file = infile;
    std::unique_ptr<parquet::arrow::FileReader> reader;
    
    // Use the Builder helper to set up the reader
    parquet::arrow::FileReaderBuilder builder;
    auto open_status = builder.Open(input_file);
    if (!open_status.ok()) {
        std::cerr << "Error opening file with builder: " << open_status.ToString() << std::endl;
        return 1;
    }

    // Build the actual reader
    auto build_status = builder.Build(&reader);
    if (!build_status.ok()) {
        std::cerr << "Error creating reader: " << build_status.ToString() << std::endl;
        return 1;
    }

    // 3. Read the entire file into a Table
    std::shared_ptr<arrow::Table> table;
    auto read_status = reader->ReadTable(&table);
    
    if (!read_status.ok()) {
        std::cerr << "Error reading table: " << read_status.ToString() << std::endl;
        return 1;
    }

    // 4. Verify the data
    std::cout << "Read successful!" << std::endl;
    std::cout << "Rows: " << table->num_rows() << std::endl;
    std::cout << "Columns: " << table->num_columns() << std::endl;
    std::cout << "---------------------------------" << std::endl;
    std::cout << table->ToString() << std::endl;

    return 0;
}