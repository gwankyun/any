# any

`std::any`的C++98實現，實現了大部分功能，無依賴。

## CMake引入

```CMake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CMakePrintHelpers)

set(Any_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Any)
if(Any_FOUND)
  cmake_print_variables(Any_INCLUDE_DIRS)
else()
    message(FATAL_ERROR "Looking for Any - not found")
endif()

target_link_libraries(${main}
  PUBLIC
    Any::Any
)
```

## C++使用

```C++
#include <any/any.hpp>

std::vector<int> vec;
vec.push_back(1);

any a = make_any<std::vector<int>>(vec);
std::vector<int>* pvec = any_cast<std::vector<int>>(&a);
if (pvec != NULL)
{
    assert(pvec->at(0) == vec[0]);
}
```
