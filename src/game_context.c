#include <stdlib.h>
#include <stdio.h>

#include "game_logic.h"
#include "game_context.h"
#include "ui.h"

GameContext *CreateGameContext(Config cfg) {
    GameContext *ctx = (GameContext *)malloc(sizeof(GameContext));

    if (!SDL_CreateWindowAndRenderer("Mindweeper", 0, 0, 0, &ctx->window, &ctx->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return NULL;
    }
    ctx->color_scheme = cfg.color_scheme;

    ctx->icon_theme = CreateIconThemeFromBMPArrays(ctx->renderer, cfg.digits_bmp, cfg.digits_bmp_len, 
																  cfg.smileys_bmp, cfg.smileys_bmp_len,
																  cfg.tiles_bmp, cfg.tiles_bmp_len);

    if (ctx->icon_theme == NULL) {
        SDL_Log("Failed to load game icons. Please check if bitmap files are present in the right locations.");
        return NULL;
    }

    ctx->game_grid = NULL;
    ctx->timer_id = 0;
    ctx->elapsed_time = 0;

    return ctx;
}

void DestroyGameContext(GameContext *ctx) {
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
    DestroyIconTheme(ctx->icon_theme);
    DestroyGameGrid(ctx->game_grid);
    SDL_RemoveTimer(ctx->timer_id);
    free(ctx);
}

bool SetContextGameGrid(GameContext *ctx, int w, int h, int mine_count) {
    if (ctx->game_grid != NULL) DestroyGameGrid(ctx->game_grid);
    ctx->game_grid = CreateGameGrid(w, h, mine_count);
    if (ctx->game_grid == NULL) return false;

    ctx->game_ui_rects.grid = (SDL_FRect){12, 56, ctx->game_grid->w * ctx->icon_theme->tiles->w, 
                                          ctx->game_grid->h * ctx->icon_theme->tiles->h_sprites};

    ctx->game_ui_rects.top_panel = (SDL_FRect){11, 12, ctx->game_ui_rects.grid.w + 1, 32};

    ctx->game_ui_rects.flags = (SDL_FRect){17, 17, 3 * ctx->icon_theme->digits->w-2, ctx->icon_theme->digits->h_sprites-2};

    ctx->game_ui_rects.smiley = (SDL_FRect){ctx->game_ui_rects.grid.x + ctx->game_grid->w * ctx->icon_theme->tiles->w / 2 - ctx->icon_theme->smileys->w / 2,
        17, ctx->icon_theme->smileys->w-2, ctx->icon_theme->smileys->h_sprites-2};

    ctx->game_ui_rects.timer = (SDL_FRect){ctx->game_ui_rects.grid.x + ctx->game_ui_rects.grid.w - 46, 17,
                                           3 * ctx->icon_theme->digits->w-2, ctx->icon_theme->digits->h_sprites-2};
        
    ctx->game_state = Playing;

    int window_width = ctx->game_ui_rects.grid.x + ctx->game_ui_rects.grid.w + 8;
    int window_height = ctx->game_ui_rects.grid.y + ctx->game_ui_rects.grid.h + 8;
    SDL_SetWindowSize(ctx->window, window_width, window_height);

    return true;
}

bool SetContextGameGridWithDifficulty(GameContext *ctx, Difficulty dif) {
    switch (dif) {
        case Beginner: return SetContextGameGrid(ctx, 9, 9, 10);
        case Intermediate: return SetContextGameGrid(ctx, 16, 16, 40);
        case Expert: return SetContextGameGrid(ctx, 30, 16, 99);
        default: return false;
    }
}

void DrawGameBackground(GameContext *ctx) {
    SDL_SetRenderDrawColorQuick(ctx->renderer, ctx->color_scheme.window_background);
    SDL_RenderClear(ctx->renderer);

    SDL_SetRenderDrawColorQuick(ctx->renderer, ctx->color_scheme.margin_top_left);
    int win_w, win_h;
    SDL_GetWindowSize(ctx->window, &win_w, &win_h);
    SDL_FRect margin_rects[2] = {
        {0, 0, win_w, 4},
        {0, 4, 3, win_h}
    };
    SDL_RenderFillRects(ctx->renderer, margin_rects, 2);
}

void DrawGameGrid(GameContext *ctx) {
    for (int i = 0; i < ctx->game_grid->h; ++i)
        for (int j = 0; j < ctx->game_grid->w; ++j)
            RenderSprite(ctx->renderer, ctx->icon_theme->tiles, ctx->game_grid->tiles[i][j].displayed_type, 
                         ctx->game_ui_rects.grid.x + j * ctx->icon_theme->tiles->w, 
                         ctx->game_ui_rects.grid.y + i * ctx->icon_theme->tiles->h_sprites);
}

void DrawGameFrame(GameContext *ctx) {
    DrawFrame(ctx->renderer, &ctx->game_ui_rects.grid, 3, ctx->color_scheme.frame_top_left, ctx->color_scheme.frame_bottom_right);
}

void SelectTileFromCursorPosition(GameContext *ctx, int mouse_x, int mouse_y) {
    if (!InsideRect(&ctx->game_ui_rects.grid, mouse_x, mouse_y)) {
        ctx->selected_tile_row = -1; ctx->selected_tile_col = -1; return;
    }

    ctx->selected_tile_row = (mouse_y - ctx->game_ui_rects.grid.y) / ctx->icon_theme->tiles->h_sprites;
    ctx->selected_tile_col = (mouse_x - ctx->game_ui_rects.grid.x) / ctx->icon_theme->tiles->w;
}

void RestartGame(GameContext *ctx) {
    ResetGameGrid(ctx->game_grid);
    ctx->game_state = Playing;
    if (ctx->timer_id != 0) StopTimer(ctx);
    ctx->elapsed_time = 0;
}

bool GameOver(GameContext *ctx) {
    return ctx->game_state == Victory || ctx->game_state == Loss;
}

Uint32 IncrementElapsedTime(void *ctx, SDL_TimerID timer_id, Uint32 interval) {
    ((GameContext*)ctx)->elapsed_time++;
    return interval;
}

void StartTimer(GameContext *ctx) {
    ctx->elapsed_time = 1; /* behavior from original game*/
    ctx->timer_id = SDL_AddTimer(1000, IncrementElapsedTime, ctx);
}

void StopTimer(GameContext *ctx) {
    SDL_RemoveTimer(ctx->timer_id);
    ctx->timer_id = 0;
}
