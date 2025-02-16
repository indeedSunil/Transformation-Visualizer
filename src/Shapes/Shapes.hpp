#pragma once
#include <SFML/Graphics.hpp>

class Shapes
{
public:
    enum class ShapeType
    {
        None,
        Rectangle,
        Triangle,
        Circle,
        Ellipse,
        Line,
        LetterA
    };
    static void updateShape();
};
