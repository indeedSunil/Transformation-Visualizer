#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode()), "SFML Cartesian Coordinates");

    // Set up the custom view
    sf::View view;
    const auto windowSize = static_cast<sf::Vector2f>(window.getSize());
    view.setCenter({-windowSize.x / 256, 0}); // Center horizontally and vertically
    view.setSize({windowSize.x / 20, -windowSize.y / 20}); // Invert Y-axis and apply scale
    window.setView(view);

    // Draw a centered circle
    sf::CircleShape shape(10);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition({0, 0});
    shape.setOrigin({10, 10}); // Center the circle's origin

    // Create coordinate axes that span the entire view
    sf::RectangleShape verticalAxis(sf::Vector2f(0.05, windowSize.y * 2)); // Double height to ensure full coverage
    verticalAxis.setFillColor(sf::Color::Blue);
    verticalAxis.setPosition({0, -windowSize.y}); // Position to cover both up and down

    sf::RectangleShape horizontalAxis(sf::Vector2f(windowSize.x * 2, 0.05)); // Double width to ensure full coverage
    horizontalAxis.setFillColor(sf::Color::Blue);
    horizontalAxis.setPosition({-windowSize.x, 0}); // Position to cover both left and right

    sf::CircleShape shape1(10);
    shape1.setFillColor(sf::Color::Red);
    shape1.setPosition({10, 10});
    shape1.setOrigin({10, 10}); // Center the circle's origin

    sf::CircleShape shape2(10);
    shape2.setFillColor(sf::Color::White);
    shape2.setPosition({-10, -10});
    shape2.setOrigin({10, 10}); // Center the circle's origin

    sf::RectangleShape rectangle({10, 10});
    rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition({0, 0});
    rectangle.setOrigin({5, 5}); // Center the circle's origin
    rectangle.rotate(sf::degrees(45)); // Rotate the rectangle by 45 degrees ( clockwise

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        // window.draw(shape);
        // window.draw(shape1);
        // window.draw(shape2);
        window.draw(rectangle);
        window.draw(verticalAxis);
        window.draw(horizontalAxis);
        window.display();
    }

    return 0;
}
