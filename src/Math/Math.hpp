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

    static void handleMouseEvent(const sf::Event& event, const sf::RenderWindow& window);
    static void render(sf::RenderWindow& window);
    static void setCurrentShape(ShapeType shape);
    static bool isDrawingEnabled() { return isDrawing; }
    static void setDrawingEnabled(bool enabled) { isDrawing = enabled; }

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
};
