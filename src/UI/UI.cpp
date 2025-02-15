#include "UI.hpp"

bool UI::initialize(sf::RenderWindow& window) {
    return ImGui::SFML::Init(window);
}

void UI::render() {
    ImGui::Begin("Hello, world!");
    ImGui::Button("Look at this pretty button");
    ImGui::End();
}

void UI::shutdown() {
    ImGui::SFML::Shutdown();
}
