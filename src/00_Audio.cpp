#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf; 

static Music musicajuego;

void iniciarMusicaJuego(const std::string& musicpath)
{
    if (!musicajuego.openFromFile(musicpath))
    {
        // Error al cargar el archivo de música
        return;
    }
    musicajuego.setLoop(true); // Configurar para que la música se repita
    musicajuego.setVolume(50); // Volumen al 50% para asegurar que se escuche
    musicajuego.play();
}
 
void detenerMusicaJuego()
{
    musicajuego.stop();
}