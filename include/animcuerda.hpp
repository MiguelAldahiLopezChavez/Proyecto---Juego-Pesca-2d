 #ifndef ANIMCUERDA_HPP
 #define ANIMCUERDA_HPP
 #include <SFML/Graphics.hpp>
 
 class AnimCuerda {
 public:
	 AnimCuerda(sf::Vector2f inicio, sf::Vector2f fin, int puntos = 10, float amplitud = 10.0f);
	 void update(float dt);
	 void draw(sf::RenderWindow& window);
	 void setExtremos(sf::Vector2f inicio, sf::Vector2f fin);
 private:
	 sf::Vector2f pInicio, pFin;
	 int nPuntos;
	 float amp;
	 float tiempo;
	 sf::VertexArray cuerda;
 };
 
 #endif
