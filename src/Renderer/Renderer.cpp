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
        break;
        // Add other shape initializations as needed
    default:
        break;
    }
}

void Renderer::finishDrawing(sf::RenderWindow& window)
{
    switch (currentShape)
    {
    case ShapeType::Rectangle:
        Shapes::rectangle.setFillColor(sf::Color(255, 255, 255, 0));
        window.draw(Shapes::rectangle);
        break;
    case ShapeType::Circle:
        Shapes::circle.setFillColor(sf::Color(255, 255, 255, 0));
        break;
    default:
        break;
    }
}

void Renderer::displayCoordinates(sf::RenderWindow& window)
{
    sf::Vector2f position = Shapes::getShapePosition();
    sf::Vector2f size;

    switch (currentShape)
    {
    case ShapeType::Rectangle:
        size = Shapes::rectangle.getSize();
        break;
        // Add cases for other shapes
    default:
        return;
    }

    const sf::Vector2f A = UI::windowToGraph(position);
    const sf::Vector2f B = UI::windowToGraph({position.x + size.x, position.y});
    const sf::Vector2f C = UI::windowToGraph({position.x + size.x, position.y + size.y});
    const sf::Vector2f D = UI::windowToGraph({position.x, position.y + size.y});

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Co-ordinates of Shape:\n";
    oss << "A: " << A.x << ", " << A.y << "\n";
    oss << "B: " << B.x << ", " << B.y << "\n";
    oss << "C: " << C.x << ", " << C.y << "\n";
    oss << "D: " << D.x << ", " << D.y;

    Shapes::coordinatesText.setString(oss.str());
    Shapes::coordinatesText.setPosition({50.f, 850.f});
    Shapes::coordinatesText.setCharacterSize(20);
    Shapes::coordinatesText.setFillColor(sf::Color::White);
    window.draw(Shapes::coordinatesText);
}
