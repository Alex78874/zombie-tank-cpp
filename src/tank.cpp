#include "tank.h"
#include <math.h>
#include "rlgl.h"
#include <raylib.h>
#include "zombie.h"

Tank::Tank(float x, float y) {
    position = {x, y};
    bodyRotation = 0.0f;
    turretRotation = 0.0f;
    
    speed = 200.0f;
    rotationSpeed = 180.0f;
    turretRotationSpeed = 180.0f;
    
    width = 40.0f;
    height = 60.0f;
    
    bodyColor = GREEN;
    turretColor = DARKGREEN;
    tracksColor = DARKBROWN;
    
    // Initialisation du tir
    shootCooldown = 0.0f;
    shootDelay = 0.5f; // Délai de 0.5 seconde entre les tirs
    
    // Initialisation de la vie
    health = 100.0f;
    maxHealth = 100.0f;
    
    // Initialisation des munitions
    ammo = 30;
    maxAmmo = 30;
    isReloading = false;
    reloadCooldown = 0.0f;
    reloadTime = 1.0f; // 1 seconde pour recharger
    
    // Initialisation du score
    score = 0;
    
    // Initialisation de l'invincibilité
    isInvincible = false;
    invincibilityTimer = 0.0f;
    invincibilityDuration = 1.0f; // 1 seconde d'invincibilité après avoir été touché
}

void Tank::Update(float deltaTime) {
    // Mise à jour de l'invincibilité
    if (isInvincible) {
        invincibilityTimer -= deltaTime;
        if (invincibilityTimer <= 0) {
            isInvincible = false;
        }
    }
    
    // Contrôles de la tourelle (flèches gauche/droite)
    if (IsKeyDown(KEY_LEFT)) turretRotation -= turretRotationSpeed * deltaTime;
    if (IsKeyDown(KEY_RIGHT)) turretRotation += turretRotationSpeed * deltaTime;
    
    // Contrôles du corps du tank (WASD)
    bool moving = false;
    
    // Rotation du tank
    if (IsKeyDown(KEY_A)) {
        bodyRotation -= rotationSpeed * deltaTime;
        moving = true;
    }
    if (IsKeyDown(KEY_D)) {
        bodyRotation += rotationSpeed * deltaTime;
        moving = true;
    }
    
    // Mouvement en avant/arrière
    if (IsKeyDown(KEY_W)) {
        float radians = bodyRotation * DEG2RAD;
        position.x += sinf(radians) * speed * deltaTime;
        position.y -= cosf(radians) * speed * deltaTime;
        moving = true;
    }
    if (IsKeyDown(KEY_S)) {
        float radians = bodyRotation * DEG2RAD;
        position.x -= sinf(radians) * speed * deltaTime;
        position.y += cosf(radians) * speed * deltaTime;
        moving = true;
    }
    
    // Gestion du rechargement manuel avec R
    if (IsKeyDown(KEY_R) && !isReloading && ammo < maxAmmo) {
        isReloading = true;
        reloadCooldown = reloadTime;
    }
    
    // Rechargement automatique quand les munitions atteignent 0
    if (ammo == 0 && !isReloading) {
        isReloading = true;
        reloadCooldown = reloadTime;
    }
    
    // Mise à jour du rechargement
    if (isReloading) {
        reloadCooldown -= deltaTime;
        if (reloadCooldown <= 0) {
            ammo = maxAmmo;
            isReloading = false;
        }
    }
    
    // Mise à jour du cooldown de tir
    if (shootCooldown > 0) {
        shootCooldown -= deltaTime;
    }
    
    // Tir avec la barre d'espace
    if (IsKeyDown(KEY_SPACE) && shootCooldown <= 0 && ammo > 0 && !isReloading) {
        // Position de départ du projectile (à l'extrémité du canon)
        float cannonLength = height / 2;
        float radians = turretRotation * DEG2RAD;
        Vector2 projectilePos = {
            position.x + sinf(radians) * cannonLength,
            position.y - cosf(radians) * cannonLength
        };
        
        // Créer un nouveau projectile
        projectiles.push_back(Projectile(projectilePos, turretRotation, 400.0f));
        
        // Réinitialiser le cooldown et décrémenter les munitions
        shootCooldown = shootDelay;
        ammo--;
    }
    
    // Mise à jour des projectiles
    for (size_t i = 0; i < projectiles.size(); i++) {
        projectiles[i].Update(deltaTime);
    }
    
    // Supprimer les projectiles inactifs
    for (size_t i = 0; i < projectiles.size(); ) {
        if (!projectiles[i].IsActive()) {
            projectiles.erase(projectiles.begin() + i);
        } else {
            i++;
        }
    }
}

