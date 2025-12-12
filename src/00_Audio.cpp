// 00_Audio.cpp
// Implementación de control de música del juego.
// Comentarios añadidos para explicar el propósito de cada función
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf; 

// Music global compartida con el header `audio.hpp`.
static Music musicajuego;

// Abre y reproduce la pista indicada por `musicpath`.
// Si la carga falla, la función retorna silenciosamente.
void iniciarMusicaJuego(const std::string& musicpath)
{
    if (!musicajuego.openFromFile(musicpath))
    {
        // Error al cargar el archivo de música
        return;
    }
    // Reproducir en bucle y fijar volumen por defecto
    musicajuego.setLoop(true); // Configurar para que la música se repita
    musicajuego.setVolume(50); // Volumen al 50% para asegurar que se escuche
    musicajuego.play();
}
 
// Detiene la reproducción de la música en `musicajuego`.
void detenerMusicaJuego()
{
    musicajuego.stop();
}