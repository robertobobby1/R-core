#include "rcpch.h"
#include "Core/Core.h"

 unsigned long long RC::heapAllocatedBytes = 0;

void* operator new(size_t size)
{
    RC::heapAllocatedBytes += size;

    return malloc(size);
}

void operator delete(void* p, size_t size)
{
    RC::heapAllocatedBytes -= size;

    free(p);
}

