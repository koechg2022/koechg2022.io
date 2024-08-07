

cmake_minimum_required(VERSION 3.3)
project(Personal_Website)





#  Set the standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# add the executables (the object files that are executed)
add_executable(test_server ${PROJECT_SOURCE_DIR}/Server/objects OUTPUT_NAME "test_server")
add_executable(test_server_gui ${PROJECT_SOURCE_DIR}/Server/objects OUTPUT_NAME "test_server_gui")
add_executable(server_with_gui ${PROJECT_SOURCE_DIR}/Server/objects OUTPUT_NAME "server_with_gui")


