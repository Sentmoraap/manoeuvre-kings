#include "Level.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include "Utils.hpp"

Wall::Wall() {}
Wall::Wall(int16_t posX, int16_t posY, uint16_t width, uint16_t height, float angle)
 : posX(posX), posY(posY), width(width), height(height), angle(angle) {}

Level::Level(const char* name) : name(name) {}

void Level::addWall(int16_t posX, int16_t posY, uint16_t width, uint16_t height, float angle)
{
    walls.emplace_back(posX, posY, width, height, angle);
}

void Level::addVehicle(const Vehicle &vehicle, double posX, double posY, double angle)
{
    vehicles.push_back(vehicle);
    vehicles.back().setPosAndAngle(posX, posY, angle);
}
void Level::addTarget(int16_t posX, int16_t posY)
{
    vehicles.back().addTarget(posX, posY);
}

const std::vector<Vehicle> &Level::getVehicles() const
{
    return vehicles;
}

void Level::draw(sf::RenderWindow &window) const
{
    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::Black);
    for(const Wall &wall : walls)
    {
        rect.setSize(sf::Vector2f(wall.width, wall.height));
        rect.setPosition(wall.posX, wall.posY);
        rect.setOrigin(wall.width / 2, wall.height / 2);
        rect.setRotation(wall.angle * 180 / M_PI);
        window.draw(rect);
    }
}

bool Level::checkSat(sf::Vector2f center, sf::Vector2f size, float rot) const
{
    for(const Wall &wall : walls)
        if(Utils::rectOverlap(center, size, rot, sf::Vector2f(wall.posX, wall.posY),
            sf::Vector2f(wall.width / 2, wall.height / 2), wall.angle)) return true;
    return false;
}

const std::string &Level::getName()
{
    return name;
}
