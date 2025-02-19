#pragma once

#include <SFML/System/Vector2.hpp>
#include <Dense>
#include <cmath>

class TransformationAlgorithm {
public:
    // Constructor
    TransformationAlgorithm();

    // Basic Transformations
    void setTranslation(float tx, float ty);
    void setScaling(float sx, float sy);
    void setRotation(float angleDegrees);
    void setShearing(float shx, float shy);

    // Reflection Transformations
    void setReflectionAboutX();  // Reflect about x-axis (y = 0)
    void setReflectionAboutY();  // Reflect about y-axis (x = 0)
    void setReflectionAboutLine_Y_Equals_X();  // Reflect about y = x
    void setReflectionAboutLine_Y_Equals_Negative_X();  // Reflect about y = -x
    void setReflectionAboutLine(float m, float c);  // Reflect about y = mx + c

    // Transform a point using the current transformation matrix
    sf::Vector2f transformPoint(const sf::Vector2f& point) const;

    // Get the current transformation matrix
    Eigen::Matrix3f getTransformationMatrix() const;

    // Reset transformation matrix to identity
    void resetTransformation();

    // Combine transformations (multiply current matrix with new transformation)
    void combineTransformation(const Eigen::Matrix3f& newTransform);

private:
    Eigen::Matrix3f m_transformMatrix;

    // Helper function to convert degrees to radians
    static float degreesToRadians(float degrees) {
        return degrees * M_PI / 180.0f;
    }
};