#pragma once
#include <clientscript/cscr_stringlist.h>

struct ClientNotifyData // sizeof=0x10
{                                       // XREF: XAnimAddClientNotify/r
    unsigned int notetrackName;         // XREF: XAnimAddClientNotify+52/w
    unsigned int notifyType;            // XREF: XAnimAddClientNotify+69/w
    unsigned int notifyName;            // XREF: XAnimAddClientNotify+5F/w
    float timeFrac;                     // XREF: XAnimAddClientNotify+5A/w
};

struct XAnimClientNotifyAdder // sizeof=0x0
{                                       // XREF: ?CG_UpdateViewWeaponAnim@@YAXHH@Z/r
};

struct XAnimClientNotifyList // sizeof=0x604 en x86
{                                       // XREF: ?CG_UpdateViewWeaponAnim@@YAXHH@Z/r
    // LP64: 64 x sizeof(XAnimClientNotify). En x86 eran 64 x 24 = 1536 bytes y el codigo indexaba con paso 24; con el puntero
    // name de 8 bytes la estructura ocupa 32 y el paso fijo solapaba entradas (y la 64 pisaba m_numNotifies).
    alignas(8) unsigned __int8 m_clientNotifyMemory[64 * 32];
    int m_numNotifies;

    XAnimClientNotifyList();
    ~XAnimClientNotifyList();

    XAnimClientNotifyList *GetNotifyList();
    void AddNotify(const ClientNotifyData *notifyData);
};

struct XAnimClientNotify // sizeof=0x14
{
    const char *name;
    unsigned int svNotetrackName;
    unsigned int clNotetrackName;
    unsigned int notifyType;
    unsigned int clNotifyName;
    float timeFrac;

    XAnimClientNotify(const ClientNotifyData *notifyData);
    ~XAnimClientNotify();

    bool IsClientAnimNotify();
    void swap(XAnimClientNotify *otherNotify);
    const char *GetNotifyStringName();
    unsigned int GetNotifyName();
    unsigned int GetNotifyType();
    unsigned int GetNotetrackCLName();
};


