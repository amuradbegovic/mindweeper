#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifndef INI2C
#include "ui.h"
#else 

typedef uint8_t Uint8;
#define SDL_MAX_UINT8   ((Uint8)0xFF)           /* 255 */
#define SDL_MIN_UINT8   ((Uint8)0x00)           /* 0 */

typedef struct SDL_Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} SDL_Color;

typedef struct {
    SDL_Color window_background, top_panel_background,
    frame_top_left, frame_bottom_right, margin_top_left;
} ColorScheme;

#endif

typedef struct {
	ColorScheme color_scheme;
	bool allocated_buffers;
	unsigned char *digits_bmp, *smileys_bmp, *tiles_bmp;
	unsigned int digits_bmp_len, smileys_bmp_len, tiles_bmp_len;
} Config;

#ifdef ENABLE_BUILTIN_CONFIG

extern const ColorScheme builtin_colors;

extern const unsigned char builtin_digits_bmp[];
extern const unsigned int builtin_digits_bmp_len;

extern const unsigned char builtin_smileys_bmp[];
extern const unsigned int builtin_smileys_bmp_len;

extern const unsigned char builtin_tiles_bmp[];
extern const unsigned int builtin_tiles_bmp_len;

extern Config builtin_config;

#endif

bool LoadConfig(Config *config, const char *filename);
bool LoadConfigFromDefaultLocation(Config *config);

void Config_FreeBuffers(Config *config);
