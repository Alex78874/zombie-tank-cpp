#pragma once
#include <raylib.h>
#include <vector>
#include "zombie.h"

class Spawner {
public:
    Spawner(float x, float y);
    void Update(float deltaTime, Vector2 playerPosition, std::vector<Zombie>& zombies);
    void Draw();
    
private:
    Vector2 position;
    float spawnCooldown;    // Temps avant la prochaine apparition
    float spawnRate;        // Taux d'apparition (secondes entre chaque zombie)
    float rotationSpeed;    // Vitesse de rotation autour du joueur
    float orbitRadius;      // Rayon de l'orbite autour du joueur
    float orbitAngle;       // Angle actuel dans l'orbite
    
    Color color;
}; 