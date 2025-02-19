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
    case ShapeType::CustomRectangle:
        {
            Shapes::CustomShape.setPointCount(4);
            Shapes::CustomShape.setPoint(0, startPos);
            Shapes::CustomShape.setPoint(1, startPos + sf::Vector2f(2.f, 0.f));  // Larger initial size
            Shapes::CustomShape.setPoint(2, startPos + sf::Vector2f(2.f, 2.f));  // Larger initial size
            Shapes::CustomShape.setPoint(3, startPos + sf::Vector2f(0.f, 2.f));  // Larger initial size
            Shapes::CustomShape.setFillColor(sf::Color::Transparent);
            Shapes::CustomShape.setOutlineColor(sf::Color::Black);
            Shapes::CustomShape.setOutlineThickness(0.1f);
            window.draw(Shapes::CustomShape);
            break;
        }


    default:
        break;
    }
}

void Renderer::finishDrawing(sf::RenderWindow& window)
{
    switch (currentShape)
    {
    case ShapeType::CustomRectangle:
        Shapes::CustomShape.setFillColor(sf::Color::Transparent);
        window.draw(Shapes::CustomShape);
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
    oss << std::fixed << std::setprecision(1); // One digit after the decimal point

    switch (currentShape)
    {
    case ShapeType::CustomRectangle:
        {

            const sf::Vector2f A = Shapes::CustomShape.getPoint(0);
            const sf::Vector2f B = Shapes::CustomShape.getPoint(1);
            const sf::Vector2f C = Shapes::CustomShape.getPoint(2);
            const sf::Vector2f D = Shapes::CustomShape.getPoint(3);

            oss << "Co-ordinates of Rectangle:\n";
            oss << "A: (" << A.x << ", " << A.y << ")\n";
            oss << "B: (" << B.x << ", " << B.y << ")\n";
            oss << "C: (" << C.x << ", " << C.y << ")\n";
            oss << "D: (" << D.x << ", " << D.y << ")";

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
