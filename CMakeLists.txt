cmake_minimum_required(VERSION 3.28)

project(Core)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

include(CTest)

if(MSVC) 
  add_compile_options(/MP /W4)
else()
  add_compile_options(-Wall -ftime-trace -Wextra -Wpedantic)
endif()

add_subdirectory(3rd_party)
add_subdirectory(tests)
