



# cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON









# cmake_minimum_required(VERSION 3.30)
# project(Personal_Website CXX)

# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

# # Add this line to recognize .h++ as a C++ header file
# set(CMAKE_CXX_SOURCE_FILE_EXTENSIONS cpp cxx cc c++ h hpp hxx h++)

# # Set the output directory for object files and executables
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../objects)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/../objects)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/../objects)

# # Create a library from the files in the libs directory
# file(GLOB LIB_SOURCES "libs/*.c++")
# add_library(project_lib STATIC ${LIB_SOURCES})
# target_include_directories(project_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs)

# # Add your source files here
# add_executable(test_server tests/test_server.c++)

# # Add compiler flags
# target_compile_options(test_server PRIVATE -Wall -Wextra)

# # If you have additional include directories, add them like this:
# target_include_directories(test_server PRIVATE headers)
# target_include_directories(test_server PRIVATE libs)

# # Link the project_lib to test_server
# target_link_libraries(test_server PRIVATE project_lib)

# # For wxWidgets and IntelliSense support
# find_package(wxWidgets COMPONENTS core base REQUIRED)
# include(${wxWidgets_USE_FILE})
# target_link_libraries(test_server PRIVATE ${wxWidgets_LIBRARIES})
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

















cmake_minimum_required(VERSION 3.30)
project(Personal_Website CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add this line to recognize .h++ as a C++ header file
set(CMAKE_CXX_SOURCE_FILE_EXTENSIONS cpp cxx cc c++ h hpp hxx h++)

# Set the OpenSSL root directory
set(OPENSSL_ROOT_DIR /usr/local/opt/openssl@3)

# Find OpenSSL package
find_package(OpenSSL REQUIRED)

# Set the output directory for object files and executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../objects)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/../objects)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/../objects)

# Create a library from the files in the libs directory
file(GLOB LIB_SOURCES "libs/*.c++")
add_library(project_lib STATIC ${LIB_SOURCES})
target_include_directories(project_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs ${OPENSSL_INCLUDE_DIR})

# Add your source files here
add_executable(test_server tests/test_server.c++)

# Add compiler flags
target_compile_options(test_server PRIVATE -Wall -Wextra)

# If you have additional include directories, add them like this:
target_include_directories(test_server PRIVATE headers libs ${OPENSSL_INCLUDE_DIR})

# Link the project_lib and OpenSSL to test_server
target_link_libraries(test_server PRIVATE project_lib ${OPENSSL_LIBRARIES})

# For wxWidgets and IntelliSense support
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})
target_link_libraries(test_server PRIVATE ${wxWidgets_LIBRARIES})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)