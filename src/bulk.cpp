#include <iostream>
#include <fstream>
#include <sstream>

#include "logger.h"
#include "command.h"



int main(int argc, char *argv[]) {
    bool executed_correctly = true;
    std::stringstream whats_wrong;
    int num_commands_in_bulk = 0;

    if (argc != 2) {
        executed_correctly = false;
        whats_wrong << "Incorrect number of arguments";
    } else {
        try {
            num_commands_in_bulk = std::stoi(argv[1]);
            if (num_commands_in_bulk <= 0) {
                whats_wrong << "Number of commands in bulk must be positive";
                executed_correctly = false;
            }
        } catch (std::exception&) {
            executed_correctly = false;
            whats_wrong << "Number of commands in bulk must be a number";
        }
    }

    if (!executed_correctly) {
        std::cout << "Error: " << whats_wrong.str() << std::endl;
        std::cout << "Execute with one argument - number of commands in bulk, e.g.:" << std::endl;
        std::cout << argv[0] << " 2" << std::endl;
        exit(0);
    }

    auto& source_stream = std::cin;

    Logger::get_logger().add_stream("stdout", std::cout);
    CommandProcessor processor(num_commands_in_bulk);
    processor.process_commands(source_stream);

    return 0;
}
