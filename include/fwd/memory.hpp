#ifndef _MEMORY_FWD_
#define _MEMORY_FWD_

template<typename T>
struct DefaultDelete;

template<typename T>
struct DefaultDelete<T[]>;

template<typename T, typename Deleter = DefaultDelete<T>>
class Unique;


#endif