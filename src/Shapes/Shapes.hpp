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
        CustomRectangle,
        CustomTriangle,
        CustomCircle,
        CustomLine,
        CustomEllipse,
    };

    // Custom shape
    static sf::ConvexShape CustomShape;
    static sf::VertexArray linePoint;

    // Number of points for a cirlc
    static constexpr int CIRCLE_POINTS = 32;  // More points = smoother circle

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
    static void updateShape(sf::RenderWindow& window);
    static bool isShapeClicked(const sf::Vector2f& mousePos, const sf::FloatRect& bounds);
    static sf::Vector2f getShapeCenter();
    static void handleDragging(const sf::Vector2f& mousePos);
    static void setShapePosition(const sf::Vector2f& position);
    static sf::Vector2f getShapePosition();

    static sf::Vector2f currentPos; // Current position of the mouse
    static sf::Vector2f startPos; // Position of the mouse when dragging started

    // Add Bresenham line drawing function
    static void drawLineBresenham(sf::RenderWindow& window, int x1, int y1, int x2, int y2, sf::Color color = sf::Color::Black);

    // Remove the current shape
    static void clearShape();

private:
    static ShapeType currentShape; // Type of current shape

    static void setShapeOutlineColor(const sf::Color& color)
    {
            CustomShape.setOutlineColor(color);
    }
};
