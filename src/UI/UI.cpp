#include "UI.hpp"

#include <iostream>
#include <ostream>

sf::Vector2u UI::windowSize; // Calculate the size of the window
std::vector<sf::Texture> UI::textures; // Define the static textures vector
float UI::dividerLinePositionX; // Divider line between options and graph
sf::Vector2f UI::origin; // Define the static member


// Load all textures
void UI::textureInit()
{
    const std::vector<std::string> textureFiles = {
        "assets/images/rectangle.png",
        "assets/images/triangle.png",
        "assets/images/circle.png",
        "assets/images/ellipse.png",
        "assets/images/line.png",
        "assets/images/actor.jpg"
    };

    textures.resize(textureFiles.size()); // Resize the vector to hold all textures

    for (size_t i = 0; i < textureFiles.size(); i++)
    {
        if (!textures[i].loadFromFile(textureFiles[i]))
        {
            std::cout << "Error loading texture: " << textureFiles[i] << std::endl;
        }
    }
}

bool UI::initialize(sf::RenderWindow& window)
{
    windowSize = window.getSize();
    dividerLinePositionX = static_cast<float>(0.25) * static_cast<float>(windowSize.x);
    origin = sf::Vector2f(
        dividerLinePositionX + (windowSize.x - dividerLinePositionX) / 2,
        windowSize.y / 2
    );
    textureInit(); // Load all textures during initialization
    return ImGui::SFML::Init(window);
}

void UI::render(sf::RenderWindow& window)
{
    // Buttons to add shape or clear canvas
    ImGui::SetNextWindowPos(ImVec2(30, 100), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 100));

    ImGui::Begin("Options ", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);
    if (ImGui::Button("Hide Grid Numbers"))
    {
        std::cout << "Add" << std::endl;
    }
    if (ImGui::Button("Clear Canvas"))
    {
        Math::clearShape();
        std::cout << "Clear Canvas clicked!" << std::endl;
    }
    ImGui::End();

    // Options for 2D transformations
    ImGui::SetNextWindowPos(ImVec2(30, 250), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 250));

    ImGui::Begin("Transformations", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);
    if (ImGui::Button("Scale"))
    {
        Math::isSelected = true;
        std::cout << "Scale clicked" << std::endl;
        Math::transformShape("Scale");
    }
    if (ImGui::Button("Translate"))
    {
        std::cout << "Translate clicked" << std::endl;
        Math::transformShape("Translate");
    }
    if (ImGui::Button("Rotate"))
    {
        std::cout << "Rotate clicked" << std::endl;
        Math::transformShape("Rotate");
    }
    if (ImGui::Button("Reflect"))
    {
        std::cout << "Reflect clicked" << std::endl;
        Math::transformShape("Reflect");
    }
    if (ImGui::Button("Shear"))
    {
        std::cout << "Shear clicked" << std::endl;
        Math::transformShape("Shear");
    }
    ImGui::End();

    // New Grid Window
    ImGui::SetNextWindowPos(ImVec2(30, 550), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(430, 250));

    ImGui::Begin("Grid Window", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);

    // Create a 2x3 grid (2 rows, 3 columns)
    constexpr int numRows = 2;
    const int numCols = 3;
    constexpr float cellWidth = 120.0f;
    constexpr float cellHeight = 100.0f;
    static int selectedTextureIndex = -1; // Add this to track selection

    for (int row = 0; row < numRows; row++)
    {
        for (int col = 0; col < numCols; col++)
        {
            if (col > 0) ImGui::SameLine();

            int textureIndex = row * numCols + col;
            const auto textureID = (ImTextureID)static_cast<uintptr_t>(textures[textureIndex].getNativeHandle());

            // Add selected state styling
            bool isSelected = (selectedTextureIndex == textureIndex);
            if (isSelected)
            {
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 0, 1));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
            }

            if (ImGui::ImageButton(std::to_string(textureIndex).c_str(), textureID,
                                   ImVec2(cellWidth, cellHeight), ImVec2(0, 0), ImVec2(1, 1)))
            {
                selectedTextureIndex = textureIndex; // Update selected index
                switch (textureIndex)
                {
                case 0:
                    {
                        Math::hasShape = false;
                        Math::setCurrentShape(Math::ShapeType::Rectangle);
                        break;
                    }
                case 1:
                    Math::setCurrentShape(Math::ShapeType::Triangle);
                    break;
                case 2:
                    Math::setCurrentShape(Math::ShapeType::Circle);
                    break;
                case 3:
                    Math::setCurrentShape(Math::ShapeType::Ellipse);
                    break;
                case 4:
                    Math::setCurrentShape(Math::ShapeType::Line);
                    break;
                case 5:
                    Math::setCurrentShape(Math::ShapeType::LetterA);
                    break;
                default:
                    break;
                }
            }

            // Add hover effect (only if not selected)
            if (ImGui::IsItemHovered() && !isSelected)
            {
                ImGui::GetWindowDrawList()->AddRect(
                    ImGui::GetItemRectMin(),
                    ImGui::GetItemRectMax(),
                    IM_COL32(255, 255, 0, 255), // Yellow border
                    0.0f, // rounding
                    0, // flags
                    2.0f // thickness
                );
            }

            // Pop selected state styling
            if (isSelected)
            {
                ImGui::PopStyleVar();
                ImGui::PopStyleColor();
            }
        }
    }

    ImGui::End();


    // Divider line
    sf::RectangleShape dividerLine({1.f, static_cast<float>(windowSize.y)});
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
    drawCartesianGraph(window);
    Math::render(window);
}

