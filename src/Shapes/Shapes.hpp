#pragma once
#include <SFML/Graphics.hpp>
#include "../Renderer/Renderer.hpp"
#include "Core/Core.hpp"

class Shapes
{
public:
    enum class ShapeType // ShapeTypes
    {
        None,
        Rectangle,
        Triangle,
        Circle,
        Ellipse,
        Line,
        LetterA
    };

    // Shapes
    static sf::RectangleShape rectangle;
    static sf::CircleShape circle;
    static sf::ConvexShape triangle;
    static sf::RectangleShape line;
    static sf::Text letterA;

    // For displaying co-ordinates of the shape
    static sf::Text coordinatesText;

    // Variables for handling dragging
    static sf::Vector2f dragOffset;
    static bool isDragging;

    // To check if a shape is currently selected
    static bool isSelected;

    // To check if a shape is being currently drawn
    static bool isDrawing;

    // Functions to control shapes
    static void handleShapeSelection(const sf::Vector2f& mousePos);
    static void setCurrentShape(const ShapeType shape);
    static void drawShape(sf::RenderWindow& window);
    static void updateShape();
    static bool isShapeClicked(const sf::Vector2f& mousePos, const sf::FloatRect& bounds);
    static sf::Vector2f getShapeCenter();
    static void handleDragging(const sf::Vector2f& mousePos);
    static void setShapePosition(const sf::Vector2f& position);
    static sf::Vector2f getShapeSize();
    static sf::Vector2f getShapePosition();
    // static void drawCoordinates(sf::RenderWindow& window);
    static sf::Vector2f currentPos; // Current position of the mouse
    static sf::Vector2f startPos; // Position of the mouse when dragging started

    // Remove the current shape
    static void clearShape()
    {
        isDrawing = false;
        Renderer::hasShape = false;
        rectangle = sf::RectangleShape();
        coordinatesText.setString("");
    }

private:
    static ShapeType currentShape; // Type of current shape

    static void setShapeOutlineColor(const sf::Color& color) {
        switch (currentShape) {
        case ShapeType::Rectangle:
            rectangle.setOutlineColor(color);
            break;
        case ShapeType::Circle:
            circle.setOutlineColor(color);
            break;
        case ShapeType::Triangle:
            triangle.setOutlineColor(color);
            break;
        case ShapeType::Line:
            line.setOutlineColor(color);
            break;
        default:
            break;
        }
    }
};
