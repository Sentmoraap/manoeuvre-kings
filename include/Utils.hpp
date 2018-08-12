#include <SFML/System/Vector2.hpp>

namespace Utils
{
    float dot(sf::Vector2f a, sf::Vector2f b);
    bool overlap(float aMin, float aMax, float bMin, float bMax);
    bool rectOverlap(sf::Vector2f center0, sf::Vector2f size0, float rot0,
            sf::Vector2f center1, sf::Vector2f size1, float rot1);
}
