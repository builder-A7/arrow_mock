#include <iostream>
#include <arrow/api.h>
#include <arrow/flight/api.h>
#include <arrow/flight/server.h>

class StockFlightServer : public arrow::flight::FlightServerBase {
public:
    arrow::Status DoGet(const arrow::flight::ServerCallContext& context,
                        const arrow::flight::Ticket& ticket,
                        std::unique_ptr<arrow::flight::FlightDataStream>* data_stream) override {
        
        std::cout << "Server: Received request for ticket: " << ticket.ticket << std::endl;

        // 1. GENERATE DATA
        arrow::DoubleBuilder price_builder;
        std::vector<double> prices = {100.0, 102.0, 104.0, 103.0, 101.0};
        // Use Check() or ignore result properly
        for (double p : prices) {
            (void)price_builder.Append(p); 
        }
        
        std::shared_ptr<arrow::Array> array;
        (void)price_builder.Finish(&array);

        auto schema = arrow::schema({arrow::field("prices", arrow::float64())});
        auto table = arrow::Table::Make(schema, {array});

        // 2. SEND IT BACK
        auto reader = std::make_shared<arrow::TableBatchReader>(*table);
        *data_stream = std::make_unique<arrow::flight::RecordBatchStream>(reader);

        return arrow::Status::OK();
    }
};

int main() {
    auto server = std::make_unique<StockFlightServer>();

    // FIX: Use Result<Location> instead of passing pointer
    auto location_result = arrow::flight::Location::ForGrpcTcp("0.0.0.0", 3000);
    if (!location_result.ok()) {
        std::cerr << "Failed to create location: " << location_result.status().ToString() << std::endl;
        return 1;
    }
    arrow::flight::Location location = *location_result;
    
    arrow::flight::FlightServerOptions options(location);

    std::cout << "Server listening on port 3000..." << std::endl;
    (void)server->Init(options);
    (void)server->Serve();
    return 0;
}