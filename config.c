#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <ini.h>

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

unsigned char *read_file(const char *filename, unsigned int *size) {
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) return NULL;

	fseek(fp, 0, SEEK_END);
	*size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
	unsigned char *buf = (unsigned char *)malloc(sizeof(unsigned char) * *size);
	if (buf == NULL) { free(fp); return NULL; }
	for (unsigned int i = 0; i < *size; ++i)
		buf[i] = getc(fp);
	fclose(fp);

	return buf;
}

/* adapted from the example found in the  */
static int handler(void* cfg, const char* section, const char* name, const char* value) {
    Config* pconfig = (Config*)cfg;
	pconfig->allocated_buffers = true;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

	#define TRY_LOADING(some_bmp)  pconfig->some_bmp = read_file(value, &pconfig->some_bmp##_len); \
		if (pconfig->some_bmp == NULL) { Config_FreeBuffers(pconfig); return -1; }

	/* colors */
    if (MATCH("colors", "window_background")) {
		pconfig->color_scheme.window_background = GetColorFromString(value);
    } else if (MATCH("colors", "top_panel_background")) {
		pconfig->color_scheme.top_panel_background = GetColorFromString(value);
	} else if (MATCH("colors", "frame_top_left")) {
		pconfig->color_scheme.frame_top_left = GetColorFromString(value);
	} else if (MATCH("colors", "frame_bottom_right")) {
		pconfig->color_scheme.frame_bottom_right = GetColorFromString(value);
	} else if (MATCH("colors", "margin_top_left")) {
		pconfig->color_scheme.margin_top_left = GetColorFromString(value);
	}
	/* paths to texture files */
	else if (MATCH("textures", "digits")) {
		TRY_LOADING(digits_bmp);
	} else if (MATCH("textures", "smileys")) {
		TRY_LOADING(smileys_bmp);
	} else if (MATCH("textures", "tiles")) {
		TRY_LOADING(tiles_bmp);
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

void Config_FreeBuffers(Config *config) {
	if (config->allocated_buffers) {
		free(config->digits_bmp);
		free(config->smileys_bmp);
		free(config->tiles_bmp);
	}
	config->allocated_buffers = false;
}
