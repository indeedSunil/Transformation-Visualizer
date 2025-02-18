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

    // Function to convert window coordinates to view coordinates
    auto getViewMousePosition = [&window](sf::Vector2i pixelPos) -> sf::Vector2f
    {
        // Get position relative to graph view if mouse is in graph area
        if (pixelPos.x > Core::getPanelWidth())
        {
            return window.mapPixelToCoords(pixelPos, Core::getGraphView());
        }
        // Get position relative to UI view if mouse is in UI panel
        return window.mapPixelToCoords(pixelPos, Core::getUIView());
    };

    if (event.getIf<sf::Event::MouseButtonPressed>() && isButtonPressed(sf::Mouse::Button::Left))
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        mousePos = getViewMousePosition(pixelPos);

        if (hasShape && !isDrawing)
        {
            Shapes::handleShapeSelection(mousePos);
            if (Shapes::isSelected)
            {
                Shapes::isDragging = true;
                // Floor both the mouse position and shape position to ensure grid alignment
                sf::Vector2f snappedMousePos = {
                    std::floor(mousePos.x),
                    std::floor(mousePos.y)
                };
                sf::Vector2f snappedShapePos = {
                    std::floor(Shapes::getShapePosition().x),
                    std::floor(Shapes::getShapePosition().y)
                };
                Shapes::dragOffset = snappedMousePos - snappedShapePos;
                std::cout << "Position: " << Shapes::getShapePosition().x << " " << Shapes::getShapePosition().y <<
                    std::endl;
            }
        }
        else if (!hasShape)
        {
            startPos = {
                std::floor(mousePos.x),
                std::floor(mousePos.y)
            };
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
        mousePos = getViewMousePosition(mouseMoved->position);

        if (Shapes::isDragging && Shapes::isSelected)
        {
            Shapes::handleDragging(mousePos);
        }
        else if (isDrawing)
        {
            currentPos = {
                std::floor(mousePos.x),
                std::floor(mousePos.y)
            };
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
    window.setView(Core::getGraphView());
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
        Shapes::rectangle.setPosition(startPos);
        Shapes::rectangle.setSize(sf::Vector2f(0, 0));
        Shapes::rectangle.setFillColor(sf::Color::Transparent);
        Shapes::rectangle.setOutlineColor(sf::Color::Black);
        Shapes::rectangle.setOutlineThickness(0.1);
        std::cout << "Rectangle initializing: " << Shapes::rectangle.getPosition().x << " " << Shapes::rectangle.
            getPosition().y << std::endl;
        window.draw(Shapes::rectangle);
        break;
    case ShapeType::Circle:
        Shapes::circle.setPosition(startPos);
        Shapes::circle.setRadius(0);
        Shapes::circle.setFillColor(sf::Color::Transparent);
        Shapes::circle.setOutlineColor(sf::Color::Black);
        Shapes::circle.setOutlineThickness(2);
        window.draw(Shapes::circle);
        break;
    case ShapeType::Triangle:
        Shapes::triangle.setPointCount(3);
    // Ensure the points are slightly different initially
        Shapes::triangle.setPoint(0, startPos);
        Shapes::triangle.setPoint(1, startPos + sf::Vector2f(1.f, 0.f));
        Shapes::triangle.setPoint(2, startPos + sf::Vector2f(1.f, 1.f));
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
    oss << std::fixed << std::setprecision(0); // One digit after the decimal

    switch (currentShape)
    {
    case ShapeType::Rectangle:
        {
            sf::Vector2f position = Shapes::rectangle.getPosition();
            sf::Vector2f size = Shapes::rectangle.getSize();

            const sf::Vector2f A = position;
            const sf::Vector2f B = {position.x + size.x, position.y};
            const sf::Vector2f C = {position.x + size.x, position.y + size.y};
            const sf::Vector2f D = {position.x, position.y + size.y};

            oss << "Co-ordinates of Rectangle:\n";
            oss << "A: (" << A.x << ", " << A.y << ")\n";
            oss << "B: (" << B.x << ", " << B.y << ")\n";
            oss << "C: (" << C.x << ", " << C.y << ")\n";
            oss << "D: (" << D.x << ", " << D.y << ")";

            break;
        }
    case ShapeType::Circle:
        {
            float radiusWindow = Shapes::circle.getRadius();
            const float radiusGraph = UI::windowDistanceToGraph({radiusWindow, radiusWindow}).x;

            // Get the top-left position of the circle's bounding box in window units.
            const sf::Vector2f topLeft = Shapes::circle.getPosition();
            // Center in window units by adding the radius.
            const sf::Vector2f centerWindow = {topLeft.x + radiusWindow, topLeft.y + radiusWindow};
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

    window.setView(Core::getUIView());
    Shapes::coordinatesText.setString(oss.str());
    Shapes::coordinatesText.setPosition({50.f, 850.f});
    window.draw(Shapes::coordinatesText);
    window.setView(Core::getGraphView());
}
