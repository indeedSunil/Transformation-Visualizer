#include "Renderer.hpp"
#include "Shapes/Shapes.hpp"
#include <iomanip>
#include "UI/UI.hpp"
#include <iostream>


// Static members initialization
Renderer::ShapeType Renderer::currentShape = ShapeType::None;
bool Renderer::isDrawing = false;
sf::Vector2f Renderer::startPos;
sf::Vector2f Renderer::currentPos;
bool Renderer::hasShape = false;

void Renderer::setCurrentShape(const ShapeType shape)
{
    currentShape = shape;
    Shapes::setCurrentShape(static_cast<Shapes::ShapeType>(shape));
    isDrawing = false;
}

void Renderer::handleMouseEvent(const sf::Event& event, sf::RenderWindow& window)
{
    sf::Vector2f mousePos;
    if (event.getIf<sf::Event::MouseButtonPressed>() && isButtonPressed(sf::Mouse::Button::Left))
    {
        mousePos = sf::Vector2f(static_cast<float>(sf::Mouse::getPosition().x),
                                static_cast<float>(sf::Mouse::getPosition().y));

        if (hasShape && !isDrawing)
        {
            Shapes::handleShapeSelection(mousePos);
            if (Shapes::isSelected)
            {
                // isDrawing = true;
                Shapes::isDragging = true;
                Shapes::dragOffset = mousePos - Shapes::getShapePosition();
            }
        }
        else if (!hasShape)
        {
            startPos = UI::windowToGraph(mousePos);
            currentPos = startPos;
            Shapes::startPos = startPos;
            Shapes::currentPos = startPos;
            isDrawing = true;
            Shapes::isDrawing = true;
            initializeShape(window);
        }
    }
    else if (event.getIf<sf::Event::MouseMoved>())
    {
        const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>();
        mousePos = sf::Vector2f(static_cast<float>(mouseMoved->position.x),
                                static_cast<float>(mouseMoved->position.y));

        if (Shapes::isDragging && Shapes::isSelected)
        {
            Shapes::handleDragging(mousePos);
        }
        else if (isDrawing)
        {
            currentPos = UI::windowToGraph(mousePos);
            Shapes::currentPos = currentPos;
            Shapes::updateShape();
        }
    }
    else if (event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (Shapes::isDragging)
        {
            Shapes::isDragging = false;
        }
        if (isDrawing)
        {
            finishDrawing(window);
            isDrawing = false;
            hasShape = true;
        }
    }
}

void Renderer::render(sf::RenderWindow& window)
{
    if (hasShape || Shapes::isDrawing)
    {
        displayCoordinates(window);
        Shapes::drawShape(window);
    }
}

void Renderer::initializeShape(sf::RenderWindow& window)
{
    switch (currentShape)
    {
    case ShapeType::Rectangle:
        Shapes::rectangle.setPosition(UI::graphToWindow(startPos));
        Shapes::rectangle.setSize(sf::Vector2f(0, 0));
        Shapes::rectangle.setFillColor(sf::Color::Transparent);
        Shapes::rectangle.setOutlineColor(sf::Color::White);
        Shapes::rectangle.setOutlineThickness(2);
        window.draw(Shapes::rectangle);
        break;
    case ShapeType::Circle:
        Shapes::circle.setPosition(UI::graphToWindow(startPos));
        Shapes::circle.setRadius(0);
        Shapes::circle.setFillColor(sf::Color::Transparent);
        Shapes::circle.setOutlineColor(sf::Color::White);
        Shapes::circle.setOutlineThickness(2);
        window.draw(Shapes::circle);
        break;
    case ShapeType::Triangle:
        Shapes::triangle.setPointCount(3);
        // Ensure the points are slightly different initially
        Shapes::triangle.setPoint(0, UI::graphToWindow(startPos));
        Shapes::triangle.setPoint(1, UI::graphToWindow(startPos + sf::Vector2f(1.f, 0.f)));
        Shapes::triangle.setPoint(2, UI::graphToWindow(startPos + sf::Vector2f(1.f, 1.f)));
        Shapes::triangle.setFillColor(sf::Color::Transparent);
        Shapes::triangle.setOutlineColor(sf::Color::White);
        Shapes::triangle.setOutlineThickness(2);
        break;
    default:
        break;
    }
}

