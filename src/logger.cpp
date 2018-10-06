#include "logger.h"
#include <string>

void Logger::remove_stream(const std::string& title) {
    where_to_log.erase(title);
}

void Logger::add_stream(const std::string& title, std::ostream& stream) {
    where_to_log[title] = &stream;
}
