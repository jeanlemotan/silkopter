#include "QStdAfx.h"

#include <memory.h>

int qMallocCount = 0;
int qFreeCount = 0;

#if !defined(Q_USE_EXTERNAL_ALLOC)

#if defined(Q_USE_NED_ALLOC)
#include "nedmalloc/nedmalloc.h"
#include "nedmalloc/nedmalloc.c"

namespace q
{
	//define these in your application, to control ALL allocations in Q
	void*	mem_alloc(size_t size) { qMallocCount++; return nedalloc::nedmalloc(size == 0 ? 1 : size); }
	void*	mem_calloc(size_t size) { qMallocCount++; return nedalloc::nedcalloc(1, size == 0 ? 1 : size); }
	void*	mem_realloc(void* p, size_t size) { qFreeCount++; qMallocCount++; return nedalloc::nedrealloc(p, size == 0 ? 1 : size); }
	void	mem_free(void* p) { nedalloc::nedfree(p); qFreeCount++; }
}

#else

namespace q
{
	//define these in your application, to control ALL allocations in Q
	void*	mem_alloc(size_t size) { qMallocCount++; return malloc(size == 0 ? 1 : size); }
	void*	mem_calloc(size_t size) { qMallocCount++; return calloc(1, size == 0 ? 1 : size); }
	void*	mem_realloc(void* p, size_t size) { qFreeCount++; qMallocCount++; return realloc(p, size == 0 ? 1 : size); }
	void	mem_free(void* p) { free(p); qFreeCount++; }
}

#endif //Q_USE_NED_ALLOC
#endif //Q_EXTERNAL_ALLOC
