#ifndef CONSTANTS_H
#define CONSTANTS_H

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 800;
constexpr unsigned int WORLD_WIDTH = 1600;
constexpr unsigned int WORLD_HEIGHT = 1600;
constexpr unsigned int TEXTURE_SIZE = 16;
constexpr unsigned int CELL_SIZE = TEXTURE_SIZE * 2;
constexpr unsigned int GRID_ROWS = (WORLD_HEIGHT + CELL_SIZE - 1) / CELL_SIZE;
constexpr unsigned int GRID_COLS = (WORLD_WIDTH + CELL_SIZE - 1) / CELL_SIZE;

#endif
