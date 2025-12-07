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

    // Usa las imágenes que ya añadiste en assets/imagen/nuevaimagen
    Reward goldenfish{RewardType::Fish, "goldenfish", "Pez Dorado", 0.05f};
    goldenfish.imagePath = "assets/imagen/nuevaimagen/golden fish.png";
    if (loadSprite(goldenfish, goldenfish.imagePath, {0.8f,0.8f})) rewards.push_back(move(goldenfish));

    Reward bluefish{RewardType::Fish, "bluefish", "Pez Azul", 0.14f};
    bluefish.imagePath = "assets/imagen/nuevaimagen/pezazul.png";
    if (loadSprite(bluefish, bluefish.imagePath, {1.0f,1.0f})) rewards.push_back(move(bluefish));

    Reward greenfish{RewardType::Fish, "greenfish", "Pez Verde", 0.14f};
    greenfish.imagePath = "assets/imagen/nuevaimagen/pezverde.png";
    if (loadSprite(greenfish, greenfish.imagePath, {1.0f,1.0f})) rewards.push_back(move(greenfish));

    Reward purplefish{RewardType::Fish, "purplefish", "Pez Morado", 0.14f};
    purplefish.imagePath = "assets/imagen/nuevaimagen/pezmorado.png";
    if (loadSprite(purplefish, purplefish.imagePath, {1.0f,1.0f})) rewards.push_back(move(purplefish));

    Reward whitefish{RewardType::Fish, "whitefish", "Pez Blanco", 0.08f};
    whitefish.imagePath = "assets/imagen/nuevaimagen/pezblanco.png";
    if (loadSprite(whitefish, whitefish.imagePath, {1.0f,1.0f})) rewards.push_back(move(whitefish));

    Reward goldenredfish{RewardType::Fish, "goldenredfish", "Pez Rojo Dorado", 0.06f};
    goldenredfish.imagePath = "assets/imagen/nuevaimagen/pezdorado-rojo.png";
    if (loadSprite(goldenredfish, goldenredfish.imagePath, {1.0f,1.0f})) rewards.push_back(move(goldenredfish));

    Reward redfish{RewardType::Fish, "redfish", "Pez Rojo", 0.06f};
    redfish.imagePath = "assets/imagen/nuevaimagen/pezrojo.png";
    if (loadSprite(redfish, redfish.imagePath, {1.0f,1.0f})) rewards.push_back(move(redfish));

    Reward bluegreenfish{RewardType::Fish, "bluegreenfish", "Pez Azul-Verde", 0.07f};
    bluegreenfish.imagePath = "assets/imagen/nuevaimagen/pezverde-azul.png";
    if (loadSprite(bluegreenfish, bluegreenfish.imagePath, {1.0f,1.0f})) rewards.push_back(move(bluegreenfish));

    Reward brownfish{RewardType::Fish, "brownfish", "Pez Cafe", 0.12f};
    brownfish.imagePath = "assets/imagen/nuevaimagen/pezcafe.png";
    if (loadSprite(brownfish, brownfish.imagePath, {1.0f,1.0f})) rewards.push_back(move(brownfish));

    Reward campurple{RewardType::Fish, "campurple", "Camaron Morado", 0.07f};
    campurple.imagePath = "assets/imagen/nuevaimagen/cammorado.png";
    if (loadSprite(campurple, campurple.imagePath, {1.0f,1.0f})) rewards.push_back(move(campurple));

    Reward camred{RewardType::Fish, "camred", "Camaron Rojo", 0.07f};
    camred.imagePath = "assets/imagen/nuevaimagen/camrojo.png";
    if (loadSprite(camred, camred.imagePath, {1.0f,1.0f})) rewards.push_back(move(camred));

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

