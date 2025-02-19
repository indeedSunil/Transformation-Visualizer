#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class Renderer
{
public:
    // Shape types
    enum class ShapeType
    {
        None,
        CustomRectangle,
        CustomTriangle,
        CustomCircle,
        CustomLine,
        CustomEllipse,
    };

    // Handle any event related to mouse
    static void handleMouseEvent(const sf::Event& event, sf::RenderWindow& window);

    // Render the shape
    static void render(sf::RenderWindow& window);

    // Set the current shape
    static void setCurrentShape(ShapeType shape);

    // Check if a shape is currently present in the scene
    static bool hasShape; // true if a shape is currently stored

    // Change the drawing state
    static void changeIsDrawing(const bool newValue)
    {
        isDrawing = newValue;
    };

private:
    // Variables to store the current shape and its state
    static ShapeType currentShape;
    static bool isDrawing;
    static sf::Vector2f startPos;
    static sf::Vector2f currentPos;

    // Initialize the shape
    static void initializeShape(sf::RenderWindow& window);

    // Finish drawing the shape
    static void finishDrawing(sf::RenderWindow& window);

    // Display the co-ordinates of the shape
    static void displayCoordinates(sf::RenderWindow& window);
};
