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
    SDL_RenderTexture(renderer, sprite_sheet->texture, &srcrect, &dstrect);
}

void DestroySpriteSheet(SpriteSheet *sprite_sheet) {
    if (sprite_sheet == NULL) return;
    SDL_DestroyTexture(sprite_sheet->texture);
    free(sprite_sheet);
}

/* TODO: error handling */
IconTheme *CreateIconThemeFromBMPFiles(SDL_Renderer *renderer, const char *digits_file, const char *smileys_file, const char *tiles_file) {
    IconTheme *icon_theme = (IconTheme*)malloc(sizeof(icon_theme));
    icon_theme->digits = CreateSpriteSheetFromFile(renderer, "textures/digits.bmp", 12);
    icon_theme->smileys = CreateSpriteSheetFromFile(renderer, "textures/smileys.bmp", 5);
    icon_theme->tiles = CreateSpriteSheetFromFile(renderer, "textures/tiles.bmp", 16);

    return icon_theme;
}

void DestroyIconTheme(IconTheme *icon_theme) {
    DestroySpriteSheet(icon_theme->digits);
    DestroySpriteSheet(icon_theme->smileys);
    DestroySpriteSheet(icon_theme->tiles);
    free(icon_theme);
}