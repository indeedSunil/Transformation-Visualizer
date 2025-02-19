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


// Custom shape
sf::ConvexShape Shapes::CustomShape;

// Initialize the shape
void Shapes::setCurrentShape(const ShapeType shape)
{
    currentShape = shape;
    isDrawing = false;
}

// Update the shape according to the mouse position if the shape is clicked and hold and dragged
void Shapes::updateShape()
{
    switch (currentShape)
    {
    case ShapeType::CustomRectangle:
        {
            sf::Vector2f size = currentPos - startPos;
            if (currentPos == startPos)
            {
                std::cout << "Error cause currentPos == startPos\n";
                return;
            }
            // Ensure minimum size is larger
            if (std::abs(size.x) < 1.0f) size.x = 1.0f;
            if (std::abs(size.y) < 1.0f) size.y = 1.0f;

            CustomShape.setPoint(0, startPos);
            CustomShape.setPoint(1, startPos + sf::Vector2f(size.x, 0));
            CustomShape.setPoint(2, startPos + size);
            CustomShape.setPoint(3, startPos + sf::Vector2f(0, size.y));
            break;
        }
    case ShapeType::CustomTriangle:
        {
            sf::Vector2f mouseOffset = currentPos - startPos;
            if (currentPos == startPos)
            {
                std::cout << "Error cause currentPos == startPos\n";
                return;
            }
            // Calculate the three points of the triangle
            sf::Vector2f point1 = startPos; // First point stays at start position
            sf::Vector2f point2 = currentPos; // Second point follows mouse

            // Calculate the third point to form an isosceles triangle
            float dx = point2.x - point1.x;
            float dy = point2.y - point1.y;
            float length = std::sqrt(dx * dx + dy * dy);

            if (length < 0.1f) length = 0.1f;

            // Calculate the third point using perpendicular vector
            sf::Vector2f perpendicular(-dy, dx); // Rotate 90 degrees
            float height = length * 0.866f; // height = base * tan(60°)
            sf::Vector2f point3 = startPos + perpendicular * (height / length);

            CustomShape.setPoint(0, point1);
            CustomShape.setPoint(1, point2);
            CustomShape.setPoint(2, point3);
            break;
        }
    case ShapeType::CustomCircle:
        {
            if (currentPos == startPos)
            {
                std::cout << "Error cause currentPos == startPos\n";
                return;
            }
            // Calculate radius based on distance from start point to current mouse position
            float dx = currentPos.x - startPos.x;
            float dy = currentPos.y - startPos.y;
            float radius = sqrt(dx * dx + dy * dy);

            // Ensure minimum radius
            if (radius < 1.0f) radius = 1.0f;

            // Update all points to form circle
            for (int i = 0; i < CIRCLE_POINTS; ++i)
            {
                float angle = (i * 2 * M_PI) / CIRCLE_POINTS;
                float x = startPos.x + radius * cos(angle);
                float y = startPos.y + radius * sin(angle);
                CustomShape.setPoint(i, sf::Vector2f(x, y));
            }
            break;
        }
    case ShapeType::CustomLine:
        {
            if (currentPos == startPos)
            {
                std::cout << "Error cause currentPos == startPos\n";
                return;
            }
            // Update end point
            sf::Vector2f start = CustomShape.getPoint(0);
            sf::Vector2f end = currentPos;

            // Calculate line direction vector
            sf::Vector2f direction = end - start;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            // Normalize direction vector
            if (length > 0)
            {
                direction.x /= length;
                direction.y /= length;
            }

            // Calculate perpendicular vector for thickness
            sf::Vector2f perpendicular(-direction.y, direction.x);
            float thickness = 0.05f; // Adjust this value to change line thickness

            // Calculate the four corners of the thick line
            CustomShape.setPointCount(4);
            CustomShape.setPoint(0, start);
            CustomShape.setPoint(1, end + perpendicular * thickness);
            CustomShape.setPoint(2, end - perpendicular * thickness);
            CustomShape.setPoint(3, start - perpendicular * thickness);
            break;
        }
    default:
        break;
    }
}


