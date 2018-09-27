#include "gtest/gtest.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "command.h"
#include "logger.h"

TEST(Test1, Test1) {
    std::fstream test_input("../data/test1.in.txt");
    std::fstream test_output_etalon_file("../data/test1.out.txt");

    std::stringstream test_output_etalon_stream;
    test_output_etalon_stream << test_output_etalon_file.rdbuf();

    Logger::get_logger().add_stream("stdout", std::cout);

    CommandProcessor processor(3);
    testing::internal::CaptureStdout();
    processor.process_commands(test_input);
    std::string output = testing::internal::GetCapturedStdout();

    ASSERT_EQ(output, test_output_etalon_stream.str());
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
