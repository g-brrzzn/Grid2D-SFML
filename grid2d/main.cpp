#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <optional>

constexpr unsigned int WINDOW_WIDTH     = 800;
constexpr unsigned int WINDOW_HEIGHT    = 800;
constexpr unsigned int WORLD_WIDTH      = 1600; 
constexpr unsigned int WORLD_HEIGHT     = 1600; 
constexpr unsigned int TEXTURE_SIZE     = 16;
constexpr unsigned int CELL_SIZE = TEXTURE_SIZE  * 2;
constexpr unsigned int GRID_ROWS = (WORLD_HEIGHT + CELL_SIZE - 1) / CELL_SIZE; // Round up division
constexpr unsigned int GRID_COLS = (WORLD_WIDTH  + CELL_SIZE - 1) / CELL_SIZE; // Round up division

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
            int posX = col * CELL_SIZE;
            int posY = row * CELL_SIZE;

            if (cellType >= 1) {
                grid[row][col].initialize(cellType, texturedata.textures[cellType], texturedata.scaleX, texturedata.scaleY, texturedata.cellScale, posX, posY);
            }
        }
    }
    return grid;
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "2D Grid with Camera");
    window.setFramerateLimit(60);

    TextureData texturedata = loadTextures();
    std::vector<std::vector<cell>> grid = generateGrid(texturedata);
    sf::View view(sf::FloatRect({ 0, 0 }, { WINDOW_WIDTH, WINDOW_HEIGHT }));

    const float cameraSpeed = 200.f;
	float fps = 0.0f;
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        fps = 1.0f / dt;
        
        window.setTitle("2D Grid with Camera - FPS: " + std::to_string(static_cast<int>(fps)) + "  View center: " + std::to_string(static_cast<int>(view.getCenter().x)) + ", " + std::to_string(static_cast<int>(view.getCenter().y)));

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) 
                window.close();
            
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::R) 
                    grid = generateGrid(texturedata);  
            } 
        }
		
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            view.move(sf::Vector2f(0, -cameraSpeed * dt));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            view.move(sf::Vector2f(0, cameraSpeed * dt));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            view.move(sf::Vector2f(-cameraSpeed * dt, 0));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            view.move(sf::Vector2f(cameraSpeed * dt, 0));


        window.clear(sf::Color::White);
        window.setView(view);

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
