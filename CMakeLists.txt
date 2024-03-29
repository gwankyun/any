﻿# cmake最低版本
cmake_minimum_required(VERSION 3.24)

# 啟用IDE目錄功能
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 包含模塊
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CMakePrintHelpers)

# 輸出目錄
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# 項目名
set(project any)
project(${project})

set(Any_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Any REQUIRED)

add_compile_options(
  "$<$<CXX_COMPILER_ID:MSVC>:/W4;/MP;/Zc:__cplusplus;>")

add_executable(${project})
target_sources(${project}
  PRIVATE
    src/main.cpp
    include/any/marco.hpp
    include/any.hpp)

# 引用庫
target_link_libraries(${project}
  PUBLIC
    Any::Any)

target_compile_features(${project} PRIVATE cxx_std_17)

# 如果找到Catch2就添加測試
find_package(Catch2 3 CONFIG)
cmake_print_variables(Catch2_FOUND)

if(Catch2_FOUND)
  set(test test)
  add_executable(${test})
  target_sources(${test}
    PRIVATE src/test.cpp)
  target_link_libraries(${test}
    PUBLIC
      Any::Any Catch2::Catch2)
  target_compile_features(${test} PRIVATE cxx_std_20)
endif()

