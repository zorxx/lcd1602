cmake_minimum_required(VERSION 3.5)
project(lcd1602)

set(LIBSRC lcd1602.c)

if(UNIX AND NOT APPLE)
   message(STATUS "Linux support")
   list(APPEND LIBSRC lowlevel/linux.c)
   set(LIBDEFS LINUX)
endif()

add_library(${CMAKE_PROJECT_NAME} SHARED ${LIBSRC})
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE ${LIBDEFS})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)
