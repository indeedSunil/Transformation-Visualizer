#include "Math.hpp"
#include "UI/UI.hpp"
#include <iostream>

Math::ShapeType Math::currentShape = Math::ShapeType::None;
bool Math::isDrawing = false;
sf::Vector2f Math::startPos;
sf::Vector2f Math::currentPos;

sf::RectangleShape Math::rectangle;
sf::CircleShape Math::circle;
sf::ConvexShape Math::triangle;
sf::RectangleShape Math::line;
// sf::EllipseShape Math::ellipse;
sf::Text Math::letterA(Core::font);

void Math::setCurrentShape(ShapeType shape)
{
    currentShape = shape;
    isDrawing = false; // Reset drawing state when shape changes
}

void Math::handleMouseEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    if (event.getIf<sf::Event::MouseButtonPressed>() && isButtonPressed(sf::Mouse::Button::Left))
    {
        // Convert window coordinates to graph coordinates
        startPos = UI::windowToGraph(sf::Vector2f(static_cast<float>(sf::Mouse::getPosition().x),
                                                  static_cast<float>(sf::Mouse::getPosition().y)));
        currentPos = startPos;
        isDrawing = true;

        // Initialize shape based on type
        switch (currentShape)
        {
        case ShapeType::Rectangle:
            rectangle.setPosition(UI::graphToWindow(startPos));
            rectangle.setSize(sf::Vector2f(0, 0));
            rectangle.setFillColor(sf::Color::Transparent);
            rectangle.setOutlineColor(sf::Color::Red);
            rectangle.setOutlineThickness(2);
            break;
        default:
            break;
        // Add other shape initializations here
        }
    }
    else if (isDrawing)
    {
        if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())
        {
            currentPos = UI::windowToGraph(sf::Vector2f(static_cast<float>(mouseMoved->position.x), static_cast<float>(mouseMoved->position.y)));
            updateShape();
        }
    }
    else if (event.getIf<sf::Event::MouseButtonReleased>() && isButtonPressed(sf::Mouse::Button::Left))
    {
        isDrawing = false;
    }
}

void Math::updateShape()
{
    switch (currentShape)
    {
    case ShapeType::Rectangle:
        {
            const sf::Vector2f size = UI::graphToWindow(currentPos) - UI::graphToWindow(startPos);
            rectangle.setSize(sf::Vector2f(std::abs(size.x), std::abs(size.y)));
            if (size.x < 0) rectangle.setPosition(UI::graphToWindow(sf::Vector2f(currentPos.x, startPos.y)));
            if (size.y < 0) rectangle.setPosition(UI::graphToWindow(sf::Vector2f(startPos.x, currentPos.y)));
            break;

        }
    default:
        break;
    // Add other shape updates here
    }
}

void Math::render(sf::RenderWindow& window)
{
    if (!isDrawing) return;

    switch (currentShape)
    {
    case ShapeType::Rectangle:
        window.draw(rectangle);
        break;
        default:
            break;
    // Add other shape rendering here
    }
}
