// Ptr32<T>: puntero de 32 bits relativo a la base de una arena (port arm64 de los assets de zona).
// En x86 de 32 bits el formato en memoria == formato del fichero .ff; en arm64 un T* ocupa 8 bytes y rompe
// los sizeof. Ptr32<T> guarda (dir - base) en 32 bits, manteniendo el layout. 0 = nulo; -1 y -2 son marcadores
// del cargador de zonas (se conservan tal cual, no se relocalizan).
#pragma once
#include <cstdint>
#include <cstddef>

#if defined(__LP64__) || defined(_WIN64)
extern uintptr_t g_ptr32Base;            // base de la arena de 4 GB (reservada con mmap PROT_NONE)
extern uintptr_t g_ptr32Size;
void Ptr32_AssertInArena(const void *p); // aborta si p no está en la arena (caza destinos fuera)
void Ptr32_InitArena();                  // reserva la arena (mmap 4 GB PROT_NONE)

template <typename T>
struct Ptr32 {
    uint32_t off;
    Ptr32() = default;
    Ptr32(std::nullptr_t) : off(0) {}
    Ptr32(T *p) { set(p); }
    Ptr32 &operator=(T *p) { set(p); return *this; }
    Ptr32 &operator=(std::nullptr_t) { off = 0; return *this; }
    void set(const T *p)
    {
        uintptr_t v = (uintptr_t)p;
        if (v == 0 || v == (uintptr_t)-1 || v == (uintptr_t)-2) { off = (uint32_t)v; return; } // marcadores
        Ptr32_AssertInArena(p);
        off = (uint32_t)(v - g_ptr32Base);
    }
    T *get() const
    {
        if (off == 0 || off == 0xFFFFFFFFu || off == 0xFFFFFFFEu) return (T *)(intptr_t)(int32_t)off;
        return (T *)(g_ptr32Base + off);
    }
    operator T *() const { return get(); }
    T *operator->() const { return get(); }
    T &operator*() const { return *get(); }
    T &operator[](ptrdiff_t i) const { return get()[i]; }
    explicit operator bool() const { return off != 0; }
};
template <typename T> using Ptr32Of = Ptr32<T>;
#define PTR32(T) Ptr32<T>
#else
#define PTR32(T) T *   // x86 de 32 bits: puntero normal, layout idéntico
#endif
static_assert(sizeof(uint32_t) == 4);
