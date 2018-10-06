cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
    project(bulk VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(bulk VERSION 0.0.1)
endif()

set(CMAKE_CXX_STANDARD 17)
include_directories("${PROJECT_SOURCE_DIR}/include")

add_library(logger src/logger.cpp include/logger.h)
add_library(command src/command.cpp include/command.h)
target_link_libraries(command logger)
add_executable(bulk src/bulk.cpp)
target_link_libraries(bulk logger command)


find_package(GTest REQUIRED)
add_executable(test_${PROJECT_NAME} test/test_bulk.cpp)
target_link_libraries(test_${PROJECT_NAME} logger command ${GTEST_LIBRARIES})
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(test_${PROJECT_NAME} pthread)
endif()


install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT artbataev@gmail.com)

include(CPack)

enable_testing()
add_test(test test_${PROJECT_NAME})
