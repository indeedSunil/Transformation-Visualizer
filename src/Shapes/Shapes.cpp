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
sf::VertexArray Shapes::LinePoints(sf::PrimitiveType::LineStrip);

// Custom shape
sf::ConvexShape Shapes::CustomShape;

sf::VertexArray Shapes::FillPoints(sf::PrimitiveType::Lines);

// Initialize the shape
void Shapes::setCurrentShape(const ShapeType shape)
{
    currentShape = shape;
    isDrawing = false;
}

void Shapes::drawLineBresenham(sf::RenderWindow& window, const int x1, const int y1, const int x2, const int y2,
                               const sf::Color color)
{
    LinePoints.clear();

    // Add start point
    sf::Vertex startVertex;
    startVertex.position = sf::Vector2f(static_cast<float>(x1), static_cast<float>(y1));
    startVertex.color = color;
    LinePoints.append(startVertex);

    // Add end point
    sf::Vertex endVertex;
    endVertex.position = sf::Vector2f(static_cast<float>(x2), static_cast<float>(y2));
    endVertex.color = color;
    LinePoints.append(endVertex);
}


// Get the circle points using the midpoint algorithm
std::vector<sf::Vector2f> Shapes::getMidPointCirclePoints(int centerX, int centerY, int radius)
{
    std::vector<sf::Vector2f> points;

    int x = radius;
    int y = 0;
    int p = 1 - radius;

    // First collect all points using midpoint algorithm
    while (x >= y)
    {
        points.push_back(sf::Vector2f(centerX + x, centerY + y));
        points.push_back(sf::Vector2f(centerX - x, centerY + y));
        points.push_back(sf::Vector2f(centerX + x, centerY - y));
        points.push_back(sf::Vector2f(centerX - x, centerY - y));
        points.push_back(sf::Vector2f(centerX + y, centerY + x));
        points.push_back(sf::Vector2f(centerX - y, centerY + x));
        points.push_back(sf::Vector2f(centerX + y, centerY - x));
        points.push_back(sf::Vector2f(centerX - y, centerY - x));

        y++;
        if (p <= 0)
        {
            p = p + 2 * y + 1;
        }
        else
        {
            x--;
            p = p + 2 * y - 2 * x + 1;
        }
    }

    // Now sort points based on their angle from center
    std::sort(points.begin(), points.end(),
              [centerX, centerY](const sf::Vector2f& a, const sf::Vector2f& b)
              {
                  // Calculate angles using atan2
                  float angleA = std::atan2(a.y - centerY, a.x - centerX);
                  float angleB = std::atan2(b.y - centerY, b.x - centerX);
                  return angleA < angleB;
              });

    // Remove duplicates if any
    points.erase(std::unique(points.begin(), points.end(),
                             [](const sf::Vector2f& a, const sf::Vector2f& b)
                             {
                                 return a.x == b.x && a.y == b.y;
                             }), points.end());

    // Close the circle by adding the first point at the end
    if (!points.empty())
    {
        points.push_back(points[0]);
    }

    return points;
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
            const sf::Vector2f point1 = startPos; // First point stays at start position
            sf::Vector2f point2 = currentPos; // Second point follows mouse

            // Calculate the third point to form an isosceles triangle
            const float dx = point2.x - point1.x;
            const float dy = point2.y - point1.y;
            const float length = std::sqrt(dx * dx + dy * dy);

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
            const sf::Vector2f perpendicular(-dy, dx); // Rotate 90 degrees
            const float height = length * 0.866f; // height = base * tan(60°)
            const sf::Vector2f point3 = startPos + perpendicular * (height / length);

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
        const float dx = currentPos.x - startPos.x;
        const float dy = currentPos.y - startPos.y;
        float radius = std::sqrt(dx * dx + dy * dy);

        // Ensure minimum radius
        if (radius < 1.0f) radius = 1.0f;

        // Update all points to form circle
        for (int i = 0; i < CIRCLE_POINTS; ++i)
        {
            const auto angle = static_cast<float>((i * 2 * M_PI) / CIRCLE_POINTS);
            const auto x = static_cast<float>(startPos.x + radius * cos(angle));
            const auto y = static_cast<float>(startPos.y + radius * sin(angle));
            CustomShape.setPoint(i, sf::Vector2f(x, y));
        }
        break;
    }

    // Using midpoint circle algorithm
    // case ShapeType::CustomCircle:
    //     {
    //         if (currentPos == startPos)
    //         {
    //             std::cout << "Error cause currentPos == startPos\n";
    //             return;
    //         }
    //
    //         const float dx = currentPos.x - startPos.x;
    //         const float dy = currentPos.y - startPos.y;
    //         float radius = std::sqrt(dx * dx + dy * dy);
    //
    //         if (radius < 1.0f) radius = 1.0f;
    //
    //         std::vector<sf::Vector2f> circlePoints = getMidPointCirclePoints(
    //             static_cast<int>(startPos.x),
    //             static_cast<int>(startPos.y),
    //             static_cast<int>(radius)
    //         );
    //
    //         CustomShape.setPointCount(circlePoints.size());
    //         for (size_t i = 0; i < circlePoints.size(); ++i)
    //         {
    //             CustomShape.setPoint(i, circlePoints[i]);
    //         }
    //         break;
    //     }

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
                const auto angle = static_cast<float>((i * 2 * M_PI) / CIRCLE_POINTS);
                const auto x = static_cast<float>(startPos.x + rx * cos(angle));
                const auto y = static_cast<float>(startPos.y + ry * sin(angle));
                CustomShape.setPoint(i, sf::Vector2f(x, y));
            }
            break;
        }
    case ShapeType::CustomLine:
        {
            if (currentPos == startPos)
            {
                std::cout << "Error: currentPos == startPos\n";
                return;
            }

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
    case ShapeType::CustomLine:
        window.draw(LinePoints); // Draw the stored line points
        break;
    default:
        window.draw(CustomShape);
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
            const sf::Vector2f center = startPos;

            // Calculate radii from the points
            float rx = 0.0f, ry = 0.0f;
            for (size_t i = 0; i < CustomShape.getPointCount(); ++i)
            {
                const sf::Vector2f point = CustomShape.getPoint(i);
                rx = std::max(rx, std::abs(point.x - center.x));
                ry = std::max(ry, std::abs(point.y - center.y));
            }

            // Check if point is inside ellipse using the equation (x-h)²/a² + (y-k)²/b² ≤ 1
            // where (h,k) is the center, a is rx, and b is ry
            const float normalizedX = (mousePos.x - center.x) / rx;
            const float normalizedY = (mousePos.y - center.y) / ry;
            const float result = (normalizedX * normalizedX) + (normalizedY * normalizedY);

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
        const sf::Vector2f center = getShapeCenter();
        const float radius = (bounds.size.x / 2.0f); // Assuming circle is not scaled
        const float dx = mousePos.x - center.x;
        const float dy = mousePos.y - center.y;
        return (dx * dx + dy * dy) <= (radius * radius);
    }
    else if (currentShape == ShapeType::CustomEllipse)
    {
        // For ellipse, check using the ellipse equation
        sf::Vector2f center = getShapeCenter();
        const float rx = bounds.size.x / 2.0f; // Semi-major axis
        const float ry = bounds.size.y / 2.0f; // Semi-minor axis

        // Using ellipse equation: (x-h)²/a² + (y-k)²/b² ≤ 1
        const float normalizedX = (mousePos.x - center.x) / rx;
        const float normalizedY = (mousePos.y - center.y) / ry;
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
    const sf::Vector2f newPos = mousePos - dragOffset;

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


// Fill the shape with a color
void Shapes::fillColorInShape(sf::RenderWindow& window, const sf::Color& color)
{
    if (currentShape == ShapeType::CustomLine || currentShape == ShapeType::None) return;
    CustomShape.setFillColor(color);
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
}


// Fill the shape with a color
void Shapes::fillColourInShape(sf::RenderWindow& window, const sf::Color color)
{
    if (currentShape == ShapeType::CustomLine || currentShape == ShapeType::None) return;

    // Get the bounds of the shape
    sf::FloatRect bounds = CustomShape.getLocalBounds();

    // Create a vertex array for filling
    sf::VertexArray fill(sf::PrimitiveType::Points);

    // Simple horizontal scan lines
    for (float y = bounds.position.y; y < bounds.position.y + bounds.size.y; y += 1.0f)
    {
        for (float x = bounds.position.x; x < bounds.position.x + bounds.size.x; x += 1.0f)
        {
            // Create a point
            sf::Vertex point;
            point.position = sf::Vector2f(x, y);
            point.color = color;

            // Add the point to fill
            fill.append(point);
        }
    }

    // Draw the fill
    window.draw(fill);
}
