#include "Level.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include "Utils.hpp"

Wall::Wall() {}
Wall::Wall(int16_t posX, int16_t posY, uint16_t width, uint16_t height, float angle)
 : posX(posX), posY(posY), width(width), height(height), angle(angle) {}

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

bool Level::checkSat(sf::FloatRect rect, sf::Vector2f center, float angle) const
{
    sf::Vector2f axis0(cos(angle), sin(angle)), axis1(-axis0.y, axis0.x);
    float a0Min = -center.x, a0Max = rect.width - center.x, a1Min = -center.y, a1Max = rect.height - center.y;
    for(const Wall &wall : walls)
    {
        sf::Vector2f axis2(cos(wall.angle), sin(wall.angle)), axis3(-axis2.y, axis2.x);
        float a2Diff = wall.width / 2, a3Diff = wall.height / 2;
        sf::Vector2f relOrigin = sf::Vector2f(wall.posX, wall.posY) - sf::Vector2f(rect.left, rect.top);
        float base = Utils::dot(relOrigin, axis0), d20 = Utils::dot(axis2, axis0), d30 = Utils::dot(axis3, axis0);
        float diff = fabs(d20) * a2Diff + fabs(d30) * a3Diff;
        if(!Utils::overlap(a0Min, a0Max, base - diff, base + diff)) continue;
        base = Utils::dot(relOrigin, axis1);
        float d21 = Utils::dot(axis2, axis1), d31 = Utils::dot(axis3, axis1);
        diff = fabs(d21) * a2Diff + fabs(d31) * a3Diff;
        if(!Utils::overlap(a1Min, a1Max, base - diff, base + diff)) continue;
        relOrigin = -relOrigin;
        base = Utils::dot(relOrigin, axis2);
        float min = base, max = base;
        if(d20 > 0)
        {
            min += a0Min * d20;
            max += a0Max * d20;
        }
        else
        {
            min += a0Max * d20;
            max += a0Min * d20;
        }
        if(d21 > 0)
        {
            min += a1Min * d21;
            max += a1Max * d21;
        }
        else
        {
            min += a1Max * d21;
            max += a1Min * d21;
        }
        if(!Utils::overlap(-a2Diff, a2Diff, min, max)) continue;
        base = Utils::dot(relOrigin, axis3);
        min = base, max = base;
        if(d30 > 0)
        {
            min += a0Min * d30;
            max += a0Max * d30;
        }
        else
        {
            min += a0Max * d30;
            max += a0Min * d30;
        }
        if(d31 > 0)
        {
            min += a1Min * d31;
            max += a1Max * d31;
        }
        else
        {
            min += a1Max * d31;
            max += a1Min * d31;
        }
        if(Utils::overlap(-a3Diff, a3Diff, min, max)) return true;
    }
    return false;
}
