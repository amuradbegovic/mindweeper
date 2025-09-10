#pragma once

#include <SDL3/SDL.h>

#include "sprite_sheet.h"

/* Colors from the classic MS Minesweeper color scheme */
extern const SDL_Color ColorWhite;
extern const SDL_Color ColorDarkGray;
extern const SDL_Color ColorLightGray; 

typedef struct {
    SDL_Color window_background, top_panel_background,
    frame_top_left, frame_bottom_right, margin_top_left;
} ColorScheme;

typedef struct {
    SDL_FRect grid, top_panel, flags, smiley, timer;
} GameUIRects;

/* Draws a "frame" with dark grey top and left edges and white bottom and right edges around a box (as seen in the classic Minesweeper).
   Called several times using a different thickness parameter. */
void DrawFrame(SDL_Renderer *renderer, const SDL_FRect *box, int thickness, SDL_Color color_top_left, SDL_Color color_bottom_right);

/* Draws a seven-segment display showing a number (using sprites from a sheet for each digit)*/
void DisplayNumber(SDL_Renderer *renderer, SpriteSheet *digit_sprites, int x, int y, int digits, int number);

bool InsideRect(const SDL_FRect *rect, float x, float y);

void SDL_SetRenderDrawColorQuick(SDL_Renderer *renderer, SDL_Color color);
