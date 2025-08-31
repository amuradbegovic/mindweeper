#pragma once

#include <SDL3/SDL.h>

#include "game_logic.h"
#include "sprite_sheet.h"

typedef enum {
    Smiley_selected, Victory, Loss, Tile_selected, Playing 
} GameState;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SpriteSheet *digits;
    SpriteSheet *smileys;
    SpriteSheet *tiles;
    GameGrid *game_grid;
    int grid_x_pos, grid_y_pos;
    int display_scale;
    int selected_tile_i, selected_tile_j;
    SDL_FRect smiley_box;
    GameState game_state;
    SDL_TimerID timer_id;
    int elapsed_time;
} GameContext;

void RenderGame(GameContext *ctx);

int GetRenderedGameWidth(GameContext *ctx);
int GetRenderedGameHeight(GameContext *ctx);

void DrawGameFrame(GameContext *ctx);

void SelectTileFromCursorPosition(GameContext *ctx, int mouse_x, int mouse_y);

void RestartGame(GameContext *ctx);

bool GameOver(GameContext *ctx);

Uint32 IncrementElapsedTime(void *ctx, SDL_TimerID timer_id, Uint32 interval);
void StartTimer(GameContext *ctx);
void StopTimer(GameContext *ctx);