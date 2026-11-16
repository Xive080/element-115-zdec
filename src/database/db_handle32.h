// Cargadores genericos de handles PTR32 (hueco de 4 bytes en el stream .ff) para los tipos del grupo C
// (GfxWorld, clipMap_t, ComWorld, EmblemSet, SndDriverGlobals). Misma logica que Load_XxxPtr de db_load.cpp,
// pero el hueco es un Ptr32 y el alta en el pool pasa por un XAssetHeader local (en LP64 es de 8 bytes).
// En 32 bits PTR32(T) == T*, asi que el comportamiento es identico al original.
#pragma once
#include <universal/ptr32.h>
#include "db_stream.h"
#include "db_stream_load.h"
#include "db_load.h"
#include "db_registry.h"

void *DB_ModLinkDeferredAlias(void *data); // db_registry.cpp
void __cdecl DB_ConvertOffsetToPointer(unsigned int *data);

#ifndef PTR32_RAW
#if defined(__LP64__)
#define PTR32_RAW(x) ((x).off)
#else
#define PTR32_RAW(x) ((unsigned int)(x))
#endif
#endif

// Handle a un asset del pool. loadT(true) carga el cuerpo; addAsset(T*) -> T* lo da de alta (DB_AddXAsset).
template <class T, class V, class LoadF, class AddF>
static void LoadHandle32(bool atStreamStart, PTR32(T) *slot, V *&var, LoadF loadT, AddF addAsset, bool modLink)
{
    Load_Stream(atStreamStart, (unsigned __int8 *)slot, 4);
    DB_PushStreamPos(0);
    if ( *slot )
    {
        unsigned int value = PTR32_RAW(*slot);
        if ( value == 0xFFFFFFFFu || value == 0xFFFFFFFEu )
        {
            *slot = (T *)AllocLoad_FxElemVisStateSample();
            var = (V *)(T *)*slot;
            unsigned int *inserted = (value == 0xFFFFFFFEu) ? DB_InsertPointer32() : 0;
            loadT(true);
            T *added = addAsset((T *)*slot);
            *slot = added;
            if ( inserted )
                *inserted = PTR32_RAW(*slot);
        }
        else
        {
            DB_ConvertOffsetToAlias((unsigned int *)slot);
            if ( modLink )
                *slot = (T *)DB_ModLinkDeferredAlias((void *)(T *)*slot);
        }
    }
    DB_PopStreamPos();
}

// Array de handles (hueco de 4 bytes por elemento)
template <class T, class V, class LoadF, class AddF>
static void LoadHandleArray32(bool atStreamStart, PTR32(T) *slots, int count, V *&var, LoadF loadT, AddF addAsset, bool modLink)
{
    Load_Stream(atStreamStart, (unsigned __int8 *)slots, 4 * count);
    for ( int i = 0; i < count; ++i )
        LoadHandle32<T, V>(false, &slots[i], var, loadT, addAsset, modLink);
}

// Handle no-asset (sin alta en el pool ni DB_PushStreamPos): XModelPieces, etc.
template <class T, class V, class LoadF>
static void LoadInlinePtr32(bool atStreamStart, PTR32(T) *slot, V *&var, LoadF loadT)
{
    Load_Stream(atStreamStart, (unsigned __int8 *)slot, 4);
    if ( *slot )
    {
        if ( PTR32_RAW(*slot) == 0xFFFFFFFFu )
        {
            *slot = (T *)AllocLoad_FxElemVisStateSample();
            var = (V *)(T *)*slot;
            loadT(true);
        }
        else
        {
            DB_ConvertOffsetToPointer((unsigned int *)slot);
        }
    }
}

// Mark_XxxPtr existentes leen un T**: se les pasa un puntero temporal con el valor del campo.
template <class U, class T, class MarkF>
static void MarkHandle32(PTR32(U) &field, T **&varPtr, MarkF markPtr)
{
    T *tmp = (T *)(U *)field;
    varPtr = &tmp;
    markPtr();
    varPtr = 0;
}

// Alta en el pool via una funcion Load_XxxAsset(XAssetHeader *) (la cabecera es una union de punteros: 8 bytes en LP64)
template <class T, class F>
static T *AddAssetViaHeader(T *p, F loadAsset)
{
    XAssetHeader h((void *)p);
    loadAsset(&h);
    return (T *)h.xmodelPieces;
}

// Alta de SndDriverGlobals (la funcion existente toma SndDriverGlobals **)
static inline SndDriverGlobals *AddSndDriverGlobals32(SndDriverGlobals *p)
{
    Load_SndDriverGlobalsAsset(&p);
    return p;
}
