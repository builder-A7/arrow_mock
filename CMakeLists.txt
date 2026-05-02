cmake_minimum_required(VERSION 3.24)
project(ArrowFlightDemo)

set(CMAKE_CXX_STANDARD 17)

# 1. Find the Main Arrow Library
find_package(Arrow REQUIRED)

# 2. Find the Flight Library explicitly (Safe for Homebrew)
find_package(ArrowFlight REQUIRED)

# 3. The Server
add_executable(flight_server src/server.cpp)
# Note the target name change: ArrowFlight::arrow_flight_shared
target_link_libraries(flight_server PRIVATE ArrowFlight::arrow_flight_shared)

# 4. The Client
add_executable(flight_client src/client.cpp)
target_link_libraries(flight_client PRIVATE ArrowFlight::arrow_flight_shared)