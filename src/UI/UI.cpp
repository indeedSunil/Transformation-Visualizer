#include "UI.hpp"

#include <iostream>
#include <ostream>

sf::Vector2u UI::windowWidth;
std::vector<sf::Texture> UI::textures; // Define the static textures vector

// Load all textures
void UI::textureInit() {
    const std::vector<std::string> textureFiles = {
        "assets/images/rectangle.png",
        "assets/images/triangle.png",
        "assets/images/circle.png",
        "assets/images/ellipse.png",
        "assets/images/line.png",
        "assets/images/a.png"
    };

    textures.resize(textureFiles.size()); // Resize the vector to hold all textures

    for (size_t i = 0; i < textureFiles.size(); i++) {
        if (!textures[i].loadFromFile(textureFiles[i])) {
            std::cout << "Error loading texture: " << textureFiles[i] << std::endl;
        }
    }
}

bool UI::initialize(sf::RenderWindow& window) {
    windowWidth = window.getSize();
    std::cout << windowWidth.x << std::endl;
    textureInit(); // Load all textures during initialization
    return ImGui::SFML::Init(window);
}
void UI::render(sf::RenderWindow& window) {
    // Buttons to add shape or clear canvas
    ImGui::SetNextWindowPos(ImVec2(30, 100), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 100));

    ImGui::Begin("Options ", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);
    if (ImGui::Button("Add Shape")) {
        std::cout << "Add" << std::endl;
    }
    if (ImGui::Button("Clear Canvas")) {
        std::cout << "Option 2 Clicked!" << std::endl;
    }
    ImGui::End();

    // Options for 2D transformations
    ImGui::SetNextWindowPos(ImVec2(30, 250), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 250));

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

    // New Grid Window
    ImGui::SetNextWindowPos(ImVec2(30, 550), ImGuiCond_Always); // Positioned below "Transformations"
    ImGui::SetNextWindowSize(ImVec2(430, 250)); // Increased height to fit 2 rows

    ImGui::Begin("Grid Window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);

    // Create a 2x3 grid (2 rows, 3 columns)
    const int numRows = 2; // Number of rows
    const int numCols = 3; // Number of columns
    const float cellWidth = 120.0f; // Fixed width for each cell
    const float cellHeight = 100.0f; // Fixed height for each cell

    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            if (col > 0) ImGui::SameLine(); // Place cells in the same row

            // Get the texture for the current cell
            int textureIndex = row * numCols + col;
            const auto textureID = (ImTextureID)static_cast<uintptr_t>(textures[textureIndex].getNativeHandle());

            // Display the texture in the cell
            ImGui::ImageButton(std::to_string(textureIndex).c_str(), textureID, ImVec2(cellWidth, cellHeight), ImVec2(0, 0), ImVec2(1, 1));

            // Add hover effect
            if (ImGui::IsItemHovered()) {
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 0, 1)); // Yellow border on hover
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f); // Increase border thickness
                ImGui::PopStyleVar(); // Reset border thickness
                ImGui::PopStyleColor(); // Reset border color
            }
        }
    }

    ImGui::End();

    //2d graph


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

void UI::shutdown() {
    ImGui::SFML::Shutdown();
}