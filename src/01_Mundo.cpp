#include <SFML/Graphics.hpp>
#include <iostream>
#include "audio.hpp"
#include "peces.hpp"
#include "fonts.hpp"
using namespace sf;

int main()
{
    Texture fondo;
       if (!fondo.loadFromFile("assets/Background.png")) 
    {

        return -1;
    }   

    Texture fondo2;
         if (!fondo2.loadFromFile("assets/imagen/fondo3.png")) 
     {
    
          return -1;
     }

    Vector2u size = fondo.getSize();
    unsigned int anchoVentana = size.x;
    unsigned int altoVentana = size.y;

    controlTexto texto;
    texto.loadFont("assets/Letras/opcion 1/Bear Days.ttf");

    Text mostrarTexto;
    Text mostrarTexto1;

    mostrarTexto.setFont(texto.getFont());
    mostrarTexto.setString("Pesca2D");
    mostrarTexto.setCharacterSize(100); 
    mostrarTexto.setFillColor(Color::Black);
    mostrarTexto.setPosition(anchoVentana/2 -150, altoVentana/2-50);

    mostrarTexto1.setFont(texto.getFont());
    mostrarTexto1.setString("(Presiona Enter para iniciar)");
    mostrarTexto1.setCharacterSize(30);
    mostrarTexto1.setFillColor(Color::White);
    mostrarTexto1.setPosition(anchoVentana/2 -180, altoVentana/2 + 70);

    RenderWindow window(VideoMode(anchoVentana, altoVentana), "Juego de Pesca 2D");
    iniciarMusicaJuego("assets/Musica/Troubadeck 25 Deep Dark Sea.ogg");

    Pez pez1("assets/imagen/op3/peces/nemo/nemo.png", 100, 100, 1, 0);
    Sprite spriteFondo(fondo);
    Sprite spriteFondo2(fondo2);

    bool pantallaInicio = true;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (pantallaInicio && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
                pantallaInicio = false;
        }
        window.clear();
        window.draw(spriteFondo);
        if (pantallaInicio) {
            window.draw(mostrarTexto);
            window.draw(mostrarTexto1);
        } else {
            window.draw(spriteFondo2);
            //pez1.mover();
            //window.draw(pez1.obtenerSprite());
        }
        window.display();
    }
    detenerMusicaJuego();
    return 0;
}