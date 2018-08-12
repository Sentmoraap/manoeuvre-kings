#include <cstdint>
#include <SFML/Graphics.hpp>
#include "Level.hpp"
#include "Vehicle.hpp"
#include "Sounds.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>
#include <iomanip>

static constexpr uint16_t FRAME_RATE = 240;

std::vector<Level> levels;
Level* currentLevel;
Vehicle currentVehicle(0, 0);
uint32_t levelTime;
uint16_t nbManoeuvres;
uint8_t currentLevelNb = 0;
int8_t currentSpeedSign;
bool counting = false;

void resetLevel()
{
    currentVehicle = currentLevel->getVehicles()[0];
    currentVehicle.setPlaySounds(true);
    levelTime = 0;
    nbManoeuvres = 0;
    currentSpeedSign = 0;
}

void changeLevel(int8_t delta)
{
    currentLevelNb = (currentLevelNb + delta + levels.size()) % levels.size();
    currentLevel = &levels[currentLevelNb];
    resetLevel();
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
    Sounds::loadSounds();
    uint64_t currentTime = clock.getElapsedTime().asMicroseconds() * FRAME_RATE;
    Vehicle car(60, 7), limo(90, 7);


    // Levels
    levels.push_back(Level("Shoshinsha"));
    levels.back().addWall(512, 0, 448, 448, M_PI / 4);
    levels.back().addWall(256, 768, 512, 512, M_PI / 4);
    levels.back().addWall(768, 768, 512, 512, M_PI / 4);
    levels.back().addWall(175, 384, 350, 768);
    levels.back().addWall(849, 384, 350, 768);
    levels.back().addWall(512, 384, 16, 256);
    levels.back().addVehicle(car, 390, 240, M_PI / 2);
    levels.back().addTarget(644, 240);

    levels.push_back(Level("Cross"));
    levels.back().addWall(241, 177, 482, 354);
    levels.back().addWall(783, 177, 482, 354);
    levels.back().addWall(241, 591, 482, 354);
    levels.back().addWall(783, 591, 482, 354);
    levels.back().addWall(128, 384, 256, 768);
    levels.back().addWall(896, 384, 256, 768);
    levels.back().addWall(512, 64, 1024, 128);
    levels.back().addWall(512, 704, 1024, 128);
    levels.back().addVehicle(car, 512, 576, -M_PI / 2);
    levels.back().addTarget(512, 192);
    levels.back().addTarget(320, 384);
    levels.back().addTarget(704, 384);

    levels.push_back(Level("Bay parking"));
    levels.back().addVehicle(car, 337, 384, 0);
    for(uint16_t x = 337; x <= 687; x += 50) for(uint16_t y = 289; y <= 479; y += 190)
        levels.back().addWall(x, y, 10, 70);
    levels.back().addWall(146, 384, 292, 768);
    levels.back().addWall(878, 384, 292, 768);
    levels.back().addWall(512, 127, 1024, 254);
    levels.back().addWall(512, 641, 1024, 254);
    levels.back().addTarget(462, 489);
    levels.back().addTarget(512, 489);
    levels.back().addTarget(662, 489);
    levels.back().addTarget(712, 289);

    levels.push_back(Level("Diagonal parking"));
    levels.back().addVehicle(car, 337, 384, 0);
    for(uint16_t x = 337; x <= 687; x += 56) for(uint16_t y = 297; y <= 471; y += 174)
        levels.back().addWall(x, y, 10, 70, y == 297 ? M_PI / 6 : -M_PI / 6);
    levels.back().addWall(146, 384, 292, 768);
    levels.back().addWall(878, 384, 292, 768);
    levels.back().addWall(512, 135, 1024, 270);
    levels.back().addWall(512, 633, 1024, 270);
    levels.back().addTarget(482, 479);
    levels.back().addTarget(532, 299);
    levels.back().addTarget(594, 479);
    levels.back().addTarget(706, 479);

    levels.push_back(Level("Enter the limousine"));
    levels.back().addVehicle(limo, 92, 386, 0);
    levels.back().addWall(512, 384, 240, 240);
    levels.back().addWall(156, 172, 312, 344);
    levels.back().addWall(156, 596, 312, 344);
    levels.back().addWall(512, 144, 400, 80);
    levels.back().addWall(238, 92, 477, 184);
    levels.back().addWall(512, 676, 400, 184);
    levels.back().addWall(32, 384, 64, 768);
    levels.back().addWall(992, 384, 64, 768);
    levels.back().addWall(868, 596, 312, 344);
    levels.back().addWall(801, 224, 178, 240);
    levels.back().addWall(512, 17, 1024, 34);
    levels.back().addTarget(512, 224);
    levels.back().addTarget(512, 544);
    levels.back().addTarget(512, 69);

    levels.push_back(Level("Parallel parking"));
    levels.back().addVehicle(limo, 467, 576, -M_PI / 2);
    levels.back().addWall(535, 169, 55, 328);
    levels.back().addWall(512, 599, 10, 328);
    levels.back().addWall(211, 384, 422, 768);
    levels.back().addWall(793, 384, 462, 768);
    levels.back().addWall(512, 64, 1024, 128);
    levels.back().addWall(512, 704, 1024, 128);
    levels.back().addTarget(540, 608);


    currentLevel = &levels[0];
    resetLevel();

    // Main loop
    while(window.isOpen())
    {
        // Handle events
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed)
            {
                switch(event.key.code)
                {
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                    case sf::Keyboard::R:
                        resetLevel();
                        break;
                    case sf::Keyboard::N:
                        changeLevel(1);
                        break;
                    case sf::Keyboard::P:
                        changeLevel(-1);
                        break;
                    default:
                        break;
                }
            }

        }

        // Update
        uint64_t newTime = clock.getElapsedTime().asMicroseconds() * FRAME_RATE;
        while(newTime > currentTime)
        {
            currentTime += 1000000;
            currentVehicle.update(1. / FRAME_RATE, *currentLevel);
            int8_t newSpeedSign = currentVehicle.getSpeedSign();
            if(newSpeedSign && currentVehicle.hasTargets())
            {
                counting = true;
                if(newSpeedSign != currentSpeedSign) nbManoeuvres++;
                currentSpeedSign = newSpeedSign;
            }
            counting &= currentVehicle.hasTargets();
            if(counting) levelTime++;
        }

        // Draw
        window.clear(sf::Color(64,64,64,0));
        currentVehicle.drawTargets(window);
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
        ss << "Product: ";
        formatTime(ss, levelTime * nbManoeuvres);
        text.setString(ss.str());
        text.setPosition(10, 74);
        window.draw(text);
        text.setString(currentLevel->getName());
        text.setPosition(512 - text.getLocalBounds().width / 2, 728);
        window.draw(text);
        window.display();
    }
}
