#include <benchmark/benchmark.h>
#include <core/vector.hpp>
#include <core/par_for.hpp>
#include <algorithm>
#include <execution>

u64 sum(const Vec<i32>& span) {
  u64 sum{};
  for(auto element : span) 
  {
    sum += element;
  }
  return sum;
}

struct Vec3 {
  float x, y, z;
};

struct GameActor {
  Vec3 position;
  Vec3 velocity;
};

static void std_foreach_add(benchmark::State& state) {
  auto size = state.range(0);
  ThreadPool::instance();

  Vec<GameActor> actors;
  actors.resize(size);

  float dt = 0.0166;

  for (auto _ : state)
  {
    std::for_each(std::execution::par, actors.begin(), actors.end(), [dt](GameActor& element) {
      element.position.x += sin(element.position.x) * cos(element.velocity.x) * dt;
      element.position.x += sin(element.position.y) * cos(element.velocity.y) * dt;
      element.position.x += sin(element.position.z) * cos(element.velocity.z) * dt;
    });

    benchmark::DoNotOptimize(actors);
  }
}
BENCHMARK(std_foreach_add)->Arg(10'000)->Arg(100'000)->Arg(1'000'000)->Arg(10'000'000);

#include <omp.h>

static void for_openmp(benchmark::State& state) {
  auto size = state.range(0);
  //omp_set_dynamic(0);
  //omp_set_num_threads(6);

  Vec<GameActor> actors;
  actors.resize(size);

  float dt = 0.0166;

  for (auto _ : state)
  {
    #pragma omp parallel
    for (auto& element : actors)
    {
      element.position.x += sin(element.position.x) * cos(element.velocity.x) * dt;
      element.position.x += sin(element.position.y) * cos(element.velocity.y) * dt;
      element.position.x += sin(element.position.z) * cos(element.velocity.z) * dt;
    }
    
    benchmark::DoNotOptimize(actors);
  }
}
BENCHMARK(for_openmp)->Arg(10'000)->Arg(100'000)->Arg(1'000'000)->Arg(10'000'000);

static void multi_threaded_add(benchmark::State& state) {
  using core::par_for;

  auto size = state.range(0);
  ThreadPool::instance();
  
  Vec<GameActor> actors;
  actors.resize(size);
  
  float dt = 0.0166;

  for(auto _ : state)
  {
    par_for(actors, [dt](std::span<GameActor> span) {
      for(auto& element : span) 
      {
        element.position.x += sin(element.position.x) * cos(element.velocity.x) * dt;
        element.position.x += sin(element.position.y) * cos(element.velocity.y) * dt;
        element.position.x += sin(element.position.z) * cos(element.velocity.z) * dt;
      }
    });

    benchmark::DoNotOptimize(actors);
  }
}
BENCHMARK(multi_threaded_add)->Arg(10'000)->Arg(100'000)->Arg(1'000'000)->Arg(10'000'000);

static void single_threaded_add(benchmark::State& state) {

  u32 size = state.range(0);
  Vec<GameActor> actors;
  actors.resize(size);
  
  float dt = 0.0166;

  for(auto _ : state)
  {
    for (auto& element : actors)
    {
      element.position.x += sin(element.position.x) * cos(element.velocity.x) * dt;
      element.position.x += sin(element.position.y) * cos(element.velocity.y) * dt;
      element.position.x += sin(element.position.z) * cos(element.velocity.z) * dt;
    }
    benchmark::DoNotOptimize(actors);
  }
}
BENCHMARK(single_threaded_add)->Arg(1000)->Arg(10'000)->Arg(100'000)->Arg(1'000'000)->Arg(10'000'000);
