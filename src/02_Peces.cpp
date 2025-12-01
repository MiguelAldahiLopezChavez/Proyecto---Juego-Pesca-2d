#include <SFML/Graphics.hpp>
#include "peces.hpp"
#include <iostream>
using namespace sf;
Pez::Pez(const string& rutatext, float posX, float posY, float velX, float velY) {
    if (!peces.loadFromFile(rutatext)) {
        cout << "Error al cargar la textura del pez desde: " << rutatext << endl;
        return;
        }
    spritepez1.setTexture(peces);
    spritepez1.setPosition(posX, posY);
    velocidad = Vector2f(velX, velY);
}

void Pez::mover(){
    spritepez1.move(velocidad);
}

Sprite& Pez::obtenerSprite(){
    return spritepez1;
}