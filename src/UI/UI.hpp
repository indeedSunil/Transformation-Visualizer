#ifndef UI_HPP
#define UI_HPP

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics/RenderWindow.hpp>

class UI {
public:
    static bool initialize(sf::RenderWindow& window);
    static void render();
    static void shutdown();
};

#endif // UI_HPP
