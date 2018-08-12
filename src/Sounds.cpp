#include "Sounds.hpp"

sf::SoundBuffer Sounds::engineSound;
sf::SoundBuffer Sounds::hitSound;
sf::SoundBuffer Sounds::targetSound;

void Sounds::loadSounds()
{
    engineSound.loadFromFile("assets/engine_sound_cut.flac");
    hitSound.loadFromFile("assets/hit_sound.flac");
    targetSound.loadFromFile("assets/target.flac");
}
