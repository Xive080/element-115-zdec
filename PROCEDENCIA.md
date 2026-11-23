# Procedencia / Provenance

This repository is a **fork** of [MisaDev4/bo1-zombies-decompiled](https://github.com/MisaDev4/bo1-zombies-decompiled), a public project that reconstructs the game-logic source of a 2010 first-person shooter's zombies mode.
The upstream history is preserved. The work added on top (branch `android-arm64`) is by this repository's author and consists of:

- an arm64 port (macOS and Android) with a 32-bit pointer arena so the original data layouts keep working;
- fixes to the game logic (for example script builtins and model field reads) needed by the zombies maps;
- hooks used by the client harness published in `element-115`.

Upstream does not state a license; this fork adds none and claims no rights over upstream code.
The redistributable Steam libraries under `src/steam/` come from upstream and belong to their owner.

**No game data (maps, textures, sounds, videos, fastfiles) is included.** You need your own legally obtained copy of the game.
This project is unofficial and not affiliated with or endorsed by the game's publisher or developer.

Related: `element-115` (engine/harness) and `element-115-android` (Android app).
