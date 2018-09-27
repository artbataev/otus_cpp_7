#include "gtest/gtest.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "command.h"
#include "logger.h"

struct InOutFiles {
    std::string in;
    std::string out;
};

class AssignmentTestFromFile : public testing::TestWithParam<std::pair<int, InOutFiles>> {};

TEST_P(AssignmentTestFromFile, AssignmentExample)
{
    auto num_commands_in_bulk = GetParam().first;
    auto in_file = GetParam().second.in;
    auto out_file = GetParam().second.out;
    std::fstream test_input(in_file);
    std::fstream test_output_etalon_file(out_file);

    std::stringstream test_output_etalon_stream;
    test_output_etalon_stream << test_output_etalon_file.rdbuf();

    Logger::get_logger().add_stream("stdout", std::cout);

    CommandProcessor processor(num_commands_in_bulk);
    testing::internal::CaptureStdout();
    processor.process_commands(test_input);
    std::string output = testing::internal::GetCapturedStdout();

    ASSERT_EQ(output, test_output_etalon_stream.str());
}

INSTANTIATE_TEST_CASE_P(MyGroup, AssignmentTestFromFile, ::testing::Values(
        std::make_pair(3, InOutFiles{"../data/test1.in.txt", "../data/test1.out.txt"}),
        std::make_pair(3, InOutFiles{"../data/test2.in.txt", "../data/test2.out.txt"}),
        std::make_pair(3, InOutFiles{"../data/test3.in.txt", "../data/test3.out.txt"}),
        std::make_pair(3, InOutFiles{"../data/test4.in.txt", "../data/test4.out.txt"}),
        std::make_pair(2, InOutFiles{"../data/test5.in.txt", "../data/test5.out.txt"}) // output all before first bracket
));

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
