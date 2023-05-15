cmake_minimum_required(VERSION 3.22)

# Set the project name and language
project(basic VERSION 0.0.1 DESCRIPTION "basic" LANGUAGES C)
include(FetchContent)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add subprojects
add_subdirectory(libretro)

add_executable(basic "basic.c")
target_include_directories(basic PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(basic PUBLIC "libretro")

# Copy Assets to the Output Directory
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_INSTALL_PREFIX})

enable_testing()
add_subdirectory(tests)