#include <SFML/Graphics.hpp>
#include "peces.hpp"
#include <iostream>
#include <cassert>
using namespace sf;
using namespace std;

static bool loadSprite(Reward& r, const string& path, const Vector2f& scale = Vector2f(1.f,1.f)) {
    if (!r.texture.loadFromFile(path)) {
        return false;
    }
    r.sprite.setTexture(r.texture);
    r.sprite.setScale(scale);
    return true;
}

void RewardManager::loadDefaults() {
    rewards.clear();

    // === PECES RAROS/LEGENDARIOS (PESO BAJO: 1-7kg) ===
    
    // Pez Dorado - Legendario (1-5kg)
    Reward goldenfish{RewardType::Fish, "goldenfish", "Pez Dorado", 0.05f, 500, 1.0f, 5.0f, 0.0f, 0.0f, 0};
    goldenfish.actualWeight = generateWeightInRange(goldenfish.minWeight, goldenfish.maxWeight);
    goldenfish.multiplier = calculateMultiplier(goldenfish.actualWeight, goldenfish.minWeight, goldenfish.maxWeight);
    goldenfish.finalPoints = calculateFinalPoints(goldenfish.basePoints, goldenfish.multiplier);
    goldenfish.imagePath = "assets/imagen/nuevaimagen/golden fish.png";
    if (loadSprite(goldenfish, goldenfish.imagePath, {0.8f,0.8f})) rewards.push_back(move(goldenfish));

    // Pez Rojo Dorado - Épico (1-6kg)
    Reward goldenredfish{RewardType::Fish, "goldenredfish", "Pez Rojo Dorado", 0.06f, 400, 1.0f, 6.0f, 0.0f, 0.0f, 0};
    goldenredfish.actualWeight = generateWeightInRange(goldenredfish.minWeight, goldenredfish.maxWeight);
    goldenredfish.multiplier = calculateMultiplier(goldenredfish.actualWeight, goldenredfish.minWeight, goldenredfish.maxWeight);
    goldenredfish.finalPoints = calculateFinalPoints(goldenredfish.basePoints, goldenredfish.multiplier);
    goldenredfish.imagePath = "assets/imagen/nuevaimagen/pezdorado-rojo.png";
    if (loadSprite(goldenredfish, goldenredfish.imagePath, {1.0f,1.0f})) rewards.push_back(move(goldenredfish));

    // Pez Rojo - Épico (1-7kg)
    Reward redfish{RewardType::Fish, "redfish", "Pez Rojo", 0.06f, 350, 1.0f, 7.0f, 0.0f, 0.0f, 0};
    redfish.actualWeight = generateWeightInRange(redfish.minWeight, redfish.maxWeight);
    redfish.multiplier = calculateMultiplier(redfish.actualWeight, redfish.minWeight, redfish.maxWeight);
    redfish.finalPoints = calculateFinalPoints(redfish.basePoints, redfish.multiplier);
    redfish.imagePath = "assets/imagen/nuevaimagen/pezrojo.png";
    if (loadSprite(redfish, redfish.imagePath, {1.0f,1.0f})) rewards.push_back(move(redfish));

    // Camarón Morado - Raro (2-8kg)
    Reward campurple{RewardType::Fish, "campurple", "Camaron Morado", 0.07f, 280, 2.0f, 8.0f, 0.0f, 0.0f, 0};
    campurple.actualWeight = generateWeightInRange(campurple.minWeight, campurple.maxWeight);
    campurple.multiplier = calculateMultiplier(campurple.actualWeight, campurple.minWeight, campurple.maxWeight);
    campurple.finalPoints = calculateFinalPoints(campurple.basePoints, campurple.multiplier);
    campurple.imagePath = "assets/imagen/nuevaimagen/cammorado.png";
    if (loadSprite(campurple, campurple.imagePath, {1.0f,1.0f})) rewards.push_back(move(campurple));

    // Camarón Rojo - Raro (2-8kg)
    Reward camred{RewardType::Fish, "camred", "Camaron Rojo", 0.07f, 280, 2.0f, 8.0f, 0.0f, 0.0f, 0};
    camred.actualWeight = generateWeightInRange(camred.minWeight, camred.maxWeight);
    camred.multiplier = calculateMultiplier(camred.actualWeight, camred.minWeight, camred.maxWeight);
    camred.finalPoints = calculateFinalPoints(camred.basePoints, camred.multiplier);
    camred.imagePath = "assets/imagen/nuevaimagen/camrojo.png";
    if (loadSprite(camred, camred.imagePath, {1.0f,1.0f})) rewards.push_back(move(camred));

    // === PECES INTERMEDIOS ===
    
    // Pez Blanco - Poco Común (3-12kg)
    Reward whitefish{RewardType::Fish, "whitefish", "Pez Blanco", 0.08f, 250, 3.0f, 12.0f, 0.0f, 0.0f, 0};
    whitefish.actualWeight = generateWeightInRange(whitefish.minWeight, whitefish.maxWeight);
    whitefish.multiplier = calculateMultiplier(whitefish.actualWeight, whitefish.minWeight, whitefish.maxWeight);
    whitefish.finalPoints = calculateFinalPoints(whitefish.basePoints, whitefish.multiplier);
    whitefish.imagePath = "assets/imagen/nuevaimagen/pezblanco.png";
    if (loadSprite(whitefish, whitefish.imagePath, {1.0f,1.0f})) rewards.push_back(move(whitefish));

    // Pez Azul-Verde - Raro (4-15kg)
    Reward bluegreenfish{RewardType::Fish, "bluegreenfish", "Pez Azul-Verde", 0.07f, 300, 4.0f, 15.0f, 0.0f, 0.0f, 0};
    bluegreenfish.actualWeight = generateWeightInRange(bluegreenfish.minWeight, bluegreenfish.maxWeight);
    bluegreenfish.multiplier = calculateMultiplier(bluegreenfish.actualWeight, bluegreenfish.minWeight, bluegreenfish.maxWeight);
    bluegreenfish.finalPoints = calculateFinalPoints(bluegreenfish.basePoints, bluegreenfish.multiplier);
    bluegreenfish.imagePath = "assets/imagen/nuevaimagen/pezverde-azul.png";
    if (loadSprite(bluegreenfish, bluegreenfish.imagePath, {1.0f,1.0f})) rewards.push_back(move(bluegreenfish));

    // === PECES COMUNES (PESO ALTO: 8-25kg) ===
    
    // Pez Azul - Común (8-25kg)
    Reward bluefish{RewardType::Fish, "bluefish", "Pez Azul", 0.14f, 120, 8.0f, 25.0f, 0.0f, 0.0f, 0};
    bluefish.actualWeight = generateWeightInRange(bluefish.minWeight, bluefish.maxWeight);
    bluefish.multiplier = calculateMultiplier(bluefish.actualWeight, bluefish.minWeight, bluefish.maxWeight);
    bluefish.finalPoints = calculateFinalPoints(bluefish.basePoints, bluefish.multiplier);
    bluefish.imagePath = "assets/imagen/nuevaimagen/pezazul.png";
    if (loadSprite(bluefish, bluefish.imagePath, {1.0f,1.0f})) rewards.push_back(move(bluefish));

    // Pez Verde - Común (8-25kg)
    Reward greenfish{RewardType::Fish, "greenfish", "Pez Verde", 0.14f, 120, 8.0f, 25.0f, 0.0f, 0.0f, 0};
    greenfish.actualWeight = generateWeightInRange(greenfish.minWeight, greenfish.maxWeight);
    greenfish.multiplier = calculateMultiplier(greenfish.actualWeight, greenfish.minWeight, greenfish.maxWeight);
    greenfish.finalPoints = calculateFinalPoints(greenfish.basePoints, greenfish.multiplier);
    greenfish.imagePath = "assets/imagen/nuevaimagen/pezverde.png";
    if (loadSprite(greenfish, greenfish.imagePath, {1.0f,1.0f})) rewards.push_back(move(greenfish));

    // Pez Morado - Común (8-25kg)
    Reward purplefish{RewardType::Fish, "purplefish", "Pez Morado", 0.14f, 120, 8.0f, 25.0f, 0.0f, 0.0f, 0};
    purplefish.actualWeight = generateWeightInRange(purplefish.minWeight, purplefish.maxWeight);
    purplefish.multiplier = calculateMultiplier(purplefish.actualWeight, purplefish.minWeight, purplefish.maxWeight);
    purplefish.finalPoints = calculateFinalPoints(purplefish.basePoints, purplefish.multiplier);
    purplefish.imagePath = "assets/imagen/nuevaimagen/pezmorado.png";
    if (loadSprite(purplefish, purplefish.imagePath, {1.0f,1.0f})) rewards.push_back(move(purplefish));

    // Pez Café - Común (10-20kg)
    Reward brownfish{RewardType::Fish, "brownfish", "Pez Cafe", 0.12f, 150, 10.0f, 20.0f, 0.0f, 0.0f, 0};
    brownfish.actualWeight = generateWeightInRange(brownfish.minWeight, brownfish.maxWeight);
    brownfish.multiplier = calculateMultiplier(brownfish.actualWeight, brownfish.minWeight, brownfish.maxWeight);
    brownfish.finalPoints = calculateFinalPoints(brownfish.basePoints, brownfish.multiplier);
    brownfish.imagePath = "assets/imagen/nuevaimagen/pezcafe.png";
    if (loadSprite(brownfish, brownfish.imagePath, {1.0f,1.0f})) rewards.push_back(move(brownfish));

    // Asegura al menos uno
    assert(!rewards.empty());
}

