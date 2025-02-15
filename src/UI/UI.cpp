#include "UI.hpp"
#include <iostream>
bool UI::initialize(sf::RenderWindow& window) {
    return ImGui::SFML::Init(window);
}

void UI::render() {


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
    if (showLastWindow) {
        ImGui::SetNextWindowPos(ImVec2(30, 500), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(400, 350));

        ImGui::Begin("Valuesss", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        ImGui::SetWindowFontScale(1.5f);

        // Your content here

        ImGui::End();
    }
}

void UI::shutdown() {
    ImGui::SFML::Shutdown();
}
