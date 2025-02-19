#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP

#include <SFML/Graphics.hpp>
#include "UI/UI.hpp"
#include <Matrix/Matrix.hpp>

class Transformation {
public:
    // Scale function
    static void scale(sf::ConvexShape& shape, sf::Vector2f scaleFactor);

    // Translate function
    static void translate(sf::ConvexShape& shape, sf::Vector2f translationFactor);

    // Rotate function
    static void rotate(sf::ConvexShape& shape, float angle, sf::Vector2f pivot);

    // Reflect function (horizontally or vertically)
    static void reflect(sf::ConvexShape& shape, float slope, float yIntercept);

    // Shear function
    static void shear(sf::ConvexShape& shape, sf::Vector2f shearFactor);
};

#endif // TRANSFORMATIONS_HPP
