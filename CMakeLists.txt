# Copyright 2024 Zorxx Software. All rights reserved.

# esp-idf component
if(IDF_TARGET)
   idf_component_register(SRCS "lib/lcd1602.c" "lib/esp-idf.c"
                          INCLUDE_DIRS "include"
                          PRIV_INCLUDE_DIRS "lib" "include/lcd1602"
                          PRIV_REQUIRES "driver" "esp_timer")
   return()
endif()

cmake_minimum_required(VERSION 3.5)
set(project lcd1602)
project(${project} LANGUAGES C VERSION 1.2.0)

add_library(lcd1602 STATIC lib/lcd1602.c lib/linux.c)
target_include_directories(lcd1602 PUBLIC include)
target_include_directories(lcd1602 PRIVATE lib include/lcd1602)
target_compile_definitions(lcd1602 PRIVATE SYS_DEBUG_ENABLE)
install(TARGETS lcd1602 LIBRARY DESTINATION lib)
install(DIRECTORY include/lcd1602 DESTINATION include)

add_subdirectory(examples/linux)
