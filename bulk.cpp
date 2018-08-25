#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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
        std::cout << value;
        return Logger::get_logger();
    }

    void open_file(std::string file_name) {
//        std::ofstream file(file_name);
    }

private:
    Logger() = default;

    ~Logger() = default;

//    std::ofstream current_file;
};

void print_commands(const std::vector<std::string>& commands) {
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
    while (std::cin >> current_command) {
        if (current_command == "{") {
            num_brackets++;
        } else if (current_command == "}") {
            if (num_brackets > 0) {
                num_brackets--;
                if (num_brackets == 0) {
                    print_commands(current_bulk);
                    current_bulk.clear();
                }
            } else {
                Logger::get_logger() << "Incorrect bracket" << "\n";
            }
        } else { // normal command
            if (current_bulk.empty()) {

            }
            current_bulk.emplace_back(current_command);
            if (num_brackets == 0 && current_bulk.size() == num_commands_in_bulk) {
                print_commands(current_bulk);
                current_bulk.clear();
            }
        }
    }

    return 0;
}
