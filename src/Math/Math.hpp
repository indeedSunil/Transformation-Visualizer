#pragma once
#include <SFML/Graphics.hpp>

class Math {
public:
    enum class ShapeType {
        None,
        Rectangle,
        Triangle,
        Circle,
        Ellipse,
        Line,
        LetterA
    };

    static bool isDragging;
    static sf::Vector2f dragOffset;
    static void handleMouseEvent(const sf::Event& event, sf::RenderWindow& window);
    static void render(sf::RenderWindow& window);
    static void setCurrentShape(ShapeType shape);
    static bool isDrawingEnabled() { return isDrawing; }
    static void setDrawingEnabled(bool enabled) { isDrawing = enabled; }
    static bool drawnOnce;
    static bool hasShape;  // true if a shape is currently stored
    static void clearShape()
    {
        hasShape = false;
        rectangle = sf::RectangleShape();
    }

    static bool isSelected;
    static void handleShapeSelection(const sf::Vector2f& mousePos);
    static void transformShape(const std::string& transformationType);

    // Add transformation functions
    static void scaleShape(float scaleX, float scaleY);
    static void translateShape(float dx, float dy);
    static void rotateShape(float angle);
    static void reflectShape(bool horizontally);
    static void shearShape(float shearX, float shearY);


private:
    static ShapeType currentShape;
    static bool isDrawing;
    static sf::Vector2f startPos;
    static sf::Vector2f currentPos;

    // Shape objects
    static sf::RectangleShape rectangle;
    static sf::CircleShape circle;
    static sf::ConvexShape triangle;
    static sf::RectangleShape line;
    // static sf::EllipseShape ellipse;
    static sf::Text letterA;

    static void updateShape();

    static bool isShapeClicked(const sf::Vector2f& mousePos, const sf::FloatRect& bounds);
    static sf::Vector2f getShapeCenter();
    static void handleDragging(const sf::Vector2f& mousePos);
};
