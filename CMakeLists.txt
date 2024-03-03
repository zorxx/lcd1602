# Copyright 2024 Zorxx Software. All rights reserved.

# esp-idf component
if(IDF_TARGET)
   idf_component_register(SRCS "lcd1602.c" "lowlevel/esp-idf.c"
                          INCLUDE_DIRS "." "include"
                          PRIV_REQUIRES "driver" "esp_timer")
   return()
endif()

cmake_minimum_required(VERSION 3.5)
project(lcd1602)

set(LIBSRC lcd1602.c)

if(UNIX AND NOT APPLE)
   message(STATUS "Linux support")
   list(APPEND LIBSRC lowlevel/linux.c)
   list(APPEND LIBDEFS LINUX=1)
endif()

#list(APPEND LIBDEFS LCD_DEBUG_ENABLE=1)

add_library(${CMAKE_PROJECT_NAME} SHARED ${LIBSRC})
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE ${LIBDEFS})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)
