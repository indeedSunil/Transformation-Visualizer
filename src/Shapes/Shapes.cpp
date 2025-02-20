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
sf::VertexArray Shapes::linePoint(sf::PrimitiveType::Points);

// Custom shape
sf::ConvexShape Shapes::CustomShape;

// Initialize the shape
void Shapes::setCurrentShape(const ShapeType shape)
{
    currentShape = shape;
    isDrawing = false;
}

void Shapes::drawLineBresenham(sf::RenderWindow& window, int x1, int y1, const int x2, const int y2,
                               const sf::Color color)
{
    sf::VertexArray points(sf::PrimitiveType::Points);

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    const int sx = (x1 < x2) ? 1 : -1;
    const int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        // Using aggregate initialization for vertex
        points.append({{static_cast<float>(x1), static_cast<float>(y1)}, color});

        if (x1 == x2 && y1 == y2) break;
        std::cout << "Line being drawn at " << x1 << ", " << y1 << std::endl;
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }

    window.draw(points);
}

// Update the shape according to the mouse position if the shape is clicked and hold and dragged
void Shapes::updateShape(sf::RenderWindow& window)
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

            // Set a minimum length to prevent division by zero
            const float MIN_LENGTH = 1.0f;
            if (length < MIN_LENGTH)
            {
                // If triangle is too small, create a minimum size triangle
                point2 = startPos + sf::Vector2f(MIN_LENGTH, 0);
                sf::Vector2f perpendicular(0, -MIN_LENGTH);
                sf::Vector2f point3 = startPos + perpendicular;

                CustomShape.setPoint(0, point1);
                CustomShape.setPoint(1, point2);
                CustomShape.setPoint(2, point3);
                break;
            }

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
    case ShapeType::CustomEllipse:
        {
            if (currentPos == startPos)
            {
                std::cout << "Error cause currentPos == startPos\n";
                return;
            }
            // Calculate radii based on distance from start point to current mouse position
            float rx = std::abs(currentPos.x - startPos.x);
            float ry = std::abs(currentPos.y - startPos.y);

            // Ensure minimum radii
            if (rx < 1.0f) rx = 1.0f;
            if (ry < 1.0f) ry = 1.0f;

            // Update all points to form ellipse
            for (int i = 0; i < CIRCLE_POINTS; ++i)
            {
                float angle = (i * 2 * M_PI) / CIRCLE_POINTS;
                float x = startPos.x + rx * cos(angle);
                float y = startPos.y + ry * sin(angle);
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

            // Use coordinates directly since view handles scaling
            drawLineBresenham(window,
                              static_cast<int>(startPos.x), static_cast<int>(startPos.y),
                              static_cast<int>(currentPos.x), static_cast<int>(currentPos.y),
                              sf::Color::Black);
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
    switch (currentShape)
    {
    case ShapeType::CustomRectangle:
    case ShapeType::CustomTriangle:
    case ShapeType::CustomCircle:
    case ShapeType::CustomEllipse:
        window.draw(CustomShape);
        break;
    // case ShapeType::CustomLine:
    //     break;
    default:
        break;
    }
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
            break;
        }
    case ShapeType::CustomEllipse:
        {
            // Get the center (startPos) and radii of the ellipse
            sf::Vector2f center = startPos;

            // Calculate radii from the points
            float rx = 0.0f, ry = 0.0f;
            for (size_t i = 0; i < CustomShape.getPointCount(); ++i)
            {
                sf::Vector2f point = CustomShape.getPoint(i);
                rx = std::max(rx, std::abs(point.x - center.x));
                ry = std::max(ry, std::abs(point.y - center.y));
            }

            // Check if point is inside ellipse using the equation (x-h)²/a² + (y-k)²/b² ≤ 1
            // where (h,k) is the center, a is rx, and b is ry
            float normalizedX = (mousePos.x - center.x) / rx;
            float normalizedY = (mousePos.y - center.y) / ry;
            float result = (normalizedX * normalizedX) + (normalizedY * normalizedY);

            // Add some tolerance for easier selection (1.1 instead of 1.0)
            if (result <= 1.1f)
            {
                isSelected = true;
                setShapeOutlineColor(sf::Color::Green);
            }
            else
            {
                isSelected = false;
                setShapeOutlineColor(sf::Color::Black);
            }
            return;
        }
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
    else if (currentShape == ShapeType::CustomEllipse)
    {
        // For ellipse, check using the ellipse equation
        sf::Vector2f center = getShapeCenter();
        float rx = bounds.size.x / 2.0f; // Semi-major axis
        float ry = bounds.size.y / 2.0f; // Semi-minor axis

        // Using ellipse equation: (x-h)²/a² + (y-k)²/b² ≤ 1
        float normalizedX = (mousePos.x - center.x) / rx;
        float normalizedY = (mousePos.y - center.y) / ry;
        return (normalizedX * normalizedX + normalizedY * normalizedY) <= 1.1f; // 1.1f for better selection
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
    case ShapeType::CustomEllipse:
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
    case ShapeType::CustomEllipse:
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
    case ShapeType::CustomEllipse:
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
