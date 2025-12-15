#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>
#include <iostream>

int main() {
    // ==========================================
    // 1. CREATE AND WRITE
    // ==========================================
    std::cout << "1. Creating data..." << std::endl;
    arrow::Int64Builder builder;
    builder.Append(1);
    builder.Append(2);
    builder.AppendNull();
    builder.Append(4);
    
    std::shared_ptr<arrow::Array> array;
    builder.Finish(&array);

    auto schema = arrow::schema({arrow::field("numbers", arrow::int64())});
    auto table = arrow::Table::Make(schema, {array});

    std::shared_ptr<arrow::io::FileOutputStream> outfile;
    auto outfile_result = arrow::io::FileOutputStream::Open("output.parquet");
    if (outfile_result.ok()) {
        outfile = *outfile_result;
        parquet::arrow::WriteTable(*table, arrow::default_memory_pool(), outfile, 10);
        std::cout << "   File written." << std::endl;
    }

    // ==========================================
    // 2. READ
    // ==========================================
    std::cout << "2. Reading data..." << std::endl;
    std::shared_ptr<arrow::io::ReadableFile> infile;
    auto infile_result = arrow::io::ReadableFile::Open("output.parquet");
    if (!infile_result.ok()) {
        std::cerr << "   Error reading file." << std::endl;
        return 1;
    }
    infile = *infile_result;

    std::unique_ptr<parquet::arrow::FileReader> reader;
    parquet::arrow::FileReaderBuilder reader_builder;
    reader_builder.Open(infile);
    reader_builder.Build(&reader);

    std::shared_ptr<arrow::Table> read_table;
    reader->ReadTable(&read_table);
    
    // ==========================================
    // 3. MANUAL COMPUTE
    // ==========================================
    std::cout << "3. Computing Sum (Manually)..." << std::endl;
    
    auto column = read_table->column(0)->Slice(2,2);
    int64_t sum = 0;
    int64_t count = 0;

    // A column in a Table is a "ChunkedArray" (it might be split into pieces).
    // We iterate over each "chunk" (which is a standard Array).
    for (const auto& chunk : column->chunks()) {
        
        // We must cast the generic Array to the specific type we expect (Int64Array)
        // This gives us access to Value() and IsValid()
        auto int64_chunk = std::static_pointer_cast<arrow::Int64Array>(chunk);
        
        for (int64_t i = 0; i < int64_chunk->length(); ++i) {
            // CRITICAL: Always check if the value is valid (not null) before using it!
            if (int64_chunk->IsValid(i)) {
                sum += int64_chunk->Value(i);
                count++;
            }
        }
    }

    std::cout << "   Success! Sum: " << sum << std::endl;
    std::cout << "   (Calculated from " << count << " non-null values)" << std::endl;

    return 0;
}