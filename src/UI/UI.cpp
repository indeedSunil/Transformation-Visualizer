#include "UI.hpp"

#include <iostream>
#include <ostream>

sf::Vector2u UI::windowSize; // Calculate the size of the window
std::vector<sf::Texture> UI::textures; // Define the static textures vector
float UI::dividerLinePositionX; // Divider line between options and graph
//for scaling
bool UI::showTransformationWindow = false;
int UI::fromWhereItCame = 0;

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
    std::cout << windowSize.x << std::endl;
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
        UI::showTransformationWindow = true;  // Set to true when button is clicked
        UI::fromWhereItCame = 1;
        std::cout << "Scale window opened" << std::endl;
    }
    if (ImGui::Button("Translate"))
    {
        UI::showTransformationWindow = true;  // Set to true when button is clicked
        UI::fromWhereItCame = 2;
        std::cout << "Option 2 Clicked!" << std::endl;
    }
    if (ImGui::Button("Rotate"))
    {
        UI::showTransformationWindow = true;  // Set to true when button is clicked
        UI::fromWhereItCame = 3;
        std::cout << "Add" << std::endl;
    }
    if (ImGui::Button("Reflect"))
    {
        UI::showTransformationWindow = true;  // Set to true when button is clicked
        UI::fromWhereItCame = 4;
        std::cout << "Option 2 Clicked!" << std::endl;
    }
    if (ImGui::Button("Shear"))
    {
        UI::showTransformationWindow = true;  // Set to true when button is clicked
        UI::fromWhereItCame = 5;
        std::cout << "Add" << std::endl;
    }
    ImGui::End();
