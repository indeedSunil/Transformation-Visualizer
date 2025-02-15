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
    // now for buttons to add shape or clear canvas
    ImGui::SetNextWindowPos(ImVec2(30,100),ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200,100));

    ImGui::Begin("Options ", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);
    if (ImGui::Button("Add Shape")) {
        std::cout << "Add" << std::endl;
    }
    if (ImGui::Button("Clear Canvas")) {
        std::cout << "Option 2 Clicked!" << std::endl;
    }

    ImGui::End();

    //now for options for 2d transformation
    ImGui::SetNextWindowPos(ImVec2(30,250),ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200,250));

    ImGui::Begin("Transformations", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);
    if (ImGui::Button("Scale")) {
        std::cout << "Add" << std::endl;
    }
    if (ImGui::Button("Translate")) {
        std::cout << "Option 2 Clicked!" << std::endl;
    }
    if (ImGui::Button("Rotate")) {
        std::cout << "Add" << std::endl;
    }
    if (ImGui::Button("Reflect")) {
        std::cout << "Option 2 Clicked!" << std::endl;
    }
    if (ImGui::Button("Shear")) {
        std::cout << "Add" << std::endl;
    }


    ImGui::End();

    //modal window for options
    static bool showLastWindow = true;

    // Conditionally render the last window
    if (showLastWindow)
    {
        ImGui::SetNextWindowPos(ImVec2(30, 500), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(400, 350));

        ImGui::Begin("Valuesss", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        ImGui::SetWindowFontScale(1.5f);

        // Your content here

        ImGui::End();
    }

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