void Renderer::finishDrawing(sf::RenderWindow& window)
{
    switch (currentShape)
    {
    case ShapeType::Rectangle:
        Shapes::rectangle.setFillColor(sf::Color::Transparent);
        window.draw(Shapes::rectangle);
        break;
    case ShapeType::Triangle:
        Shapes::triangle.setFillColor(sf::Color::Transparent);
        window.draw(Shapes::triangle);
        break;
    case ShapeType::Circle:
        Shapes::circle.setFillColor(sf::Color::Transparent);
        window.draw(Shapes::circle);
        break;
    case ShapeType::Line:
        Shapes::line.setFillColor(sf::Color::Transparent);
        break;
    default:
        break;
    }
}

void Renderer::displayCoordinates(sf::RenderWindow& window)
{
    Shapes::coordinatesText.setCharacterSize(20);
    Shapes::coordinatesText.setFillColor(sf::Color::White);

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);  // One digit after the decimal

    switch (currentShape)
    {
    case ShapeType::Rectangle:
        {
            // Get the rectangle's position and size in window units.
            sf::Vector2f position = Shapes::rectangle.getPosition();
            sf::Vector2f size = Shapes::rectangle.getSize();

            // Convert the four corners from window to graph coordinates.
            const sf::Vector2f A = UI::windowToGraph(position);
            const sf::Vector2f B = UI::windowToGraph({position.x + size.x, position.y});
            const sf::Vector2f C = UI::windowToGraph({position.x + size.x, position.y + size.y});
            const sf::Vector2f D = UI::windowToGraph({position.x, position.y + size.y});

            oss << "Co-ordinates of Rectangle:\n";
            oss << "A: " << A.x << ", " << A.y << "\n";
            oss << "B: " << B.x << ", " << B.y << "\n";
            oss << "C: " << C.x << ", " << C.y << "\n";
            oss << "D: " << D.x << ", " << D.y;

            sf::Vector2f A_win = UI::graphToWindow(A);
            sf::Vector2f B_win = UI::graphToWindow(B);
            sf::Vector2f C_win = UI::graphToWindow(C);
            sf::Vector2f D_win = UI::graphToWindow(D);

            // Create and draw labels at the corners
            sf::Text labelA(Core::font ,"A", 20);
            sf::Text labelB(Core::font ,"B", 20);
            sf::Text labelC(Core::font ,"C", 20);
            sf::Text labelD(Core::font ,"D", 20);

            labelA.setFillColor(sf::Color::Yellow);
            labelB.setFillColor(sf::Color::Yellow);
            labelC.setFillColor(sf::Color::Yellow);
            labelD.setFillColor(sf::Color::Yellow);

            // Set positions slightly offset for better visibility
            labelA.setPosition({A_win.x - 20, A_win.y - 20});
            labelB.setPosition({B_win.x + 10, B_win.y - 15});
            labelC.setPosition({C_win.x + 5, C_win.y + 5});
            labelD.setPosition({D_win.x - 10, D_win.y + 5});

            // Draw labels on the window
            window.draw(labelA);
            window.draw(labelB);
            window.draw(labelC);
            window.draw(labelD);

            break;
        }
    case ShapeType::Circle:
        {
            float radiusWindow = Shapes::circle.getRadius();
            const float radiusGraph = UI::windowDistanceToGraph({radiusWindow, radiusWindow}).x;

            // Get the top-left position of the circle's bounding box in window units.
            const sf::Vector2f topLeft = Shapes::circle.getPosition();
            // Center in window units by adding the radius.
            const sf::Vector2f centerWindow = { topLeft.x + radiusWindow, topLeft.y + radiusWindow };
            // Convert the center position from window to graph coordinates.
            const sf::Vector2f centerGraph = UI::windowToGraph(centerWindow);

            oss << "Co-ordinates of Circle:\n";
            oss << "Center: " << centerGraph.x << ", " << centerGraph.y << "\n";
            oss << "Radius: " << radiusGraph;
            break;
        }


    default:
        return;
    }

    Shapes::coordinatesText.setString(oss.str());
    Shapes::coordinatesText.setPosition({50.f, 850.f});
    window.draw(Shapes::coordinatesText);
}

