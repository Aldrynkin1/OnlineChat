#include "../include/NetworkServer.hpp"

void start_app_speed_test()
{
    NetworkServer server;
    Logger logger;
    auto time_start = std::chrono::high_resolution_clock::now();
    server.start(8080);
    server.stop(-1);
    auto time_end = std::chrono::high_resolution_clock::now();

    const auto time = time_end - time_start;

    logger.log(
        LogLevel::Level::INFO,
        "server was start and end in " +
            std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(time).count()) +
            " microseconds");
}
