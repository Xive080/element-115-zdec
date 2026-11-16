#pragma once
#include <gfx_d3d/r_material.h>
#include <gfx_d3d/fxprimitives.h>
#include <universal/ptr32.h>

struct GlassState // sizeof=0x4
{                                       // XREF: GlassClient/r

    enum State : __int32
    {                                       // XREF: ?SetState@GlassServer@@QAEXW4State@GlassState@@QBM1@Z/r
        PRISTINE  = 0x0,
        CRACKED   = 0x1,
        SHATTERED = 0x2,
        INVALID   = 0x3,
    };
    //GlassState::<unnamed_type_val> val;
    union //GlassState::<unnamed_type_val> // sizeof=0x4
    {                                       // XREF: GlassState/r
        unsigned int i;
        //$01920CFBCEBBC7D8417C03082B059610 __s1;
        struct
        {
            unsigned __int32 state : 4;
            unsigned __int32 id : 12;
        };
    } val;
};

struct GlassDef // sizeof=0x3C
{                                       // XREF: GlassDefLoad/r
    PTR32(const char) name;
    int maxHealth;
    float thickness;
    float minShardSize;
    float maxShardSize;
    float shardLifeProbablility;
    int maxShards;
    PTR32(Material) pristineMaterial;
    PTR32(Material) crackedMaterial;
    PTR32(Material) shardMaterial;
    PTR32(const char) crackSound;
    PTR32(const char) shatterShound;
    PTR32(const char) autoShatterShound;
    PTR32(const FxEffectDef) crackEffect;
    PTR32(const FxEffectDef) shatterEffect;
};

const struct Glass // sizeof=0x7C
{
    PTR32(GlassDef) glassDef;
    unsigned int index;
    unsigned int brushModel;
    float origin[3];
    float angles[3];
    float absmin[3];
    float absmax[3];
    bool isPlanar;
    unsigned __int8 numOutlineVerts;
    // padding byte
    // padding byte
    PTR32(float[2]) outline;
    float outlineAxis[3][3];
    float outlineOrigin[3];
    float uvScale;
    float thickness;
};

struct Glasses // sizeof=0x38
{                                       // XREF: .data:glasses/r
    PTR32(const char) name;             // XREF: GetGlasses_LoadObj(void)+4A/w
    unsigned int numGlasses;            // XREF: GetGlasses_LoadObj(void)+40/w
    PTR32(Glass) glasses;               // XREF: GetGlasses_LoadObj(void)+3A/w
    PTRRT32(unsigned __int8) workMemory;
    unsigned int workMemorySize;
    unsigned int smallAllocatorBlocks;
    unsigned int maxGroups;
    unsigned int maxShards;
    unsigned int maxPhysics;
    unsigned int shardMemorySize;
    unsigned int maxFreeCmd;
    unsigned int numSlots;
    unsigned int numVerts;
    unsigned int numIndices;
};

struct __declspec(align(8)) GlassTimer // sizeof=0x10
{                                       // XREF: ?Triangulate@Triangles@GlassShard@@QAE_NXZ/r
                                        // ?Shatter@GlassShard@@QAEHQAPAU1@H@Z/r ...
    unsigned __int64 start;             // XREF: ShardGroup::GenerateVerts(bool,int)+2F5/w
                                        // ShardGroup::GenerateVerts(bool,int)+2F8/w ...
    unsigned __int64 *counter;          // XREF: ShardGroup::GenerateVerts(bool,int)+2ED/w
                                        // ShardGroup::GenerateVerts(bool,int)+426/r ...
    // padding byte
    // padding byte
    // padding byte
    // padding byte
};