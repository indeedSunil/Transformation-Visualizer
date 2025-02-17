#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP

#include <SFML/Graphics.hpp>
#include "UI/UI.hpp"

class Transformation {
public:
    // Scale functions
    static void scale(sf::RectangleShape& shape, sf::Vector2f scaleFactor);
    static void scale(sf::CircleShape& shape, sf::Vector2f scaleFactor);
    static void scale(sf::ConvexShape& shape, sf::Vector2f scaleFactor);

    // Translate functions
    static void translate(sf::RectangleShape& shape, sf::Vector2f translationFactor);
    static void translate(sf::CircleShape& shape, sf::Vector2f translationFactor);
    static void translate(sf::ConvexShape& shape, sf::Vector2f translationFactor);

    // Rotate functions
    static void rotate(sf::RectangleShape& shape, float angle, sf::Vector2f pivot);
    static void rotate(sf::CircleShape& shape, float angle, sf::Vector2f pivot);
    static void rotate(sf::ConvexShape& shape, float angle, sf::Vector2f pivot);

    // Reflect functions (horizontally or vertically)
    static void reflect(sf::RectangleShape& shape, bool horizontally);
    static void reflect(sf::CircleShape& shape, bool horizontally);
    static void reflect(sf::ConvexShape& shape, bool horizontally);

    // Shear functions
    static void shear(sf::RectangleShape& shape, sf::Vector2f shearFactor);
    static void shear(sf::CircleShape& shape, sf::Vector2f shearFactor);
    static void shear(sf::ConvexShape& shape, sf::Vector2f shearFactor);
};

#endif // TRANSFORMATIONS_HPP
