#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "game_logic.h"

/* TODO: better error handling */
GameGrid *CreateGameGrid(int w, int h, int mine_count) {
    if (w < 2 || h < 2) return NULL;
    GameGrid *game_grid = (GameGrid *)malloc(sizeof(GameGrid));
    if (game_grid == NULL) return NULL;
    game_grid->w = w;
    game_grid->h = h;
    game_grid->mine_count = mine_count;
    game_grid->tiles = (Tile **)malloc(sizeof(Tile*) * h);
    for (int i = 0; i < h; ++i)
        game_grid->tiles[i] = (Tile *)malloc(sizeof(Tile) * w);

    ResetGameGrid(game_grid);
    return game_grid;    
}

GameGrid *CreateGameGridWithDifficulty(Difficulty dif) {
    switch (dif) {
        case Beginner: return CreateGameGrid(9, 9, 10);
        case Intermediate: return CreateGameGrid(16, 16, 40);
        case Expert: return CreateGameGrid(30, 16, 99);
        default: return NULL;
    }
}

int CountNeighboringMines(GameGrid *game_grid, int row, int col) {
    int count = 0;
    for (int i = row - 1; i <= row + 1; ++i)
        for (int j = col - 1; j <= col + 1; ++j)
            count += (i >= 0 && j >= 0 && i < game_grid->h && j < game_grid->w) && game_grid->tiles[i][j].mined;
    return count;
}

void ResetGameGrid(GameGrid *game_grid) {
    /* clear the grid */
    for (int i = 0; i < game_grid->h; ++i)
        for (int j = 0; j < game_grid->w; ++j)
            game_grid->tiles[i][j] = (Tile){false, true, false, 0};

    /* pseudo-randomly plant mines */
    srand(time(0));
    for (int planted_mines = 0; planted_mines < game_grid->mine_count; ) {
        int row = rand() % game_grid->h;
        int col = rand() % game_grid->w;
        if (!game_grid->tiles[row][col].mined) {
            game_grid->tiles[row][col].mined = true;
            ++planted_mines;
        }
    }

    /* count neighboring mines for clear */ 
    for (int i = 0; i < game_grid->h; ++i) 
        for (int j = 0; j < game_grid->w; ++j)
            game_grid->tiles[i][j].neighboring_mines = CountNeighboringMines(game_grid, i, j);

    /* set the number of remaining flags to the number of mines*/
    game_grid->flags_left = game_grid->mine_count;
}

void DestroyGameGrid(GameGrid *game_grid) {
    if (game_grid == NULL) return;
    for (int i = 0; i < game_grid->h; ++i)
        free(game_grid->tiles[i]);
    free(game_grid->tiles);
    free(game_grid);
}

bool CheckCoordinates(GameGrid *game_grid, int row, int col) {
    return row >= 0 && col >= 0 && row < game_grid->h && col < game_grid->w;
}


int RevealTile(GameGrid *game_grid, int row, int col) {
    if (!CheckCoordinates(game_grid, row, col) || !game_grid->tiles[row][col].hidden || game_grid->tiles[row][col].displayed_type == Flagged) return -1;
   
    Tile *tilep = &game_grid->tiles[row][col];
    tilep->hidden = false;
    tilep->displayed_type = (tilep->mined ? MineStepped : Zero - tilep->neighboring_mines);

    if (tilep->mined) return 0;

    if (tilep->neighboring_mines == 0) {
        RevealTile(game_grid, row-1, col);
        RevealTile(game_grid, row-1, col-1);
        RevealTile(game_grid, row+1, col);
        RevealTile(game_grid, row, col-1);
        RevealTile(game_grid, row, col+1);
        RevealTile(game_grid, row+1, col+1);
        RevealTile(game_grid, row+1, col-1);
        RevealTile(game_grid, row-1, col+1);
    }
    return 1;
}

int FlagTile(GameGrid *game_grid, int row, int col) {
    if (!CheckCoordinates(game_grid, row, col) || !game_grid->tiles[row][col].hidden) return -1;
    if (game_grid->tiles[row][col].displayed_type == Hidden) game_grid->flags_left--;
    else if (game_grid->tiles[row][col].displayed_type == Flagged) game_grid->flags_left++;

    game_grid->tiles[row][col].displayed_type++;
    game_grid->tiles[row][col].displayed_type %= 3;
    return 1;
}

void UncoverAllMines(GameGrid *game_grid) {
    for (int i = 0; i < game_grid->h; ++i)
        for (int j = 0; j < game_grid->w; ++j)
            if (game_grid->tiles[i][j].mined && game_grid->tiles[i][j].hidden && !game_grid->tiles[i][j].displayed_type == Flagged) {
                game_grid->tiles[i][j].hidden = false;
                game_grid->tiles[i][j].displayed_type = MineUncovered;
            } else if (!game_grid->tiles[i][j].mined && game_grid->tiles[i][j].displayed_type == Flagged) {
                game_grid->tiles[i][j].hidden = false;
                game_grid->tiles[i][j].displayed_type = MineWrong;
            }
}

bool IsGameGridCleared(GameGrid *game_grid) {
    for (int i = 0; i < game_grid->h; ++i)
        for (int j = 0; j < game_grid->w; ++j)
            if (game_grid->tiles[i][j].hidden && !game_grid->tiles[i][j].mined) return false;
    return true;
}