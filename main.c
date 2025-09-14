#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "sprite_sheet.h"
#include "game_context.h"
#include "ui.h"
#include "config.h"


/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SDL_SetAppMetadata("Mindweeper", "1.0", "org.amuradbegovic.games");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
	
	Config cfg;
	if (!LoadConfig(&cfg, "mindweeper.ini")) {
		SDL_Log("%s: Error loading config file. ", argv[0]);
	#ifdef ENABLE_BUILTIN_CONFIG
		SDL_Log("%s: Falling back to built in configuration", argv[0]);
		cfg = GetBuiltInConfig();
	#else 
		return SDL_APP_FAILURE;
	#endif 
	}
	
	GameContext *ctx = CreateGameContext(cfg);

    if (ctx == NULL) {
        SDL_Log("Failed to create game context.");
        return SDL_APP_FAILURE;
    }

    SetContextGameGridWithDifficulty(ctx, Expert);

	Config_FreeBuffers(&cfg);

    *appstate = ctx;
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    GameContext *ctx = appstate;
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (!GameOver(ctx) && event->button.button == SDL_BUTTON_RIGHT) 
            FlagTile(ctx->game_grid, ctx->selected_tile_row, ctx->selected_tile_col);
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (!GameOver(ctx) && event->button.button == SDL_BUTTON_LEFT && ctx->selected_tile_row >= 0 && ctx->selected_tile_col >= 0)
        if (RevealTile(ctx->game_grid, ctx->selected_tile_row, ctx->selected_tile_col) == 0) {
            ctx->game_state = Loss;
            UncoverAllMines(ctx->game_grid);
        } else if (IsGameGridCleared(ctx->game_grid)) ctx->game_state = Victory;
        else if (ctx->timer_id == 0) StartTimer(ctx);
        if (ctx->game_state == Smiley_selected) RestartGame(ctx);
        break;
    case SDL_EVENT_MOUSE_MOTION:
        SelectTileFromCursorPosition(ctx, event->motion.x, event->motion.y);
        break;
    case SDL_EVENT_KEY_DOWN:
        if (event->key.key == 'r') RestartGame(ctx);
        break;
    default: break;
    }

    if (InsideRect(&ctx->game_ui_rects.smiley, event->motion.x, event->motion.y) && 
        (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) && 
        (event->button.button == SDL_BUTTON_LEFT))
        ctx->game_state = Smiley_selected;

    if (GameOver(ctx)) StopTimer(ctx);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
    GameContext *ctx = appstate;

    /* color game background and the top-left margin as seen in the original game */
    DrawGameBackground(ctx);

    /* top panel */
    DrawFrame(ctx->renderer, &ctx->game_ui_rects.top_panel, 2, ctx->color_scheme.frame_top_left, ctx->color_scheme.frame_bottom_right);

    /* the number of remaining flags to use */
    DrawFrame(ctx->renderer, &ctx->game_ui_rects.flags, 1, ctx->color_scheme.frame_top_left, ctx->color_scheme.frame_bottom_right);
    DisplayNumber(ctx->renderer, ctx->icon_theme->digits, ctx->game_ui_rects.flags.x, ctx->game_ui_rects.flags.y, 3, ctx->game_grid->flags_left);

    /* timer showing time since the current game started */
    DrawFrame(ctx->renderer, &ctx->game_ui_rects.timer, 1, ctx->color_scheme.frame_top_left, ctx->color_scheme.frame_bottom_right);
    DisplayNumber(ctx->renderer, ctx->icon_theme->digits, ctx->game_ui_rects.timer.x, ctx->game_ui_rects.timer.y, 3, ctx->elapsed_time);

    /* smiley button */
    DrawFrame(ctx->renderer, &ctx->game_ui_rects.smiley, 1, ctx->color_scheme.frame_top_left, ctx->color_scheme.frame_top_left);
    RenderSprite(ctx->renderer, ctx->icon_theme->smileys, ctx->game_state, ctx->game_ui_rects.smiley.x, ctx->game_ui_rects.smiley.y);

    /* draw the fancy frame around the game grid */
    DrawGameFrame(ctx);
    /* and the grid itself */
    DrawGameGrid(ctx);
    
    SDL_RenderPresent(ctx->renderer);
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    GameContext *ctx = appstate;
    DestroyGameContext(ctx);
}