void Tank::Draw() {
    // Sauvegarder la transformation actuelle
    rlPushMatrix();
        rlTranslatef(position.x, position.y, 0);
        rlRotatef(bodyRotation, 0, 0, 1);
        
        // Si invincible, faire clignoter le tank
        Color currentBodyColor = bodyColor;
        Color currentTurretColor = turretColor;
        Color currentTracksColor = tracksColor;

        if (isInvincible && ((int)(invincibilityTimer * 10) % 2 == 0)) {
            currentBodyColor = WHITE;
            currentTurretColor = LIGHTGRAY;
            currentTracksColor = GRAY;
        }

        // Chenilles (rectangles arrondis)
        DrawRectangleRounded({-width/2 - 5, -height/2, 10, height}, 0.3, 6, currentTracksColor);
        DrawRectangleRounded({width/2 - 5, -height/2, 10, height}, 0.3, 6, currentTracksColor);
        
        // Corps principal
        DrawRectangleRounded({-width/2, -height/2, width, height}, 0.3, 6, currentBodyColor);
                
        // Détails du corps
        DrawRectangleRounded({-width/4, -height/2 - 5, width/2, 10}, 0.4, 4, currentTurretColor);
        
        // Tourelle
        rlPushMatrix();
            rlRotatef(turretRotation - bodyRotation, 0, 0, 1);
            
            // Base de la tourelle
            DrawCircle(0, 0, width/3, currentTurretColor);
            
            // Canon
            DrawRectangle(-5, -height/2, 10, height/2, currentTurretColor);
        rlPopMatrix();
    rlPopMatrix();
    
    // Dessiner les projectiles
    for (size_t i = 0; i < projectiles.size(); i++) {
        projectiles[i].Draw();
    }
}

void Tank::DrawHealthBar() {
    // Obtenir les dimensions de l'écran
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Dimensions de la barre de vie
    float barWidth = 300.0f;
    float barHeight = 30.0f;
    
    // Position de la barre de vie (en bas au milieu)
    float barX = (screenWidth - barWidth) / 2.0f - 160.0f; // Déplacée vers la gauche pour faire place à la barre de munitions
    float barY = screenHeight - barHeight - 20.0f;  // 20 pixels de marge en bas
    
    // Couleur de la barre selon le niveau de vie
    Color healthColor;
    if (health > 70.0f) {
        healthColor = GREEN;
    } else if (health > 30.0f) {
        healthColor = YELLOW;
    } else {
        healthColor = RED;
    }
    
    // Dessiner le cadre de la barre de vie
    DrawRectangleLines(barX - 2, barY - 2, barWidth + 4, barHeight + 4, DARKGRAY);
    
    // Dessiner le fond de la barre de vie
    DrawRectangle(barX, barY, barWidth, barHeight, LIGHTGRAY);
    
    // Dessiner la partie remplie de la barre de vie
    float fillWidth = (health / maxHealth) * barWidth;
    DrawRectangle(barX, barY, fillWidth, barHeight, healthColor);
    
    // Afficher le texte au centre de la barre de vie
    char healthText[10];
    snprintf(healthText, sizeof(healthText), "%.0f HP", health);
    
    // Calculer la position du texte pour le centrer
    int textWidth = MeasureText(healthText, 20);
    float textX = barX + (barWidth - textWidth) / 2.0f;
    float textY = barY + (barHeight - 20) / 2.0f;
    
    // Créer un rectangle de fond pour le texte
    float textBgWidth = textWidth + 10;  // 5 pixels de padding de chaque côté
    float textBgHeight = 24;             // Légèrement plus grand que la taille du texte
    float textBgX = textX - 5;           // 5 pixels à gauche du texte
    float textBgY = textY - 2;           // 2 pixels au-dessus du texte
    
    // Dessiner le rectangle de fond
    DrawRectangle(textBgX, textBgY, textBgWidth, textBgHeight, DARKGRAY);
    
    // Dessiner le texte
    DrawText(healthText, textX, textY, 20, WHITE);
}

