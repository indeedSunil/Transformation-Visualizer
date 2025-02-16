#include "UI.hpp"
#include <Shapes/Shapes.hpp>
#include "Transformations/Transformations.hpp"

#include <iostream>
#include <ostream>

sf::Vector2u UI::windowSize; // Calculate the size of the window
std::vector<sf::Texture> UI::textures; // Define the static textures vector
float UI::dividerLinePositionX; // Divider line between options and graph
//for scaling
bool UI::showTransformationWindow = false;
bool UI::showColorWindow = false;
int UI::fromWhereItCame = 0;

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
    ImGui::SetNextWindowSize(ImVec2(200, 130));

    ImGui::Begin("Options ", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::Separator();
    ImGui::SetWindowFontScale(1.5f);
    if (ImGui::Button("Fill Color"))
    {
        showColorWindow = true;
        std::cout << "Add" << std::endl;
    }
    if (ImGui::Button("Hide Grid Numbers"))
    {
        std::cout << "Add" << std::endl;
    }
    if (ImGui::Button("Clear Canvas"))
    {
        Shapes::clearShape();
    }
    ImGui::End();

    //color window:
    if (showColorWindow)
    {
        ImGui::SetNextWindowPos(ImVec2(240, 100), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(200, 130), ImGuiCond_Always);

        if (ImGui::Begin("Color Selection", &showColorWindow,
                         ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
        {
            ImGui::SetWindowFontScale(1.5f);

            // Define colors
            constexpr ImVec4 colors[] = {
                ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // Red
                ImVec4(0.0f, 1.0f, 0.0f, 1.0f), // Green
                ImVec4(0.0f, 0.0f, 1.0f, 1.0f) // Blue
            };

            // Create a 1x3 grid of colored buttons
            for (int i = 0; i < 3; i++)
            {
                if (i > 0) ImGui::SameLine();

                // Push color style for the button
                ImGui::PushStyleColor(ImGuiCol_Button, colors[i]);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colors[i]);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, colors[i]);

                // Create a square button
                if (ImGui::Button(("##Color" + std::to_string(i)).c_str(), ImVec2(45, 45)))
                {
                    // Handle color selection
                    switch (i)
                    {
                    case 0:
                        std::cout << "Red color selected" << std::endl;
                        break;
                    case 1:
                        std::cout << "Green color selected" << std::endl;
                        break;
                    case 2:
                        std::cout << "Blue color selected" << std::endl;
                        break;
                    default:
                        break;
                    }
                }

                // Pop color style
                ImGui::PopStyleColor(3);
            }

            // Add a cancel button
            ImGui::Spacing();
            ImGui::Spacing();


            ImGui::End();
        }
    }

    // Options for 2D transformations
    ImGui::SetNextWindowPos(ImVec2(30, 250), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 200));

    ImGui::Begin("Transformations", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::Separator();
    ImGui::SetWindowFontScale(1.5f);
    if (ImGui::Button("Scale"))
    {
        UI::showTransformationWindow = true; // Set to true when button is clicked
        UI::fromWhereItCame = 1;
    }
    if (ImGui::Button("Translate"))
    {
        UI::showTransformationWindow = true; // Set to true when button is clicked
        UI::fromWhereItCame = 2;
        std::cout << "Option 2 Clicked!" << std::endl;
        std::cout << "Translate clicked" << std::endl;
    }
    if (ImGui::Button("Rotate"))
    {
        UI::showTransformationWindow = true; // Set to true when button is clicked
        UI::fromWhereItCame = 3;
        std::cout << "Add" << std::endl;
        std::cout << "Rotate clicked" << std::endl;
    }
    if (ImGui::Button("Reflect"))
    {
        UI::showTransformationWindow = true; // Set to true when button is clicked
        UI::fromWhereItCame = 4;
        std::cout << "Option 2 Clicked!" << std::endl;
        std::cout << "Reflect clicked" << std::endl;
    }
    if (ImGui::Button("Shear"))
    {
        UI::showTransformationWindow = true; // Set to true when button is clicked
        UI::fromWhereItCame = 5;
        std::cout << "Add" << std::endl;
        std::cout << "Shear clicked" << std::endl;
    }
    ImGui::End();
    //transformations window-scaling window
    if (UI::showTransformationWindow && UI::fromWhereItCame == 1)
    {
        ImGui::SetNextWindowPos(ImVec2(240, 250), ImGuiCond_Always); // Set fixed position
        ImGui::SetNextWindowSize(ImVec2(250, 200), ImGuiCond_Always); // Set fixed size

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
            }

            // Input field for X
            ImGui::SameLine();
            if (ImGui::InputFloat("##InputX", &scaleX, 0.1f, 1.0f, "%.1f"))
            {
                // Clamp the input value between 0.1 and 5.0
                scaleX = std::max(0.1f, std::min(scaleX, 5.0f));
            }

            // Y Scale
            ImGui::Text("Scale Factor Y:");

            // Slider for Y
            if (ImGui::SliderFloat("##SliderY", &scaleY, 0.1f, 5.0f))
            {
            }

            // Input field for Y
            ImGui::SameLine();
            if (ImGui::InputFloat("##InputY", &scaleY, 0.1f, 1.0f, "%.1f"))
            {
                // Clamp the input value between 0.1 and 5.0
                scaleY = std::max(0.1f, std::min(scaleY, 5.0f));
            }
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::Button("Apply Scale"))
            {
                Shapes::isSelected = true;
                Transformation::scale(Shapes::rectangle, {scaleX, scaleY});
            }
        }
        ImGui::End();
    }

    //transformations window-translation window
    if (UI::showTransformationWindow && UI::fromWhereItCame == 2)
    {
        ImGui::SetNextWindowPos(ImVec2(240, 250), ImGuiCond_Always); // Set fixed position
        ImGui::SetNextWindowSize(ImVec2(250, 200), ImGuiCond_Always);
        // Made slightly taller to accommodate new elements

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
            }

            // Input field for X
            ImGui::SameLine();
            if (ImGui::InputFloat("##InputX", &translateX, 1.0f, 10.0f, "%.1f"))
            {
                // Clamp the input value between -100 and 100
                translateX = std::max(-100.0f, std::min(translateX, 100.0f));
            }

            // Y Translation
            ImGui::Text("Translate Y:");

            // Slider for Y
            if (ImGui::SliderFloat("##SliderY", &translateY, -100.0f, 100.0f))
            {
            }

            // Input field for Y
            ImGui::SameLine();
            if (ImGui::InputFloat("##InputY", &translateY, 1.0f, 10.0f, "%.1f"))
            {
                // Clamp the input value between -100 and 100
                translateY = std::max(-100.0f, std::min(translateY, 100.0f));
            }
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::Button("Apply Translate"))
            {
                Shapes::isSelected = true;
                Transformation::translate(Shapes::rectangle, {translateX, translateY});
            }
        }
        ImGui::End();
    }

    //transformations window-rotation window
    if (UI::showTransformationWindow && UI::fromWhereItCame == 3)
    {
        ImGui::SetNextWindowPos(ImVec2(240, 250), ImGuiCond_Always); // Set fixed position
        ImGui::SetNextWindowSize(ImVec2(250, 200), ImGuiCond_Always);
        // Made slightly taller to accommodate new elements

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
            }

            // Input field for rotation
            ImGui::SameLine();
            if (ImGui::InputFloat("##InputAngle", &rotateAngle, 1.0f, 10.0f, "%.1f"))
            {
                // Clamp the input value between -180 and 180
                rotateAngle = std::max(-180.0f, std::min(rotateAngle, 180.0f));
                std::cout << "Rotation Angle changed via input: " << rotateAngle << std::endl;
            }
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::Button("Apply Rotate"))
            {
                Shapes::isSelected = true;
                Transformation::rotate(Shapes::rectangle, rotateAngle);
            }
        }
        ImGui::End();
    }

    //transformations window-reflection window
    if (UI::showTransformationWindow && UI::fromWhereItCame == 4)
    {
        ImGui::SetNextWindowPos(ImVec2(240, 250), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(250, 200), ImGuiCond_Always); // Increased size to accommodate more options

        if (ImGui::Begin("Reflect", &showTransformationWindow,
                         ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
        {
            ImGui::SetWindowFontScale(1.5f);

            // Static variables to store the reflection options
            static int reflectionType = 0;
            static float m = 1.0f; // Slope for y=mx+c
            static float c = 0.0f; // Y-intercept for y=mx+c
            static bool showLineParams = false;

            // Radio buttons for reflection options
            ImGui::Text("Reflection About:");
            if (ImGui::RadioButton("X-axis (y=0)", &reflectionType, 0))
            {
                showLineParams = false;
            }
            if (ImGui::RadioButton("Y-axis (x=0)", &reflectionType, 1))
            {
                showLineParams = false;
            }
            if (ImGui::RadioButton("Line y=x", &reflectionType, 2))
            {
                showLineParams = false;
            }
            if (ImGui::RadioButton("Line y=-x", &reflectionType, 3))
            {
                showLineParams = false;
            }
            if (ImGui::RadioButton("Line y=mx+c", &reflectionType, 4))
            {
                showLineParams = true;
            }
            // X Reflection
            ImGui::Text("Reflect X:");

            // Show slope and y-intercept inputs if y=mx+c is selected
            if (showLineParams)
            {
                ImGui::Spacing();
                ImGui::Text("Line Parameters:");
                ImGui::PushItemWidth(150);

                // Input for slope (m)
                ImGui::Text("Slope (m):");
                ImGui::SameLine();
                ImGui::InputFloat("##slope", &m, 0.1f, 1.0f, "%.2f");

                // Input for y-intercept (c)
                ImGui::Text("Y-intercept (c):");
                ImGui::SameLine();
                ImGui::InputFloat("##intercept", &c, 0.1f, 1.0f, "%.2f");

                ImGui::PopItemWidth();
            }
            // Y Reflection
            ImGui::Text("Reflect Y:");

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // Apply button
            if (ImGui::Button("Apply Reflection", ImVec2(-1, 40)))
            {
                // Print the selected reflection type and parameters
                switch (reflectionType)
                {
                case 0:
                    std::cout << "Reflecting about X-axis (y=0)" << std::endl;
                    break;
                case 1:
                    std::cout << "Reflecting about Y-axis (x=0)" << std::endl;
                    break;
                case 2:
                    std::cout << "Reflecting about y=x" << std::endl;
                    break;
                case 3:
                    std::cout << "Reflecting about y=-x" << std::endl;
                    break;
                case 4:
                    std::cout << "Reflecting about y=" << m << "x+" << c << std::endl;
                    break;
                default:
                    break;
                }
                if (ImGui::Button("Apply Reflect"))
                {
                    Shapes::isSelected = true;
                    // Transformation::reflect(Math::rectangle, reflectX);
                }
            }
            ImGui::End();
        }
    }

    //transformations window-shear window
    if (UI::showTransformationWindow && UI::fromWhereItCame == 5)
    {
        ImGui::SetNextWindowPos(ImVec2(240, 250), ImGuiCond_Always); // Set fixed position
        ImGui::SetNextWindowSize(ImVec2(250, 200), ImGuiCond_Always);
        // Made slightly taller to accommodate new elements

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
            }

            // Input field for X
            ImGui::SameLine();
            if (ImGui::InputFloat("##InputX", &shearX, 0.1f, 1.0f, "%.1f"))
            {
                // Clamp the input value between -5.0 and 5.0
                shearX = std::max(-5.0f, std::min(shearX, 5.0f));
            }

            // Y Shear
            ImGui::Text("Shear Y:");

            // Slider for Y
            if (ImGui::SliderFloat("##SliderY", &shearY, -5.0f, 5.0f))
            {
            }

            // Input field for Y
            ImGui::SameLine();
            if (ImGui::InputFloat("##InputY", &shearY, 0.1f, 1.0f, "%.1f"))
            {
                // Clamp the input value between -5.0 and 5.0
                shearY = std::max(-5.0f, std::min(shearY, 5.0f));
            }
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            // Add your scale transformation logic here

            if (ImGui::Button("Apply Shear"))
            {
                Shapes::isSelected = true;
                Transformation::shear(Shapes::rectangle, {shearX, shearY});
            }
        }
        ImGui::End();
    }


    // New Grid Window
    ImGui::SetNextWindowPos(ImVec2(30, 500), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(417, 260));

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
