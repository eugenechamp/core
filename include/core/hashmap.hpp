#ifndef _CORE_HASHMAP_
#define _CORE_HASHMAP_
#include <unordered_map>

template<typename K, typename V>
using HashMap = std::unordered_map<K, V>;

#endif