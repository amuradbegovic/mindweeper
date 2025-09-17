#pragma once

#include <stdbool.h>
#include <SDL3/SDL.h>

typedef struct {
    int w, h;
    int n_sprites;
    int h_sprites;
    SDL_Texture *texture;
} SpriteSheet;

/* Creates a new sprite sheet from an SDL surface */
SpriteSheet *CreateSpriteSheetFromSurface(SDL_Renderer *renderer, SDL_Surface *surface, int n_sprites);

SpriteSheet *CreateSpriteSheetFromArray(SDL_Renderer *renderer, unsigned char *array, unsigned int size, int n_sprites);

/* Creates a new sprite sheet from file */
SpriteSheet *CreateSpriteSheetFromFile(SDL_Renderer *renderer, const char* filename, int n_sprites);

bool RenderSprite(SDL_Renderer *renderer, SpriteSheet* sprite_sheet, int sprite_i, float x, float y);

void DestroySpriteSheet(SpriteSheet *sprite_sheet);

typedef struct {
    SpriteSheet *digits;
    SpriteSheet *smileys;
    SpriteSheet *tiles;
} IconTheme;

IconTheme *CreateIconThemeFromBMPFiles(SDL_Renderer *renderer, const char *digits_file, const char *smileys_file, const char *tiles_file);
IconTheme *CreateIconThemeFromBMPArrays(SDL_Renderer *renderer, unsigned char *digits_bmp, unsigned int digits_bmp_len,
																unsigned char *smileys_bmp, unsigned int smileys_bmp_len,
																unsigned char *tiles_bmp, unsigned int tiles_bmp_len);
IconTheme *CreateIconThemeFromSurfaces(SDL_Renderer *renderer, SDL_Surface *digits_surface, SDL_Surface *smileys_surface, SDL_Surface *tiles_surface);
void DestroyIconTheme(IconTheme *icon_theme);
