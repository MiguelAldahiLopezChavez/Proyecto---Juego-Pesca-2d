#ifndef PESCADOR_HPP
#define PESCADOR_HPP
#include <SFML/Graphics.hpp>
using namespace sf;

class Pescador {
    public:
        Pescador(Texture& textura);
        void update();
        void iniciarAnimacion(bool bucle = true); // true: bucle, false: animación completa
        Sprite& obtenerSprite();
        void mostrarpescando();
        int getFrameActual() const { return currentFrame; }
        bool isAnimacionActiva() const { return animacionActiva; }
        bool isEnBucleFinal() const { return modoBucle && currentFrame >= numFrames - 3; }
        int currentFrame;

    private:
        Sprite sprite;
        Clock clock;
        float frameTime;
        bool animacionActiva;
        bool modoBucle = true;
        static const int numFrames = 11;
        IntRect framesRects[11] = {
            IntRect(0, 0, 155, 500),      // Frame 0
            IntRect(155, 0, 155, 500),    // Frame 1
            IntRect(310, 0, 155, 500),    // Frame 2
            IntRect(465, 0, 155, 500),    // Frame 3
            IntRect(620, 0, 155, 500),    // Frame 4
            IntRect(775, 0, 155, 500),    // Frame 5
            IntRect(930, 0, 155, 500),   // Frame 6
            IntRect(1085, 0, 155, 500),   // Frame 7
            IntRect(1240, 0, 155, 500),   // Frame 8
            IntRect(1395, 0, 155, 500),   // Frame 9
            IntRect(1550, 0, 155, 500),    // Frame  10 
        };
};

#endif

