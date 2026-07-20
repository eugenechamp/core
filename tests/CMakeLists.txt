if(BUILD_TESTING)

add_executable(vector-test)
target_sources(vector-test PRIVATE vector-test.cpp)
target_include_directories(vector-test PRIVATE "${CMAKE_SOURCE_DIR}/include/")
target_link_libraries(vector-test PRIVATE core GTest::gtest GTest::gtest_main)

add_executable("optional-test")
target_sources("optional-test" PRIVATE optional-test.cpp)
target_include_directories("optional-test" PRIVATE "${CMAKE_SOURCE_DIR}/include/")
target_link_libraries("optional-test" PRIVATE GTest::gtest GTest::gtest_main)


add_executable(fn-test)
target_sources(fn-test PRIVATE fn-test.cpp)
target_include_directories(fn-test PRIVATE "${CMAKE_SOURCE_DIR}/include/")
target_link_libraries(fn-test GTest::gtest GTest::gtest_main)

add_executable(pair-test)

target_sources(pair-test PRIVATE pair-test.cpp)
target_include_directories(pair-test PRIVATE "${CMAKE_SOURCE_DIR}/include/")

add_executable(unique-ptr-test)
target_sources(unique-ptr-test PRIVATE unique-ptr-test.cpp)
target_include_directories(unique-ptr-test PRIVATE "${CMAKE_SOURCE_DIR}/include/")
target_link_libraries(unique-ptr-test GTest::gtest GTest::gtest_main)

add_executable(thread_pool_test)
target_sources(thread_pool_test PRIVATE thread_pool_test.cpp)
target_include_directories(thread_pool_test PRIVATE "${CMAKE_SOURCE_DIR}/include/")
target_link_libraries(thread_pool_test PRIVATE core GTest::gtest GTest::gtest_main)


add_executable(string_hash_test)
target_sources(string_hash_test PRIVATE string_hash_test.cpp)
target_include_directories(string_hash_test PRIVATE "${CMAKE_SOURCE_DIR}/include/")
target_link_libraries(string_hash_test PRIVATE core GTest::gtest GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(vector-test)
gtest_discover_tests(unique-ptr-test)
gtest_discover_tests("optional-test")
gtest_discover_tests("fn-test")
gtest_discover_tests("string_hash_test")

endif()