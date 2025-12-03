#include "pescador.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;

Pescador::Pescador(Texture& textura)
    : currentFrame(0), frameTime(0.1f), animacionActiva(false)
{
    sprite.setTexture(textura);
    sprite.setTextureRect(framesRects[0]); // Frame inicial
}

void Pescador::update()
{
    // Actualizar el frame de la animación 
    if(animacionActiva && clock.getElapsedTime().asSeconds() >= frameTime)
    {
        if (currentFrame < numFrames - 1) {
            currentFrame++;
            sprite.setTextureRect(framesRects[currentFrame]);
            clock.restart();
        } else {
            // Al llegar al frame 10, detener animación y quedarse ahí
            animacionActiva = false;
            sprite.setTextureRect(framesRects[numFrames - 1]);
        }
    }
}

void Pescador::iniciarAnimacion()
{
    animacionActiva = true;
    currentFrame = 0;
    sprite.setTextureRect(framesRects[0]);
    clock.restart();
}

Sprite& Pescador::obtenerSprite()
{
    return sprite;
}

void Pescador::mostrarpescando()
{
    animacionActiva = false;
    sprite.setTextureRect(framesRects[10]);
}
