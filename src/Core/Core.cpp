#include "Core.hpp"

Core::Core() : window(sf::VideoMode({640, 480}), "ImGui + SFML = <3") {
    window.setFramerateLimit(60);
    if (!UI::initialize(window)) {
        throw std::runtime_error("Failed to initialize ImGui-SFML");
    }
}

void Core::run() {
    while (window.isOpen()) {
        processEvents();

        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear();
        UI::render();
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
