#include "Core.hpp"

sf::Font Core::font;

Core::Core() : window(sf::VideoMode(sf::VideoMode::getDesktopMode()), "Transformation Visualizer") {
    window.setFramerateLimit(60);
    if (!UI::initialize(window)) {
        throw std::runtime_error("Failed to initialize ImGui-SFML");
    }
}

void Core::run() {
    while (window.isOpen()) {
        processEvents();
        if (!font.openFromFile("./assets/fonts/JetBrainsMono-Light.ttf"))
        {
            throw std::runtime_error("Failed to load font");
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear();
        UI::render(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    UI::shutdown();
}

void Core::processEvents() {
    while (const auto event = window.pollEvent()) {
        ImGui::SFML::ProcessEvent(window, *event);

        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}
