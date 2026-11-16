#pragma once

#include "pathnode.h"

struct PathData // sizeof=0x28
{                                       // XREF: GameWorldSp/r
    unsigned int nodeCount;             // XREF: Bot_UpdatePath+47B/r
                                        // Bot_UpdatePath+484/r
    PTR32(pathnode_t) nodes;                  // XREF: Bot_UpdatePath+49E/r
                                        // Bot_UpdateDirection+98D/r
    PTR32(pathbasenode_t) basenodes;
    unsigned int chainNodeCount;
    PTR32(unsigned __int16) chainNodeForNode;
    PTR32(unsigned __int16) nodeForChainNode;
    int visBytes;
    PTR32(unsigned __int8) pathVis;
    int nodeTreeCount;
    PTR32(pathnode_tree_t) nodeTree;

    PathData()
    {
        memset(this, 0, sizeof(PathData)); // add
    }
};

struct GameWorldSp // sizeof=0x2C
{
    PTR32(const char) name;
    PathData path;
};

struct GameWorldMp // sizeof=0x2C
{                                       // XREF: .data:GameWorldMp gameWorldMp/r
    PTR32(const char) name;
    PathData path;                      // XREF: Bot_UpdatePath+47B/r
};


#ifdef BO1_SP
extern GameWorldSp gameWorldSp;
#else
extern GameWorldMp gameWorldMp;
#endif

extern GameWorldMp *gameWorldCurrent;
