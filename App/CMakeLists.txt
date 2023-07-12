cmake_minimum_required (VERSION 3.22)
set(BOARD esp32)

find_package(Zephyr)
project(App)

target_sources(app PRIVATE 
  src/main.c
  src/DisplayThread.c
  src/AccelThread.c
  include/DisplayThread.h
  include/AccelThread.h
  )

target_include_directories(app PRIVATE include)