//transformations window-scaling window
    if (UI::showTransformationWindow && UI::fromWhereItCame == 1)
    {
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Always);  // Made slightly taller to accommodate new elements

        if (ImGui::Begin("Scale Transform", &showTransformationWindow,
                         ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
        {
            ImGui::SetWindowFontScale(1.5f);

            // Static variables to store the scale values
            static float scaleX = 1.0f;
            static float scaleY = 1.0f;

            // X Scale
            ImGui::Text("Scale Factor X:");
            // Slider for X
            if (ImGui::SliderFloat("##SliderX", &scaleX, 0.1f, 5.0f))
            {
                // The slider was changed
                std::cout << "X Scale changed via slider: " << scaleX << std::endl;
            }
            // Input field for X
            ImGui::SameLine();
            if (ImGui::InputFloat("##InputX", &scaleX, 0.1f, 1.0f, "%.1f"))
            {
                // Clamp the input value between 0.1 and 5.0
                scaleX = std::max(0.1f, std::min(scaleX, 5.0f));
                std::cout << "X Scale changed via input: " << scaleX << std::endl;
            }

            // Y Scale
            ImGui::Text("Scale Factor Y:");
            // Slider for Y
            if (ImGui::SliderFloat("##SliderY", &scaleY, 0.1f, 5.0f))
            {
                // The slider was changed
                std::cout << "Y Scale changed via slider: " << scaleY << std::endl;
            }
            // Input field for Y
            ImGui::SameLine();
            if (ImGui::InputFloat("##InputY", &scaleY, 0.1f, 1.0f, "%.1f"))
            {
                // Clamp the input value between 0.1 and 5.0
                scaleY = std::max(0.1f, std::min(scaleY, 5.0f));
                std::cout << "Y Scale changed via input: " << scaleY << std::endl;
            }

            if (ImGui::Button("Apply Scale")) {
                // Add your scale transformation logic here
                std::cout << "Applying scale: " << scaleX << ", " << scaleY << std::endl;
            }
        }
        ImGui::End();
    }
    //transformations window-translation window
    if (UI::showTransformationWindow && UI::fromWhereItCame == 2)
    {
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Always);  // Made slightly taller to accommodate new elements

        if (ImGui::Begin("Translate", &showTransformationWindow,
                         ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
        {
            ImGui::SetWindowFontScale(1.5f);

            // Static variables to store the translation values
            static float translateX = 0.0f;
            static float translateY = 0.0f;

            // X Translation
            ImGui::Text("Translate X:");
            // Slider for X
            if (ImGui::SliderFloat("##SliderX", &translateX, -100.0f, 100.0f))
            {
                // The slider was changed
                std::cout << "X Translation changed via slider: " << translateX << std::endl;
            }
            // Input field for X
            ImGui::SameLine();
            if (ImGui::InputFloat("##InputX", &translateX, 1.0f, 10.0f, "%.1f"))
            {
                // Clamp the input value between -100 and 100
                translateX = std::max(-100.0f, std::min(translateX, 100.0f));
                std::cout << "X Translation changed via input: " << translateX << std::endl;
            }

            // Y Translation
            ImGui::Text("Translate Y:");
            // Slider for Y
            if (ImGui::SliderFloat("##SliderY", &translateY, -100.0f, 100.0f))
            {
                // The slider was changed
                std::cout << "Y Translation changed via slider: " << translateY << std::endl;
            }
            // Input field for Y
            ImGui::SameLine();
            if (ImGui::InputFloat("##InputY", &translateY, 1.0f, 10.0f, "%.1f"))
            {
                // Clamp the input value between -100 and 100
                translateY = std::max(-100.0f, std::min(translateY, 100.0f));
                std::cout << "Y Translation changed via input: " << translateY << std::endl;
            }
            if (ImGui::Button("Apply Translate")) {
                // Add your scale transformation logic here

            }
        }
        ImGui::End();
    }
    //transformations window-rotation window
    if (UI::showTransformationWindow && UI::fromWhereItCame == 3)
    {
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Always);  // Made slightly taller to accommodate new elements

        if (ImGui::Begin("Rotate", &showTransformationWindow,
                         ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
        {
            ImGui::SetWindowFontScale(1.5f);

            // Static variables to store the rotation values
            static float rotateAngle = 0.0f;

            // Rotation Angle
            ImGui::Text("Rotation Angle:");
            // Slider for rotation
            if (ImGui::SliderFloat("##SliderAngle", &rotateAngle, -180.0f, 180.0f))
            {
                // The slider was changed
                std::cout << "Rotation Angle changed via slider: " << rotateAngle << std::endl;
            }
            // Input field for rotation
            ImGui::SameLine();
            if (ImGui::InputFloat("##InputAngle", &rotateAngle, 1.0f, 10.0f, "%.1f"))
            {
                // Clamp the input value between -180 and 180
                rotateAngle = std::max(-180.0f, std::min(rotateAngle, 180.0f));
                std::cout << "Rotation Angle changed via input: " << rotateAngle << std::endl;
            }

            if (ImGui::Button("Apply Rotate")) {
                // Add your scale transformation logic here
                std::cout << "Applying rotation: " << rotateAngle << std::endl;
            }
        }
        ImGui::End();
    }
    //transformations window-reflection window
    if (UI::showTransformationWindow && UI::fromWhereItCame == 4)
    {
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Always);  // Made slightly taller to accommodate new elements

        if (ImGui::Begin("Reflect", &showTransformationWindow,
                         ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
        {
            ImGui::SetWindowFontScale(1.5f);

            // Static variables to store the reflection values
            static bool reflectX = false;
            static bool reflectY = false;

            // X Reflection
            ImGui::Text("Reflect X:");
            // Checkbox for X
            if (ImGui::Checkbox("##CheckboxX", &reflectX))
            {
                // The checkbox was changed
                std::cout << "X Reflection changed via checkbox: " << reflectX << std::endl;
            }

            // Y Reflection
            ImGui::Text("Reflect Y:");
            // Checkbox for Y
            if (ImGui::Checkbox("##CheckboxY", &reflectY))
            {
                // The checkbox was changed
                std::cout << "Y Reflection changed via checkbox: " << reflectY << std::endl;
            }

            if (ImGui::Button("Apply Reflect")) {
                // Add your scale transformation logic here
                std::cout << "Applying reflection: " << reflectX << ", " << reflectY << std::endl;
            }
        }
        ImGui::End();
    }
    //transformations window-shear window
    if (UI::showTransformationWindow && UI::fromWhereItCame == 5)
    {
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Always);  // Made slightly taller to accommodate new elements

        if (ImGui::Begin("Shear", &showTransformationWindow,
                         ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
        {
            ImGui::SetWindowFontScale(1.5f);

            // Static variables to store the shear values
            static float shearX = 0.0f;
            static float shearY = 0.0f;

            // X Shear
            ImGui::Text("Shear X:");
            // Slider for X
            if (ImGui::SliderFloat("##SliderX", &shearX, -5.0f, 5.0f))
            {
                // The slider was changed
                std::cout << "X Shear changed via slider: " << shearX << std::endl;
            }
            // Input field for X
            ImGui::SameLine();
            if (ImGui::InputFloat("##InputX", &shearX, 0.1f, 1.0f, "%.1f"))
            {
                // Clamp the input value between -5.0 and 5.0
                shearX = std::max(-5.0f, std::min(shearX, 5.0f));
                std::cout << "X Shear changed via input: " << shearX << std::endl;
            }

            // Y Shear
            ImGui::Text("Shear Y:");
            // Slider for Y
            if (ImGui::SliderFloat("##SliderY", &shearY, -5.0f, 5.0f))
            {
                // The slider was changed
                std::cout << "Y Shear changed via slider: " << shearY << std::endl;
            }
            // Input field for Y
            ImGui::SameLine();
            if (ImGui::InputFloat("##InputY", &shearY, 0.1f, 1.0f, "%.1f"))
            {
                // Clamp the input value between -5.0 and 5.0
                shearY = std::max(-5.0f, std::min(shearY, 5.0f));
                std::cout << "Y Shear changed via input: " << shearY << std::endl;
            }
            if (ImGui::Button("Apply Shear")) {
                // Add your scale transformation logic here

            }
        }
        ImGui::End();
    }






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
    dividerLinePositionX = static_cast<float>(0.25) * static_cast<float>(windowSize.x);
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

    // Calculate the origin (center of the graph)
    sf::Vector2f origin(dividerLinePositionX + graphWidth / 2, graphHeight / 2);

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
    xAxis.setFillColor(sf::Color(255,255,255, 30));
    window.draw(xAxis);

    // Y-axis
    sf::RectangleShape yAxis(sf::Vector2f(AXIS_THICKNESS, graphHeight));
    yAxis.setPosition({origin.x, 0});
    yAxis.setFillColor(sf::Color(255,255,255, 30));
    window.draw(yAxis);
}

// Utility functions for coordinate conversion
sf::Vector2f UI::windowToGraph(const sf::Vector2f windowCoord)
{
    const float dividerX = 0.25f * windowSize.x;
    const sf::Vector2f origin(dividerX + (windowSize.x - dividerX) / 2, windowSize.y / 2);

    return sf::Vector2f(
        (windowCoord.x - origin.x) / GRID_SIZE,
        (origin.y - windowCoord.y) / GRID_SIZE // Inverted Y because SFML Y grows downward
    );
}

sf::Vector2f UI::graphToWindow(sf::Vector2f graphCoord)
{
    const float dividerX = 0.25f * windowSize.x;
    const sf::Vector2f origin(dividerX + (windowSize.x - dividerX) / 2, windowSize.y / 2);

    return sf::Vector2f(
        origin.x + graphCoord.x * GRID_SIZE,
        origin.y - graphCoord.y * GRID_SIZE // Inverted Y because SFML Y grows downward
    );
}

void UI::shutdown()
{
    ImGui::SFML::Shutdown();
}
