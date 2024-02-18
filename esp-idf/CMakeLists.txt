# Copyright 2024 Zorxx Software. All rights reserved.

# esp-idf component
if(IDF_TARGET)
   idf_component_register(SRCS "lcd1602.c"
                          PRIV_INCLUDE_DIRS "."
                          INCLUDE_DIRS "./include"
                          REQUIRES "driver")
   return()
endif()
