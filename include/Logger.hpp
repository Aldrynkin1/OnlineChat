#pragma once

#include <iostream>
#include <string>

#include "Types.hpp"

class Logger
{
public:
    Logger() = default;
    ~Logger() = default;

    void log(LogLevel::Level level, const std::string &text);
};

inline void Logger::log(LogLevel::Level level, const std::string &text)
{
    if (level == LogLevel::Level::INFO)
    {
        std::cout << "[INFO]: ";
    }
    else if (level == LogLevel::Level::WARNING)
    {
        std::cout << "[WARNING]: ";
    }
    else if (level == LogLevel::Level::ERROR)
    {
        std::cout << "[ERROR]: ";
    }
    else if (level == LogLevel::Level::FATAL)
    {
        std::cout << "[FATAL]: ";
    }

    std::cout << text.c_str() << std::endl;
}