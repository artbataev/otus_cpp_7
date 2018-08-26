#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "logger.h"

class CommandAccumulator {
public:

};

class CommandReader {
public:

};

std::string get_log_file_name(long long int first_command_time) {
    std::stringstream ss;
    ss << "bulk" << first_command_time << ".log";
    return ss.str();
}

void print_commands(const std::vector<std::string>& commands, long long int first_command_time) {
    std::ofstream f(get_log_file_name(first_command_time));
    Logger::get_logger().add_stream("file", f);
    for (size_t i = 0; i < commands.size(); i++) {
        if (i > 0) {
            Logger::get_logger() << " ";
        }
        Logger::get_logger() << commands[i];
    }
    Logger::get_logger() << "\n";
    Logger::get_logger().remove_stream("file");
}

int main(int argc, char *argv[]) {
    if (argc != 2) throw std::logic_error("Execute with one argument - number of commands in bulk");

    size_t num_commands_in_bulk;
    num_commands_in_bulk = static_cast<size_t>(std::stoi(argv[1]));
    int num_brackets = 0;
    std::string current_command;
    std::vector<std::string> current_bulk;
    current_bulk.reserve(num_commands_in_bulk);

//    std::fstream from_file("../data/test1.in.txt");
//    auto& source_stream = from_file;
    auto& source_stream = std::cin;

    Logger::get_logger().add_stream("stdout", std::cout);
    long long int command_time = 0;

    while (source_stream >> current_command) {
        if (current_command == "{") {
            num_brackets++;
        } else if (current_command == "}") {
            if (num_brackets <= 0) throw std::logic_error("Incorrect bracket");

            num_brackets--;
            if (num_brackets == 0) {
                print_commands(current_bulk, command_time);
                current_bulk.clear();
            }
        } else { // normal command
            if (current_bulk.empty()) {
                command_time = std::chrono::system_clock::now().time_since_epoch().count();
            }
            current_bulk.emplace_back(current_command);
            if (num_brackets == 0 && current_bulk.size() == num_commands_in_bulk) {
                print_commands(current_bulk, command_time);
                current_bulk.clear();
            }
        }
    }
    if (!current_bulk.empty() && num_brackets == 0) {
        print_commands(current_bulk, command_time);
        current_bulk.clear();
    }

    return 0;
}
