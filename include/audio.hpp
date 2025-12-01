#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <SFML/Audio.hpp>
#include <string>

using namespace sf; 

static Music musicajuego;

void iniciarMusicaJuego(const std::string& musicpath);
void detenerMusicaJuego();
#endif 