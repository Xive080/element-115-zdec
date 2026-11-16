// BO1_LAYOUT_ASSERT: comprobacion de sizeof de structs de RUNTIME (no de zona), que solo valen en 32 bits.
// En LP64 los punteros miden 8 bytes y el tamano cambia legitimamente, asi que no se comprueba.
#pragma once
#if defined(__LP64__) || defined(_WIN64)
#define BO1_LAYOUT_ASSERT(...) static_assert(true)
#else
#define BO1_LAYOUT_ASSERT(...) static_assert(__VA_ARGS__)
#endif
