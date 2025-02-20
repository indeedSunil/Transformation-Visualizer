#include "Matrix.hpp"

TransformationAlgorithm::TransformationAlgorithm() {
    resetTransformation();
}

void TransformationAlgorithm::resetTransformation() {
    m_transformMatrix = Eigen::Matrix3f::Identity();
}

void TransformationAlgorithm::setTranslation(float tx, float ty) {
    m_transformMatrix = Eigen::Matrix3f::Identity();
    m_transformMatrix(0, 2) = tx;
    m_transformMatrix(1, 2) = ty;
}

void TransformationAlgorithm::setScaling(const float sx = 1, const float sy = 1) {
    m_transformMatrix = Eigen::Matrix3f::Identity();
    m_transformMatrix(0, 0) = sx;
    m_transformMatrix(1, 1) = sy;
}

void TransformationAlgorithm::setRotation(float angleDegrees) {
    float angleRad = degreesToRadians(angleDegrees);
    float cosTheta = std::cos(angleRad);
    float sinTheta = std::sin(angleRad);

    m_transformMatrix = Eigen::Matrix3f::Identity();
    m_transformMatrix(0, 0) = cosTheta;
    m_transformMatrix(0, 1) = -sinTheta;
    m_transformMatrix(1, 0) = sinTheta;
    m_transformMatrix(1, 1) = cosTheta;
}

void TransformationAlgorithm::setShearing(float shx, float shy) {
    m_transformMatrix = Eigen::Matrix3f::Identity();
    m_transformMatrix(0, 1) = shx;  // Shear in x-direction
    m_transformMatrix(1, 0) = shy;  // Shear in y-direction
}

void TransformationAlgorithm::setReflectionAboutX() {
    m_transformMatrix = Eigen::Matrix3f::Identity();
    m_transformMatrix(1, 1) = -1;  // Negate y-coordinate
}

void TransformationAlgorithm::setReflectionAboutY() {
    m_transformMatrix = Eigen::Matrix3f::Identity();
    m_transformMatrix(0, 0) = -1;  // Negate x-coordinate
}

void TransformationAlgorithm::setReflectionAboutLine_Y_Equals_X() {
    m_transformMatrix = Eigen::Matrix3f::Identity();
    m_transformMatrix(0, 0) = 0;
    m_transformMatrix(0, 1) = 1;
    m_transformMatrix(1, 0) = 1;
    m_transformMatrix(1, 1) = 0;
}

void TransformationAlgorithm::setReflectionAboutLine_Y_Equals_Negative_X() {
    m_transformMatrix = Eigen::Matrix3f::Identity();
    m_transformMatrix(0, 0) = 0;
    m_transformMatrix(0, 1) = -1;
    m_transformMatrix(1, 0) = -1;
    m_transformMatrix(1, 1) = 0;
}

void TransformationAlgorithm::setReflectionAboutLine(float m, float c) {
    // Reflection about line y = mx + c
    float m2 = m * m;
    float denominator = 1 + m2;

    m_transformMatrix = Eigen::Matrix3f::Identity();

    // Matrix elements for reflection about y = mx + c
    m_transformMatrix(0, 0) = (1 - m2) / denominator;
    m_transformMatrix(0, 1) = (2 * m) / denominator;
    m_transformMatrix(1, 0) = (2 * m) / denominator;
    m_transformMatrix(1, 1) = (m2 - 1) / denominator;

    // Translation components to handle the y-intercept c
    float t = (2 * m * c) / denominator;
    m_transformMatrix(0, 2) = -t;
    m_transformMatrix(1, 2) = t/m;
}

void TransformationAlgorithm::combineTransformation(const Eigen::Matrix3f& newTransform) {
    m_transformMatrix = newTransform * m_transformMatrix;
}

sf::Vector2f TransformationAlgorithm::transformPoint(const sf::Vector2f& point) const {
    Eigen::Vector3f eigenPoint;
    eigenPoint << point.x, point.y, 1.0f;

    Eigen::Vector3f transformedPoint = m_transformMatrix * eigenPoint;
    return {transformedPoint.x(), transformedPoint.y()};
}

Eigen::Matrix3f TransformationAlgorithm::getTransformationMatrix() const {
    return m_transformMatrix;
}