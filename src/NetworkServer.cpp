#include "../include/NetworkServer.hpp"

void NetworkServer::start(int port)
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        std::cerr << "in func start cannot start" << std::endl;
        return;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    std::memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "in func start bind failed" << std::endl;
        return;
    }

    if (listen(server_fd, SOMAXCONN) < 0)
    {
        std::cerr << "in func start listen failed";
        return;
    }

    is_running = true;
}

int NetworkServer::accept_client()
{
    int new_client_fd = accept(server_fd, nullptr, nullptr);
    if (new_client_fd < 0)
    {
        std::cerr << "in func accept client client can't connected" << std::endl;
        return -1;
    }
    std::lock_guard<std::mutex> lock(client_mutex);
    client new_client;
    new_client.id = static_cast<int>(clients_fd.size() + 1);
    new_client.client_fd = new_client_fd;
    clients_fd.push_back(std::move(new_client));
    clients_fd.back().read_thread = std::thread(&NetworkServer::read_loop, this, new_client_fd);
    return new_client_fd;
}

void NetworkServer::read_loop(int client_fd)
{
    char buffer[1024];

    while (is_running)
    {
        std::memset(buffer, 0, sizeof(buffer));
        int bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0)
            break;
        
        ChatMessage msg;
        msg.sender_fd = client_fd;
        msg.text = ": " + std::string(buffer, bytes) + "\n";
        msg_queue.push(msg.sender_fd, msg.text);
    }

    close(client_fd);
}

void NetworkServer::send_message(int client_fd_id, const std::string &message)
{
    if (client_fd_id < 0)
        return;

    std::string chat_msg = std::to_string(client_fd_id) + message;
    send(client_fd_id, chat_msg.c_str(), message.size(), 0);
}

void NetworkServer::stop(int client_fd)
{
    for (auto it = clients_fd.begin(); it != clients_fd.end(); ++it)
    {
        if (it->client_fd == client_fd)
        {
            if (client_fd >= 0)
            {
                close(client_fd);
            }

            if (it->read_thread.joinable())
            {
                it->read_thread.join();
            }

            clients_fd.erase(it);
            break;
        }
    }

    if (clients_fd.empty())
    {
        is_running = false;
        if (server_fd >= 0)
        {
            close(server_fd);
            server_fd = -1;
        }
    }
}

void NetworkServer::execute(int _port)
{
    this->start(_port);

    std::thread accept_thread([this]() {
        while (is_running) {
            accept_client();
        } 
    });
    accept_thread.detach();

    while (true)
    {
        while (auto msg = msg_queue.pop()) 
        {
            int sender_fd = msg->sender_fd;
            std::string text = msg->text;

            std::string broadcast_msg = "\r" + std::to_string(sender_fd) + text;

            std::lock_guard<std::mutex> lock(client_mutex);
            for (const auto &client : clients_fd)
            {
                if (client.client_fd != sender_fd)
                {
                    send(client.client_fd, broadcast_msg.c_str(), broadcast_msg.size(), 0);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