void Tank::DrawAmmoBar() {
    // Obtenir les dimensions de l'écran
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Dimensions de la barre de munitions
    float barWidth = 300.0f;
    float barHeight = 30.0f;
    
    // Position de la barre de munitions (à droite de la barre de vie)
    float barX = (screenWidth - barWidth) / 2.0f + 160.0f; // Déplacée vers la droite
    float barY = screenHeight - barHeight - 20.0f;  // Même hauteur que la barre de vie
    
    // Dessiner le cadre de la barre de munitions
    DrawRectangleLines(barX - 2, barY - 2, barWidth + 4, barHeight + 4, DARKGRAY);
    
    // Dessiner le fond de la barre de munitions (noir)
    DrawRectangle(barX, barY, barWidth, barHeight, BLACK);
    
    // Dessiner chaque segment de munition
    float segmentWidth = barWidth / maxAmmo;
    for (int i = 0; i < ammo; i++) {
        DrawRectangle(barX + i * segmentWidth, barY, segmentWidth - 1, barHeight, YELLOW);
    }
    
    // Afficher le texte des munitions
    char ammoText[20];
    if (isReloading) {
        snprintf(ammoText, sizeof(ammoText), "Rechargement");
        
        // Calculer la position du texte pour le centrer
        int textWidth = MeasureText(ammoText, 20);
        float textX = barX + (barWidth - textWidth) / 2.0f;
        float textY = barY + (barHeight - 20) / 2.0f;
        
        // Créer un rectangle de fond pour le texte
        float textBgWidth = textWidth + 10;  // 5 pixels de padding de chaque côté
        float textBgHeight = 24;             // Légèrement plus grand que la taille du texte
        float textBgX = textX - 5;           // 5 pixels à gauche du texte
        float textBgY = textY - 2;           // 2 pixels au-dessus du texte
        
        // Dessiner le rectangle de fond
        DrawRectangle(textBgX, textBgY, textBgWidth, textBgHeight, DARKGRAY);
        
        // Dessiner le texte en jaune
        DrawText(ammoText, textX, textY, 20, YELLOW);
    } else {
        snprintf(ammoText, sizeof(ammoText), "%d/%d", ammo, maxAmmo);
        
        // Calculer la position du texte pour le centrer
        int textWidth = MeasureText(ammoText, 20);
        float textX = barX + (barWidth - textWidth) / 2.0f;
        float textY = barY + (barHeight - 20) / 2.0f;
        
        // Créer un rectangle de fond pour le texte
        float textBgWidth = textWidth + 10;  // 5 pixels de padding de chaque côté
        float textBgHeight = 24;             // Légèrement plus grand que la taille du texte
        float textBgX = textX - 5;           // 5 pixels à gauche du texte
        float textBgY = textY - 2;           // 2 pixels au-dessus du texte
        
        // Dessiner le rectangle de fond
        DrawRectangle(textBgX, textBgY, textBgWidth, textBgHeight, DARKGRAY);
        
        // Dessiner le texte
        DrawText(ammoText, textX, textY, 20, WHITE);
    }
}

bool Tank::TakeDamage(float damage) {
    // Si le tank est invincible, il ne prend pas de dégâts
    if (isInvincible) return false;
    
    // Appliquer les dégâts
    health -= damage;
    
    // Activer l'invincibilité
    isInvincible = true;
    invincibilityTimer = invincibilityDuration;
    
    // Vérifier si le tank est détruit
    if (health <= 0) {
        health = 0;
        return true; // Le tank est détruit
    }
    
    return false; // Le tank n'est pas détruit
}

void Tank::CheckProjectileCollisions(std::vector<Zombie>& zombies) {
    // Vérifier les collisions entre projectiles et zombies
    for (size_t i = 0; i < projectiles.size(); ) {
        bool collisionDetected = false;
        
        for (size_t j = 0; j < zombies.size(); j++) {
            if (zombies[j].IsActive()) {
                // Distance entre le projectile et le zombie
                float distance = Vector2Distance(projectiles[i].position, zombies[j].position);
                
                // Si la distance est inférieure à la taille du zombie, il y a collision
                if (distance < 15.0f) {  // 15 = rayon approximatif du zombie
                    if (zombies[j].TakeDamage(10)) {  // 10 points de dégâts par projectile
                        // Le zombie est mort, augmenter le score
                        score += 10;
                    }
                    
                    // Désactiver le projectile
                    projectiles[i].active = false;
                    collisionDetected = true;
                    break;
                }
            }
        }
        
        if (collisionDetected || !projectiles[i].IsActive()) {
            projectiles.erase(projectiles.begin() + i);
        } else {
            i++;
        }
    }
} 