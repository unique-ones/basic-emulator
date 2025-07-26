include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git)
FetchContent_MakeAvailable(glfw)
FetchContent_Declare(harfbuzz GIT_REPOSITORY https://github.com/harfbuzz/harfbuzz.git GIT_TAG "11.3.2")
FetchContent_MakeAvailable(harfbuzz)
FetchContent_Declare(freetype GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git GIT_TAG "VER-2-13-3")
FetchContent_MakeAvailable(freetype)

file(GLOB RETRO_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/basic.c)

add_executable(basic ${RETRO_SOURCES} "${CMAKE_SOURCE_DIR}/extern/glad/glad.h" "${CMAKE_SOURCE_DIR}/extern/glad/glad.c" "${CMAKE_SOURCE_DIR}/extern/stb_image.h")
target_include_directories(basic PUBLIC ${CMAKE_SOURCE_DIR}/extern/ ${CMAKE_SOURCE_DIR}/source)
target_link_libraries(basic PUBLIC "glfw" "freetype" "harfbuzz")

target_compile_definitions(basic PUBLIC LIBRETRO_PLATFORM=${TARGET_BUILD_PLATFORM})

if (WIN32)
    target_compile_definitions(basic PUBLIC LIBRETRO_PLATFORM_WIN32=1 _CRT_SECURE_NO_WARNINGS=1)
endif ()
