#pragma once
// rand() con el rango de MSVC (0..32767). El codigo original divide por 32767/32768 o
// multiplica por 1/32768 contando con RAND_MAX=0x7FFF; glibc/bionic/macOS dan hasta 2^31-1.
#include <stdlib.h>
static inline int Com_RandMsvc() { return rand() & 0x7FFF; }
