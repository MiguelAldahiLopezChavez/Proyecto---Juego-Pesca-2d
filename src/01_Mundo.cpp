#include <SFML/Graphics.hpp>
#include "audio.hpp"
using namespace sf;

int main()
{
    RenderWindow window(VideoMode(800, 600), "Juego de Pesca 2D");
    iniciarMusicaJuego("assets/Musica/Troubadeck 25 Deep Dark Sea.ogg");
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        window.clear(Color::Black);
        // Aquí se pueden agregar dibujos y actualizaciones de la ventana
        window.display();
    }
    detenerMusicaJuego();
    return 0;
}