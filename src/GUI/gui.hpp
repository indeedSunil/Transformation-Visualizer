#ifndef GUI_H
#define GUI_H

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <Shapes/Shapes.hpp>
#include <UI/UI.hpp>
#include <Transformations/Transformations.hpp>

class GUI
{
private:
    static bool showTransformationWindow;
    static int fromWhereItCame;
    static bool showColorWindow;

public:
    static void initGui(sf::RenderWindow& window);
    static void textureInit();
    static std::vector<sf::Texture> textures;
};

#endif //GUI_H
