#pragma once

#include <cstdint>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#define _USE_MATH_DEFINES
#include <cmath>

class Level;

struct Target
{
    static constexpr uint8_t RADIUS = 10;
    static constexpr uint16_t SQR_RADIUS = RADIUS * RADIUS;
    int16_t posX, posY;
};

class Vehicle
{
    private:
        static constexpr uint8_t OUTLINE_WIDTH = 5;

        uint16_t width = 30;
        uint16_t length = 60;
        uint16_t turnAxisPos = 30;//7;
        double posX = 540;
        double posY = 390;
        double speed = 0;
        float speedPitchMult = 0.01;
        float minPitch = 0.5, maxPitch = 2;
        float speedVolumeMult = 1;
        float minVolume = 25, maxVolume = 100;
        float minSpeedHitSound = 10;
        double angle = -M_PI / 2;
        double accelerationBase = 10000;
        double accelerationMinSpeed = 10;
        double brake = 500;
        double engineBrake = 100;
        double turnSpeedBase = 0.02;
        double maxTurnSpeed = 1;
        bool playSounds = false;
        std::vector<Target> targets;
        sf::Sound engineSound;
        sf::Sound hitSound;
        sf::Sound targetSound;

    public:
        Vehicle();
        Vehicle(const Vehicle &) = default;
        Vehicle(Vehicle &&) = default;
        Vehicle& operator=(const Vehicle &) = default;
        void setPosAndAngle(double posX, double posY, float angle);
        void addTarget(int16_t posX, int16_t posY);
        void update(double dt, const Level &level);
        int8_t getSpeedSign() const;
        bool hasTargets() const;
        void setPlaySounds(bool playSounds);
        void draw(sf::RenderWindow &window) const;
        void drawTargets(sf::RenderWindow &window) const;
};
