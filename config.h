#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "ui.h"

typedef struct {
	ColorScheme color_scheme;
	bool allocated_buffers;
	unsigned char *digits_bmp, *smileys_bmp, *tiles_bmp;
	unsigned int digits_bmp_len, smileys_bmp_len, tiles_bmp_len;
} Config;

#ifdef ENABLE_BUILTIN_CONFIG

extern const ColorScheme default_colors;

extern const unsigned char textures_digits_bmp[];
extern const unsigned int textures_digits_bmp_len;

extern const unsigned char textures_smileys_bmp[];
extern const unsigned int textures_smileys_bmp_len;

extern const unsigned char textures_tiles_bmp[];
extern const unsigned int textures_tiles_bmp_len;

extern Config builtin_config;

#endif

bool LoadConfig(Config *config, const char *filename);
bool LoadConfigFromDefaultLocation(Config *config);

void Config_FreeBuffers(Config *config);
