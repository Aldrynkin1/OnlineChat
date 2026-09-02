#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <optional>
#include <thread>

#include "MessageQueue.hpp"
#include "Types.hpp"

class NetworkServer
{

private:
    int port;
    int server_fd = -1;
    std::vector<client> clients_fd;
    bool is_running = false;
    sockaddr_in address{};
    socklen_t address_len = sizeof(address);
    std::thread worked_thread;
    std::mutex client_mutex;

    MessageQueue msg_queue;

public:
    NetworkServer() {}
    ~NetworkServer() = default;

    // start app by port
    void start(int port);

    // accept client request to connect
    int accept_client();

    // send message
    void send_message(int client_fd_id, const std::string &message);

    // read messages
    void read_loop(int client_fd);

    // stop app
    void stop(int client_fd);

    // execute all class methods
    void execute(int _port);

    inline std::optional<ChatMessage> get_next_message()
    {
        return msg_queue.pop();
    };

    inline const std::vector<client> &get_clients() const { return clients_fd; };
};