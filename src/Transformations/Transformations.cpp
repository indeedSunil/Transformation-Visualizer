#include "transformations.hpp"
#include <iostream>

// Scale the shape by the given scale factor
void Transformation::scale(sf::ConvexShape& shape, const sf::Vector2f scaleFactor)
{
    const auto pointCount = shape.getPointCount();

    // Get the center of the shape
    sf::Vector2f center(0, 0);
    for (int i = 0; i < pointCount; i++) {
        center += shape.getPoint(i);
    }
    center /= static_cast<float>(pointCount);

    // Create transformation matrices
    TransformationAlgorithm toOriginTransform;
    toOriginTransform.setTranslation(-center.x, -center.y);

    TransformationAlgorithm scaleTransform;
    scaleTransform.setScaling(scaleFactor.x, scaleFactor.y);

    TransformationAlgorithm backTransform;
    backTransform.setTranslation(center.x, center.y);

    // Combine transformations in correct order
    // Final = Translation_back * Scale * Translation_to_origin
    const Eigen::Matrix3f finalTransform = backTransform.getTransformationMatrix() *
                                   scaleTransform.getTransformationMatrix() *
                                   toOriginTransform.getTransformationMatrix();

    // Create a transform with the final matrix
    TransformationAlgorithm finalTransformAlgo;
    finalTransformAlgo.resetTransformation();
    finalTransformAlgo.combineTransformation(finalTransform);

    // Apply transformation to all points
    for (int i = 0; i < pointCount; i++)
    {
        sf::Vector2f originalPoint = shape.getPoint(i);
        const sf::Vector2f transformedPoint = finalTransformAlgo.transformPoint(originalPoint);
        shape.setPoint(i, transformedPoint);
    }
}



// Translate the shape by the given translation factor
void Transformation::translate(sf::ConvexShape& shape, sf::Vector2f translationFactor)
{
    const auto pointCount = shape.getPointCount();
    TransformationAlgorithm transform;

    // Set translation transformation
    transform.setTranslation(translationFactor.x, translationFactor.y);

    // Apply transformation to all points
    for (int i = 0; i < pointCount; i++)
    {
        sf::Vector2f originalPoint = shape.getPoint(i);
        sf::Vector2f transformedPoint = transform.transformPoint(originalPoint);
        shape.setPoint(i, transformedPoint);
    }
}


// Rotate the shape by the given angle around the pivot point
void Transformation::rotate(sf::ConvexShape& shape, const float angle, const sf::Vector2f pivot)
{
    const auto pointCount = shape.getPointCount();

    // Create transformation matrices
    TransformationAlgorithm toPivotTransform;
    toPivotTransform.setTranslation(-pivot.x, -pivot.y);

    TransformationAlgorithm rotateTransform;
    rotateTransform.setRotation(angle);

    TransformationAlgorithm backTransform;
    backTransform.setTranslation(pivot.x, pivot.y);

    // Combine transformations in correct order
    // Final = Translation_back * Rotation * Translation_to_pivot
    const Eigen::Matrix3f finalTransform = backTransform.getTransformationMatrix() *
                                   rotateTransform.getTransformationMatrix() *
                                   toPivotTransform.getTransformationMatrix();

    // Create a transform with the final matrix
    TransformationAlgorithm finalTransformAlgo;
    finalTransformAlgo.resetTransformation();
    finalTransformAlgo.combineTransformation(finalTransform);

    // Apply transformation to all points
    for (int i = 0; i < pointCount; i++)
    {
        sf::Vector2f originalPoint = shape.getPoint(i);
        sf::Vector2f transformedPoint = finalTransformAlgo.transformPoint(originalPoint);
        shape.setPoint(i, transformedPoint);
    }
}


// Reflect the shape about the x-axis or y-axis
void Transformation::reflect(sf::ConvexShape& shape, float slope, float yIntercept)
{
    const auto pointCount = shape.getPointCount();
    TransformationAlgorithm transform;

    // Debug: Print original points and reflection parameters
    for (int i = 0; i < pointCount; i++)
    {
        std::cout << "Original Point " << i << ": " << shape.getPoint(i).x << ", " << shape.getPoint(i).y << std::endl;
    }
    std::cout << "Reflection line: y = " << slope << "x + " << yIntercept << std::endl;

    // Special cases for common reflections
    if (slope == 0 && yIntercept == 0) {
        // Reflection about x-axis
        transform.setReflectionAboutX();
    }
    else if (slope == 9999 && yIntercept == 0) {
        // Reflection about y-axis
        transform.setReflectionAboutY();
    }
    else if (slope == 1 && yIntercept == 0) {
        // Reflection about y = x
        transform.setReflectionAboutLine_Y_Equals_X();
    }
    else if (slope == -1 && yIntercept == 0) {
        // Reflection about y = -x
        transform.setReflectionAboutLine_Y_Equals_Negative_X();
    }
    else {
        // General case: reflection about y = mx + c
        transform.setReflectionAboutLine(slope, yIntercept);
    }

    // Get the transformation matrix
    Eigen::Matrix3f reflectionMatrix = transform.getTransformationMatrix();

    // Create final transform
    TransformationAlgorithm finalTransform;
    finalTransform.resetTransformation();
    finalTransform.combineTransformation(reflectionMatrix);

    // Apply transformation to all points
    for (int i = 0; i < pointCount; i++)
    {
        sf::Vector2f originalPoint = shape.getPoint(i);
        sf::Vector2f reflectedPoint = finalTransform.transformPoint(originalPoint);
        shape.setPoint(i, reflectedPoint);
        std::cout << "Reflected Point " << i << ": " << reflectedPoint.x << ", " << reflectedPoint.y << std::endl;
    }
}



// Shear the shape by the given shear factor
void Transformation::shear(sf::ConvexShape& shape, sf::Vector2f shearFactor)
{
    const auto pointCount = shape.getPointCount();

    // Get the center of the shape
    sf::Vector2f center(0, 0);
    for (int i = 0; i < pointCount; i++) {
        center += shape.getPoint(i);
    }
    center /= static_cast<float>(pointCount);
    std::cout << "Center: " << center.x << ", " << center.y << std::endl;

    // Create transformation matrices
    TransformationAlgorithm toCenterTransform;
    toCenterTransform.setTranslation(-center.x, -center.y);

    TransformationAlgorithm shearTransform;
    shearTransform.setShearing(shearFactor.x, shearFactor.y);

    TransformationAlgorithm backTransform;
    backTransform.setTranslation(center.x, center.y);

    // Combine transformations in correct order
    // Final = Translation_back * Shear * Translation_to_center
    const Eigen::Matrix3f finalTransform = backTransform.getTransformationMatrix() *
                                   shearTransform.getTransformationMatrix() *
                                   toCenterTransform.getTransformationMatrix();

    // Create a transform with the final matrix
    TransformationAlgorithm finalTransformAlgo;
    finalTransformAlgo.resetTransformation();
    finalTransformAlgo.combineTransformation(finalTransform);

    // Apply transformation to all points
    for (int i = 0; i < pointCount; i++)
    {
        sf::Vector2f originalPoint = shape.getPoint(i);
        const sf::Vector2f transformedPoint = finalTransformAlgo.transformPoint(originalPoint);
        shape.setPoint(i, transformedPoint);
    }
}