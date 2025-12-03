#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "audio.hpp"
#include "peces.hpp"
#include "fonts.hpp"
#include "pescador.hpp"
using namespace sf;

int main()
{
    Texture fondo;
       if (!fondo.loadFromFile("assets/Background.png")) 
    {

        return -1;
    }   

    Texture fondo2;
         if (!fondo2.loadFromFile("assets/imagen/nuevaimagen/background.png")) 
     {
    
          return -1;
     }

     Texture sombrapescador;
        if (!sombrapescador.loadFromFile("assets/imagen/nuevaimagen/sombra.png")) 
        {
        
            return -1;
        }


    Vector2u size = fondo.getSize();
    unsigned int anchoVentana = size.x;
    unsigned int altoVentana = size.y;

    Sprite spriteSombra(sombrapescador);
    spriteSombra.setPosition(505, 460); 


    Texture pescadoranim;
    if (!pescadoranim.loadFromFile("assets/imagen/nuevaimagen/cast bobbin Sheet.png")) {
        return -1;
    }
    Pescador pescador(pescadoranim);
    float escala = 1.5f;  
    pescador.obtenerSprite().setScale(escala, escala);
    pescador.obtenerSprite().setPosition(anchoVentana/2-120, altoVentana/2-195);
    bool animacionBucleIniciada = false;
    bool bucleFinalIniciado = false;


    controlTexto texto;
    texto.loadFont("assets/Letras/opcion 1/Bear Days.ttf");

    Text mostrarTexto;
    Text mostrarTexto1;

    Clock reloj;
    bool mostrarTexto1Visible = true;
    float intervaloparpadeo =0.3f;
    bool animacionCortaMostrada = false;
    

    mostrarTexto.setFont(texto.getFont());
    mostrarTexto.setString("Pesca2D");
    mostrarTexto.setCharacterSize(200); 
    mostrarTexto.setFillColor(Color::White);
    mostrarTexto.setPosition(anchoVentana/2 -350, altoVentana/2-200);

    mostrarTexto1.setFont(texto.getFont());
    mostrarTexto1.setString("(Presiona Enter para iniciar)");
    mostrarTexto1.setCharacterSize(30);
    mostrarTexto1.setFillColor(Color::White);
    mostrarTexto1.setPosition(anchoVentana/2 -180, altoVentana/2 + 70);

    RenderWindow window(VideoMode(anchoVentana, altoVentana), "Juego de Pesca 2D");
    iniciarMusicaJuego("assets/Musica/Troubadeck 25 Deep Dark Sea.ogg");

    //Pez pez1("assets/imagen/op3/peces/nemo/nemo.png", 100, 100, 1, 0);
    Sprite spriteFondo(fondo);
    Sprite spriteFondo2(fondo2);

    enum EstadoPantalla { INICIO, TRANSICION, JUEGO };
    EstadoPantalla estado = INICIO;

    int opacidad = 0;
    RectangleShape fadeRect(Vector2f(anchoVentana, altoVentana));
    fadeRect.setFillColor(Color(0, 0, 0, 0));


    while (window.isOpen())
    {
        Clock reloj1;
        float dt = reloj1.restart().asSeconds();
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (estado == INICIO && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                estado = TRANSICION;
                opacidad = 0; // Inicializa opacidad solo al entrar en transición
            }
            if (estado == JUEGO && event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                // Iniciar animación completa desde el inicio
                bucleFinalIniciado = false;
                animacionBucleIniciada = false;
                pescador.iniciarAnimacion(false); 

            }
        }

        if(reloj.getElapsedTime().asSeconds() > intervaloparpadeo)
        {
            mostrarTexto1Visible = !mostrarTexto1Visible;
            reloj.restart();
        }
        window.clear();
        window.draw(spriteFondo);



if (estado == INICIO) {
            window.draw(mostrarTexto);
            if (mostrarTexto1Visible) {
                window.draw(mostrarTexto1);
            }

        } else if (estado == JUEGO) {
            window.clear();
            window.draw(spriteFondo2);
            // Inicia el bucle de los primeros 3 frames solo si no hay animación activa
            if (!pescador.isAnimacionActiva() && pescador.getFrameActual() < 3) {
                pescador.iniciarAnimacion(true); // true = bucle (primeros 3 frames)
            }
            pescador.update();
            window.draw(pescador.obtenerSprite());
            window.draw(spriteSombra);
        }

             // Transición de fundido
        if (estado == TRANSICION) {
            if (opacidad < 255) {
                opacidad += 1; // velocidad del fundido
                if (opacidad > 255) opacidad = 255;
                fadeRect.setFillColor(Color(0, 0, 0, opacidad));
                window.draw(fadeRect);
            } else {
                // Cuando opacidad llega a 255, se mantiene el frame negro un ciclo
                fadeRect.setFillColor(Color(0, 0, 0, 255));
                window.draw(fadeRect);
                window.display();
                sf::sleep(sf::milliseconds(250)); // Pausa breve para el efecto
                estado = JUEGO;
                animacionCortaMostrada = false;
                fadeRect.setFillColor(Color(0, 0, 0, 0));
                continue;
            }
        }
        window.display();
        
        }

        detenerMusicaJuego();
        return 0;
    }

