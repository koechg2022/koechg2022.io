# cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../..

cmake_minimum_required(VERSION 3.3)
project(Personal_Website)



#  Set the standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})



# Include directories
include_directories(${PROJECT_SOURCE_DIR}/Server/headers)



# Output directories for object files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Server/objects)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Server/objects)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Server/objects)



# Add executable targets
add_executable(test_server ${PROJECT_SOURCE_DIR}/Server/source/test_server.c++)
add_executable(test_server_gui ${PROJECT_SOURCE_DIR}/Server/source/test_server_gui.c++)
add_executable(server_with_gui ${PROJECT_SOURCE_DIR}/Server/source/server_with_gui.c++)



# set the output directories for each target
set_target_properties(test_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Server/objects OUTPUT_NAME "test_server")
set_target_properties(test_server_gui PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Server/objects OUTPUT_NAME "test_server_gui")
set_target_properties(server_with_gui PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Server/objects OUTPUT_NAME "server_with_gui")



# link wxWidgets libraries to executable targets
target_link_libraries(test_server_gui ${wxWidgets_LIBRARIES})
target_link_libraries(server_with_gui ${wxWidgets_LIBRARIES})



# set the compiler options
target_compile_options(test_server PRIVATE -Wall -Wextra)
target_compile_options(test_server_gui PRIVATE -Wall -Wextra)
target_compile_options(server_with_gui PRIVATE -Wall -Wextra)