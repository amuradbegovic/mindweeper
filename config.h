#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "ui.h"

#ifdef ENABLE_BUILTIN_CONFIG

extern ColorScheme default_colors;

extern unsigned char textures_digits_bmp[];
extern unsigned int textures_digits_bmp_len;

extern unsigned char textures_smileys_bmp[];
extern unsigned int textures_smileys_bmp_len;

extern unsigned char textures_tiles_bmp[];
extern unsigned int textures_tiles_bmp_len;

#endif
/*
typedef uint8_t Uint8;
typedef struct {
	Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} SDL_Color;

typedef struct {
    SDL_Color window_background, top_panel_background,
    frame_top_left, frame_bottom_right, margin_top_left;
} ColorScheme;*/

typedef struct {
	ColorScheme color_scheme;
	unsigned char *digits_bmp, *smileys_bmp, *tiles_bmp;
	unsigned int digits_bmp_len, smileys_bmp_len, tiles_bmp_len;
	//SDL_Surface *digits_surface, *smileys_surface, *tiles_surface;
} Config;

bool LoadConfig(Config *config, const char *filename);
bool LoadConfigFromDefaultLocation(Config *config);

void Config_FreeBuffers(Config *config);

Config GetBuiltInConfig();
