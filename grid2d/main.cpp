#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 800;

constexpr unsigned int GRID_ROWS = 12;
constexpr unsigned int GRID_COLS = 12;
constexpr unsigned int CELL_SIZE = 64;

constexpr unsigned int TEXTURE_SIZE = 16;

std::mt19937 gen(std::random_device{}());
int getRandomBinary() {
    static std::bernoulli_distribution dist(0.5);
    return dist(gen);
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "2D Grid with Sprites");

    sf::RenderTexture renderTexture({ TEXTURE_SIZE, TEXTURE_SIZE });

    renderTexture.clear(sf::Color::Transparent);

    sf::RectangleShape tileShape(sf::Vector2f(TEXTURE_SIZE, TEXTURE_SIZE));
    tileShape.setFillColor(sf::Color::Red);
    tileShape.setOutlineThickness(1);
    tileShape.setOutlineColor(sf::Color::Black);
    renderTexture.draw(tileShape);

    renderTexture.display();
    const sf::Texture& texture = renderTexture.getTexture();

    std::vector<std::vector<int>> grid(GRID_ROWS, std::vector<int>(GRID_COLS));
    std::vector<std::vector<sf::Sprite>> sprites;

    for (unsigned int row = 0; row < GRID_ROWS; ++row) {
        for (unsigned int col = 0; col < GRID_COLS; ++col) {
            grid[row][col] = getRandomBinary();
        }
    }
    
    sprites.resize(GRID_ROWS);
    float scale = static_cast<float>(CELL_SIZE) / TEXTURE_SIZE;

    for (unsigned int row = 0; row < GRID_ROWS; ++row) {
        sprites[row].reserve(GRID_COLS);
		std::cout << std::endl;
        for (unsigned int col = 0; col < GRID_COLS; ++col) {
            std::cout << grid[row][col] << " ";
            sf::Sprite sprite(texture);
            sprite.setPosition(sf::Vector2f(col * CELL_SIZE, row * CELL_SIZE));
            sprite.setScale(sf::Vector2(scale, scale));
            sprites[row].push_back(sprite);
        }
    }

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color::White);

        for (unsigned int row = 0; row < GRID_ROWS; ++row) {
            for (unsigned int col = 0; col < GRID_COLS; ++col) {
                if (grid[row][col] >= 1) {
                    window.draw(sprites[row][col]);
                }
                sf::RectangleShape border(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                border.setPosition(sf::Vector2f(col * CELL_SIZE, row * CELL_SIZE));
                border.setFillColor(sf::Color::Transparent);
                border.setOutlineThickness(1);
                border.setOutlineColor(sf::Color::Black);
                window.draw(border);
            }
        }

        window.display();
    }

    return 0;
}
