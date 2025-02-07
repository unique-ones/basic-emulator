cmake_minimum_required(VERSION 3.22)

# Set the project name and language
project(basic VERSION 0.0.1 DESCRIPTION "basic" LANGUAGES C)
include(FetchContent)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add subprojects
add_subdirectory(source)


# Copy Assets to the Output Directory
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_INSTALL_PREFIX})

# Copy compile commands
# Compile commands for clangd
if (EXISTS "${CMAKE_INSTALL_PREFIX}/compile_commands.json")
    file(CREATE_LINK "${CMAKE_INSTALL_PREFIX}/compile_commands.json" "${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json")
endif ()
