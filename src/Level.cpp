#include "Level.hpp"
#define _USE_MATH_DEFINES
#include <cmath>

Wall::Wall() {}
Wall::Wall(int16_t posX, int16_t posY, uint16_t width, uint16_t length, float angle)
 : posX(posX), posY(posY), width(width), length(length), angle(angle) {}

void Level::addWall(int16_t posX, int16_t posY, uint16_t width, uint16_t length, float angle)
{
    walls.emplace_back(posX, posY, width, length, angle);
}

void Level::addVehicle(const Vehicle &vehicle, double posX, double posY, double angle)
{
    vehicles.push_back(vehicle);
    vehicles.back().setPosAndAngle(posX, posY, angle);
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
        rect.setSize(sf::Vector2f(wall.width, wall.length));
        rect.setPosition(wall.posX, wall.posY);
        rect.setOrigin(wall.width / 2, wall.length / 2);
        rect.setRotation(wall.angle * 180 / M_PI);
        window.draw(rect);
    }
}
