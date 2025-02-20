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
                // sf::Vector2f snappedMousePos = {
                //     std::floor(mousePos.x),
                //     std::floor(mousePos.y)
                // };
                sf::Vector2f snappedMousePos = mousePos;
                // sf::Vector2f snappedShapePos = {
                //     std::floor(Shapes::getShapePosition().x),
                //     std::floor(Shapes::getShapePosition().y)
                // };
                sf::Vector2f snappedShapePos = Shapes::getShapePosition();
                Shapes::dragOffset = snappedMousePos - snappedShapePos;
            }
        }
        else if (!hasShape)
        {
            // startPos = {
            //     std::floor(mousePos.x),
            //     std::floor(mousePos.y)
            // };
            startPos = mousePos;
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
            // currentPos = {
            //     std::floor(mousePos.x),
            //     std::floor(mousePos.y)
            // };
            currentPos = mousePos;
            Shapes::currentPos = currentPos;
            Shapes::updateShape(window);
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
            Shapes::CustomShape.setPoint(1, startPos + sf::Vector2f(1.f, 0.f));
            Shapes::CustomShape.setPoint(2, startPos + sf::Vector2f(1.f, 1.f));
            Shapes::CustomShape.setPoint(3, startPos + sf::Vector2f(0.f, 1.f));
            Shapes::CustomShape.setFillColor(sf::Color::Transparent);
            Shapes::CustomShape.setOutlineColor(sf::Color::Black);
            Shapes::CustomShape.setOutlineThickness(0.1f);
            window.draw(Shapes::CustomShape);
            break;
        }
    case ShapeType::CustomTriangle:
        {
            Shapes::CustomShape.setPointCount(3); // Triangle has 3 points
            Shapes::CustomShape.setPoint(0, startPos); // First point at click position
            // Initialize with a small equilateral triangle
            Shapes::CustomShape.setPoint(1, startPos + sf::Vector2f(2.f, 0.f)); // Second point to the right
            Shapes::CustomShape.setPoint(2, startPos + sf::Vector2f(1.f, 1.732f)); // Third point (using 60° angle)
            Shapes::CustomShape.setFillColor(sf::Color::Transparent);
            Shapes::CustomShape.setOutlineColor(sf::Color::Black);
            Shapes::CustomShape.setOutlineThickness(0.1f);
            window.draw(Shapes::CustomShape);
            break;
        }
    case ShapeType::CustomCircle:
        {
            Shapes::CustomShape.setPointCount(Shapes::CIRCLE_POINTS);
            // Initially set a small circle
            const float radius = 1.0f;
            for (int i = 0; i < Shapes::CIRCLE_POINTS; ++i)
            {
                const float angle = static_cast<float>(i * 2 * M_PI) / Shapes::CIRCLE_POINTS;
                const float x = (startPos.x) + radius * cos(static_cast<double>(angle));
                const float y = (startPos.y) + radius * sin(static_cast<double>(angle));
                Shapes::CustomShape.setPoint(i, sf::Vector2f(x, y));
            }
            Shapes::CustomShape.setFillColor(sf::Color::Transparent);
            Shapes::CustomShape.setOutlineColor(sf::Color::Black);
            Shapes::CustomShape.setOutlineThickness(0.1f);
            window.draw(Shapes::CustomShape);
            break;
        }
    case ShapeType::CustomEllipse:
        {
            Shapes::CustomShape.setPointCount(Shapes::CIRCLE_POINTS);
            // Initially set a small ellipse
            const float rx = 1.0f;  // radius in x direction
            const float ry = 0.5f;  // radius in y direction (make it half to see the ellipse effect)

            for (int i = 0; i < Shapes::CIRCLE_POINTS; ++i)
            {
                const float angle = static_cast<float>(i * 2 * M_PI) / Shapes::CIRCLE_POINTS;
                const auto x = static_cast<float>(startPos.x + rx * cos(static_cast<double>(angle)));
                const auto y = static_cast<float>(startPos.y + ry * sin(static_cast<double>(angle)));
                Shapes::CustomShape.setPoint(i, sf::Vector2f(x, y));
            }

            Shapes::CustomShape.setFillColor(sf::Color::Transparent);
            Shapes::CustomShape.setOutlineColor(sf::Color::Black);
            Shapes::CustomShape.setOutlineThickness(0.1f);
            window.draw(Shapes::CustomShape);
            break;
        }
    case ShapeType::CustomLine:
        {
            Shapes::drawLineBresenham(window, startPos.x, startPos.y, startPos.x + 1, startPos.y, sf::Color::Black);
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
    case ShapeType::CustomTriangle:
    case ShapeType::CustomCircle:
    case ShapeType::CustomEllipse:
        Shapes::CustomShape.setFillColor(sf::Color::Transparent);
        window.draw(Shapes::CustomShape);
        break;
    case ShapeType::CustomLine:
        Shapes::drawLineBresenham(window, startPos.x, startPos.y, currentPos.x, currentPos.y, sf::Color::Black);
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
    case ShapeType::CustomTriangle:
        {
            const int pointCount = Shapes::CustomShape.getPointCount();

            oss << "Co-ordinates of "
                << (currentShape == ShapeType::CustomRectangle ? "Rectangle" : "Triangle")
                << ":\n";

            for (int i = 0; i < pointCount; ++i)
            {
                const sf::Vector2f point = Shapes::CustomShape.getPoint(i);
                oss << static_cast<char>('A' + i) << ": ("
                    << point.x << ", " << point.y << ")"
                    << (i < pointCount - 1 ? "\n" : ""); // Add newline except for last point
            }
            break;
        }
    case ShapeType::CustomCircle:
        {
            sf::Vector2f center = Shapes::getShapeCenter();
            // Calculate radius (distance from center to any point)
            sf::Vector2f firstPoint = Shapes::CustomShape.getPoint(0);
            float dx = firstPoint.x - center.x;
            float dy = firstPoint.y - center.y;
            float radius = std::sqrt(dx * dx + dy * dy);

            oss << "Circle Properties:\n";
            oss << "Center: (" << center.x << ", " << center.y << ")\n";
            oss << "Radius: " << radius;
            break;
        }
    case ShapeType::CustomEllipse:
        {
            sf::Vector2f center = Shapes::getShapeCenter();
            // Calculate semi-major and semi-minor axes
            float rx = 0.0f, ry = 0.0f;
            for (size_t i = 0; i < Shapes::CustomShape.getPointCount(); ++i)
            {
                sf::Vector2f point = Shapes::CustomShape.getPoint(i);
                rx = std::max(rx, std::abs(point.x - center.x));
                ry = std::max(ry, std::abs(point.y - center.y));
            }

            oss << "Ellipse Properties:\n";
            oss << "Center: (" << center.x << ", " << center.y << ")\n";
            oss << "Semi-major axis: " << std::max(rx, ry) << "\n";
            oss << "Semi-minor axis: " << std::min(rx, ry);
            break;
        }
    case ShapeType::CustomLine:
        {
            oss << "Line Properties:\n";
            if (Shapes::LinePoints.getVertexCount() >= 2) {
                const sf::Vector2f startPoint = Shapes::LinePoints[0].position;
                const sf::Vector2f endPoint = Shapes::LinePoints[1].position;
                oss << "Start Point: (" << startPoint.x << ", " << startPoint.y << ")\n";
                oss << "End Point: (" << endPoint.x << ", " << endPoint.y << ")";
            }
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