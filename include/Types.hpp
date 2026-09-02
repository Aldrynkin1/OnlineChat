#pragma once

#include <string>
#include <vector>
#include <thread>

struct client
{
    int id;
    int client_fd;
    std::optional<std::string> name;

    std::thread read_thread;
};

struct Message
{
    int sender_fd;
    std::string text;
};

struct ChatMessage {
    int sender_fd;
    std::string text;
};