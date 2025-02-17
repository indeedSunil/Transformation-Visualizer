#include "transformations.hpp"
#include <cmath>
#include <iostream>

// ---------------------- SCALE -------------------------

// Rectangle: simply scale the size.
void Transformation::scale(sf::RectangleShape& shape, const sf::Vector2f scaleFactor) {
    const sf::Vector2f currentSizeWindow = UI::windowDistanceToGraph(shape.getSize()); // Size wrt window units

    // const sf::Vector2f currentSizeGraph = UI::windowDistanceToGraph(currentSizeWindow); // Size wrt graph units

    // const sf::Vector2f newSizeGraph = { currentSizeGraph.x * scaleFactor.x, currentSizeGraph.y * scaleFactor.y }; // Scaling

    // const sf::Vector2f newSizeWindow = UI::graphDistanceToWindow(newSizeGraph); // Back to window units

    const sf::Vector2f position = UI::windowToGraph(shape.getPosition());

    const sf::Vector2f shapeCenter = (position + currentSizeWindow) / 2.f;
    shape.move({-shapeCenter.x, -shapeCenter.y});

    shape.setSize({currentSizeWindow.x * scaleFactor.x, currentSizeWindow.y * scaleFactor.y});
    shape.move(shapeCenter);
}

// Circle: assume a uniform scale factor. If scaleFactor.x and scaleFactor.y differ, choose one (or average).
void Transformation::scale(sf::CircleShape& shape, sf::Vector2f scaleFactor) {
    // Here we use the x-component as the uniform factor.
    float factor = scaleFactor.x;
    float newRadius = shape.getRadius() * factor;
    // If the circle’s origin is not set to its center, adjust if needed.
    shape.setRadius(newRadius);
    // Optionally re-center if you want to preserve the current center:
    sf::Vector2f currentPos = shape.getPosition(); // top-left of bounding box
    shape.setOrigin({newRadius, newRadius});
    shape.setPosition({currentPos.x + newRadius, currentPos.y + newRadius});
}

// ConvexShape (e.g., Triangle): scale each point relative to the shape’s center.
void Transformation::scale(sf::ConvexShape& shape, sf::Vector2f scaleFactor) {
    sf::FloatRect bounds = shape.getGlobalBounds();
    sf::Vector2f center = { bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f };
    for (size_t i = 0; i < shape.getPointCount(); i++) {
        sf::Vector2f pt = shape.getPoint(i);
        // Calculate the vector from the center to the point and scale it.
        sf::Vector2f offset = { pt.x - center.x, pt.y - center.y };
        offset.x *= scaleFactor.x;
        offset.y *= scaleFactor.y;
        shape.setPoint(i, { center.x + offset.x, center.y + offset.y });
    }
}

// ---------------------- TRANSLATE -------------------------

// Use the built-in move() function which is available in all Transformable classes.
void Transformation::translate(sf::RectangleShape& shape, sf::Vector2f translationFactor) {
    const sf::Vector2f windowTranslation = UI::graphToWindow(translationFactor) - UI::graphToWindow({0, 0}); // window to graph conversion
    shape.move(windowTranslation);
}

void Transformation::translate(sf::CircleShape& shape, sf::Vector2f translationFactor) {
    const sf::Vector2f windowTranslation = UI::graphToWindow(translationFactor) - UI::graphToWindow({0, 0}); // window to graph conversion
    shape.move(windowTranslation);
}

void Transformation::translate(sf::ConvexShape& shape, sf::Vector2f translationFactor) {
    const sf::Vector2f windowTranslation = UI::graphToWindow(translationFactor) - UI::graphToWindow({0, 0}); // window to graph conversion
    shape.move(windowTranslation);
}

// ---------------------- ROTATE -------------------------

