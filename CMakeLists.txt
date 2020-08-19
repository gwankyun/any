# cmake最低版本
cmake_minimum_required(VERSION 3.18)

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

set(Any_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Any)
if(Any_FOUND)
  cmake_print_variables(Any_INCLUDE_DIRS)
endif()

add_compile_options(
  "$<$<CXX_COMPILER_ID:MSVC>:/W4;/MP>")

add_executable(${project})
target_sources(${project}
  PRIVATE
    src/main.cpp
    include/any/any.hpp)
target_include_directories(${project} PRIVATE ${Any_INCLUDE_DIRS})
