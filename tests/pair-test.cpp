#include <core/pair.hpp>
#include <core/def.hpp>
#include <core/memory.hpp>
#include <memory>
#include <tuple>
#include <iostream>

#include <core/vector.hpp>

struct Empty    {};
struct EmptyToo {};
struct Body { u32 height; u32 weight; };

static_assert(sizeof(Pair<EmptyToo, Empty>) == 1);
static_assert(sizeof(Pair<i32, Empty>) == sizeof(i32));
static_assert(std::is_same_v<Pair<Body>, Pair<Body, Body>>);

int main()
{
  auto [body1, body2] = Pair<Body>{.first = {175, 80}, .second = {183, 82}};
}