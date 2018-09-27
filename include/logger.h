#pragma once

#include <ostream>
#include <map>
#include <string>

class Logger {
public:
    Logger(Logger const&) = delete;

    Logger& operator=(Logger const&) = delete;

    static Logger& get_logger() {
        static Logger logger;
        return logger;
    }

    template<typename T>
    Logger& operator<<(const T& value) {
        for(auto& [_, stream]: where_to_log) {
            *stream << value;
        }
        return Logger::get_logger();
    }

    void add_stream(const std::string& title, std::ostream& stream);

    void remove_stream(const std::string& title);

private:
    Logger() = default;

    ~Logger() = default;

    std::map<std::string, std::ostream*> where_to_log;
};
