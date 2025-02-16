#include "Math.hpp"

#include <cmath>

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
bool Math::hasShape = false;
bool Math::isSelected = false;
bool Math::isDragging = false;
sf::Vector2f Math::dragOffset;

void Math::setCurrentShape(const ShapeType shape)
{
    currentShape = shape;
    isDrawing = false; // Reset drawing state when shape changes
}

void Math::handleMouseEvent(const sf::Event& event, sf::RenderWindow& window)
{
    sf::Vector2f mousePos;
    if (event.getIf<sf::Event::MouseButtonPressed>() && isButtonPressed(sf::Mouse::Button::Left))
    {
        mousePos = sf::Vector2f(static_cast<float>(sf::Mouse::getPosition().x),
                                static_cast<float>(sf::Mouse::getPosition().y));

        if (hasShape && !isDrawing)
        {
            sf::FloatRect bounds = rectangle.getGlobalBounds();
            if (isShapeClicked(mousePos, bounds))
            {
                std::cout << "Shape Selected" << std::endl;
                isSelected = true;
                isDragging = true;
                rectangle.setOutlineColor(sf::Color::Yellow);

                // Calculate the offset between mouse position and shape position
                dragOffset = sf::Vector2f(
                    mousePos.x - rectangle.getPosition().x,
                    mousePos.y - rectangle.getPosition().y
                );
            }
            else
            {
                isSelected = false;
                isDragging = false;
                rectangle.setOutlineColor(sf::Color::White);
            }
        }
        else if (!hasShape)
        {
            // Convert window coordinates to graph coordinates
            startPos = UI::windowToGraph(mousePos);
            currentPos = startPos;
            isDrawing = true;

            // Initialize shape based on type
            switch (currentShape)
            {
            case ShapeType::Rectangle:
                rectangle.setPosition(UI::graphToWindow(startPos));
                rectangle.setSize(sf::Vector2f(0, 0));
                rectangle.setFillColor(sf::Color::Transparent);
                rectangle.setOutlineColor(sf::Color::White);
                rectangle.setOutlineThickness(2);
                break;
            default:
                break;
            // Add other shape initializations here
            }
        }
    }
    else if (event.getIf<sf::Event::MouseMoved>())
    {
        const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>();
        mousePos = sf::Vector2f(static_cast<float>(mouseMoved->position.x),
                                static_cast<float>(mouseMoved->position.y));
        if (isDragging && isSelected)
        {
            std::cout << "Selected and dragging" << std::endl;
            handleDragging(mousePos);
        }
        else if (isDrawing)
        {
            std::cout << "Drawing" << std::endl;
            // Existing shape drawing logic
            currentPos = UI::windowToGraph(mousePos);
            updateShape();
        }
    }
    else if (event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (isDragging)
        {
            isDragging = false;
        }
        if (isDrawing)
        {
            switch (currentShape)
            {
            case ShapeType::Rectangle:
                {
                    rectangle.setFillColor(sf::Color(255, 255, 255, 0));
                    break;
                }
            default:
                break;
            }
            isDrawing = false;
            hasShape = true;
        }
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
    if (hasShape || isDrawing)
    {
        window.draw(rectangle);
    }
    // if (!isDrawing) return;

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

void Math::handleShapeSelection(const sf::Vector2f& mousePos)
{
    if (hasShape)
    {
        std::cout << "handleShapeSelection function called" << std::endl;
        sf::FloatRect bounds = rectangle.getGlobalBounds();
        if (isShapeClicked(mousePos, bounds))
        {
            isSelected = true;
            rectangle.setOutlineColor(sf::Color::Yellow); // Visual feedback for selection
        }
        else
        {
            isSelected = false;
            rectangle.setOutlineColor(sf::Color::White);
        }
    }
}

bool Math::isShapeClicked(const sf::Vector2f& mousePos, const sf::FloatRect& bounds)
{
    return bounds.contains(mousePos);
}

sf::Vector2f Math::getShapeCenter()
{
    sf::FloatRect bounds = rectangle.getGlobalBounds();
    return sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f);
}

void Math::transformShape(const std::string& transformationType)
{
    std::cout << "hasShape: " << hasShape << " isSelected: " << isSelected << std::endl;
    if (!isSelected || !hasShape) return;

    if (transformationType == "Scale")
    {
        scaleShape(1.5f, 1.5f); // Example: Scale by 1.5
    }
    else if (transformationType == "Translate")
    {
        translateShape(UI::GRID_SIZE, 0); // Example: Move right by one grid unit
    }
    else if (transformationType == "Rotate")
    {
        rotateShape(45.f); // Example: Rotate 45 degrees
    }
    else if (transformationType == "Reflect")
    {
        reflectShape(true); // Example: Reflect horizontally
    }
    else if (transformationType == "Shear")
    {
        shearShape(0.5f, 0.0f); // Example: Shear horizontally by 0.5
    }
}


void Math::scaleShape(float scaleX, float scaleY)
{
    std::cout << "Scaling" << std::endl;
    const sf::Vector2f currentSize = rectangle.getSize();
    rectangle.setSize(sf::Vector2f(currentSize.x * scaleX, currentSize.y * scaleY));
    // clearShape();
    // window.draw(rectangle);
}

void Math::translateShape(float dx, float dy)
{
    const sf::Vector2f currentPos = rectangle.getPosition();
    rectangle.setPosition({currentPos.x + dx, currentPos.y + dy});
}

void Math::rotateShape(float angle)
{
    const sf::Vector2f center = getShapeCenter();
    rectangle.setOrigin({rectangle.getSize().x / 2.f, rectangle.getSize().y / 2.f});
    rectangle.setPosition(center);
    rectangle.rotate(sf::radians(angle));
}

void Math::reflectShape(const bool horizontally)
{
    sf::Vector2f scale = rectangle.getScale();
    if (horizontally)
    {
        rectangle.setScale({-scale.x, scale.y});
    }
    else
    {
        rectangle.setScale({scale.x, -scale.y});
    }
}


void Math::shearShape(const float shearX, const float shearY)
{
    // Note: SFML doesn't directly support shearing,
    // you would need to implement a custom transform matrix
    // This is a simplified version that scales one side
    sf::Vector2f size = rectangle.getSize();
    if (shearX != 0.0f)
    {
        rectangle.setSize(sf::Vector2f(size.x + size.y * shearX, size.y));
    }
    if (shearY != 0.0f)
    {
        rectangle.setSize(sf::Vector2f(size.x, size.y + size.x * shearY));
    }
}


void Math::handleDragging(const sf::Vector2f& mousePos)
{
    // Calculate new position based on mouse position and drag offset
    sf::Vector2f newPos = sf::Vector2f(
        mousePos.x - dragOffset.x,
        mousePos.y - dragOffset.y
    );

    // Optional: Snap to grid
    float gridSize = static_cast<float>(UI::GRID_SIZE);
    newPos.x = std::round(newPos.x / gridSize) * gridSize;
    newPos.y = std::round(newPos.y / gridSize) * gridSize;

    const sf::Vector2f& origin = UI::getOrigin();
    const sf::Vector2f shapeSize = rectangle.getSize();

    // Left boundary
    newPos.x = std::max(newPos.x, UI::dividerLinePositionX);
    // Right boundary
    newPos.x = std::min(newPos.x, static_cast<float>(UI::windowSize.x) - shapeSize.x);
    // Top boundary
    newPos.y = std::max(newPos.y, 0.0f);
    // Bottom boundary
    newPos.y = std::min(newPos.y, static_cast<float>(UI::windowSize.y) - shapeSize.y);

    // Update rectangle position
    rectangle.setPosition(newPos);
}
