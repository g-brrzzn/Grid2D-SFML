#include "Grid.h"
#include "Constants.h"
#include <random>
#include <algorithm>

std::mt19937 gen(std::random_device{}());
std::discrete_distribution<int> cellTypeDist({ 0.0, 0.25, 0.25, 0.5 });

std::vector<std::vector<cell>> generateGrid(const TextureData& textureData) {
    std::vector<std::vector<cell>> grid(GRID_ROWS, std::vector<cell>(GRID_COLS));

    for (unsigned int row = 0; row < GRID_ROWS; ++row) {
        for (unsigned int col = 0; col < GRID_COLS; ++col) {
            int cellType = cellTypeDist(gen);
            int posX = col * CELL_SIZE;
            int posY = row * CELL_SIZE;

            if (cellType >= 1) {
                grid[row][col].initialize(cellType, textureData.textures[cellType],
                    textureData.scaleX, textureData.scaleY,
                    textureData.cellScale, posX, posY);
            }
        }
    }
    return grid;
}

void renderGrid(sf::RenderWindow& window, const sf::View& view, const std::vector<std::vector<cell>>& grid) {
    sf::FloatRect viewRect(
        { view.getCenter().x - view.getSize().x / 2.f,
          view.getCenter().y - view.getSize().y / 2.f },
        { view.getSize().x, view.getSize().y }
    );

    //culling
    int minRow = std::max<int>(0, static_cast<int>(viewRect.position.y / CELL_SIZE));
    int maxRow = std::min<int>(static_cast<int>(GRID_ROWS), static_cast<int>((viewRect.position.y + viewRect.size.y) / CELL_SIZE) + 1);
    int minCol = std::max<int>(0, static_cast<int>(viewRect.position.x / CELL_SIZE));
    int maxCol = std::min<int>(static_cast<int>(GRID_COLS), static_cast<int>((viewRect.position.x + viewRect.size.x) / CELL_SIZE) + 1);

    for (int row = minRow; row < maxRow; ++row) {
        for (int col = minCol; col < maxCol; ++col) {
            if (grid[row][col].sprite)
                window.draw(*grid[row][col].sprite);
        }
    }
}
