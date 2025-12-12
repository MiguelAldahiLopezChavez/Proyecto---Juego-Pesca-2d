// audio.hpp
// Proporciona funciones simples para controlar la música de fondo del juego.
// Nota: este archivo expone un `Music` global (`musicajuego`) para uso sencillo
// en el juego. No se eliminan líneas existentes: solo se añaden comentarios.
#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <SFML/Audio.hpp>
#include <string>

using namespace sf; 

// Objeto `Music` global que se usa para reproducir la música principal del juego.
static Music musicajuego;

// Inicia la música de fondo a partir de la ruta proporcionada.
void iniciarMusicaJuego(const std::string& musicpath);

// Detiene la reproducción de la música de fondo.
void detenerMusicaJuego();
#endif 