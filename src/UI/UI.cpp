#include "UI.hpp"
#include <Shapes/Shapes.hpp>
#include "Transformations/Transformations.hpp"

#include <iostream>
#include <ostream>

// Initialize the UI
bool UI::initialize(sf::RenderWindow& window)
{
    GUI::textureInit(); // Load all textures during initialization
    return ImGui::SFML::Init(window);
}

void UI::render(sf::RenderWindow& window)
{
    window.setView(Core::getUIView());
    sf::RectangleShape uiBackground({Core::getPanelWidth(), Core::getWindowSize().y});
    uiBackground.setFillColor(sf::Color::Black);
    uiBackground.setPosition({0, 0});
    window.draw(uiBackground);

    GUI::initGui(window); // Initialize the GUI

    // View begins here
    sf::Text titleText(Core::font);
    titleText.setString("Transformation Visualizer");
    titleText.setCharacterSize(20);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition({(Core::getPanelWidth() - titleText.getLocalBounds().size.x)/2, 10});
    window.draw(titleText);

    drawCartesianGraph(window);
    Renderer::render(window);
}

void UI::drawCartesianGraph(sf::RenderWindow& window)
{
    window.setView(Core::getGraphView());
    const auto graphSize = Core::getGraphSize();
    const float SCALE = Core::getScale();

    // Calculate view dimensions
    const float VIEW_WIDTH = graphSize.x / SCALE;
    const float VIEW_HEIGHT = graphSize.y / SCALE;
    constexpr float GRID_THICKNESS = 0.05f;
    constexpr float AXIS_THICKNESS = 0.05f;
    constexpr float GRID_INTERVAL = 1.f;

    const int horizontalLines = static_cast<int>((VIEW_HEIGHT * 2) / GRID_INTERVAL);
    const int verticalLines = static_cast<int>((VIEW_WIDTH * 2) / GRID_INTERVAL);

    // Draw vertical grid lines
    for (int i = -verticalLines/2; i <= verticalLines/2; i++)
    {
        if(i == 0) continue; // Skip center line as it will be drawn as axis
        float x = i * GRID_INTERVAL;

        sf::RectangleShape line({GRID_THICKNESS, VIEW_HEIGHT * 2.f});
        line.setOrigin({GRID_THICKNESS/2.f, VIEW_HEIGHT});
        line.setPosition({x, 0.f});
        line.setFillColor(sf::Color(200, 200, 200));
        window.draw(line);
    }

    // Draw horizontal grid lines
    for (int i = -horizontalLines/2; i <= horizontalLines/2; i++)
    {
        if(i == 0) continue; // Skip center line as it will be drawn as axis
        float y = i * GRID_INTERVAL;

        sf::RectangleShape line({VIEW_WIDTH * 2.f, GRID_THICKNESS});
        line.setOrigin({VIEW_WIDTH, GRID_THICKNESS/2.f});
        line.setPosition({0.f, y});
        line.setFillColor(sf::Color(200, 200, 200));
        window.draw(line);
    }

    // Draw X-axis
    sf::RectangleShape xAxis({VIEW_WIDTH * 2.f, AXIS_THICKNESS});
    xAxis.setOrigin({VIEW_WIDTH, AXIS_THICKNESS/2.f});
    xAxis.setPosition({0.f, 0.f});
    xAxis.setFillColor(sf::Color::Black);
    window.draw(xAxis);

    // Draw Y-axis
    sf::RectangleShape yAxis({AXIS_THICKNESS, VIEW_HEIGHT * 2.f});
    yAxis.setOrigin({AXIS_THICKNESS/2.f, VIEW_HEIGHT});
    yAxis.setPosition({0.f, 0.f});
    yAxis.setFillColor(sf::Color::Black);
    window.draw(yAxis);
}


void UI::shutdown()
{
    ImGui::SFML::Shutdown();
}
