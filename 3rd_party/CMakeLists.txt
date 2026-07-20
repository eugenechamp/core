include(FetchContent)

if(BUILD_TESTING)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
  )
  FetchContent_MakeAvailable(googletest)

  set_property(GLOBAL PROPERTY ENVIRONMENT "GTEST_COLOR=yes")

endif()

if(CORE_BUILD_BENCHMARK)
  
endif()
# WHAT IN THE FUCK IS THIS?
FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.9.5
)
FetchContent_MakeAvailable(googlebenchmark)

add_library(tuplet INTERFACE)
target_include_directories(tuplet INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/tuplet/include)

if(TARGET tuplet)
    message(STATUS "tuplet exists")
endif()

message(${CMAKE_CURRENT_SOURCE_DIR})