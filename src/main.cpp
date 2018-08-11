#include <cstdint>
#include <SFML/Graphics.hpp>
#include "Level.hpp"
#include "Vehicle.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>
#include <iomanip>

static constexpr uint16_t FRAME_RATE = 240;

std::vector<Level> levels;
Level* currentLevel;
Vehicle currentVehicle;
uint32_t levelTime;
uint16_t nbManoeuvres;
int8_t currentSpeedSign;
bool counting = false;

void resetLevel()
{
    currentVehicle = currentLevel->getVehicles()[0];
    levelTime = 0;
    nbManoeuvres = 0;
    currentSpeedSign = 0;
}

void formatTime(std::stringstream &ss, uint32_t time)
{
    uint16_t minutes = time / (FRAME_RATE * 60);
    if(minutes >= 60) ss << minutes / 60 << ":";
    ss << std::setw(2) << std::setfill('0') << (minutes % 60)
        << ":" << std::setw(2) << std::setfill('0') << ((time / FRAME_RATE) % 60)
        << "." << std::setw(2) << std::setfill('0') << ((time * 100 / FRAME_RATE) % 100);
}

int main(int argc, char **argv)
{
    // Init
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Manoeuvre Kings");
    sf::Clock clock;
    sf::Font font;
    font.loadFromFile("assets/sansation_regular.ttf");
    uint64_t currentTime = clock.getElapsedTime().asMicroseconds() * FRAME_RATE;
    levels.push_back(Level());
    Vehicle car;
    levels[0].addWall(512, 0, 448, 448, M_PI / 4);
    levels[0].addWall(256, 768, 512, 512, M_PI / 4);
    levels[0].addWall(768, 768, 512, 512, M_PI / 4);
    levels[0].addWall(175, 384, 350, 768);
    levels[0].addWall(849, 384, 350, 768);
    levels[0].addWall(512, 384, 16, 256);
    levels[0].addVehicle(car, 400, 250, M_PI / 2);
    currentLevel = &levels[0];

    resetLevel();
    while(window.isOpen())
    {
        // Handle events
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) resetLevel();

        // Update
        uint64_t newTime = clock.getElapsedTime().asMicroseconds() * FRAME_RATE;
        while(newTime > currentTime)
        {
            currentTime += 1000000;
            currentVehicle.update(1. / FRAME_RATE);
            int8_t newSpeedSign = currentVehicle.getSpeedSign();
            if(newSpeedSign)
            {
                counting = true;
                if(newSpeedSign != currentSpeedSign) nbManoeuvres++;
                currentSpeedSign = newSpeedSign;
            }
            if(counting) levelTime++;
        }

        // Draw
        window.clear(sf::Color(64,64,64,0));
        currentVehicle.draw(window);
        currentLevel->draw(window);

        // Texts
        std::stringstream ss;
        ss << "Time: ";
        formatTime(ss, levelTime);
        sf::Text text(ss.str(), font);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(32);
        text.setPosition(10, 10);
        window.draw(text);
        ss.str("");
        ss << "Manoeuvres: " << nbManoeuvres;
        text.setString(ss.str());
        text.setPosition(10, 42);
        window.draw(text);
        ss.str("");
        ss << "Compound: ";
        formatTime(ss, levelTime * nbManoeuvres);
        text.setString(ss.str());
        text.setPosition(10, 74);
        window.draw(text);
        window.display();
    }
}
