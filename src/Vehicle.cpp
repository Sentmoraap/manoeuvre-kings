#include "Vehicle.hpp"
#include "Level.hpp"

void Vehicle::setPosAndAngle(double posX, double posY, float angle)
{
    speed = 0;
    this->posX = posX;
    this->posY = posY;
    this->angle = angle;
}

void Vehicle::update(double dt, const Level &level)
{
    int8_t accelInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Up) - sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    if(accelInput)
    {
        if((accelInput > 0) == (speed >= 0))
        {
            speed += accelInput * accelerationBase * dt / std::max(fabs(speed), accelerationMinSpeed);
        }
        else
        {
            speed += accelInput * dt * brake;
        }
    }
    else
    {
        if(speed > 0)
        {
            speed -= dt * engineBrake;
            if(speed < 0) speed = 0;
        }
        else
        {
            speed += dt * engineBrake;
            if(speed > 0) speed = 0;
        }
    }
    int8_t turnInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) - sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    double newAngle = angle + turnInput * std::min(fabs(speed) * turnSpeedBase, maxTurnSpeed) * dt * getSpeedSign();
    double newPosX = posX + speed * dt * cos(angle);
    double newPosY = posY + speed * dt * sin(angle);
    if(level.checkSat(sf::FloatRect(sf::Vector2f(newPosX, newPosY), sf::Vector2f(length, width)),
        sf::Vector2f(turnAxisPos, width / 2), newAngle))
            speed = 0;
    else
    {
        angle = newAngle;
        posX = newPosX;
        posY = newPosY;
    }
}

int8_t Vehicle::getSpeedSign() const
{
    return speed == 0 ? 0 : speed > 0 ? 1 : -1;
}

void Vehicle::draw(sf::RenderWindow &window) const
{
    sf::RectangleShape rect(sf::Vector2f(length, width));
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(-OUTLINE_WIDTH);
    rect.setFillColor(sf::Color::Green);
    rect.setPosition(posX, posY);
    rect.setOrigin(turnAxisPos, width / 2);
    rect.setRotation(angle * 180 / M_PI);
    window.draw(rect);
}
