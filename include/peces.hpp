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
	int basePoints;          // puntos base sin multiplicador
	float minWeight;         // peso mínimo del rango (kg)
	float maxWeight;         // peso máximo del rango (kg)
	float actualWeight;      // peso actual generado dentro del rango
	float multiplier;        // multiplicador calculado basado en peso
	int finalPoints;         // puntos finales con multiplicador aplicado
	string imagePath;    // ruta del asset para cargas directas
	Texture texture;     // recurso gráfico
	Sprite  sprite;      // sprite listo para dibujar
};

// Gestor de recompensas: carga y selecciona aleatoriamente
class RewardManager {
public:
	// Cargar lista de recompensas disponibles
	void loadDefaults();

	// Selección una recompensa aleatoria ponderada por "rarity"
	const Reward& pickRandom();

	// Generar peso aleatorio dentro del rango específico del pez
	float generateWeightInRange(float minWeight, float maxWeight);

	// Calcular multiplicador basado en peso relativo
	float calculateMultiplier(float actualWeight, float minWeight, float maxWeight);

	// Calcular puntos finales con multiplicador
	int calculateFinalPoints(int basePoints, float multiplier);

	// Acceso a todas
	const vector<Reward>& all() const { return rewards; }

private:
	vector<Reward> rewards;
	mt19937 rng{random_device{}()};
};

#endif