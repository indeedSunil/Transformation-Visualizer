#include "Shapes.hpp"
#include "UI/UI.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>

// Static member initialization
Shapes::ShapeType Shapes::currentShape = ShapeType::None;
bool Shapes::isDrawing = false;
bool Shapes::isSelected = false;
bool Shapes::isDragging = false;
sf::Vector2f Shapes::startPos;
sf::Vector2f Shapes::currentPos;
sf::Vector2f Shapes::dragOffset;
sf::Text Shapes::coordinatesText(Core::font);

sf::RectangleShape Shapes::rectangle;
sf::CircleShape Shapes::circle;
sf::ConvexShape Shapes::triangle;
sf::RectangleShape Shapes::line;
sf::Text Shapes::letterA(Core::font);

void Shapes::setCurrentShape(const ShapeType shape)
{
    currentShape = shape;
    isDrawing = false; // Reset drawing state when shape changes
}

void Shapes::updateShape()
{
    switch (currentShape)
    {
    case ShapeType::Rectangle:
        {
            const sf::Vector2f size = currentPos - startPos;
            rectangle.setSize(sf::Vector2f(std::abs(size.x), std::abs(size.y)));
            if (size.x < 0) rectangle.setPosition(sf::Vector2f(currentPos.x, startPos.y));
            if (size.y < 0) rectangle.setPosition(sf::Vector2f(startPos.x, currentPos.y));
            break;
        }
    case ShapeType::Circle:
        {
            break;
        }
    case ShapeType::Triangle:
        {
            break;
        }
    case ShapeType::Line:
        {
        }
    default:
        break;
    }
    // Shapes::drawCoordinates(window);
}

void Shapes::drawShape(sf::RenderWindow& window)
{
    window.setView(Core::getGraphView());
    switch (currentShape)
    {
    case ShapeType::Rectangle:
        window.draw(rectangle);
        break;
    case ShapeType::Circle:
        window.draw(circle);
        break;
    case ShapeType::Triangle:
        window.draw(triangle);
        break;
    case ShapeType::Line:
        window.draw(line);
        break;
    case ShapeType::LetterA:
        window.draw(letterA);
        break;
    default:
        break;
    }

}

void Shapes::handleShapeSelection(const sf::Vector2f& mousePos)
{
    sf::FloatRect bounds;
    switch (currentShape)
    {
    case ShapeType::Rectangle:
        bounds = rectangle.getGlobalBounds();
        break;
    case ShapeType::Circle:
        bounds = circle.getGlobalBounds();
        break;
    case ShapeType::Triangle:
        bounds = triangle.getGlobalBounds();
        break;
    case ShapeType::Line:
        bounds = line.getGlobalBounds();
        break;
    default:
        return;
    }

    if (isShapeClicked(mousePos, bounds))
    {
        isSelected = true;
        setShapeOutlineColor(sf::Color::Green);
    }
    else
    {
        isSelected = false;
        setShapeOutlineColor(sf::Color::Black);
    }
}

bool Shapes::isShapeClicked(const sf::Vector2f& mousePos, const sf::FloatRect& bounds)
{
    return bounds.contains(mousePos);
}

sf::Vector2f Shapes::getShapeCenter()
{
    sf::FloatRect bounds;
    switch (currentShape)
    {
    case ShapeType::Rectangle:
        bounds = rectangle.getGlobalBounds();
        break;
    case ShapeType::Circle:
        bounds = circle.getGlobalBounds();
        break;
    case ShapeType::Triangle:
        bounds = triangle.getGlobalBounds();
        break;
    case ShapeType::Line:
        bounds = line.getGlobalBounds();
        break;
    default:
        return sf::Vector2f(0, 0);
    }
    return sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f);
}

void Shapes::handleDragging(const sf::Vector2f& mousePos)
{
    if (!isSelected || !isDragging) return;

    // Calculate new position
    sf::Vector2f newPos = mousePos - dragOffset;

    // Snap the new position to grid points
    newPos.x = std::floor(newPos.x);
    newPos.y = std::floor(newPos.y);

    setShapePosition(newPos);
}
// Helper functions

void Shapes::setShapePosition(const sf::Vector2f& position)
{
    switch (currentShape)
    {
    case ShapeType::Rectangle:
        rectangle.setPosition(position);
        break;
    case ShapeType::Circle:
        circle.setPosition(position);
        break;
    case ShapeType::Triangle:
        {
            sf::Vector2f offset = position - triangle.getPosition();
            for (size_t i = 0; i < triangle.getPointCount(); ++i)
            {
                triangle.setPoint(i, triangle.getPoint(i) + offset);
            }
        }
        break;
    case ShapeType::Line:
        line.setPosition(position);
        break;
    default:
        break;
    }
}

sf::Vector2f Shapes::getShapeSize()
{
    switch (currentShape)
    {
    case ShapeType::Rectangle:
        return rectangle.getSize();
    case ShapeType::Circle:
        return sf::Vector2f(circle.getRadius() * 2, circle.getRadius() * 2);
    case ShapeType::Triangle:
        {
            sf::FloatRect bounds = triangle.getGlobalBounds();
            return sf::Vector2f(bounds.size.x, bounds.size.y);
        }
    case ShapeType::Line:
        return line.getSize();
    default:
        return sf::Vector2f(0, 0);
    }
}

sf::Vector2f Shapes::getShapePosition()
{
    switch (currentShape)
    {
    case ShapeType::Rectangle:
        return rectangle.getPosition();
    case ShapeType::Circle:
        return circle.getPosition();
    case ShapeType::Triangle:
        return triangle.getPosition();
    case ShapeType::Line:
        return line.getPosition();
    case ShapeType::LetterA:
        return letterA.getPosition();
    default:
        return sf::Vector2f(0, 0);
    }
}