// For rotation, we first set the origin to the center (if not already set) then rotate.
void Transformation::rotate(sf::RectangleShape& shape, const float angle, const sf::Vector2f pivot) {
    const sf::Vector2f originalOrigin = shape.getOrigin();
    const sf::Vector2f originalPosition = shape.getPosition();

    // Convert pivot to window coordinates
    const sf::Vector2f pivotAccToGraph = UI::graphToWindow(pivot);
    std::cout << "pivotAccToGraph: " << pivotAccToGraph.x << ", " << pivotAccToGraph.y << std::endl;

    // Calculate vector from pivot to shape's top-left corner
    const sf::Vector2f relativePos = -shape.getOrigin() + pivotAccToGraph;
    std::cout << "relativePos: " << relativePos.x << ", " << relativePos.y << std::endl;
    // Set the origin relative to the shape's local coordinate system
    shape.setOrigin({0,0});

    // Set the shape's position to the pivot point
    // shape.setPosition(pivotAccToGraph);

    // Perform the rotation
    shape.rotate(sf::degrees(angle));

    // Reset the origin and adjust position to maintain the same world position
    shape.setOrigin(originalOrigin);
    // shape.setPosition(originalPosition);
}


void Transformation::rotate(sf::CircleShape& shape, float angle, sf::Vector2f pivot) {
    float radius = shape.getRadius();
    // Set origin to center
    shape.setOrigin({radius, radius});
    shape.rotate(sf::degrees(angle));;
}

void Transformation::rotate(sf::ConvexShape& shape, float angle, sf::Vector2f pivot) {
    // Compute the center of the convex shape.
    sf::FloatRect bounds = shape.getGlobalBounds();
    shape.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
    shape.rotate(sf::degrees(angle));
}

// ---------------------- REFLECT -------------------------

// Reflection is implemented by negating the corresponding component of the scale.
void Transformation::reflect(sf::RectangleShape& shape, bool horizontally) {
    const sf::Vector2f currentScale = shape.getScale();
    if (horizontally)
        shape.setScale(sf::Vector2f(-currentScale.x, currentScale.y));
    else
        shape.setScale(sf::Vector2f(currentScale.x, -currentScale.y));
}

void Transformation::reflect(sf::CircleShape& shape, bool horizontally) {
    // For a circle, reflection doesn't change its appearance if it’s symmetric.
    // But if you want to simulate it, you can change its scale.
    const sf::Vector2f currentScale = shape.getScale();
    if (horizontally)
        shape.setScale(sf::Vector2f(-currentScale.x, currentScale.y));
    else
        shape.setScale(sf::Vector2f(currentScale.x, -currentScale.y));
}

void Transformation::reflect(sf::ConvexShape& shape, bool horizontally) {
    const sf::Vector2f currentScale = shape.getScale();
    if (horizontally)
        shape.setScale(sf::Vector2f(-currentScale.x, currentScale.y));
    else
        shape.setScale(sf::Vector2f(currentScale.x, -currentScale.y));
}

// ---------------------- SHEAR -------------------------

// Shearing is not provided out-of-the-box in SFML. One workaround for a rectangle is to
// adjust its size based on the shear factor. This is a simplistic approach.
void Transformation::shear(sf::RectangleShape& shape, sf::Vector2f shearFactor) {
    const sf::Vector2f size = shape.getSize();
    if (shearFactor.x != 0.0f)
        shape.setSize(sf::Vector2f(size.x + size.y * shearFactor.x, size.y));
    if (shearFactor.y != 0.0f)
        shape.setSize(sf::Vector2f(size.x, size.y + size.x * shearFactor.y));
}

void Transformation::shear(sf::CircleShape& shape, sf::Vector2f shearFactor) {
    // For circles, shearing will destroy the circle's symmetry.
    // You might instead want to convert the circle into an ellipse or skip shearing.
    // For demonstration, we do nothing.
}

void Transformation::shear(sf::ConvexShape& shape, sf::Vector2f shearFactor) {
    // For a convex shape, you could adjust each point. This example applies a basic shear.
    for (size_t i = 0; i < shape.getPointCount(); i++) {
        sf::Vector2f pt = shape.getPoint(i);
        // For horizontal shear, add a fraction of the y value to x.
        pt.x += pt.y * shearFactor.x;
        // For vertical shear, add a fraction of the x value to y.
        pt.y += pt.x * shearFactor.y;
        shape.setPoint(i, pt);
    }
}
