#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/writer.h>
#include <iostream>

int main() {
    // 1. Create Data
    arrow::Int64Builder builder;
    builder.Append(1);
    builder.Append(2);
    builder.AppendNull();
    builder.Append(4);
    
    std::shared_ptr<arrow::Array> array;
    builder.Finish(&array);

    // 2. Create Schema & Table
    auto schema = arrow::schema({
        arrow::field("numbers", arrow::int64())
    });
    auto table = arrow::Table::Make(schema, {array});

    // 3. Open File (THE FIX IS HERE)
    // We capture the "Result" object first.
    auto outfile_result = arrow::io::FileOutputStream::Open("output.parquet");
    
    // Check if the file opened successfully
    if (!outfile_result.ok()) {
        std::cerr << "Error opening file: " << outfile_result.status().ToString() << std::endl;
        return 1;
    }

    // Extract the actual file pointer from the Result using the dereference operator (*)
    std::shared_ptr<arrow::io::FileOutputStream> outfile = *outfile_result;

    // 4. Write to Parquet
    // Now 'outfile' is a valid pointer, so this won't crash.
    parquet::arrow::WriteTable(*table, arrow::default_memory_pool(), outfile, 10);

    std::cout << "Parquet file 'output.parquet' written successfully!" << std::endl;

    return 0;
}