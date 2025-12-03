#include "pescador.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;

Pescador::Pescador(Texture& textura)
    : currentFrame(0), frameTime(0.2f), animacionActiva(false)
{
    sprite.setTexture(textura);
    sprite.setTextureRect(framesRects[0]); // Frame inicial

}

void Pescador::update()
{
    if(animacionActiva && clock.getElapsedTime().asSeconds() >= frameTime)
    {
        if (modoBucle) {
            // Bucle entre frames 0-3 si está al inicio, o entre 8-10 si está al final
            int inicio = (currentFrame >= numFrames - 3) ? numFrames - 3 : 0;
            int fin = (currentFrame >= numFrames - 3) ? numFrames - 1 : 3;
            if (currentFrame < fin) {
                currentFrame++;
            } else {
                currentFrame = inicio;
            }
            sprite.setTextureRect(framesRects[currentFrame]);
            clock.restart();
        } else {
            // Animación completa (frames 0 a 10)
            if (currentFrame < numFrames - 1) {
                currentFrame++;
                sprite.setTextureRect(framesRects[currentFrame]);
                clock.restart();
            } else {
                // Cuando termina, bucle entre los últimos 3 frames
                modoBucle = true;
                currentFrame = numFrames - 3;
            }
        }
    }
}

void Pescador::iniciarAnimacion(bool bucle)
{
    animacionActiva = true;
    modoBucle = bucle;
    if (bucle) {
        // Si estaba en el bucle final, vuelve al bucle inicial
        currentFrame = (currentFrame >= numFrames - 3) ? 0 : currentFrame;
        sprite.setTextureRect(framesRects[currentFrame]);
    } else {
        // Si estaba en el bucle final, reinicia desde 0
        if (currentFrame >= numFrames - 3) {
            currentFrame = 0;
        }
        sprite.setTextureRect(framesRects[currentFrame]);
    }
    clock.restart();
}

Sprite& Pescador::obtenerSprite()
{
    return sprite;
}

void Pescador::mostrarpescando()
{
    animacionActiva = false;
    sprite.setTextureRect(framesRects[numFrames-1]);
}