cmake_minimum_required(VERSION 3.28)

project(Core)
set(CMAKE_CXX_STANDARD 20)

option(CORE_BUILD_BENCHMARK ON)

enable_testing()

include(CTest)

if(MSVC) 
  add_compile_options(/MP /W4)
else()
  add_compile_options(-Wall -ftime-trace -Wextra -Wpedantic)
endif()

add_library(core)

target_include_directories(core PUBLIC include)
set(CORE_INCLUDES 
PUBLIC
  include/core/memory.hpp
  include/core/def.hpp
  include/core/concepts.hpp
  include/core/vector.hpp
  include/core/function.hpp
  include/core/ts_queue.hpp
  include/core/thread_pool.hpp
  include/core/string_hash.hpp
  include/core/strong_type.hpp
  include/core/tuple.hpp
)

set(CORE_SOURCES 
PUBLIC
  src/thread_pool.cpp
)


target_sources(core PRIVATE ${CORE_INCLUDES} ${CORE_SOURCES})
target_link_libraries(core PUBLIC tuplet)

get_target_property(INCLUDES core INCLUDE_DIRECTORIES)
message("${INCLUDES}")

get_target_property(INTERFACE_INCLUDES core INTERFACE_INCLUDE_DIRECTORIES)
message("${INTERFACE_INCLUDES}")

add_subdirectory(3rd_party)
add_subdirectory(tests)
add_subdirectory(benches)