// Draw the shape
void Shapes::drawShape(sf::RenderWindow& window)
{
    window.setView(Core::getGraphView());
    window.draw(CustomShape);
    // switch (currentShape)
    // {
    // case ShapeType::CustomRectangle:
    //     window.draw(CustomShape);
    //     break;
    // case ShapeType::CustomTriangle:
    //     window.draw(CustomShape);
    //     break;
    // default:
    //     break;
    // }
}

// Handle the selection of the shape
void Shapes::handleShapeSelection(const sf::Vector2f& mousePos)
{
    sf::FloatRect bounds;
    switch (currentShape)
    {
    case ShapeType::CustomRectangle:
    case ShapeType::CustomTriangle:
    case ShapeType::CustomCircle:
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


// Check if the shape is clicked by checking if the mouse click is inside the bounds of the shape
bool Shapes::isShapeClicked(const sf::Vector2f& mousePos, const sf::FloatRect& bounds)
{
    if (currentShape == ShapeType::CustomCircle)
    {
        // For circle, check if point is within radius
        sf::Vector2f center = getShapeCenter();
        float radius = (bounds.size.x / 2.0f); // Assuming circle is not scaled
        float dx = mousePos.x - center.x;
        float dy = mousePos.y - center.y;
        return (dx * dx + dy * dy) <= (radius * radius);
    }
    return bounds.contains(mousePos);
}

sf::Vector2f Shapes::getShapeCenter()
{
    sf::FloatRect bounds;
    switch (currentShape)
    {
    case ShapeType::CustomRectangle:
    case ShapeType::CustomTriangle:
    case ShapeType::CustomCircle:
        bounds = CustomShape.getGlobalBounds();
        break;
    default:
        return {0, 0};
    }
    return {bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f};
}


// Handle dragging of the shape
void Shapes::handleDragging(const sf::Vector2f& mousePos)
{
    if (!isSelected || !isDragging) return;

    // Calculate new position
    sf::Vector2f newPos = mousePos - dragOffset;

    // Snap the new position to grid points
    // newPos.x = std::floor(newPos.x);
    // newPos.y = std::floor(newPos.y);

    setShapePosition(newPos);
}


// Set the position of the shape
void Shapes::setShapePosition(const sf::Vector2f& position)
{
    switch (currentShape)
    {
    case ShapeType::CustomRectangle:
    case ShapeType::CustomTriangle:
    case ShapeType::CustomLine:
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
    case ShapeType::CustomCircle:
        {
            // Get current center from existing points
            // sf::Vector2f currentCenter = getShapeCenter();
            // sf::Vector2f offset = position - currentCenter;

            sf::Vector2f currentTopLeft = CustomShape.getPoint(0);
            sf::Vector2f offset = position - currentTopLeft;

            // Move all points by the offset
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


// Get the position of the shape
sf::Vector2f Shapes::getShapePosition()
{
    switch (currentShape)
    {
    case ShapeType::CustomRectangle:
    case ShapeType::CustomTriangle:
    case ShapeType::CustomCircle:
        return CustomShape.getPoint(0);
    default:
        return {0, 0};
    }
}


// Remove the shape from the view
void Shapes::clearShape()

{
    isDrawing = false;
    Renderer::changeIsDrawing(false);

    isSelected = false; // Reset selection state
    isDragging = false;

    Renderer::hasShape = false;
    coordinatesText.setString("");
    currentShape = ShapeType::None; // Reset shape type

    // // Properly reinitialize CustomShape
    // CustomShape = sf::ConvexShape(4); // Create new shape with 4 points
    // CustomShape.setPoint(0, sf::Vector2f(0, 0));
    // CustomShape.setPoint(1, sf::Vector2f(0.1f, 0.1f)); // Use 1.0f instead of 0.1f
    // CustomShape.setPoint(2, sf::Vector2f(0.1f, 0.1f));
    // CustomShape.setPoint(3, sf::Vector2f(0, 0.1f));
    // CustomShape.setFillColor(sf::Color::Transparent);
    // CustomShape.setOutlineColor(sf::Color::Black);
    // CustomShape.setOutlineThickness(0.1f);
}
