#ifndef UI_HPP
#define UI_HPP

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <Core/Core.hpp>
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

class UI {
public:
    static bool initialize(sf::RenderWindow& window);
    static void render(sf::RenderWindow& window);
    static void shutdown();
    static sf::Vector2u windowWidth;
};

#endif // UI_HPP
