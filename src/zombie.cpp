#include "zombie.h"
#include <math.h>
#include <raymath.h>
#include "rlgl.h"

Zombie::Zombie(float x, float y) {
    position = {x, y};
    rotation = 0.0f;
    speed = 100.0f;
    health = 10;
    active = true;
    
    width = 20.0f;
    height = 30.0f;
    
    bodyColor = GREEN;
    armColor = DARKGREEN;
}

void Zombie::Update(float deltaTime, Vector2 playerPosition) {
    // Calculer la direction vers le joueur
    Vector2 direction = Vector2Subtract(playerPosition, position);
    
    // Calculer l'angle de rotation pour faire face au joueur
    rotation = atan2f(direction.x, -direction.y) * RAD2DEG;
    
    // Normaliser la direction
    float distance = Vector2Length(direction);
    if (distance > 0) {
        direction.x /= distance;
        direction.y /= distance;
    }
    
    // Déplacer le zombie vers le joueur
    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;
}

bool Zombie::TakeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        active = false;
        return true;  // Le zombie est mort
    }
    return false;
}

void Zombie::Draw() {
    // Sauvegarder la transformation actuelle
    rlPushMatrix();
        rlTranslatef(position.x, position.y, 0);
        rlRotatef(rotation, 0, 0, 1);
        
        // Corps du zombie (cercle vert)
        DrawCircle(0, 0, width/2, bodyColor);
        
        // Bras du zombie (rectangles qui dépassent)
        DrawRectangle(-width/2 - 5, -5, 10, 5, armColor);  // Bras gauche
        DrawRectangle(width/2 - 5, -5, 10, 5, armColor);   // Bras droit
        
    rlPopMatrix();
} 