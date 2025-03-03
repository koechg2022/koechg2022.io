

# cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON






# cmake_minimum_required(VERSION 3.30)
# project(Personal_Website_server CXX)

# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)


# # Add this line to recognize .h++ as a C++ header file
# set(CMAKE_CXX_SOURCE_FILE_EXTENSIONS cpp cxx cc c++ h hpp hxx h++)

# # Set the OpenSSL root directory
# set(OPENSSL_ROOT_DIR /usr/local/opt/openssl@3)

# # Find OpenSSL package
# find_package(OpenSSL REQUIRED)


# # Set the output directory for the object files and executables
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Server/objects)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Server/objects)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Server/objects)

# # Create a library from the files in the libs directory
# file(GLOB LIB_SOURCES "libs/*c++")
# add_library(project_lib STATIC ${LIB_SOURCES})
# target_include_directories(project_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source/libs ${OPENSSL_INCLUDE_DIR})


# # Add your source files here
# add_executable(test_server Server/tests/test_server.c++)

# #Add compiler flags
# target_compile_options(test_server PRIVATE -Wall -Wextra)

# # If you have additional include directories, add them like this:
# target_include_directories(test_server PRIVATE headers headers libs ${OPENSSL_INCLUDE_DIR})


# #Link the project_lib OpenSSL to test_server
# target_link_libraries(test_server PRIVATE project_lib ${OPENSSL_LIBRARIES})


# # For wxWidgets and intelliSense support
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)













































# cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cmake_minimum_required(VERSION 3.30)
project(Personal_Website_server CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_SOURCE_FILE_EXTENSIONS cpp cxx cc c++ h hpp hxx h++)

set(OPENSSL_ROOT_DIR /usr/local/opt/openssl@3)

find_package(OpenSSL REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/objects)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/objects)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/objects)

file(GLOB LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Server/libs/*c++")
add_library(project_lib STATIC ${LIB_SOURCES})
target_include_directories(project_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Server/libs ${OPENSSL_INCLUDE_DIR})

add_executable(test_server Server/tests/test_server.c++)

target_compile_options(test_server PRIVATE -Wall -Wextra)

target_include_directories(test_server PRIVATE Server/headers Server/libs ${OPENSSL_INCLUDE_DIR})

target_link_libraries(test_server PRIVATE project_lib ${OPENSSL_LIBRARIES})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
