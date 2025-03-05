#pragma once
#include <raylib.h>

class Projectile {
public:
    Projectile(Vector2 position, float rotation, float speed);
    void Update(float deltaTime);
    void Draw();
    bool IsActive() const { return active; }
    
    Vector2 position;
    Vector2 velocity;
    float lifeTime;   // Temps de vie du projectile
    bool active;      // Indique si le projectile est actif
    
private:
    float speed;
    float radius;
    Color color;
}; 