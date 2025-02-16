#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP

#include <SFML/Graphics.hpp>

class Transformation {
public:
    static void scale(sf::RectangleShape& shape, sf::Vector2f scaleFactor);
    static void translate(sf::RectangleShape& shape, sf::Vector2f translationFactor);
    static void rotate(sf::RectangleShape& shape, float angle);
    static void reflect(sf::RectangleShape& shape, bool horizontally);
    static void shear(sf::RectangleShape& shape, sf::Vector2f shearFactor);
};

#endif // TRANSFORMATIONS_HPP
