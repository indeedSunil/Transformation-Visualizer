#include "UI.hpp"

#include <iostream>
#include <ostream>


sf::Vector2u UI::windowWidth;

bool UI::initialize(sf::RenderWindow& window)
{
    windowWidth = window.getSize();
    std::cout << windowWidth.x << std::endl;
    return ImGui::SFML::Init(window);
}

void UI::render(sf::RenderWindow& window)
{
    ImGui::Begin("Hello, world!");
    ImGui::Button("Look at this pretty button");
    ImGui::End();

    // Divider line
    float dividerLinePositionX = static_cast<float>(0.25) * static_cast<float>(windowWidth.x);
    sf::RectangleShape dividerLine({1.f, static_cast<float>(windowWidth.y)});
    dividerLine.setPosition({dividerLinePositionX, 0.f});

    // Title text container
    sf::RectangleShape titleTextContainer({460.f, 40.f});
    titleTextContainer.setPosition({(dividerLinePositionX - titleTextContainer.getLocalBounds().size.x) / 2.f, 20.f});
    titleTextContainer.setFillColor(sf::Color(255, 255, 255, 0));
    titleTextContainer.setOutlineThickness(2.f);
    titleTextContainer.setOutlineColor(sf::Color::Green);
    titleTextContainer.setOutlineColor(sf::Color(250, 150, 100));

    // Title text
    sf::Text titleText(Core::font);
    titleText.setString("Transformation Visualizer");
    titleText.setCharacterSize(24);
    titleText.setPosition({(dividerLinePositionX - titleText.getLocalBounds().size.x) / 2, 25.f});
    titleText.setFillColor(sf::Color::White);

    window.draw(dividerLine);
    window.draw(titleTextContainer);
    window.draw(titleText);
}

void UI::shutdown()
{
    ImGui::SFML::Shutdown();
}
