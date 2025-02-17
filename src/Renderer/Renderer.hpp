#pragma once
#include <SFML/Graphics.hpp>

class Renderer {
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

    static void handleMouseEvent(const sf::Event& event, sf::RenderWindow& window);
    static void render(sf::RenderWindow& window);
    static void setCurrentShape(ShapeType shape);
    static bool hasShape;  // true if a shape is currently stored


private:
    static ShapeType currentShape;
    static bool isDrawing;
    static sf::Vector2f startPos;
    static sf::Vector2f currentPos;

    static void initializeShape(sf::RenderWindow& window);
    static void finishDrawing(sf::RenderWindow& window);
    static void displayCoordinates(sf::RenderWindow& window);
};