void UI::drawCartesianGraph(sf::RenderWindow& window)
{
    // Calculate the graph area (right side of the divider)
    const float graphWidth = static_cast<float>(windowSize.x) - dividerLinePositionX;
    const auto graphHeight = static_cast<float>(windowSize.y);

    // Draw grid lines
    const int numLinesX = static_cast<int>(graphWidth) / GRID_SIZE;
    const int numLinesY = static_cast<int>(graphHeight) / GRID_SIZE;

    // Vertical grid lines
    for (int i = -numLinesX / 2; i <= numLinesX / 2; i++)
    {
        sf::RectangleShape line(sf::Vector2f(GRID_LINE_THICKNESS, graphHeight));
        line.setPosition({origin.x + static_cast<float>(i * GRID_SIZE), 0});
        line.setFillColor(sf::Color(50, 50, 50));
        window.draw(line);
    }

    // Horizontal grid lines
    for (int i = -numLinesY / 2; i <= numLinesY / 2; i++)
    {
        sf::RectangleShape line(sf::Vector2f(graphWidth, GRID_LINE_THICKNESS));
        line.setPosition({dividerLinePositionX, (origin.y + static_cast<float>(i) * GRID_SIZE)});
        line.setFillColor(sf::Color(50, 50, 50));
        window.draw(line);
    }

    // Draw axes
    // X-axis
    sf::RectangleShape xAxis(sf::Vector2f(graphWidth, AXIS_THICKNESS));
    xAxis.setPosition({dividerLinePositionX, origin.y});
    xAxis.setFillColor(sf::Color(255, 255, 255, 30));
    window.draw(xAxis);

    // Y-axis
    sf::RectangleShape yAxis(sf::Vector2f(AXIS_THICKNESS, graphHeight));
    yAxis.setPosition({origin.x, 0});
    yAxis.setFillColor(sf::Color(255, 255, 255, 30));
    window.draw(yAxis);
}

// Utility functions for coordinate conversion
sf::Vector2f UI::windowToGraph(const sf::Vector2f windowCoord)
{
    return sf::Vector2f(
        (windowCoord.x - origin.x) / GRID_SIZE,
        (origin.y - windowCoord.y) / GRID_SIZE
    );
}

sf::Vector2f UI::graphToWindow(sf::Vector2f graphCoord)
{
    return sf::Vector2f(
        origin.x + graphCoord.x * GRID_SIZE,
        origin.y - graphCoord.y * GRID_SIZE
    );
}

void UI::shutdown()
{
    ImGui::SFML::Shutdown();
}
