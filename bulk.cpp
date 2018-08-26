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

void print_commands(const std::vector<std::string>& commands) {
    Logger::get_logger().add_stream("stdout", std::cout);
    for (size_t i = 0; i < commands.size(); i++) {
        if (i > 0) {
            Logger::get_logger() << " ";
        }
        Logger::get_logger() << commands[i];
    }
    Logger::get_logger() << "\n";
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

    while (source_stream >> current_command) {
        if (current_command == "{") {
            num_brackets++;
        } else if (current_command == "}") {
            if (num_brackets <= 0) throw std::logic_error("Incorrect bracket");

            num_brackets--;
            if (num_brackets == 0) {
                print_commands(current_bulk);
                current_bulk.clear();
            }
        } else { // normal command
            current_bulk.emplace_back(current_command);
            if (num_brackets == 0 && current_bulk.size() == num_commands_in_bulk) {
                print_commands(current_bulk);
                current_bulk.clear();
            }
        }
    }
    if (!current_bulk.empty() && num_brackets == 0) {
        print_commands(current_bulk);
        current_bulk.clear();
    }

    return 0;
}
