



# cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON




cmake_minimum_required(VERSION 3.30)
project(Personal_Website CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add this line to recognize .h++ as a C++ header file
set(CMAKE_CXX_SOURCE_FILE_EXTENSIONS cpp cxx cc c++ h hpp hxx h++)

# Set the output directory for object files and executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../objects)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/../objects)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/../objects)

# Add your source files here
add_executable(test_server tests/test_server.c++)

# Add compiler flags
target_compile_options(test_server PRIVATE -Wall -Wextra)

# If you have additional include directories, add them like this:
target_include_directories(test_server PRIVATE headers)
target_include_directories(test_server PRIVATE libs)

# For wxWidgets and IntelliSense support
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})
target_link_libraries(test_server ${wxWidgets_LIBRARIES})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
