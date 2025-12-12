// pescador.hpp
// Clase que encapsula la animación y el sprite del pescador.
// Provee control de la animación (bucle o secuencia completa) y acceso al sprite.
#ifndef PESCADOR_HPP
#define PESCADOR_HPP
#include <SFML/Graphics.hpp>
using namespace sf;

class Pescador {
    public:
        // Construye el `Pescador` a partir de una textura con la hoja de frames.
        Pescador(Texture& textura);

        // Actualiza el estado de animación (debe llamarse cada frame).
        void update();

        // Inicia la animación. Si `bucle` es true, entra en modo bucle;
        // si es false, reproduce la animación completa y luego pasa a bucle final.
        void iniciarAnimacion(bool bucle = true); // true: bucle, false: animación completa

        // Devuelve referencia al `Sprite` para posicionarlo/dibujarlo.
        Sprite& obtenerSprite();

        // Forzar a mostrar la pose final de pesca (frame final).
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
        // Número de frames en la hoja de animación (constante).
        static const int numFrames = 11;
        // Rectángulos que definen cada frame dentro de la textura.
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

