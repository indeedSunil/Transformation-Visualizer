#include "transformations.hpp"

void Transformation::scale(sf::RectangleShape& shape, const sf::Vector2f scaleFactor) {
    const sf::Vector2f currentSize = shape.getSize();
    shape.setSize(sf::Vector2f(currentSize.x * scaleFactor.x, currentSize.y * scaleFactor.y));
}

void Transformation::translate(sf::RectangleShape& shape, const sf::Vector2f translationFactor) {
    const sf::Vector2f pos = shape.getPosition();
    shape.setPosition(sf::Vector2f(pos.x + translationFactor.x, pos.y + translationFactor.y));
}

void Transformation::rotate(sf::RectangleShape& shape, const float angle) {
    const sf::Vector2f size = shape.getSize();
    shape.setOrigin({size.x / 2.f, size.y / 2.f});
    shape.rotate(sf::degrees(angle));
}

void Transformation::reflect(sf::RectangleShape& shape, const bool horizontally) {
    const sf::Vector2f currentScale = shape.getScale();
    if (horizontally)
        shape.setScale(sf::Vector2f(-currentScale.x, currentScale.y));
    else
        shape.setScale(sf::Vector2f(currentScale.x, -currentScale.y));
}

void Transformation::shear(sf::RectangleShape& shape, const sf::Vector2f shearFactor) {
    const sf::Vector2f size = shape.getSize();
    if (shearFactor.x != 0.0f)
        shape.setSize(sf::Vector2f(size.x + size.y * shearFactor.x, size.y));
    if (shearFactor.y != 0.0f)
        shape.setSize(sf::Vector2f(size.x, size.y + size.x * shearFactor.y));
}
