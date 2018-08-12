#include "Vehicle.hpp"
#include "Level.hpp"
#include "Utils.hpp"
#include "Sounds.hpp"

Vehicle::Vehicle()
{
    engineSound.setBuffer(Sounds::engineSound);
    engineSound.setLoop(true);
    hitSound.setBuffer(Sounds::hitSound);
    targetSound.setBuffer(Sounds::targetSound);
}

void Vehicle::setPosAndAngle(double posX, double posY, float angle)
{
    speed = 0;
    this->posX = posX;
    this->posY = posY;
    this->angle = angle;
}

void Vehicle::addTarget(int16_t posX, int16_t posY)
{
    targets.push_back(Target{posX, posY});
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
    sf::Vector2f dx(cos(angle), sin(angle)), dy(dx.y, -dx.x);
    double newPosX = posX + speed * dt * dx.x;
    double newPosY = posY + speed * dt * dx.y;
    if(level.checkSat(sf::FloatRect(sf::Vector2f(newPosX, newPosY), sf::Vector2f(length, width)),
        sf::Vector2f(turnAxisPos, width / 2), newAngle))
    {
        if(playSounds && fabs(speed) > minSpeedHitSound && hitSound.getStatus() != sf::SoundSource::Status::Playing)
        {
            hitSound.play();
        }
        speed = 0;
    }
    else
    {
        angle = newAngle;
        posX = newPosX;
        posY = newPosY;
        for(uint8_t i = 0; i < targets.size(); i++)
        {
            Target &target = targets[i];
            sf::Vector2f dp = sf::Vector2f(target.posX - posX, target.posY - posY);
            dp = sf::Vector2f(Utils::dot(dp, dx), Utils::dot(dp, dy));
            double dAngle = atan2(dp.y, dp.x), w = length - turnAxisPos, h = width / 2;
            if(dAngle < 0)
            {
                dAngle = -dAngle;
                dp.y = -dp.y;
            }
            if(dp.y > h + Target::RADIUS) continue;
            if(dAngle > M_PI / 2)
            {
                dAngle = M_PI - dAngle;
                w = turnAxisPos;
                dp.x = -dp.x;
            }
            if(dp.x > w + Target::RADIUS) continue;
            bool col = dp.x <= w || dp.y <= h;
            if(!col)
            {
                dp -= sf::Vector2f(w, h);
                col = Utils::dot(dp, dp) <= Target::SQR_RADIUS;
            }
            if(col)
            {
                if(playSounds) targetSound.play();
                std::swap(target, targets.back());
                targets.pop_back();
                i--;
            }
        }
    }
    float pitch = std::min(maxPitch, std::max(minPitch, static_cast<float>(fabs(speed) * speedPitchMult)));
    engineSound.setPitch(pitch);
    float volume = std::min(maxVolume, std::max(minVolume, static_cast<float>(fabs(speed) * speedVolumeMult)));
    engineSound.setVolume(volume);
}

int8_t Vehicle::getSpeedSign() const
{
    return speed == 0 ? 0 : speed > 0 ? 1 : -1;
}

bool Vehicle::hasTargets() const
{
    return !targets.empty();
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

void Vehicle::drawTargets(sf::RenderWindow &window) const
{
    sf::CircleShape circle(Target::RADIUS);
    circle.setFillColor(sf::Color::Green);
    circle.setOrigin(Target::RADIUS, Target::RADIUS);
    for(const Target &target : targets)
    {
        circle.setPosition(target.posX, target.posY);
        window.draw(circle);
    }
}

void Vehicle::setPlaySounds(bool playSounds)
{
    this->playSounds = playSounds;
    if(playSounds)
    {
        engineSound.play();
    }
    else
    {
        engineSound.stop();
    }
}
