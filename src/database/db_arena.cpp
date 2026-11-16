// Arena de 4 GB para Ptr32 (punteros de 32 bits relativos a g_ptr32Base). Solo LP64 (Android/macOS arm64).
#if defined(__LP64__)
#include <universal/ptr32.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

uintptr_t g_ptr32Base = 0;
uintptr_t g_ptr32Size = 0;

// La arena se reserva antes de los constructores globales por defecto (los pools de assets de db_registry.cpp
// se alojan dentro con Ptr32_ArenaAlloc en su inicializacion estatica).
__attribute__((constructor(101))) static void Ptr32_AutoInit() { Ptr32_InitArena(); }

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

// Sub-asignador: bump + lista libre (first-fit con fusion no implementada; los bloques de zona son pocos y grandes).
// Cada bloque lleva una cabecera de 64 bytes (conserva alineacion hasta 64). El tramo usado se mprotect(RW) por paginas.
#include <mutex>
#include <string.h>
#include <unistd.h>
struct ArenaHdr { size_t size; ArenaHdr *next; size_t pad[6]; };   // 64 bytes
static_assert(sizeof(ArenaHdr) == 64, "ArenaHdr");
static size_t g_arenaTop = 64;        // offset 0 reservado: off==0 es nulo en Ptr32
static size_t g_arenaCommitted = 0;
static ArenaHdr *g_arenaFree = nullptr;
static std::mutex g_arenaLock;

static void ArenaCommit(size_t upTo)
{
    const size_t pg = 1u << 16;
    if (upTo <= g_arenaCommitted) return;
    size_t n = (upTo + pg - 1) & ~(pg - 1);
    if (n > g_ptr32Size) { fprintf(stderr, "Ptr32_ArenaAlloc: arena agotada\n"); abort(); }
    if (mprotect((void *)(g_ptr32Base + g_arenaCommitted), n - g_arenaCommitted, PROT_READ | PROT_WRITE) != 0)
    { perror("mprotect arena"); abort(); }
    g_arenaCommitted = n;
}

void *Ptr32_ArenaAlloc(size_t size, size_t align)
{
    if (!g_ptr32Base) Ptr32_InitArena();
    if (align < 64) align = 64;
    size = (size + 63) & ~(size_t)63;
    std::lock_guard<std::mutex> g(g_arenaLock);
    for (ArenaHdr **pp = &g_arenaFree; *pp; pp = &(*pp)->next)   // first-fit (misma alineacion 64; align>64 solo por bump)
        if ((*pp)->size >= size && align == 64) {
            ArenaHdr *h = *pp; *pp = h->next; memset(h + 1, 0, h->size); return h + 1; }
    size_t start = (g_arenaTop + 64 + align - 1) & ~(align - 1);     // dato alineado; cabecera justo antes
    size_t end = start + size;
    ArenaCommit(end);
    g_arenaTop = end;
    ArenaHdr *h = (ArenaHdr *)(g_ptr32Base + start - 64);
    h->size = size; h->next = nullptr;
    return (void *)(g_ptr32Base + start);
}

void Ptr32_ArenaFree(void *p)
{
    if (!p) return;
    Ptr32_AssertInArena(p);
    std::lock_guard<std::mutex> g(g_arenaLock);
    ArenaHdr *h = (ArenaHdr *)p - 1;
    h->next = g_arenaFree; g_arenaFree = h;
}
size_t Ptr32_ArenaUsed() { return g_arenaTop; }
const char *Ptr32_ArenaStrdup(const char *s) { size_t n = strlen(s) + 1; char *p = (char *)Ptr32_ArenaAlloc(n, 64); memcpy(p, s, n); return p; }

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
