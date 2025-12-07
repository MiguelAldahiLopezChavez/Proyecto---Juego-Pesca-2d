#ifndef PECES_HPP
#define PECES_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <random>
using namespace sf;
using namespace std;

// Tipos de recompensa posibles
enum class RewardType {
	Fish
};

// Datos de una recompensa (pez u objeto)
struct Reward {
	RewardType type;
	string id;          // identificador interno
	string displayName; // nombre para mostrar
	float rarity;            // 0.0 (común) a 1.0 (muy raro)
	string imagePath;    // ruta del asset para cargas directas
	Texture texture;     // recurso gráfico
	Sprite  sprite;      // sprite listo para dibujar
};

// Gestor de recompensas: carga y selecciona aleatoriamente
class RewardManager {
public:
	// Cargar lista de recompensas disponibles
	void loadDefaults();

	// Seleccionar una recompensa aleatoria ponderada por "rarity"
	const Reward& pickRandom();

	// Acceso a todas
	const vector<Reward>& all() const { return rewards; }

private:
	vector<Reward> rewards;
	mt19937 rng{random_device{}()};
};

#endif