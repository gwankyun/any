# cmake最低版本
cmake_minimum_required(VERSION 3.16)

# 啟用IDE目錄功能
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 包含模塊
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
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

# 源目錄
set(MAIN_DIR src)

# 可執行文件名
set(MAIN_FILE main)

# set(CMAKE_FIND_LIBRARY_PREFIXES lib)
# if(NOT BOOST_ROOT)
# 	set(BOOST_ROOT "") # boost的根目錄
# endif()
# find_package(Boost COMPONENTS date_time context coroutine regex REQUIRED)
# if(Boost_FOUND)
# 	cmake_print_variables(Boost_INCLUDE_DIRS)
# 	cmake_print_variables(Boost_LIBRARIES)
# 	cmake_print_variables(Boost_LIB_VERSION)
# endif()

add_executable(${project} "")
target_sources(${project} PRIVATE ${MAIN_DIR}/${MAIN_FILE}.cpp)
target_include_directories(${project} PRIVATE ${Any_INCLUDE_DIRS})
# target_link_libraries(${PROJECT} ${Boost_LIBRARIES})

if(${MSVC})
	add_definitions(/W4 /MP)
endif()
