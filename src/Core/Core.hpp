#ifndef CORE_HPP
#define CORE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include "UI/UI.hpp"
#include <SFML/Graphics/Font.hpp>

class Core {
public:
    Core();
    void run();
    static sf::Font font;

    // Static getters for views and measurements
    static float getPanelWidth() { return leftPanelWidth; }
    static float getScale() { return SCALE; }
    static sf::Vector2f getGraphSize() { return graphSize; }
    static const sf::View& getGraphView() { return graphView; }
    static const sf::View& getUIView() { return uiView; }
    static sf::Vector2f getWindowSize() { return windowSize; }

private:
    sf::RenderWindow window;
    sf::Clock deltaClock;
    void processEvents();

    // Static members for views and measurements
    static sf::View graphView;
    static sf::View uiView;
    static sf::Vector2f windowSize;
    static sf::Vector2f graphSize;
    static float leftPanelWidth;
    static const float SCALE;
};

#endif // CORE_HPP
