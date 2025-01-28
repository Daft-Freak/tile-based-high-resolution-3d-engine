#pragma once

// maximum (post-transform) vertices, each entry is 24 bytes
#ifndef THR3E_MAX_OUTPUT_VERTICES
#define THR3E_MAX_OUTPUT_VERTICES 1024
#endif

#ifndef THR3E_MAX_TEXTURES
#define THR3E_MAX_TEXTURES 1
#endif

// size of tiles, smaller = less memory, larger = less passes
// uses width*height*4 bytes for colour+depth tile buffers
// (* 2 if multicore is enabled)
#ifndef THR3E_TILE_WIDTH
#define THR3E_TILE_WIDTH 160
#endif

#ifndef THR3E_TILE_HEIGHT
#define THR3E_TILE_HEIGHT 80
#endif

// skip tile buffer for colour buffer, must be 0 for PicoVision
#ifndef THR3E_COLOUR_DIRECT
#ifdef BLIT_BOARD_PIMORONI_PICOVISION
#define THR3E_COLOUR_DIRECT 0
#else
#define THR3E_COLOUR_DIRECT 1
#endif
#endif

#ifdef PICO_BUILD
// default to fast
#ifndef THR3E_PICO_INTERP
#define THR3E_PICO_INTERP 1
#endif

#ifndef THR3E_PICO_MULTICORE
#define THR3E_PICO_MULTICORE 1
#endif
#else
// not a pico so these make no sense
#undef THR3E_PICO_INTERP
#undef THR3E_PICO_MULTICORE
#endif