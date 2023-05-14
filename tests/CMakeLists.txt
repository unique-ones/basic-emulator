# tests cmake configuration

set(TEST_TARGETS "list_test" "stack_test" "map_test")

foreach (TEST_TARGET ${TEST_TARGETS})
    add_executable(${TEST_TARGET} "${TEST_TARGET}.c")
    target_link_libraries(${TEST_TARGET} PUBLIC "libretro")
    target_include_directories(${TEST_TARGET} PUBLIC ${CMAKE_SOURCE_DIR})
    add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})
endforeach ()