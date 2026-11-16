#pragma once
#include <cstddef>
#include <universal/ptr32.h>

enum XAssetType : __int32
{                                                                             // XREF: XAsset/r
    ASSET_TYPE_XMODELPIECES = 0x0,
    ASSET_TYPE_PHYSPRESET = 0x1,
    ASSET_TYPE_PHYSCONSTRAINTS = 0x2,
    ASSET_TYPE_DESTRUCTIBLEDEF = 0x3,
    ASSET_TYPE_XANIMPARTS = 0x4,
    ASSET_TYPE_XMODEL = 0x5,
    ASSET_TYPE_MATERIAL = 0x6,
    ASSET_TYPE_TECHNIQUE_SET = 0x7,
    ASSET_TYPE_IMAGE = 0x8,
    ASSET_TYPE_SOUND = 0x9,
    ASSET_TYPE_SOUND_PATCH = 0xA,
    ASSET_TYPE_CLIPMAP = 0xB,
    ASSET_TYPE_CLIPMAP_PVS = 0xC,
    ASSET_TYPE_COMWORLD = 0xD,
    ASSET_TYPE_GAMEWORLD_SP = 0xE,
    ASSET_TYPE_GAMEWORLD_MP = 0xF,
    ASSET_TYPE_MAP_ENTS = 0x10,
    ASSET_TYPE_GFXWORLD = 0x11,
    ASSET_TYPE_LIGHT_DEF = 0x12,
    ASSET_TYPE_UI_MAP = 0x13,
    ASSET_TYPE_FONT = 0x14,
    ASSET_TYPE_MENULIST = 0x15,
    ASSET_TYPE_MENU = 0x16,
    ASSET_TYPE_LOCALIZE_ENTRY = 0x17,
    ASSET_TYPE_WEAPON = 0x18,
    ASSET_TYPE_WEAPONDEF = 0x19,
    ASSET_TYPE_WEAPON_VARIANT = 0x1A,
    ASSET_TYPE_SNDDRIVER_GLOBALS = 0x1B,
    ASSET_TYPE_FX = 0x1C,
    ASSET_TYPE_IMPACT_FX = 0x1D,
    ASSET_TYPE_AITYPE = 0x1E,
    ASSET_TYPE_MPTYPE = 0x1F,
    ASSET_TYPE_MPBODY = 0x20,
    ASSET_TYPE_MPHEAD = 0x21,
    ASSET_TYPE_CHARACTER = 0x22,
    ASSET_TYPE_XMODELALIAS = 0x23,
    ASSET_TYPE_RAWFILE = 0x24,
    ASSET_TYPE_STRINGTABLE = 0x25,
    ASSET_TYPE_PACK_INDEX = 0x26,
    ASSET_TYPE_XGLOBALS = 0x27,
    ASSET_TYPE_DDL = 0x28,
    ASSET_TYPE_GLASSES = 0x29,
    ASSET_TYPE_EMBLEMSET = 0x2A,
    ASSET_TYPE_COUNT = 0x2B,
    ASSET_TYPE_STRING = 0x2B,
    ASSET_TYPE_ASSETLIST = 0x2C,
};
inline XAssetType &operator++(XAssetType &t)
{
    t = static_cast<XAssetType>((static_cast<int>(t) + 1));
    return t;
}
inline XAssetType operator++(XAssetType &t, int)
{
    XAssetType old = t;
    t = static_cast<XAssetType>((static_cast<int>(t) + 1));
    return old;
}


struct XModelPieces;
struct PhysPreset;
struct PhysConstraints;
struct DestructibleDef;
struct XAnimParts;
struct XModel;
struct Material;
struct MaterialPixelShader;
struct MaterialVertexShader;
struct MaterialTechniqueSet;
struct GfxImage;
struct SndBank;
struct SndPatch;
struct clipMap_t;
struct ComWorld;
struct GameWorldSp;
struct GameWorldMp;
struct MapEnts;
struct GfxWorld;
struct GfxLightDef;
struct Font_s;
struct MenuList;
struct menuDef_t;
struct LocalizeEntry;
struct WeaponVariantDef;
struct SndDriverGlobals;
struct FxEffectDef;
struct FxImpactTable;
struct RawFile;
struct StringTable;
struct PackIndex;
struct XGlobals;
struct ddlRoot_t;
struct Glasses;
struct TextureList;
struct EmblemSet;

union XAssetHeader // sizeof=0x4
{                                                                             // XREF: CG_AddVisionSetMenuItem+13/r
    PTR32(XModelPieces) xmodelPieces;
    PTR32(PhysPreset) physPreset;
    PTR32(PhysConstraints) physConstraints;
    PTR32(DestructibleDef) destructibleDef;
    PTR32(XAnimParts) parts;
    PTR32(XModel) model;
    PTR32(Material) material;
    PTR32(MaterialPixelShader) pixelShader;
    PTR32(MaterialVertexShader) vertexShader;
    PTR32(MaterialTechniqueSet) techniqueSet;
    PTR32(GfxImage) image;
    PTR32(SndBank) sound;
    PTR32(SndPatch) soundPatch;
    PTR32(clipMap_t) clipMap;
    PTR32(ComWorld) comWorld;
    PTR32(GameWorldSp) gameWorldSp;
    PTR32(GameWorldMp) gameWorldMp;
    PTR32(MapEnts) mapEnts;
    PTR32(GfxWorld) gfxWorld;
    PTR32(GfxLightDef) lightDef;
    PTR32(Font_s) font;
    PTR32(MenuList) menuList;
    PTR32(menuDef_t) menu;
    PTR32(LocalizeEntry) localize;
    PTR32(WeaponVariantDef) weapon;
    PTR32(SndDriverGlobals) sndDriverGlobals;
    PTR32(FxEffectDef) fx;
    PTR32(FxImpactTable) impactFx;
    PTR32(RawFile) rawfile;
    PTR32(StringTable) stringTable;
    PTR32(PackIndex) packIndex;
    PTR32(XGlobals) xGlobals;
    PTR32(ddlRoot_t) ddlRoot;
    PTR32(Glasses) glasses;
    PTR32(TextureList) textureList;
    PTR32(EmblemSet) emblemSet;
    PTR32(void) data;

    XAssetHeader()
    {
        data = nullptr;
    }

    XAssetHeader(void *p)
    {
        data = p;
    }
};