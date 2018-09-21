#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

#include "logger.h"

class CommandAccumulator {
public:
    void add_command(const std::string& command) {
        if (empty()) {
            using namespace std::chrono;
            bulk_first_command_time = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
        }
        commands.emplace_back(command);
    }

    void clear() {
        commands.clear();
    }

    void log_commands() {
        if (empty()) return; // log nothing
        std::ofstream f(get_log_file_name(bulk_first_command_time));
        Logger::get_logger().add_stream("file", f);
        Logger::get_logger() << "bulk:";
        for (size_t i = 0; i < commands.size(); i++) {
            Logger::get_logger() << " " << commands[i];
        }
        Logger::get_logger() << "\n";
        Logger::get_logger().remove_stream("file");
    }

    size_t size() {
        return commands.size();
    }

    bool empty() {
        return commands.empty();
    }

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
    CommandProcessor(int num_commands_in_bulk_) : num_commands_in_bulk(num_commands_in_bulk_) {};

    void process_commands(std::istream& source_stream) {
        std::string current_command;

        while (source_stream >> current_command) {
            if (current_command == "{") {
                num_brackets++;
            } else if (current_command == "}") {
                if (num_brackets <= 0) throw std::logic_error("Incorrect bracket");
                num_brackets--;
                if (num_brackets == 0) {
                    accumulator.log_commands();
                    accumulator.clear();
                }
            } else { // normal command
                accumulator.add_command(current_command);
                if (num_brackets == 0 && accumulator.size() == num_commands_in_bulk) {
                    accumulator.log_commands();
                    accumulator.clear();
                }
            }
        }
        if (!accumulator.empty() && num_brackets == 0) {
            accumulator.log_commands();
            accumulator.clear();
        }
    }

private:
    CommandAccumulator accumulator;
    int num_brackets = 0;
    int num_commands_in_bulk = 0;
};


int main(int argc, char *argv[]) {
    if (argc != 2) throw std::logic_error("Execute with one argument - number of commands in bulk");

    int num_commands_in_bulk;
    num_commands_in_bulk = std::stoi(argv[1]);

//    std::fstream from_file("../data/test1.in.txt");
//    auto& source_stream = from_file;
    auto& source_stream = std::cin;

    Logger::get_logger().add_stream("stdout", std::cout);
    CommandProcessor processor(num_commands_in_bulk);
    processor.process_commands(source_stream);

    return 0;
}
