#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <optional>

constexpr unsigned int WINDOW_WIDTH     = 800;
constexpr unsigned int WINDOW_HEIGHT    = 800;
constexpr unsigned int TEXTURE_SIZE     = 16;
constexpr unsigned int CELL_SIZE = TEXTURE_SIZE * 2;
constexpr unsigned int GRID_ROWS = (WINDOW_WIDTH  + CELL_SIZE - 1) / CELL_SIZE; // Round up division
constexpr unsigned int GRID_COLS = (WINDOW_HEIGHT + CELL_SIZE - 1) / CELL_SIZE; // Round up division


std::mt19937 gen(std::random_device{}());
std::discrete_distribution<int> cellTypeDist({ 0.0, 0.25, 0.25, 0.5 }); 

struct cell {
    int type;
    std::optional<sf::Sprite> sprite;

    cell() : type(0), sprite(std::nullopt) {}

    void initialize(int cellType, const sf::Texture& texture, float scaleX, float scaleY, float cellScale, float posX, float posY) {
        type = cellType;
        if (type >= 1) {
            sprite.emplace(texture);
            sprite->setScale(sf::Vector2f(scaleX * cellScale, scaleY * cellScale));
            sprite->setPosition(sf::Vector2f(posX, posY));
        }
    }
};

struct TextureData {
    std::vector<sf::Texture> textures;
    float scaleX;
    float scaleY;
    float cellScale;
};

TextureData loadTextures() {
    TextureData data;
    data.textures.resize(4); // 0 = empty

    if (!data.textures[1].loadFromFile("tile1.png"))
        std::cerr << "Error loading tile1 texture" << std::endl;

    if (!data.textures[2].loadFromFile("tile2.png"))
        std::cerr << "Error loading tile2 texture" << std::endl;

    if (!data.textures[3].loadFromFile("tile3.png"))
        std::cerr << "Error loading tile3 texture" << std::endl;

    sf::Vector2u textureSize = data.textures[1].getSize();
    data.scaleX = static_cast<float>(TEXTURE_SIZE) / textureSize.x;
    data.scaleY = static_cast<float>(TEXTURE_SIZE) / textureSize.y;
    data.cellScale = static_cast<float>(CELL_SIZE) / TEXTURE_SIZE;

    return data;
}

std::vector<std::vector<cell>> generateGrid(const TextureData& texturedata) {
    std::vector<std::vector<cell>> grid(GRID_ROWS, std::vector<cell>(GRID_COLS));

    for (unsigned int row = 0; row < GRID_ROWS; ++row) {
        for (unsigned int col = 0; col < GRID_COLS; ++col) {
            int cellType = cellTypeDist(gen);

            float posX = col * CELL_SIZE;
            float posY = row * CELL_SIZE;

            if (cellType >= 1) {
                grid[row][col].initialize(cellType, texturedata.textures[cellType], texturedata.scaleX, texturedata.scaleY, texturedata.cellScale, posX, posY);
            }
        }
    }

    return grid;
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "2D Grid with Sprites");

    TextureData texturedata = loadTextures();
    std::vector<std::vector<cell>> grid = generateGrid(texturedata);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
                    grid = generateGrid(texturedata);
                }
            }
        }
        

        window.clear(sf::Color::White);

        for (unsigned int row = 0; row < GRID_ROWS; ++row) {
            for (unsigned int col = 0; col < GRID_COLS; ++col) {
                if (grid[row][col].sprite) { 
                    window.draw(*grid[row][col].sprite);
                }
            }
        }

        window.display();
    }

    return 0;
}