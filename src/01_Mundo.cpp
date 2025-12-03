#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "audio.hpp"
#include "peces.hpp"
#include "fonts.hpp"
#include "pescador.hpp"
#include "animcuerda.hpp"
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

     Texture sombrapescador;
        if (!sombrapescador.loadFromFile("assets/imagen/op3/personaje/sombra.png")) 
        {
        
            return -1;
        }


    Vector2u size = fondo.getSize();
    unsigned int anchoVentana = size.x;
    unsigned int altoVentana = size.y;

    Sprite spriteSombra(sombrapescador);
    spriteSombra.setPosition(510, 460); // Ajusta la posición 

    Texture pescadoranim;
    if (!pescadoranim.loadFromFile("assets/imagen/op3/personaje/animpesca2.png")) {
        return -1;
    }
    Pescador pescador(pescadoranim);
    // Escalar el sprite de pescador
    float escala = 0.7f;  
    pescador.obtenerSprite().setScale(escala, escala);
    // Posicionar 
    pescador.obtenerSprite().setPosition(anchoVentana/2 - (166*escala)/2, altoVentana/2- (1080*escala)/2 - 230);

    AnimCuerda cuerda (Vector2f(anchoVentana/2, altoVentana/2), Vector2f(anchoVentana/2, altoVentana - 400), 12, 12.0f);

    controlTexto texto;
    texto.loadFont("assets/Letras/opcion 1/Bear Days.ttf");

    Text mostrarTexto;
    Text mostrarTexto1;

    Clock reloj;
    bool mostrarTexto1Visible = true;
    float intervaloparpadeo =0.3f;
    

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

    Pez pez1("assets/imagen/op3/peces/nemo/nemo.png", 100, 100, 1, 0);
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
            if(estado == JUEGO && event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                // Iniciar animación completa con una pulsación
                pescador.iniciarAnimacion();
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
            pescador.update();
            window.draw(pescador.obtenerSprite());
            window.draw(spriteSombra);

            // --- CUERDA ANIMADA ---
            if(pescador.getFrameActual()==10)
            {
                int N = 12; // número de puntos
            Vector2f inicioCuerda(anchoVentana/2-36, altoVentana/2 - 262); // ajusta según sprite pescador
            Vector2f finCuerda(anchoVentana/2-60, altoVentana - 150);      // ajusta según sprite anzuelo
            VertexArray cuerda(LineStrip, N);
            float tiempoCuerda = reloj.getElapsedTime().asSeconds();
            for (int i = 0; i < N; ++i) {
                float t = (float)i / (N - 1);
                float x = inicioCuerda.x + (finCuerda.x - inicioCuerda.x) * t;
                float y = inicioCuerda.y + (finCuerda.y - inicioCuerda.y) * t + sin(t * 3.14 + tiempoCuerda * 2.0) * 8 * (1.0 - t);
                cuerda[i].position = Vector2f(x, y);
                cuerda[i].color = Color::White;
            }
            window.draw(cuerda);
            // --- FIN CUERDA ANIMADA ---

            }
            
        }

        // Transición de fundido
        if (estado == TRANSICION) {
            if (opacidad < 255) {
                opacidad += 1; // velocidad del fundido
                if (opacidad > 255) opacidad = 255;
                fadeRect.setFillColor(Color(0, 0, 0, opacidad));
                window.draw(fadeRect);
            } else {
                // Cuando opacidad llega a 255, mantenemos el frame negro un ciclo
                fadeRect.setFillColor(Color(0, 0, 0, 255));
                window.draw(fadeRect);
                window.display();
                sf::sleep(sf::milliseconds(250)); // Pausa breve para el efecto
                estado = JUEGO;
                fadeRect.setFillColor(Color(0, 0, 0, 0));
                continue;
            }
        }

        window.display();
    }
    detenerMusicaJuego();
    return 0;
}