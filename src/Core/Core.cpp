#include "Core.hpp"

// Initialize static members
sf::Font Core::font;
sf::View Core::graphView;
sf::View Core::uiView;
sf::Vector2f Core::windowSize;
sf::Vector2f Core::graphSize;
float Core::leftPanelWidth;
const float Core::SCALE = 20.f;

Core::Core() : window(sf::VideoMode(sf::VideoMode::getDesktopMode()), "Transformation Visualizer")
{
    windowSize = static_cast<sf::Vector2f>(window.getSize());

    // Calculate dimensions
    graphSize = {0.75f * windowSize.x, windowSize.y};  // Width and height of graph area
    leftPanelWidth = 0.25f * windowSize.x;

    // Set up the graph view
    graphView.setSize({graphSize.x / SCALE, -graphSize.y / SCALE});  // Negative height to invert Y axis
    graphView.setCenter({0.f, 0.f});  // Center at origin
    graphView.setViewport(sf::FloatRect({leftPanelWidth / windowSize.x, 0.f},
                                      {graphSize.x / windowSize.x, 1.f}));

    // Set up the UI view for the left panel
    uiView.setSize({leftPanelWidth, graphSize.y});
    uiView.setCenter({leftPanelWidth / 2.f, graphSize.y / 2.f});
    uiView.setViewport(sf::FloatRect({0.f, 0.f}, {leftPanelWidth / windowSize.x, 1.f}));
    // uiView.setco

    window.setFramerateLimit(60);
    if (!UI::initialize(window))
    {
        throw std::runtime_error("Failed to initialize ImGui-SFML");
    }
}

void Core::run()
{
    while (window.isOpen())
    {
        processEvents();
        if (!font.openFromFile("./assets/fonts/JetBrainsMono-Light.ttf"))
        {
            throw std::runtime_error("Failed to load font");
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear(sf::Color(240, 240, 240));
        UI::render(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    UI::shutdown();
}

void Core::processEvents()
{
    while (const auto event = window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(window, *event);

        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        Renderer::handleMouseEvent(event.value(), window);
    }
}
