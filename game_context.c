#include "game_logic.h"
#include "game_context.h"
#include "ui.h"

void RenderGame(GameContext *ctx) {
    for (int i = 0; i < ctx->game_grid->h; ++i)
        for (int j = 0; j < ctx->game_grid->w; ++j)
            RenderSprite(ctx->renderer, ctx->tiles, ctx->game_grid->tiles[i][j].displayed_type, ctx->grid_x_pos + j * ctx->tiles->w, ctx->grid_y_pos + i * ctx->tiles->h_sprites);
}

int GetRenderedGameWidth(GameContext *ctx) {
    return ctx->game_grid->w * ctx->tiles->w;
}

int GetRenderedGameHeight(GameContext *ctx) {
    return ctx->game_grid->h * ctx->tiles->h_sprites;
}

void DrawGameFrame(GameContext *ctx) {
    SDL_FRect box = {ctx->grid_x_pos, ctx->grid_y_pos, GetRenderedGameWidth(ctx), GetRenderedGameHeight(ctx)};
    DrawFrame(ctx->renderer, &box, 3, ColorDarkGray, ColorWhite);
}

void SelectTileFromCursorPosition(GameContext *ctx, int mouse_x, int mouse_y) {
    int grid_width = ctx->game_grid->w * ctx->tiles->w;
    int grid_height = ctx->game_grid->h * ctx->tiles->h_sprites;
    
    if (mouse_x < ctx->grid_x_pos || mouse_y < ctx->grid_y_pos || 
        mouse_x > ctx->grid_x_pos + grid_width || mouse_y > ctx->grid_y_pos + grid_height) {
        ctx->selected_tile_i = -1; ctx->selected_tile_j = -1; return;
    }

    ctx->selected_tile_i = (mouse_y - ctx->grid_y_pos) / ctx->tiles->h_sprites;
    ctx->selected_tile_j = (mouse_x - ctx->grid_x_pos) / ctx->tiles->w;
}

void RestartGame(GameContext *ctx) {
    ResetGameGrid(ctx->game_grid);
    ctx->game_state = Playing;
}

bool GameOver(GameContext *ctx) {
    return ctx->game_state == Victory || ctx->game_state == Loss;
}