#include <string>
#include <vector>
#include <optional>
#include <mutex>
#include <future>
#include <thread>
#include <queue>

#include "Types.hpp"

class MessageQueue
{
private:
    std::queue<ChatMessage> queue_;
    std::mutex mutex_;
public:
    inline void push(int sender_fd, const std::string &message) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push({sender_fd, message});
    }

    inline std::optional<ChatMessage> pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) return std::nullopt;

        ChatMessage front = queue_.front();
        queue_.pop();
        return front;
    }
};
