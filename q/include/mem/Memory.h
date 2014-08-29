#pragma once

#include "../Config.h"

#if defined(Q_USE_EXTERNAL_ALLOC)

namespace q
{
	//define these in your application, to control ALL allocations in Q
	extern void*	mem_alloc(size_t);
	extern void*	mem_calloc(size_t);
	extern void*	mem_realloc(void*, size_t);
	extern void		mem_free(void*);
}

#else

namespace q
{
	//define these in your application, to control ALL allocations in Q
	void*	mem_alloc(size_t);
	void*	mem_calloc(size_t);
	void*	mem_realloc(void*, size_t);
	void	mem_free(void*);
}

#endif //Q_EXTERNAL_ALLOC

// inline void* operator new(size_t size)
// {
// 	return q::memAlloc(size);
// }
// inline void* operator new[](size_t size)
// {
// 	return q::memAlloc(size);
// }
// inline void operator delete(void* p)
// {
// 	if (p) q::memFree(p);
// }
// inline void operator delete[](void* p)
// {
// 	if (p) q::memFree(p);
// }
