#include <chrono>
#include <thread>
#include "include/NetworkServer.hpp"

int main() {
    NetworkServer server;
    server.execute(8080);

    return 0;
}
