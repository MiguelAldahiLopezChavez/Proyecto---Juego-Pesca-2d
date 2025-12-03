#ifndef PESCADOR_HPP
#define PESCADOR_HPP
#include <SFML/Graphics.hpp>
using namespace sf;

class Pescador {
public:
    Pescador(Texture& textura);
    void update(); // Método automático como Pikachu
    void iniciarAnimacion();
    Sprite& obtenerSprite();
    void mostrarpescando();
    int getFrameActual() const { return currentFrame; }

private:
    Sprite sprite;
    int currentFrame;
    int numFrames = 11;
    Clock clock;
    float frameTime;
    bool animacionActiva;
    
    // Coordenadas FIJAS para cada frame como en el código de Pikachu
    IntRect framesRects[11] = {
        IntRect(0, 0, 190, 1080),      // Frame 0
        IntRect(190, 0, 190, 1080),    // Frame 1
        IntRect(380, 0, 190, 1080),    // Frame 2
        IntRect(570, 0, 190, 1080),    // Frame 3
        IntRect(760, 0, 190, 1080),    // Frame 4
        IntRect(950, 0, 190, 1080),    // Frame 5
        IntRect(1140, 0, 190, 1080),   // Frame 6
        IntRect(1330, 0, 190, 1080),   // Frame 7
        IntRect(1520, 0, 190, 1080),   // Frame 8
        IntRect(1710, 0, 190, 1080),   // Frame 9
        IntRect(1900, 0, 190, 1080)    // Frame 10
    };
};

#endif

