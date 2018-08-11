#include <SFML/Graphics.hpp>

int main(int argc, char **argv)
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Manoeuvre Kings");
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
    }
}
