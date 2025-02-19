#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class Renderer {
public:
    enum class ShapeType {
        None,
        Rectangle,
        Triangle,
        Circle,
        Ellipse,
        Line,
        LetterA,
        CustomRectangle
    };

    static void handleMouseEvent(const sf::Event& event, sf::RenderWindow& window);
    static void render(sf::RenderWindow& window);
    static void setCurrentShape(ShapeType shape);
    static bool hasShape;  // true if a shape is currently stored
    static void changeIsDrawing(const bool newValue){
        isDrawing = newValue;
    };


private:
    static ShapeType currentShape;
    static bool isDrawing;
    static sf::Vector2f startPos;
    static sf::Vector2f currentPos;

    static void initializeShape(sf::RenderWindow& window);
    static void finishDrawing(sf::RenderWindow& window);
    static void displayCoordinates(sf::RenderWindow& window);
};
