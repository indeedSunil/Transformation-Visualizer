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

// Custom shapes
sf::ConvexShape Shapes::CustomShape;

void Shapes::setCurrentShape(const ShapeType shape)
{
    currentShape = shape;
    isDrawing = false;
}

void Shapes::updateShape()
{
    switch (currentShape)
    {
    case ShapeType::CustomRectangle:
        {
            sf::Vector2f size = currentPos - startPos;

            // Ensure minimum size is larger
            if (std::abs(size.x) < 2.0f) size.x = 2.0f;
            if (std::abs(size.y) < 2.0f) size.y = 2.0f;

            CustomShape.setPoint(0, startPos);
            CustomShape.setPoint(1, startPos + sf::Vector2f(size.x, 0));
            CustomShape.setPoint(2, startPos + size);
            CustomShape.setPoint(3, startPos + sf::Vector2f(0, size.y));
            break;
        }
    default:
        break;
    }
}

void Shapes::drawShape(sf::RenderWindow& window)
{
    window.setView(Core::getGraphView());
    switch (currentShape)
    {
    case ShapeType::CustomRectangle:
        window.draw(CustomShape);
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
    case ShapeType::CustomRectangle:
        {
            // Create bounds from points
            float minX = CustomShape.getPoint(0).x;
            float minY = CustomShape.getPoint(0).y;
            float maxX = minX;
            float maxY = minY;

            for (size_t i = 1; i < CustomShape.getPointCount(); ++i)
            {
                sf::Vector2f point = CustomShape.getPoint(i);
                minX = std::min(minX, point.x);
                minY = std::min(minY, point.y);
                maxX = std::max(maxX, point.x);
                maxY = std::max(maxY, point.y);
            }

            bounds = sf::FloatRect({minX, minY}, {maxX - minX, maxY - minY});
        }
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
    case ShapeType::CustomRectangle:
        bounds = CustomShape.getGlobalBounds();
        break;
    default:
        return {0, 0};
    }
    return {bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f};
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
    case ShapeType::CustomRectangle:
        {
            sf::Vector2f currentTopLeft = CustomShape.getPoint(0);
            sf::Vector2f offset = position - currentTopLeft;

            // Update all points by the offset
            for (size_t i = 0; i < CustomShape.getPointCount(); ++i)
            {
                CustomShape.setPoint(i, CustomShape.getPoint(i) + offset);
            }
            break;
        }
    default:
        break;
    }
}

sf::Vector2f Shapes::getShapeSize()
{
    switch (currentShape)
    {
    case ShapeType::CustomRectangle:
        {
            sf::FloatRect bounds = CustomShape.getGlobalBounds();
            return {bounds.size.x, bounds.size.y};
        }
    default:
        return {0, 0};
    }
}

sf::Vector2f Shapes::getShapePosition()
{
    switch (currentShape)
    {
    case ShapeType::CustomRectangle:
        return CustomShape.getPoint(0);
    default:
        return {0, 0};
    }
}

void Shapes::clearShape()

{
    isDrawing = false;
    Renderer::changeIsDrawing(false);

    isSelected = false; // Reset selection state
    isDragging = false;

    Renderer::hasShape = false;
    coordinatesText.setString("");
    currentShape = ShapeType::None;  // Reset shape type

    // Properly reinitialize CustomShape
    CustomShape = sf::ConvexShape(4);  // Create new shape with 4 points
    CustomShape.setPoint(0, sf::Vector2f(0, 0));
    CustomShape.setPoint(1, sf::Vector2f(1, 0.1f));  // Use 1.0f instead of 0.1f
    CustomShape.setPoint(2, sf::Vector2f(1, 1));
    CustomShape.setPoint(3, sf::Vector2f(0, 1));
    CustomShape.setFillColor(sf::Color::Transparent);
    CustomShape.setOutlineColor(sf::Color::Black);
    CustomShape.setOutlineThickness(1.0f);
}