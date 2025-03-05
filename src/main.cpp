#include <raylib.h>
#include <raymath.h>
#include "view.h"
#include "tank.h"
#include "zombie.h"
#include "spawner.h"
#include <vector>

// États du jeu
enum GameState {
    GAMEPLAY,
    GAME_OVER
};

int main()
{
    // Initial window viewport setup. Check View for scaling
    View Viewport;                          
    InitWindow(Viewport.Width, Viewport.Height, "Tank Game");
    SetTargetFPS(60);
    
    // État du jeu
    GameState currentState = GAMEPLAY;
    
    // Créer le tank au centre de l'écran
    Tank playerTank(Viewport.Center.X, Viewport.Center.Y);
    
    // Créer le spawner à une certaine distance du tank
    Spawner zombieSpawner(Viewport.Center.X + 400, Viewport.Center.Y);
    
    // Vecteur pour stocker les zombies
    std::vector<Zombie> zombies;
    
    // Boucle principale
    while (!WindowShouldClose()) {
        // Mettre à jour
        float deltaTime = GetFrameTime();
        
        switch (currentState) {
            case GAMEPLAY:
                // Mettre à jour le tank
                playerTank.Update(deltaTime);
                
                // Mettre à jour le spawner
                zombieSpawner.Update(deltaTime, playerTank.position, zombies);
                
                // Mettre à jour les zombies
                for (size_t i = 0; i < zombies.size(); i++) {
                    if (zombies[i].IsActive()) {
                        zombies[i].Update(deltaTime, playerTank.position);
                        
                        // Vérifier collision avec le tank
                        float distance = Vector2Distance(zombies[i].position, playerTank.position);
                        if (distance < playerTank.width / 2 + 15.0f) { // 15 = rayon approximatif du zombie
                            if (playerTank.TakeDamage(10)) {
                                // Le tank est détruit, passage à l'état Game Over
                                currentState = GAME_OVER;
                            }
                        }
                    }
                }
                
                // Supprimer les zombies inactifs
                for (size_t i = 0; i < zombies.size(); ) {
                    if (!zombies[i].IsActive()) {
                        zombies.erase(zombies.begin() + i);
                    } else {
                        i++;
                    }
                }
                
                // Vérifier les collisions entre projectiles et zombies
                playerTank.CheckProjectileCollisions(zombies);
                break;
                
            case GAME_OVER:
                // Vérifier si le joueur appuie sur R pour redémarrer
                if (IsKeyPressed(KEY_R)) {
                    // Réinitialiser le tank
                    playerTank = Tank(Viewport.Center.X, Viewport.Center.Y);
                    
                    // Supprimer tous les zombies
                    zombies.clear();
                    
                    // Réinitialiser l'état du jeu
                    currentState = GAMEPLAY;
                }
                break;
        }
        
        // Dessiner
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            switch (currentState) {
                case GAMEPLAY:
                    // Dessiner une simple grille pour référence
                    DrawGrid(20, 50);
                    
                    // Dessiner les zombies
                    for (size_t i = 0; i < zombies.size(); i++) {
                        zombies[i].Draw();
                    }
                    
                    // Dessiner le spawner
                    zombieSpawner.Draw();
                    
                    // Dessiner le tank
                    playerTank.Draw();
                    
                    // Dessiner la barre de vie
                    playerTank.DrawHealthBar();
                    
                    // Dessiner la barre de munitions
                    playerTank.DrawAmmoBar();
                    
                    // Afficher le score
                    char scoreText[30];
                    snprintf(scoreText, sizeof(scoreText), "Score: %d", playerTank.score);
                    DrawText(scoreText, GetScreenWidth() - 150, 20, 24, DARKBLUE);
                    
                    // Afficher les instructions
                    DrawText("W/A/S/D: Déplacer le tank", 20, 20, 20, DARKGRAY);
                    DrawText("Flèches gauche/droite: Tourner la tourelle", 20, 50, 20, DARKGRAY);
                    DrawText("ESPACE: Tirer", 20, 80, 20, DARKGRAY);
                    DrawText("R: Recharger", 20, 110, 20, DARKGRAY);
                    break;
                    
                case GAME_OVER:
                    // Afficher l'écran de game over
                    int screenWidth = GetScreenWidth();
                    int screenHeight = GetScreenHeight();
                    
                    // Dessiner un fond semi-transparent
                    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
                    
                    // Afficher le texte "GAME OVER"
                    const char* gameOverText = "GAME OVER";
                    int gameOverFontSize = 60;
                    int gameOverTextWidth = MeasureText(gameOverText, gameOverFontSize);
                    DrawText(gameOverText, (screenWidth - gameOverTextWidth) / 2, screenHeight / 2 - 60, gameOverFontSize, RED);
                    
                    // Afficher le score final
                    char finalScoreText[50];
                    snprintf(finalScoreText, sizeof(finalScoreText), "Score final: %d", playerTank.score);
                    int scoreTextWidth = MeasureText(finalScoreText, 30);
                    DrawText(finalScoreText, (screenWidth - scoreTextWidth) / 2, screenHeight / 2, 30, WHITE);
                    
                    // Afficher "Press R to restart"
                    const char* restartText = "Appuyez sur R pour recommencer";
                    int restartTextWidth = MeasureText(restartText, 24);
                    DrawText(restartText, (screenWidth - restartTextWidth) / 2, screenHeight / 2 + 60, 24, YELLOW);
                    break;
            }
            
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}