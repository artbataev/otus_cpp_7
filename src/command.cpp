#include "command.h"

void CommandAccumulator::add_command(const std::string& command) {
    if (empty()) {
        using namespace std::chrono;
        bulk_first_command_time = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
    }
    commands.emplace_back(command);
}

void CommandAccumulator::clear() {
    commands.clear();
}

void CommandAccumulator::log_commands() {
    if (empty()) return; // log nothing
    std::ofstream f(get_log_file_name(bulk_first_command_time));
    Logger::get_logger().add_stream("file", f);
    Logger::get_logger() << "bulk: ";
    for (size_t i = 0; i < commands.size(); i++) {
        if(i > 0)
            Logger::get_logger() << ", ";
        Logger::get_logger() << commands[i];
    }
    Logger::get_logger() << "\n";
    Logger::get_logger().remove_stream("file");
}

size_t CommandAccumulator::size() {
    return commands.size();
}

bool CommandAccumulator::empty() {
    return commands.empty();
}

CommandProcessor::CommandProcessor(int num_commands_in_bulk_) : num_commands_in_bulk(num_commands_in_bulk_) {}

void CommandProcessor::process_commands(std::istream& source_stream) {
    std::string current_command;

    while (source_stream >> current_command) {
        if (current_command == "{") {
            if (num_brackets == 0) {
                accumulator.log_commands();
                accumulator.clear();
            }
            num_brackets++;
        } else if (current_command == "}") {
            if (num_brackets <= 0) {
                std::cerr << "Warning: Unexpected bracket, ignoring" << std::endl;
            } else {
                num_brackets--;
                if (num_brackets == 0) {
                    accumulator.log_commands();
                    accumulator.clear();
                }
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
