#include <stdlib.h>
#include <stdio.h>

#include "sprite_sheet.h"

SpriteSheet *CreateSpriteSheetFromSurface(SDL_Renderer *renderer, SDL_Surface *surface, int n_sprites) {
    SpriteSheet *sprite_sheet = (SpriteSheet*)malloc(sizeof(SpriteSheet));
    if (sprite_sheet == NULL) return NULL;
    sprite_sheet->w = surface->w;
    sprite_sheet->h = surface->h;
    sprite_sheet->n_sprites = n_sprites;
    sprite_sheet->h_sprites = sprite_sheet->h / sprite_sheet->n_sprites;
    sprite_sheet->texture = SDL_CreateTextureFromSurface(renderer, surface);
    return sprite_sheet;
}

SDL_Surface *IMG_ReadBMPFromArray(unsigned char *array, unsigned int len) {
	SDL_IOStream *stream = SDL_IOFromMem(array, len);
	return SDL_LoadBMP_IO(stream, true);
} 

SpriteSheet *CreateSpriteSheetFromArray(SDL_Renderer *renderer, unsigned char *array, unsigned int size, int n_sprites) {
	SDL_Surface *surface = IMG_ReadBMPFromArray(array, size);
	SpriteSheet *sprite_sheet = CreateSpriteSheetFromSurface(renderer, surface, n_sprites);
	SDL_DestroySurface(surface);
	return sprite_sheet;
}

SpriteSheet *CreateSpriteSheetFromFile(SDL_Renderer *renderer, const char* filename, int n_sprites) {
    SDL_Surface *surface = SDL_LoadBMP(filename);
    if (surface == NULL) {
        SDL_Log(SDL_GetError());
        return NULL;
    }
    SpriteSheet *sprite_sheet = CreateSpriteSheetFromSurface(renderer, surface, n_sprites);
    SDL_DestroySurface(surface);
    return sprite_sheet; /* will return NULL on failure */
}

bool RenderSprite(SDL_Renderer *renderer, SpriteSheet* sprite_sheet, int sprite_i, float x, float y) {
    SDL_FRect srcrect = {0, sprite_i * sprite_sheet->h_sprites, sprite_sheet->w, sprite_sheet->h_sprites};
    SDL_FRect dstrect = {x, y, sprite_sheet->w, sprite_sheet->h_sprites};
    return SDL_RenderTexture(renderer, sprite_sheet->texture, &srcrect, &dstrect);
}

void DestroySpriteSheet(SpriteSheet *sprite_sheet) {
    if (sprite_sheet == NULL) return;
    SDL_DestroyTexture(sprite_sheet->texture);
    free(sprite_sheet);
}

/* TODO: error handling */
IconTheme *CreateIconThemeFromBMPFiles(SDL_Renderer *renderer, const char *digits_file, const char *smileys_file, const char *tiles_file) {
    IconTheme *icon_theme = (IconTheme*)malloc(sizeof(IconTheme));
    icon_theme->digits = CreateSpriteSheetFromFile(renderer, digits_file, 12);
    icon_theme->smileys = CreateSpriteSheetFromFile(renderer, smileys_file, 5);
    icon_theme->tiles = CreateSpriteSheetFromFile(renderer, tiles_file, 16);

    return icon_theme;
}

IconTheme *CreateIconThemeFromBMPArrays(SDL_Renderer *renderer, unsigned char *digits_bmp, unsigned int digits_bmp_len, 
																unsigned char *smileys_bmp, unsigned int smileys_bmp_len,
																unsigned char *tiles_bmp, unsigned int tiles_bmp_len) {
    IconTheme *icon_theme = (IconTheme*)malloc(sizeof(IconTheme));
	icon_theme->digits = CreateSpriteSheetFromArray(renderer, digits_bmp, digits_bmp_len, 12);
    icon_theme->smileys = CreateSpriteSheetFromArray(renderer, smileys_bmp, smileys_bmp_len, 5);
    icon_theme->tiles = CreateSpriteSheetFromArray(renderer, tiles_bmp, tiles_bmp_len, 16);

	if (icon_theme->digits == NULL || icon_theme->smileys == NULL || icon_theme->tiles == NULL) puts("error");
	return icon_theme;
}

IconTheme *CreateIconThemeFromSurfaces(SDL_Renderer *renderer, SDL_Surface *digits_surface, SDL_Surface *smileys_surface, SDL_Surface *tiles_surface) {
    IconTheme *icon_theme = (IconTheme*)malloc(sizeof(IconTheme));
    icon_theme->digits = CreateSpriteSheetFromSurface(renderer, digits_surface, 12);
    icon_theme->smileys = CreateSpriteSheetFromSurface(renderer, smileys_surface, 5);
    icon_theme->tiles = CreateSpriteSheetFromSurface(renderer, tiles_surface, 16);

    return icon_theme;
}

void DestroyIconTheme(IconTheme *icon_theme) {
    DestroySpriteSheet(icon_theme->digits);
    DestroySpriteSheet(icon_theme->smileys);
    DestroySpriteSheet(icon_theme->tiles);
    free(icon_theme);
}
