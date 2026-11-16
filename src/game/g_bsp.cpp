#include "g_bsp.h"

#include <universal/ptr32.h>
GameWorldMp &gameWorldMp = *new (Ptr32_ArenaAlloc(sizeof(GameWorldMp), 64)) GameWorldMp; // arm64: singleton de assets dentro de la arena (se referencia con Ptr32)
