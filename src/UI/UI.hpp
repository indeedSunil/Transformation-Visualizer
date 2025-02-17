#ifndef UI_HPP
#define UI_HPP

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <Core/Core.hpp>
#include <Renderer/Renderer.hpp>
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include <SFML/Graphics/Texture.hpp>


class UI {
private:
    static constexpr int AXIS_THICKNESS = 2;
    static constexpr int GRID_LINE_THICKNESS = 1;

    static void drawCartesianGraph(sf::RenderWindow& window);
    static sf::Vector2f origin; // Add this line

    //for transformation windows
    static bool showTransformationWindow;
    static int fromWhereItCame;
    static bool showColorWindow;

public:
    static constexpr int GRID_SIZE = 20; // Size of each grid cell in pixels
    static bool initialize(sf::RenderWindow& window);
    static float scale_factor;
    static void render(sf::RenderWindow& window);
    static std::vector<sf::Texture> textures;
    static void textureInit(); // Load the texture
    static void shutdown();
    static sf::Vector2u windowSize;

    static sf::Texture texture; // Static texture member
    static float dividerLinePositionX;

    static sf::Vector2f windowToGraph(sf::Vector2f windowCoord);
    static sf::Vector2f graphToWindow(sf::Vector2f graphCoord);

    static const sf::Vector2f& getOrigin() { return origin; } // Add getter
};

#endif // UI_HPP