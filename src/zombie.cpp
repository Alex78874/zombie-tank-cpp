#include "zombie.h"
#include <math.h>
#include <raymath.h>
#include "rlgl.h"

Zombie::Zombie(float x, float y) {
    position = {x, y};
    rotation = 0.0f;
    speed = 100.0f;
    health = 10;
    maxHealth = 10;
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
    
    // Dessiner la barre de vie
    DrawHealthBar();
}

void Zombie::DrawHealthBar() {
    // Dimensions de la barre de vie
    float barWidth = 25.0f;
    float barHeight = 4.0f;
    
    // Position de la barre de vie (au-dessus du zombie)
    float barX = position.x - barWidth / 2.0f;
    float barY = position.y - height / 2.0f - 10.0f;  // 10 pixels au-dessus du zombie
    
    // Couleur de la barre selon le niveau de vie
    Color healthColor;
    float healthPercent = (float)health / maxHealth;
    
    if (healthPercent > 0.7f) {
        healthColor = GREEN;
    } else if (healthPercent > 0.3f) {
        healthColor = YELLOW;
    } else {
        healthColor = RED;
    }
    
    // Dessiner le cadre de la barre de vie
    DrawRectangleLines(barX - 1, barY - 1, barWidth + 2, barHeight + 2, BLACK);
    
    // Dessiner le fond de la barre de vie d
    DrawRectangle(barX, barY, barWidth, barHeight, GRAY);
    
    // Dessiner la partie remplie de la barre de vie
    float fillWidth = healthPercent * barWidth;
    DrawRectangle(barX, barY, fillWidth, barHeight, healthColor);
} 