const Reward& RewardManager::pickRandom() {
    assert(!rewards.empty());
    // Ruleta ponderada por rarity
    uniform_real_distribution<float> dist(0.f, 1.f);
    float total = 0.f;
    for (auto& r : rewards) total += (r.rarity > 0.f ? r.rarity : 0.01f);
    float pick = dist(rng) * total;
    float acc = 0.f;
    for (auto& r : rewards) {
        float w = (r.rarity > 0.f ? r.rarity : 0.01f);
        acc += w;
        if (pick <= acc) return r;
    }
    return rewards.back();
}

float RewardManager::generateWeightInRange(float minWeight, float maxWeight) {
    uniform_real_distribution<float> weightDist(minWeight, maxWeight);
    return weightDist(rng);
}

float RewardManager::calculateMultiplier(float actualWeight, float minWeight, float maxWeight) {
    // Fórmula ajustada: multiplicador = 0.7 + (porcentajeDelPeso * 1.1)
    // Rango resultante: [0.7, 1.8]
    float weightPercentage = (actualWeight - minWeight) / (maxWeight - minWeight);
    if (weightPercentage < 0.0f) weightPercentage = 0.0f;
    if (weightPercentage > 1.0f) weightPercentage = 1.0f;
    return 0.7f + (weightPercentage * 1.1f);
}

int RewardManager::calculateFinalPoints(int basePoints, float multiplier) {
    return static_cast<int>(std::round(basePoints * multiplier));
}

