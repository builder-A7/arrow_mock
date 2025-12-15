#include <iostream>
#include <arrow/api.h>
#include <arrow/csv/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/writer.h>

int main() {
    std::string csv_filename = "../input.csv";
    
    std::cout << "Attempting to read CSV: " << csv_filename << std::endl;

    // 1. Open File
    auto input_file_result = arrow::io::ReadableFile::Open(csv_filename);
    if (!input_file_result.ok()) {
        std::cerr << "Error opening file: " << input_file_result.status().ToString() << std::endl;
        return 1;
    }
    std::shared_ptr<arrow::io::ReadableFile> input_file = *input_file_result;

    // 2. Configure CSV Reader
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
    auto reader = *reader_result;

    // 4. READ THE TABLE (The moment of truth)
    std::cout << "Reading table..." << std::endl;
    auto table_result = reader->Read();
    if (!table_result.ok()) {
        std::cerr << "Error reading table: " << table_result.status().ToString() << std::endl;
        return 1;
    }
    std::shared_ptr<arrow::Table> table = *table_result;

    // 5. Verify the Data
    std::cout << "Read Successful!" << std::endl;
    std::cout << "Rows: " << table->num_rows() << std::endl;
    std::cout << "Columns: " << table->num_columns() << std::endl;
    std::cout << "---------------------------------" << std::endl;
    
    // Print the Schema (Did it guess 'int64' for score?)
    std::cout << table->schema()->ToString() << std::endl;
    std::cout << "---------------------------------" << std::endl;

    // 6. Write to Parquet
    std::cout << "Writing to output.parquet..." << std::endl;
    std::shared_ptr<arrow::io::FileOutputStream> outfile;
    auto outfile_result = arrow::io::FileOutputStream::Open("output.parquet");
    
    if (!outfile_result.ok()) {
        std::cerr << "Error creating output file: " << outfile_result.status().ToString() << std::endl;
        return 1;
    }
    outfile = *outfile_result;

    // Write the table
    // We use a small chunk_size (e.g., 10) for this tiny example.
    auto write_status = parquet::arrow::WriteTable(
        *table, 
        arrow::default_memory_pool(), 
        outfile, 
        10
    );

    if (!write_status.ok()) {
        std::cerr << "Error writing parquet: " << write_status.ToString() << std::endl;
        return 1;
    }

    std::cout << "✅ Conversion Complete: input.csv -> output.parquet" << std::endl;

    return 0;
}