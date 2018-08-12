#include "Utils.hpp"
#include <cmath>

float Utils::dot(sf::Vector2f a, sf::Vector2f b)
{
    return a.x * b.x + a.y * b.y;
}

bool Utils::overlap(float aMin, float aMax, float bMin, float bMax)
{
    return bMax >= aMin && bMin <= aMax;
}

bool Utils::rectOverlap(sf::Vector2f center0, sf::Vector2f halfSize0, float rot0,
        sf::Vector2f center1, sf::Vector2f halfSize1, float rot1)
{
    sf::Vector2f axes[4];
    axes[0] = sf::Vector2f(cos(rot0), sin(rot0));
    axes[1] = sf::Vector2f(-axes[0].y, axes[0].x);
    axes[2] = sf::Vector2f(cos(rot1), sin(rot1));
    axes[3] = sf::Vector2f(-axes[2].y, axes[2].x);

    sf::Vector2f centerDiff = center1 - center0;
    float base = dot(centerDiff, axes[0]), d20 = fabs(dot(axes[2], axes[0])), d30 = fabs(dot(axes[3], axes[0]));
    float diff = d20 * halfSize1.x + d30 * halfSize1.y;
    if(!overlap(-halfSize0.x, halfSize0.x, base - diff, base + diff)) return false;

    base = dot(centerDiff, axes[1]);
    float d21 = fabs(dot(axes[2], axes[1])), d31 = fabs(dot(axes[3], axes[1]));
    diff = d21 * halfSize1.x + d31 * halfSize1.y;
    if(!overlap(-halfSize0.y, halfSize0.y, base - diff, base + diff)) return false;

    centerDiff = -centerDiff;
    base = Utils::dot(centerDiff, axes[2]);
    diff = d20 * halfSize0.x + d21 * halfSize0.y;
    if(!overlap(-halfSize1.x, halfSize1.x, base - diff, base + diff)) return false;

    base = Utils::dot(centerDiff, axes[3]);
    diff = d30 * halfSize0.x + d31 * halfSize0.y;
    return overlap(-halfSize1.y, halfSize1.y, base - diff, base + diff);
}
