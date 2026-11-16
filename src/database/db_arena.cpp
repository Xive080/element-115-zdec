// Arena de 4 GB para Ptr32 (punteros de 32 bits relativos a g_ptr32Base). Solo LP64 (Android/macOS arm64).
#if defined(__LP64__)
#include <universal/ptr32.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

uintptr_t g_ptr32Base = 0;
uintptr_t g_ptr32Size = 0;

void Ptr32_InitArena()
{
    if (g_ptr32Base)
        return;
    const size_t size = (size_t)4 << 30;
    void *p = mmap(NULL, size, PROT_NONE, MAP_NORESERVE | MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (p == MAP_FAILED)
    {
        fprintf(stderr, "Ptr32_InitArena: mmap de 4 GB fallo\n");
        abort();
    }
    g_ptr32Base = (uintptr_t)p;
    g_ptr32Size = size;
}

void Ptr32_AssertInArena(const void *p)
{
    uintptr_t v = (uintptr_t)p;
    if (v < g_ptr32Base || v - g_ptr32Base >= g_ptr32Size)
    {
        fprintf(stderr, "Ptr32_AssertInArena: %p fuera de la arena [%p, +0x%llx)\n", p, (void *)g_ptr32Base,
                (unsigned long long)g_ptr32Size);
        abort();
    }
}

// Tabla de manejadores para PtrRt32 (punteros de runtime en campos de 4 bytes de los assets)
#include <mutex>
#include <unordered_map>
static const size_t kRtMax = 1u << 20;
static void *g_rtTable[kRtMax];
static uint32_t g_rtCount = 0;
static std::mutex g_rtLock;
static std::unordered_map<const void *, uint32_t> g_rtMap;

uint32_t PtrRt32_Register(const void *p)
{
    if (!p)
        return 0;
    std::lock_guard<std::mutex> g(g_rtLock);
    auto it = g_rtMap.find(p);
    if (it != g_rtMap.end())
        return it->second;
    if (g_rtCount + 1 >= kRtMax)
    {
        fprintf(stderr, "PtrRt32_Register: tabla llena\n");
        abort();
    }
    g_rtTable[++g_rtCount] = (void *)p;
    g_rtMap[p] = g_rtCount;
    return g_rtCount;
}

void *PtrRt32_Resolve(uint32_t idx) { return idx ? g_rtTable[idx] : nullptr; }
#endif
