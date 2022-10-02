cmake_minimum_required(VERSION 3.22)

# Set the project name and language
project(basic VERSION 0.0.1 DESCRIPTION "basic" LANGUAGES CXX C)
include(FetchContent)

# Set the C standard
set(CMAKE_C_STANDARD 11)
# Add subprojects
add_subdirectory(libretro)

add_executable(basic "basic.c")
target_include_directories(basic PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(basic PUBLIC "libretro")

# Copy Assets to the Output Directory
file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets DESTINATION ${CMAKE_INSTALL_PREFIX})
