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

/* Creates a new sprite sheet from file */
SpriteSheet *CreateSpriteSheetFromFile(SDL_Renderer *renderer, const char* filename, int n_sprites);

bool RenderSprite(SDL_Renderer *renderer, SpriteSheet* sprite_sheet, int sprite_i, float x, float y);

void DestroySpriteSheet(SpriteSheet *sprite_sheet);