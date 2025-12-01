#ifndef PECES_HPP
#define PECES_HPP
#include <string>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

class Pez {

public:
    Pez(const string&  rutatext, float posX, float posY, float velX, float VelY);
    
    void mover();

    Sprite& obtenerSprite();

private:
    Texture peces;
    Sprite spritepez1;
    Vector2f velocidad;

    
};

#endif