#include <SFML/Audio.hpp>

 class Sounds
 {
    public:
        static void loadSounds();
        static sf::SoundBuffer engineSound;
        static sf::SoundBuffer hitSound;
        static sf::SoundBuffer targetSound;
 };
