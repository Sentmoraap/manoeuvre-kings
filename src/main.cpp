#include <cstdint>
#include <SFML/Graphics.hpp>
#include "Vehicle.hpp"

static constexpr uint16_t FRAME_RATE = 240;

int main(int argc, char **argv)
{
    // Init
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Manoeuvre Kings");
    sf::Clock clock;
    uint64_t currentTime = clock.getElapsedTime().asMicroseconds() * FRAME_RATE;
    Vehicle car;
    while(window.isOpen())
    {
        // Close window
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();

        // Update
        uint64_t newTime = clock.getElapsedTime().asMicroseconds() * FRAME_RATE;
        while(newTime > currentTime)
        {
            currentTime += 1000000;
            car.update(1. / FRAME_RATE);
        }

        // Draw
        window.clear(sf::Color(64,64,64,0));
        car.draw(window);
        window.display();
    }
}
