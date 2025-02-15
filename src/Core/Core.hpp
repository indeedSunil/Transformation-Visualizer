#ifndef CORE_HPP
#define CORE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include "UI/UI.hpp"

class Core {
public:
    Core();
    void run();

private:
    sf::RenderWindow window;
    sf::Clock deltaClock;
    void processEvents();
};

#endif // CORE_HPP
