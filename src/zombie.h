#pragma once
#include <raylib.h>

class Zombie {
public:
    Zombie(float x, float y);
    void Update(float deltaTime, Vector2 playerPosition);
    void Draw();
    bool IsActive() const { return active; }
    bool TakeDamage(int damage);  // Retourne true si le zombie meurt
    
    Vector2 position;
    float rotation;
    float speed;
    int health;
    bool active;
    
private:
    float width;
    float height;
    Color bodyColor;
    Color armColor;
}; 