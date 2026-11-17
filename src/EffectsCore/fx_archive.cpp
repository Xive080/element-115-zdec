#include "fx_archive.h"
#include "fx_random.h"

FxEffectContainer *__cdecl FX_EffectFromHandle(FxSystem *system, unsigned __int16 handle)
{
    const char *v2; // eax

    if ( !system
        && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\effectscore\\fx_system.h", 399, 0, "%s", "system") )
    {
        __debugbreak();
    }
    // LP64: 0xD000 / 0x34 eran 1024 * sizeof(FxEffectContainer) / 4 y sizeof(FxEffectContainer) / 4 de x86 (0xD0 bytes)
    if ( handle >= 1024 * sizeof(FxEffectContainer) / 4 || handle % (sizeof(FxEffectContainer) / 4) )
    {
        v2 = va("%p %i", system->effects, handle);
        if ( !Assert_MyHandler(
                        "c:\\projects_pc\\cod\\codsrc\\src\\effectscore\\fx_system.h",
                        400,
                        0,
                        "%s\n\t%s",
                        "handle < FX_EFFECT_LIMIT * sizeof( FxEffectContainer ) / FxEffect::HANDLE_SCALE && handle % (sizeof( FxEffec"
                        "tContainer ) / FxEffect::HANDLE_SCALE) == 0",
                        v2) )
            __debugbreak();
    }
    return (FxEffectContainer *)((char *)system->effects + 4 * handle);
}

FxPool<FxElem,FxElemContainer> *__cdecl FX_ElemFromHandle(FxSystem *system, unsigned __int16 handle)
{
    if ( !system
        && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\effectscore\\fx_system.h", 479, 0, "%s", "system") )
    {
        __debugbreak();
    }
    return FX_PoolFromHandle_Generic<FxElem,FxElemContainer,FX_ELEM_POOL_MAX>(system->elems, handle);
}

FxElemVisuals __cdecl FX_GetElemVisuals(const FxElemDef *elemDef, int randomSeed)
{
    if ( !elemDef->visualCount
        && !Assert_MyHandler(
                    "c:\\projects_pc\\cod\\codsrc\\src\\effectscore\\fx_draw.h",
                    92,
                    0,
                    "%s\n\t(elemDef->visualCount) = %i",
                    "(elemDef->visualCount > 0)",
                    elemDef->visualCount) )
    {
        __debugbreak();
    }
    if ( elemDef->visualCount == 1 )
        return elemDef->visuals.instance;
    else
        return (FxElemVisuals)elemDef->visuals.array[(elemDef->visualCount * LOWORD(fx_randomTable[randomSeed + 21])) >> 16];
}
