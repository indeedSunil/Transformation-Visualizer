#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

int main()
{
    sf::VideoMode const desktopMode = sf::VideoMode::getDesktopMode();

    sf::RenderWindow window(desktopMode, "Transformation Visualizer");
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window))
    {
        return -1;
    }

    // Get window width
    sf::Vector2u const windowWidth = window.getSize();

    // Load font
    sf::Font const font("./assets/fonts/JetBrainsMono-Light.ttf");

    // Divider Line
    constexpr float dividerLinePositionX = 600.f;
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
    sf::Text titleText(font);
    titleText.setString("Transformation Visualizer");
    titleText.setCharacterSize(24);
    titleText.setPosition({(dividerLinePositionX - titleText.getLocalBounds().size.x) / 2, 25.f});
    titleText.setFillColor(sf::Color::White);

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::ShowDemoWindow();

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        window.clear();
        window.draw(dividerLine);
        window.draw(titleTextContainer);
        window.draw(titleText);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
