#pragma once
#include <raylib.h>
#include <raymath.h>
#include "projectile.h"
#include "zombie.h"
#include <vector>

class Tank {
public:
    Tank(float x, float y);
    void Update(float deltaTime);
    void Draw();
    void DrawHealthBar();  // Nouvelle méthode pour dessiner la barre de vie
    void DrawAmmoBar();  // Nouvelle méthode pour dessiner la barre de munitions
    void CheckProjectileCollisions(std::vector<Zombie>& zombies);
    bool TakeDamage(float damage); // Nouvelle méthode pour prendre des dégâts
    
    // Position et rotation
    Vector2 position;
    float bodyRotation;
    float turretRotation;
    
    // Vitesse et propriétés
    float speed;
    float rotationSpeed;
    float turretRotationSpeed;
    
    // Taille
    float width;
    float height;
    
    // Gestion des projectiles
    std::vector<Projectile> projectiles;
    
    // Vie
    float health;        // Vie actuelle
    float maxHealth;     // Vie maximale
    
    // Score
    int score;           // Score du joueur
    
    // Invincibilité
    bool isInvincible;          // État d'invincibilité
    float invincibilityTimer;   // Durée restante d'invincibilité
    float invincibilityDuration; // Durée totale d'invincibilité
    
    // Mode de contrôle de la tourelle par la souris
    bool mouseTurretControl;
    
    // Getter pour les munitions
    int GetAmmo() const { return ammo; }
    
    // Getter for reloading state
    bool IsReloading() const { return isReloading; }
    
private:
    // Couleurs
    Color bodyColor;
    Color turretColor;
    Color tracksColor;
    
    // Tir
    float shootCooldown;    // Temps avant le prochain tir possible
    float shootDelay;       // Délai minimum entre les tirs
    
    // Munitions
    int ammo;             // Munitions actuelles
    int maxAmmo;          // Munitions maximales
    bool isReloading;     // État de rechargement
    float reloadCooldown; // Temps restant avant la fin du rechargement
    float reloadTime;     // Temps total de rechargement
}; 