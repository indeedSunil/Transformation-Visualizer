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

    // Special cases
    if (slope == 0 && yIntercept == 0) {
        transform.setReflectionAboutX();
    }
    else if (slope == 9999 && yIntercept == 0) {
        transform.setReflectionAboutY();
    }
    else if (slope == 1 && yIntercept == 0) {
        transform.setReflectionAboutLine_Y_Equals_X();
    }
    else if (slope == -1 && yIntercept == 0) {
        transform.setReflectionAboutLine_Y_Equals_Negative_X();
    }
    else {
        // General case using composite transformations
        TransformationAlgorithm finalTransform;
        finalTransform.resetTransformation();

        // 1. Translate to move line through origin
        TransformationAlgorithm translate1;
        translate1.setTranslation(0, -yIntercept);
        finalTransform.combineTransformation(translate1.getTransformationMatrix());

        // 2. Rotate to align with x-axis
        TransformationAlgorithm rotate1;
        float angle = -std::atan(slope) * 180.0f / M_PI; // Convert to degrees for setRotation
        rotate1.setRotation(angle);
        finalTransform.combineTransformation(rotate1.getTransformationMatrix());

        // 3. Reflect about x-axis
        TransformationAlgorithm reflectX;
        reflectX.setReflectionAboutX();
        finalTransform.combineTransformation(reflectX.getTransformationMatrix());

        // 4. Rotate back
        TransformationAlgorithm rotate2;
        rotate2.setRotation(-angle);
        finalTransform.combineTransformation(rotate2.getTransformationMatrix());

        // 5. Translate back
        TransformationAlgorithm translate2;
        translate2.setTranslation(0, yIntercept);
        finalTransform.combineTransformation(translate2.getTransformationMatrix());

        transform = finalTransform;
    }

    // Apply transformation to all points
    for (int i = 0; i < pointCount; i++)
    {
        sf::Vector2f originalPoint = shape.getPoint(i);
        sf::Vector2f reflectedPoint = transform.transformPoint(originalPoint);
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