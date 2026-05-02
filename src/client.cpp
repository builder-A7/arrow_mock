#include <iostream>
#include <arrow/api.h>
#include <arrow/flight/api.h>

int main() {
    // 1. DEFINE LOCATION
    auto location_result = arrow::flight::Location::ForGrpcTcp("localhost", 3000);
    if (!location_result.ok()) return 1;
    arrow::flight::Location location = *location_result;

    // 2. CONNECT TO SERVER (New API returns Result<unique_ptr>)
    auto client_result = arrow::flight::FlightClient::Connect(location, arrow::flight::FlightClientOptions());
    if (!client_result.ok()) {
        std::cerr << "Could not connect: " << client_result.status().ToString() << std::endl;
        return 1;
    }
    std::unique_ptr<arrow::flight::FlightClient> client = std::move(*client_result);
    
    std::cout << "Client: Connected to " << location.ToString() << std::endl;

    // 3. REQUEST DATA
    arrow::flight::Ticket ticket{"stock-data"};
    
    // DoGet now returns Result<unique_ptr<StreamReader>>
    auto stream_result = client->DoGet(ticket);
    if (!stream_result.ok()) {
        std::cerr << "DoGet failed: " << stream_result.status().ToString() << std::endl;
        return 1;
    }
    std::unique_ptr<arrow::flight::FlightStreamReader> stream = std::move(*stream_result);

    // 4. READ THE STREAM
    // Use ToTable() instead of ReadAll()
    auto table_result = stream->ToTable();
    if (!table_result.ok()) {
        std::cerr << "Read failed: " << table_result.status().ToString() << std::endl;
        return 1;
    }
    std::shared_ptr<arrow::Table> table = *table_result;

    // 5. VERIFY
    std::cout << "Client: Downloaded Table!" << std::endl;
    std::cout << "Rows: " << table->num_rows() << std::endl;
    std::cout << "Data: " << table->ToString() << std::endl;

    return 0;
}