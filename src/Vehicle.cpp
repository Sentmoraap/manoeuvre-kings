#include "Vehicle.hpp"
#include "Level.hpp"
#include "Utils.hpp"
#include "Sounds.hpp"

Vehicle::Vehicle(uint16_t length, uint16_t turnAxisPos) : length(length), turnAxisPos(turnAxisPos)
{
    engineSound.setBuffer(Sounds::engineSound);
    engineSound.setLoop(true);
    hitSound.setBuffer(Sounds::hitSound);
    targetSound.setBuffer(Sounds::targetSound);
}

Vehicle::Vehicle() : Vehicle(0, 0) {}

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

void Vehicle::update(double dt, bool current, const Level &level, const std::vector<Vehicle> &vehicles)
{
    int8_t accelInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Up) - sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    if(current && accelInput)
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
    int8_t turnInput = current
        ? sf::Keyboard::isKeyPressed(sf::Keyboard::Right) - sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
        : 0;
    double newAngle = angle + turnInput * std::min(fabs(speed) * turnSpeedBase, maxTurnSpeed) * dt * getSpeedSign();
    sf::Vector2f dx(cos(newAngle), sin(newAngle)), dy(dx.y, -dx.x);
    double newPosX = posX + speed * dt * dx.x;
    double newPosY = posY + speed * dt * dx.y;
    uint8_t dc = (length / 2 - turnAxisPos);
    sf::Vector2f testPos = sf::Vector2f(newPosX + dx.x * dc, newPosY + dx.y * dc);
    sf::Vector2f testSize = sf::Vector2f(length / 2, width / 2);
    bool collide = level.checkSat(testPos, testSize, newAngle);
    if(!collide) for(const Vehicle &v : vehicles)
    {
        if(this == &v) continue;
        sf::Vector2f othDx(cos(v.angle), sin(v.angle));//, othDy(othDx.y, -othDx.x);
        uint8_t othDc = (v.length / 2 - v.turnAxisPos);
        sf::Vector2f testPosOth = sf::Vector2f(v.posX + othDx.x * othDc, v.posY + othDx.y * othDc);
        sf::Vector2f testSizeOth = sf::Vector2f(v.length / 2, v.width / 2);
        collide = Utils::rectOverlap(testPos, testSize, newAngle, testPosOth, testSizeOth, v.angle);
        if(collide) break;
    }
    if(collide)
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

void Vehicle::draw(sf::RenderWindow &window, bool current, sf::Color color) const
{
    sf::RectangleShape rect(sf::Vector2f(length, width));
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(current ? -OUTLINE_WIDTH : 0);
    rect.setFillColor(color);
    rect.setPosition(posX, posY);
    rect.setOrigin(turnAxisPos, width / 2);
    rect.setRotation(angle * 180 / M_PI);
    window.draw(rect);
}

void Vehicle::drawTargets(sf::RenderWindow &window, sf::Color color) const
{
    sf::CircleShape circle(Target::RADIUS);
    circle.setFillColor(color);
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
