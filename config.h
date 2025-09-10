#pragma once

#include <stdbool.h>

#include "ui.h"

#ifdef ENABLE_BUILTIN_CONFIG
#include "builtin_config.h"
#endif

typedef struct {
	ColorScheme color_scheme;
	SDL_Surface *digits_surface, *smileys_surface, *tiles_surface;
} Config;

bool LoadConfig(Config *config, const char *filename);
bool LoadConfigFromDefaultLocation(Config *config);

void Config_DestroySurfaces(Config *config);

Config GetBuiltInConfig();
