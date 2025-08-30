#pragma once 

#include <stdbool.h>

typedef enum  {Hidden, Flagged, Unsure, MineStepped, MineWrong, MineUncovered, UnsureClicked, Eight, Seven, Six, Five, Four, Three, Two, One, Zero} 
DisplayedTileType;

typedef struct {
    bool mined;
    bool hidden;
    int neighboring_mines;
    DisplayedTileType displayed_type;
} Tile;

typedef struct {
    int w, h, mine_count, flags_left;
    Tile **tiles;
} GameGrid;

GameGrid *CreateGameGrid(int w, int h, int mine_count);

void ResetGameGrid(GameGrid *game_grid);
void DestroyGameGrid(GameGrid *game_grid);

int CountNeighboringMines(GameGrid *game_grid, int row, int col);
bool CheckCoordinates(GameGrid *game_grid, int row, int col);

int RevealTile(GameGrid *game_grid, int row, int col);
int FlagTile(GameGrid *game_grid, int row, int col);

void UncoverAllMines(GameGrid *game_grid);

bool IsGameGridCleared(GameGrid *game_grid);