#pragma once

#include <cstdint>
#include <SFML/Graphics.hpp>
#include "Vehicle.hpp"

struct Wall
{
    int16_t posX, posY;
    uint16_t width, height;
    float angle = 0;

    Wall();
    Wall(int16_t posX, int16_t posY, uint16_t width, uint16_t height, float angle = 0);
};

class Level
{
    private:
        std::vector<Wall> walls;
        std::vector<Vehicle> vehicles;
        std::string name;

    public:
        Level(const char *name);
        void addWall(int16_t posX, int16_t posY, uint16_t width, uint16_t length, float angle = 0);
        void addVehicle(const Vehicle &vehicle, double posX, double posY, double angle);
        void addTarget(int16_t posX, int16_t posY);
        const std::vector<Vehicle> &getVehicles() const;
        void draw(sf::RenderWindow &window) const;
        bool checkSat(sf::Vector2f center, sf::Vector2f size, float rot) const;
        const std::string &getName();
};
