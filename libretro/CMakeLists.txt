include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git)
FetchContent_Declare(freetype GIT_REPOSITORY https://github.com/aseprite/freetype2.git)
FetchContent_MakeAvailable(glfw freetype)

file(GLOB RETRO_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/*.c
        ${CMAKE_CURRENT_LIST_DIR}/*.h
        ${CMAKE_CURRENT_LIST_DIR}/gpu/*.c
        ${CMAKE_CURRENT_LIST_DIR}/gpu/*.h
        ${CMAKE_CURRENT_LIST_DIR}/emu/*.c
        ${CMAKE_CURRENT_LIST_DIR}/emu/*.h
        ${CMAKE_CURRENT_LIST_DIR}/util/*.c
        ${CMAKE_CURRENT_LIST_DIR}/util/*.h
        ${CMAKE_CURRENT_LIST_DIR}/arch/*.c
        ${CMAKE_CURRENT_LIST_DIR}/arch/*.h
        ${CMAKE_CURRENT_LIST_DIR}/arch/${TARGET_BUILD_PLATFORM}/*.c
        ${CMAKE_CURRENT_LIST_DIR}/arch/${TARGET_BUILD_PLATFORM}/*.h)

add_library(libretro ${RETRO_SOURCES} "${CMAKE_SOURCE_DIR}/extern/glad/glad.h" "${CMAKE_SOURCE_DIR}/extern/glad/glad.c" "${CMAKE_SOURCE_DIR}/extern/stb_image.h")
target_include_directories(libretro PUBLIC ${CMAKE_SOURCE_DIR}/extern/ ${CMAKE_SOURCE_DIR}/libretro)
target_link_libraries(libretro PUBLIC "glfw" "freetype")
  
if(WIN32)
    target_compile_definitions(libretro PUBLIC LIBRETRO_PLATFORM_WIN32=1 _CRT_SECURE_NO_WARNINGS=1)
endif()
