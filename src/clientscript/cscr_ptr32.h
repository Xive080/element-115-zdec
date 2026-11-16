// Punteros de script de 32 bits (port LP64). En el exe x86 la VM GSC guarda punteros como enteros de 4 bytes:
// posiciones de codigo (codepos) dentro del bytecode, vectores, buffers de pila de hilos y nodos del arbol de parseo
// (sval_u, VariableUnion, VariableStackBuffer::pos). En arm64 esos valores se guardan como DESPLAZAMIENTO de 32 bits
// sobre la arena Ptr32 (g_ptr32Base): se conserva el layout y la aritmetica (p + 4, p1 - p2) sigue siendo valida.
// Todo lo que se apunte asi tiene que vivir en la arena: programBuffer (hunk de usuario), arbol de memoria (SL_*,
// vectores, pilas), nodos de parseo (hunk de usuario), memoria temporal (Hunk_AllocateTempMemory*) y g_EndPos.
//   SCR_P2U(p): puntero -> uint32 (el (unsigned int)p del decompilado)
//   SCR_U2P(T, u): uint32 -> T (el (T)u del decompilado, p.ej. *(int *)u)
#pragma once
#include <universal/ptr32.h>
#include <stdint.h>

#if defined(__LP64__) || defined(_WIN64)
inline unsigned int Scr_PtrToU32(const void *p)
{
    if (!p)
        return 0;
    Ptr32_AssertInArena(p);
    return (unsigned int)((uintptr_t)p - g_ptr32Base);
}
inline char *Scr_U32ToPtr(unsigned int u) { return u ? (char *)(g_ptr32Base + u) : nullptr; }
#define SCR_P2U(p) Scr_PtrToU32((const void *)(p))
#define SCR_U2P(T, u) ((T)Scr_U32ToPtr((unsigned int)(u)))
// punteros a funciones nativas (builtins cacheados en variables de script): indice de la tabla PtrRt32
#define SCR_F2U(f) ((unsigned int)PtrRt32_Register((const void *)(f)))
#define SCR_U2F(T, u) ((T)PtrRt32_Resolve((unsigned int)(u)))
#else
#define SCR_P2U(p) ((unsigned int)(p))
#define SCR_U2P(T, u) ((T)(u))
#define SCR_F2U(f) ((unsigned int)(f))
#define SCR_U2F(T, u) ((T)(u))
#endif
