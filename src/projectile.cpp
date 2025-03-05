#include "projectile.h"
#include <math.h>

Projectile::Projectile(Vector2 position, float rotation, float speed) {
    this->position = position;
    this->speed = speed;
    this->active = true;
    this->lifeTime = 2.0f;  // Durée de vie de 2 secondes
    this->radius = 3.0f;    // Petit rayon pour le projectile
    this->color = RED;      // Couleur rouge
    
    // Calculer la vélocité en fonction de la rotation
    float radians = rotation * DEG2RAD;
    velocity.x = sinf(radians) * speed;
    velocity.y = -cosf(radians) * speed;
}

void Projectile::Update(float deltaTime) {
    // Mise à jour de la position
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    
    // Mise à jour du temps de vie
    lifeTime -= deltaTime;
    
    // Désactiver le projectile s'il a dépassé sa durée de vie
    if (lifeTime <= 0) {
        active = false;
    }
}

void Projectile::Draw() {
    DrawCircleV(position, radius, color);
} 