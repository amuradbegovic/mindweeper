#include "ui.h"

const SDL_Color ColorWhite = {0xff, 0xff, 0xff, 255};
const SDL_Color ColorDarkGray = {0x80, 0x80, 0x80, 255};
const SDL_Color ColorLightGray = {0xc0, 0xc0, 0xc0, 255};

void DrawFrame(SDL_Renderer *renderer, const SDL_FRect *box, int thickness, SDL_Color color_top_left, SDL_Color color_bottom_right) {
    int frame_x = box->x -thickness;
    int frame_y = box->y - thickness;

    for (int d = 0; d < thickness; ++d) {
        SDL_SetRenderDrawColorQuick(renderer, color_top_left);
        /* top part */
        SDL_RenderLine(renderer, frame_x, frame_y+d, frame_x + box->w + thickness + 1 - d, frame_y + d);
        /* left part */
        SDL_RenderLine(renderer, frame_x + d, frame_y, frame_x + d, frame_y + box->h + thickness + 1 - d);
        
        SDL_SetRenderDrawColorQuick(renderer, color_bottom_right);
        /* right part */
        SDL_RenderLine(renderer, frame_x + box->w + thickness + 2 - d, frame_y+1+d, frame_x + box->w + thickness + 2 - d, frame_y + box->h + thickness + 2);
        /* bottom part */
        SDL_RenderLine(renderer, frame_x + 1 + d, frame_y + box->h + thickness + 2 - d, frame_x + box->w + thickness + 2, frame_y + box->h + thickness + 2 - d);
    }
}

void DisplayNumber(SDL_Renderer *renderer, SpriteSheet *digit_sprites, int x, int y, int digits, int number) {
    bool negative = number < 0;
    if (negative) number *= -1;
    for (int i = digits-1; i >= 0; --i, number /= 10)
        RenderSprite(renderer, digit_sprites, 11 - (number % 10), x + i * digit_sprites->w, y);
    if (negative) RenderSprite(renderer, digit_sprites, 0, x, y);
}

bool InsideRect(const SDL_FRect *rect, float x, float y) {
    return (x >= rect->x) && (x <= rect->x + rect->w) && (y >= rect->y) && (y <= rect->y + rect->h);
}

void SDL_SetRenderDrawColorQuick(SDL_Renderer *renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}