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
std::vector<Vehicle> vehicles;
uint8_t currentVehicle = 0;
uint32_t levelTime;
uint16_t nbManoeuvres;
uint8_t currentLevelNb = 0;
int8_t currentSpeedSign;
bool counting = false;

void resetLevel()
{
    vehicles = currentLevel->getVehicles();
    currentVehicle = 0;
    for(Vehicle &v : vehicles) v.setPlaySounds(true);
    levelTime = 0;
    nbManoeuvres = 0;
    currentSpeedSign = 0;
    counting = false;
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

    levels.push_back(Level("Congested"));
    levels.back().addWall(241, 177, 482, 354);
    levels.back().addWall(783, 177, 482, 354);
    levels.back().addWall(128, 384, 256, 768);
    levels.back().addWall(896, 384, 256, 768);
    levels.back().addWall(512, 64, 1024, 128);
    levels.back().addWall(512, 591, 1024, 354);
    levels.back().addVehicle(car, 512, 192, M_PI / 2);
    levels.back().addTarget(288, 384);
    levels.back().addVehicle(car, 320, 384, 0);
    levels.back().addTarget(736, 384);
    levels.back().addVehicle(car, 704, 384, M_PI);
    levels.back().addTarget(512, 160);

    levels.push_back(Level("The pound"));
    levels.back().addWall(226, 384, 452, 768);
    levels.back().addWall(798, 384, 452, 768);
    levels.back().addWall(512, 162, 1024, 324);
    levels.back().addWall(512, 606, 1024, 324);
    levels.back().addVehicle(car, 557, 424, M_PI);
    levels.back().addTarget(552, 344);
    levels.back().addVehicle(car, 482, 427, -M_PI / 2);
    levels.back().addVehicle(car, 557, 384, M_PI);

    levels.push_back(Level("Grid"));
    levels.back().addWall(392, 264, 180, 180);
    levels.back().addWall(632, 264, 180, 180);
    levels.back().addWall(392, 504, 180, 180);
    levels.back().addWall(632, 504, 180, 180);
    levels.back().addWall(121, 384, 242, 768);
    levels.back().addWall(903, 384, 242, 768);
    levels.back().addWall(512, 57, 1024, 114);
    levels.back().addWall(512, 711, 1024, 114);
    levels.back().addVehicle(car, 272, 407, -M_PI / 2);
    levels.back().addTarget(272, 204);
    levels.back().addTarget(692, 144);
    levels.back().addTarget(752, 564);
    levels.back().addTarget(332, 624);
    levels.back().addVehicle(car, 489, 144, 0);
    levels.back().addTarget(272, 144);
    levels.back().addTarget(752, 144);
    levels.back().addTarget(752, 624);
    levels.back().addTarget(272, 624);
    levels.back().addVehicle(car, 752, 361, M_PI / 2);
    levels.back().addTarget(332, 144);
    levels.back().addTarget(752, 204);
    levels.back().addTarget(692, 624);
    levels.back().addTarget(272, 564);

    currentLevel = &levels[0];
    resetLevel();

    sf::Color vehicleColors[3] = {sf::Color::Green, sf::Color::Blue, sf::Color::Red};

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
            for(uint8_t i = 0; i < vehicles.size(); i++)
                if(sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(sf::Keyboard::Key::Num1 + i)))
            {
                currentVehicle = i;
                currentSpeedSign = vehicles[currentVehicle].getSpeedSign();
            }
            bool targetsRemaining = false;
            for(uint8_t i = 0; i < vehicles.size(); i++)
            {
                vehicles[i].update(1. / FRAME_RATE, i == currentVehicle, *currentLevel, vehicles);
                targetsRemaining |= vehicles[i].hasTargets();
            }
            if(targetsRemaining)
            {
                int8_t newSpeedSign = vehicles[currentVehicle].getSpeedSign();
                if(newSpeedSign)
                {
                    counting = true;
                    if(newSpeedSign != currentSpeedSign) nbManoeuvres++;
                    currentSpeedSign = newSpeedSign;
                }
            }
            else counting = false;
            if(counting) levelTime++;
        }

        // Draw
        window.clear(sf::Color(64,64,64,0));
        for(uint8_t i = 0; i < vehicles.size(); i++) vehicles[i].drawTargets(window, vehicleColors[i]);
        for(uint8_t i = 0; i < vehicles.size(); i++) vehicles[i].draw(window, i == currentVehicle, vehicleColors[i]);
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
