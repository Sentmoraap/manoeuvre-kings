#pragma once

#include <cstdint>
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <cmath>

class Vehicle
{
    private:
        static constexpr uint8_t OUTLINE_WIDTH = 5;
        uint16_t width = 30;
        uint16_t length = 65;
        uint16_t turnAxisPos = 7;
        double posX = 540;
        double posY = 390;
        double speed = 0;
        double angle = -M_PI / 2;
        double accelerationBase = 10000;
        double accelerationMinSpeed = 10;
        double brake = 500;
        double engineBrake = 100;
        double turnSpeedBase = 0.02;
        double maxTurnSpeed = 1;
    public:
        void setPosAndAngle(double posX, double posY, float angle);
        void update(double dt);
        int8_t getSpeedSign() const;
        void draw(sf::RenderWindow &window) const;
};
