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
            const sf::Vector2f size = UI::graphToWindow(currentPos) - UI::graphToWindow(startPos);
            rectangle.setSize(sf::Vector2f(std::abs(size.x), std::abs(size.y)));
            if (size.x < 0) rectangle.setPosition(UI::graphToWindow(sf::Vector2f(currentPos.x, startPos.y)));
            if (size.y < 0) rectangle.setPosition(UI::graphToWindow(sf::Vector2f(startPos.x, currentPos.y)));
            break;
        }
    case ShapeType::Circle:
        {
            float radius = std::sqrt(
                std::pow(currentPos.x - startPos.x, 2) +
                std::pow(currentPos.y - startPos.y, 2)
            );
            circle.setRadius(radius);
            circle.setPosition(UI::graphToWindow(startPos));
            break;
        }
    case ShapeType::Triangle:
        {
            triangle.setPointCount(3);
            triangle.setPoint(0, UI::graphToWindow(startPos));
            triangle.setPoint(1, UI::graphToWindow(sf::Vector2f(currentPos.x, startPos.y)));
            triangle.setPoint(2, UI::graphToWindow(currentPos));
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

    // Draw coordinates if shape is selected
    if (isSelected)
    {
        // drawCoordinates(window);
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
        setShapeOutlineColor(sf::Color::Yellow);
    }
    else
    {
        isSelected = false;
        setShapeOutlineColor(sf::Color::White);
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

    sf::Vector2f newPos = mousePos - dragOffset;

    // Snap to grid
    float gridSize = static_cast<float>(UI::GRID_SIZE);
    newPos.x = std::round(newPos.x / gridSize) * gridSize;
    newPos.y = std::round(newPos.y / gridSize) * gridSize;

    // Apply boundaries
    newPos.x = std::max(newPos.x, UI::dividerLinePositionX);
    newPos.x = std::min(newPos.x, static_cast<float>(UI::windowSize.x) - getShapeSize().x);
    newPos.y = std::max(newPos.y, 0.0f);
    newPos.y = std::min(newPos.y, static_cast<float>(UI::windowSize.y) - getShapeSize().y);

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

// void Shapes::drawCoordinates(sf::RenderWindow& window)
// {
//     sf::Vector2f position = getShapePosition();
//     sf::Vector2f size;
//
//     const sf::Vector2f A = UI::windowToGraph(position);
//     const sf::Vector2f B = UI::windowToGraph({position.x + size.x, position.y});
//     const sf::Vector2f C = UI::windowToGraph({position.x + size.x, position.y + size.y});
//     const sf::Vector2f D = UI::windowToGraph({position.x, position.y + size.y});
//
//     std::ostringstream oss;
//     oss << std::fixed << std::setprecision(2);
//     oss << "Co-ordinates of Shape:\n";
//     oss << "A: " << A.x << ", " << A.y << "\n";
//     oss << "B: " << B.x << ", " << B.y << "\n";
//     oss << "C: " << C.x << ", " << C.y << "\n";
//     oss << "D: " << D.x << ", " << D.y;
//
//     coordinatesText.setString(oss.str());
//     coordinatesText.setPosition({50.f, 850.f});
//     coordinatesText.setCharacterSize(20);
//     coordinatesText.setFillColor(sf::Color::White);
//     window.draw(coordinatesText);
// }
