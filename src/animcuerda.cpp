#include "animcuerda.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

AnimCuerda::AnimCuerda(sf::Vector2f inicio, sf::Vector2f fin, int puntos, float amplitud)
    : pInicio(inicio), pFin(fin), nPuntos(puntos), amp(amplitud), tiempo(0.0f)
{
    cuerda.setPrimitiveType(sf::LineStrip);
    cuerda.resize(nPuntos);
}

void AnimCuerda::update(float dt)
{
    tiempo += dt;
    for (int i = 0; i < nPuntos; ++i) {
        float t = (float)i / (nPuntos - 1);
        float x = pInicio.x + (pFin.x - pInicio.x) * t;
        float y = pInicio.y + (pFin.y - pInicio.y) * t + std::sin(t * 3.14f + tiempo * 2.0f) * amp * (1.0f - t); // animación senoidal
        cuerda[i].position = sf::Vector2f(x, y);
        cuerda[i].color = sf::Color::White;
    }
}

void AnimCuerda::draw(sf::RenderWindow& window)
{
    window.draw(cuerda);
}

void AnimCuerda::setExtremos(sf::Vector2f inicio, sf::Vector2f fin)
{
    pInicio = inicio;
    pFin = fin;
}
