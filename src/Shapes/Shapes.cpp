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
            // Convert positions to window coordinates first
            sf::Vector2f windowStartPos = UI::graphToWindow(startPos);
            sf::Vector2f windowCurrentPos = UI::graphToWindow(currentPos);

            // Calculate radius in window coordinates
            const float radius = std::sqrt(
                static_cast<float>(std::pow(windowCurrentPos.x - windowStartPos.x, 2)) +
                static_cast<float>(std::pow(windowCurrentPos.y - windowStartPos.y, 2))
            );

            circle.setRadius(radius);
            circle.setPosition(windowStartPos);
            break;
        }
    case ShapeType::Triangle:
        {
            // Convert positions to window coordinates first
            sf::Vector2f windowStartPos = UI::graphToWindow(startPos);
            sf::Vector2f windowCurrentPos = UI::graphToWindow(currentPos);

            // Calculate the midpoint for the second point (instead of using currentPos.x directly)
            sf::Vector2f midPoint = sf::Vector2f(
                windowStartPos.x + (windowCurrentPos.x - windowStartPos.x) / 2.0f,
                windowStartPos.y
            );

            triangle.setPointCount(3);
            triangle.setPoint(0, windowStartPos);                    // Left point
            triangle.setPoint(1, midPoint);                         // Top point
            triangle.setPoint(2, windowCurrentPos);                 // Right point

            // Ensure the triangle stays within window bounds
            sf::FloatRect bounds = triangle.getGlobalBounds();
            if (bounds.position.x + bounds.size.x > UI::windowSize.x) {
                // Adjust currentPos to keep triangle within window
                float scale = (UI::windowSize.x - bounds.position.x) / bounds.size.x;
                windowCurrentPos.x = windowStartPos.x + (windowCurrentPos.x - windowStartPos.x) * scale;
                triangle.setPoint(2, windowCurrentPos);
            }
            break;
        }
    case ShapeType::Line:
        {
            const sf::Vector2f direction = UI::graphToWindow(currentPos) - UI::graphToWindow(startPos);
            const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            line.setSize(sf::Vector2f(length, 2.0f));
            line.setPosition(UI::graphToWindow(startPos));
            float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159f;
            line.setRotation(sf::radians(angle));
            break;
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
