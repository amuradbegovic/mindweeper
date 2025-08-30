#include <stdio.h>
#include <stdlib.h>

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "sprite_sheet.h"
#include "game_context.h"
#include "ui.h"

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SDL_SetAppMetadata("Mindweeper", "1.0", "org.amuradbegovic.games");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    *appstate = (GameContext *)malloc(sizeof(GameContext));
    GameContext *ctx = *appstate;
    if (!SDL_CreateWindowAndRenderer("Mindweeper", 0, 0, 0, &ctx->window, &ctx->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    ctx->game_grid = CreateGameGrid(16, 16, 40);
    int counted_mines = 0;
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            counted_mines += (ctx->game_grid->tiles[i][j].mined);
    ctx->grid_x_pos = 12;
    ctx->grid_y_pos = 56;
    ctx->display_scale = 1;

    ctx->digits = CreateSpriteSheetFromFile(ctx->renderer, "textures/digits.bmp", 12);
    ctx->smileys = CreateSpriteSheetFromFile(ctx->renderer, "textures/smileys.bmp", 5);
    ctx->tiles = CreateSpriteSheetFromFile(ctx->renderer, "textures/tiles.bmp", 16);
    
    int window_width = ctx->grid_x_pos + GetRenderedGameWidth(ctx) + 8;
    int window_height = ctx->grid_y_pos + GetRenderedGameHeight(ctx)+ 8;
    SDL_SetRenderLogicalPresentation(ctx->renderer, window_width, window_height, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
    SDL_SetWindowSize(ctx->window, window_width*ctx->display_scale, window_height*ctx->display_scale);

    int smiley_x = ctx->grid_x_pos + ctx->game_grid->w * ctx->tiles->w / 2 - ctx->smileys->w / 2;
    int smiley_y = 17;
    ctx->smiley_box = (SDL_FRect){smiley_x, smiley_y, ctx->smileys->w-2, ctx->smileys->h_sprites-2};
    ctx->game_state = Playing;

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    GameContext *ctx = appstate;
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (!GameOver(ctx) && event->button.button == SDL_BUTTON_RIGHT) FlagTile(ctx->game_grid, ctx->selected_tile_i, ctx->selected_tile_j);
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (!GameOver(ctx) && event->button.button == SDL_BUTTON_LEFT && ctx->selected_tile_i >= 0 && ctx->selected_tile_j >= 0)
        if (RevealTile(ctx->game_grid, ctx->selected_tile_i, ctx->selected_tile_j) == 0) {
            ctx->game_state = Loss;
            UncoverAllMines(ctx->game_grid);
        } else if (IsGameGridCleared(ctx->game_grid)) ctx->game_state = Victory;
        if (ctx->game_state == Smiley_selected) RestartGame(ctx);
        break;
    case SDL_EVENT_MOUSE_MOTION:
        SelectTileFromCursorPosition(ctx, event->motion.x/ctx->display_scale, event->motion.y/ctx->display_scale);
        break;
    case SDL_EVENT_KEY_DOWN:
        if (event->key.key == 'r') RestartGame(ctx);
        break;
    default: break;
    }

    if (InsideRect(&ctx->smiley_box, event->motion.x, event->motion.y) && (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) && (event->button.button == SDL_BUTTON_LEFT))
        ctx->game_state = Smiley_selected;

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
    GameContext *ctx = appstate;
    SDL_SetRenderDrawColor(ctx->renderer, 192, 192, 192, 255);
    SDL_RenderClear(ctx->renderer);

    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);
    int win_w, win_h;
    SDL_GetWindowSize(ctx->window, &win_w, &win_h);
    SDL_FRect white_rects[2] = {
        {0, 0, win_w, 4},
        {0, 4, 3, win_h}
    };
    SDL_RenderFillRects(ctx->renderer, white_rects, 2);

    /* top panel */
    SDL_FRect top_panel_box = {11, 12, GetRenderedGameWidth(ctx) + 1, 32};
    DrawFrame(ctx->renderer, &top_panel_box, 2, ColorDarkGray, ColorWhite);

    DisplayNumber(ctx->renderer, ctx->digits, 17, 17, 3, ctx->game_grid->flags_left);
    DisplayNumber(ctx->renderer, ctx->digits, ctx->grid_x_pos + GetRenderedGameWidth(ctx) + 10 - 16 - 40, 17, 3, 0);

    /* smiley button */
    DrawFrame(ctx->renderer, &ctx->smiley_box, 1, ColorDarkGray, ColorDarkGray);
    RenderSprite(ctx->renderer, ctx->smileys, ctx->game_state, ctx->smiley_box.x, ctx->smiley_box.y);

    /* draw the fancy frame around the game grid */
    DrawGameFrame(ctx);
    /* and the grid itself */
    RenderGame(ctx);
    
    SDL_RenderPresent(ctx->renderer);
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    GameContext *ctx = appstate;
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
    DestroySpriteSheet(ctx->digits);
    DestroySpriteSheet(ctx->smileys);
    DestroySpriteSheet(ctx->tiles);
    DestroyGameGrid(ctx->game_grid);
}