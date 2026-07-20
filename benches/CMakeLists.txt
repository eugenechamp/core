add_executable(vec-benches)

target_sources(vec-benches PRIVATE vector/vector_grow.cpp)

target_link_libraries(vec-benches INTERFACE core PRIVATE benchmark::benchmark benchmark::benchmark_main)
target_include_directories(vec-benches PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(fn-call)

target_sources(fn-call PRIVATE fn/fn-call.cpp)

target_link_libraries(fn-call INTERFACE core PRIVATE benchmark::benchmark benchmark::benchmark_main)
target_include_directories(fn-call PUBLIC ${CMAKE_SOURCE_DIR}/include)


add_executable(fn-creation-bench)

target_sources(fn-creation-bench PRIVATE fn/fn-creation-bench.cpp)

target_link_libraries(fn-creation-bench INTERFACE core PRIVATE benchmark::benchmark benchmark::benchmark_main)
target_include_directories(fn-creation-bench PUBLIC ${CMAKE_SOURCE_DIR}/include)


add_executable(par_for_bench)

target_sources(par_for_bench PRIVATE par_for_benchmark.cpp)

target_link_libraries(par_for_bench PRIVATE core PRIVATE benchmark::benchmark benchmark::benchmark_main)
target_include_directories(par_for_bench PUBLIC ${CMAKE_SOURCE_DIR}/include)