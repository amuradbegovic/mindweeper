#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_surface.h>
#include <string.h>
#include <ctype.h>

#include <ini.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3_image/SDL_image.h>

#include "config.h"

const char hex_digits[] = "0123456789ABCDEF";

static inline int HexToDecDigit(char hex_digit) {
	char *pos = strchr(hex_digits, toupper(hex_digit));
	return (pos == NULL) ? -1 : pos-hex_digits;
}

static inline bool HasIllegalChars(const char *string) {
	for (size_t i = 0; i < strlen(string); ++i)
		if (HexToDecDigit(string[i]) == -1) return true;
	return false;
}

/* r,g,b,a are set to -1 on error */
SDL_Color GetColorFromString(const char *string) {
	unsigned long len = strlen(string);
	if ((len != 4 && len != 7) || string[0] != '#' || HasIllegalChars(string+1))
		return (SDL_Color){-1, -1, -1, -1};
	/* not very clean but no need for that now */
	if (len == 4) return (SDL_Color){HexToDecDigit(string[1]), HexToDecDigit(string[2]), HexToDecDigit(string[3])};
    #define TWODIGITS(i) HexToDecDigit(string[i]) * 16 + HexToDecDigit(string[i])
	return (SDL_Color){TWODIGITS(1), TWODIGITS(3), TWODIGITS(5)};
}

/* adapted from the example found in the  */
static int handler(void* cfg, const char* section, const char* name, const char* value) {
    Config* pconfig = (Config*)cfg;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

	/* colors */
    if (MATCH("colors", "window_background")) {
        //strcpy(pconfig->window_background_color, value);
		pconfig->color_scheme.window_background = GetColorFromString(value);
    } else if (MATCH("colors", "top_panel_background")) {
		//strcpy(pconfig->top_panel_background_color, value);
		pconfig->color_scheme.top_panel_background = GetColorFromString(value);
	} else if (MATCH("colors", "frame_top_left")) {
		//strcpy(pconfig->frame_top_left_color, value);
		pconfig->color_scheme.frame_top_left = GetColorFromString(value);
	} else if (MATCH("colors", "frame_bottom_right")) {
		//strcpy(pconfig->frame_bottom_right_color, value);
		pconfig->color_scheme.frame_bottom_right = GetColorFromString(value);
	} else if (MATCH("colors", "margin_top_left")) {
		//strcpy(pconfig->margin_top_left_color, value);
		pconfig->color_scheme.margin_top_left = GetColorFromString(value);
	}
	/* paths to texture files */
	else if (MATCH("textures", "digits")) {
		//strcpy(pconfig->digits_texture_path, value);
		pconfig->digits_surface = SDL_LoadBMP(value);
	} else if (MATCH("textures", "smileys")) {
		//strcpy(pconfig->smileys_texture_path, value);
		pconfig->smileys_surface = SDL_LoadBMP(value);
	} else if (MATCH("textures", "tiles")) {
		//strcpy (pconfig->tiles_texture_path, value);
		pconfig->tiles_surface = SDL_LoadBMP(value);
	} else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

bool LoadConfig(Config *config, const char *filename) {
	if (ini_parse(filename, handler, config) < 0)
        return false;
	return true;
}


bool LoadConfigFromDefaultLocation(Config *config) {
	return LoadConfig(config, "");
}

void Config_DestroySurfaces(Config *config) {
	SDL_DestroySurface(config->digits_surface);
	SDL_DestroySurface(config->smileys_surface);
	SDL_DestroySurface(config->tiles_surface);
}

SDL_Surface *IMG_ReadBMPFromArray(const unsigned char *array, unsigned int len) {
	SDL_IOStream *stream = SDL_IOFromMem(array, len);
	return SDL_LoadBMP_IO(stream, true);
} 

Config GetBuiltInConfig() {

	Config cfg;
//#define ENABLE_BUILTIN_CONFIG
#ifdef ENABLE_BUILTIN_CONFIG
	cfg.color_scheme = default_colors;
	cfg.digits_surface = IMG_ReadBMPFromArray(textures_digits_bmp, textures_digits_bmp_len);
	cfg.smileys_surface = IMG_ReadBMPFromArray(textures_smileys_bmp, textures_smileys_bmp_len);
	cfg.tiles_surface = IMG_ReadBMPFromArray(textures_tiles_bmp, textures_tiles_bmp_len);

#else 
	SDL_Log("Error: built in config not enabled. Enable it by defining the ENABLE_BUILTIN_CONFIG macro during compile-time.");
#endif
	return cfg;
}
