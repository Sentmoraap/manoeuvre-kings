#include <SFML/System/Vector2.hpp>

namespace Utils
{
    float dot(sf::Vector2f a, sf::Vector2f b);
    bool overlap(float aMin, float aMax, float bMin, float bMax);
}
