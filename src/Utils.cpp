#include "Utils.hpp"

float Utils::dot(sf::Vector2f a, sf::Vector2f b)
{
    return a.x * b.x + a.y * b.y;
}

bool Utils::overlap(float aMin, float aMax, float bMin, float bMax)
{
    return bMax >= aMin && bMin <= aMax;
}
