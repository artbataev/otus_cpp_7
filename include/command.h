#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iostream>

#include "logger.h"

class CommandAccumulator {
public:
    void add_command(const std::string& command);
    void clear();
    void log_commands();
    size_t size();
    bool empty();

private:
    std::vector<std::string> commands;
    long long int bulk_first_command_time = 0;

    static std::string get_log_file_name(long long int first_command_time) {
        std::stringstream ss;
        ss << "bulk" << first_command_time << ".log";
        return ss.str();
    }
};


class CommandProcessor {
public:
    explicit CommandProcessor(int num_commands_in_bulk_);

    void process_commands(std::istream& source_stream);

private:
    CommandAccumulator accumulator;
    int num_brackets = 0;
    int num_commands_in_bulk = 0;
};
