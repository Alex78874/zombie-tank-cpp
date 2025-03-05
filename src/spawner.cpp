#include "spawner.h"
#include <math.h>

Spawner::Spawner(float x, float y) {
    position = {x, y};
    spawnCooldown = 0.0f;
    spawnRate = 2.0f;        // Un zombie toutes les 2 secondes
    rotationSpeed = 0.5f;    // Vitesse de rotation
    orbitRadius = 400.0f;    // Distance par rapport au joueur
    orbitAngle = 0.0f;
    
    color = RED;
}

void Spawner::Update(float deltaTime, Vector2 playerPosition, std::vector<Zombie>& zombies) {
    // Mise à jour de la position du spawner (rotation autour du joueur)
    orbitAngle += rotationSpeed * deltaTime;
    position.x = playerPosition.x + cosf(orbitAngle) * orbitRadius;
    position.y = playerPosition.y + sinf(orbitAngle) * orbitRadius;
    
    // Gestion du spawn des zombies
    spawnCooldown -= deltaTime;
    if (spawnCooldown <= 0) {
        // Créer un nouveau zombie à la position du spawner
        zombies.push_back(Zombie(position.x, position.y));
        
        // Réinitialiser le cooldown
        spawnCooldown = spawnRate;
    }
}

void Spawner::Draw() {
    // Dessiner le spawner comme un cercle rouge
    DrawCircle(position.x, position.y, 15.0f, color);
    DrawCircle(position.x, position.y, 10.0f, BLACK);
    DrawCircle(position.x, position.y, 5.0f, RED);
} 