#include "rcpch.h"
#include "Core/Core.h"

#ifdef RC_DEBUG
unsigned long long heapAllocatedBytes = 0;

void *operator new(size_t size) {
    heapAllocatedBytes += size;
    return malloc(size);
}
void operator delete(void *p, size_t size) {
    heapAllocatedBytes -= size;
    free(p);
}
#endif
