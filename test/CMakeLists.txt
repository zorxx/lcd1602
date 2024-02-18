set(APP lcd1602_test)
add_executable(${APP} main.c)
target_link_libraries(${APP} lcd1602)
target_include_directories(${APP} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)