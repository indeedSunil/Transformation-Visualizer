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
#include <GUI/gui.hpp>


class UI {
private:
    static constexpr int AXIS_THICKNESS = 2;
    static constexpr int GRID_LINE_THICKNESS = 1;

    static void drawCartesianGraph(sf::RenderWindow& window);

public:
    static constexpr int GRID_SIZE = 20; // Size of each grid cell in pixels
    static bool initialize(sf::RenderWindow& window);
    static float scale_factor;
    static void render(sf::RenderWindow& window);
    static void shutdown();

    static sf::Texture texture; // Static texture member

};

#endif // UI_HPP