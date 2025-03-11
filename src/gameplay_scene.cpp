#include "gameplay_scene.h"

void GameplayScene::Init() {
    // Initialiser le tank du joueur
    playerTank = Tank(GetScreenWidth() / 2, GetScreenHeight() / 2);
    
    // Initialiser le curseur
    gameCursor = Cursor();
    
    // Initialiser le spawner de zombies
    zombieSpawner = Spawner(GetScreenWidth() / 2 + 400, GetScreenHeight() / 2);
    
    // Initialiser les zombies
    zombies.clear();
    
    // Réinitialiser les états
    isPaused = false;
    gameOver = false;
}

void GameplayScene::ProcessInput() {
    // Gestion de la touche P pour la pause
    if (IsKeyPressed(KEY_P)) {
        TogglePause();
    }
    
    // Si le jeu est en pause ou terminé, ne pas traiter les autres entrées
    if (isPaused || gameOver) return;
    
    // Traitement des entrées pour le tank et d'autres entités
    // (Les détails dépendront de votre implémentation actuelle)
}

void GameplayScene::Update(float deltaTime) {
    // Si le jeu est en pause ou terminé, ne pas mettre à jour la logique
    if (isPaused || gameOver) return;
    
    // Mettre à jour le curseur avec la position de la souris
    gameCursor.Update();
    
    // Mettre à jour le tank avec les entrées du joueur
    playerTank.Update(deltaTime);
    
    // Vérifier les collisions des projectiles
    playerTank.CheckProjectileCollisions(zombies);
    
    // Mettre à jour les zombies
    for (size_t i = 0; i < zombies.size(); i++) {
        zombies[i].Update(deltaTime, playerTank.position);
        
        // Vérifier si un zombie a atteint le tank
        float distance = Vector2Distance(zombies[i].position, playerTank.position);
        if (distance < playerTank.width / 2 + 15.0f) { // 15 = rayon approximatif du zombie
            if (playerTank.TakeDamage(10)) { // Supposons que chaque zombie fait 10 dégâts
                SetGameOver(true);
            }
            zombies[i].TakeDamage(100); // On tue le zombie qui a atteint le tank
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
    
    // Mettre à jour le spawner et ajouter de nouveaux zombies si nécessaire
    zombieSpawner.Update(deltaTime, playerTank.position, zombies);
}

void GameplayScene::Draw() {
    // Dessiner la grille de référence
    DrawGrid(20, 50);
    
    // Dessiner les zombies
    for (size_t i = 0; i < zombies.size(); i++) {
        zombies[i].Draw();
    }
    
    // Dessiner le spawner
    zombieSpawner.Draw();
    
    // Dessiner le tank
    playerTank.Draw();
    
    // Dessiner les barres de vie et de munitions
    playerTank.DrawHealthBar();
    playerTank.DrawAmmoBar();
    
    // Dessiner le curseur en dernier pour qu'il soit au-dessus de tout
    gameCursor.Draw();
    
    // Si le jeu est en pause, dessiner l'overlay de pause
    if (isPaused) {
        DrawPauseOverlay();
    }
    
    // Si le jeu est terminé, dessiner l'écran de fin
    if (gameOver) {
        DrawGameOverScreen();
    }
}

void GameplayScene::Unload() {
    // Libérer les ressources si nécessaire
}

void GameplayScene::DrawPauseOverlay() {
    // Dessiner un message de pause
    DrawText("PAUSE", GetScreenWidth() / 2 - MeasureText("PAUSE", 40) / 2, GetScreenHeight() / 2, 40, DARKGRAY);
    DrawText("Appuyez sur P pour reprendre", GetScreenWidth() / 2 - MeasureText("Appuyez sur P pour reprendre", 20) / 2, GetScreenHeight() / 2 + 40, 20, DARKGRAY);
}

void GameplayScene::DrawGameOverScreen() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Dessiner un fond semi-transparent
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
    
    // Afficher le texte "GAME OVER"
    const char* gameOverText = "GAME OVER";
    int gameOverFontSize = 60;
    int gameOverTextWidth = MeasureText(gameOverText, gameOverFontSize);
    DrawText(gameOverText, screenWidth / 2 - gameOverTextWidth / 2, screenHeight / 2 - 30, gameOverFontSize, RED);
    
    // Afficher le score ou d'autres informations ici
    
    // Afficher le message pour rejouer
    const char* restartText = "Appuyez sur ENTER pour rejouer";
    DrawText(restartText, screenWidth / 2 - MeasureText(restartText, 20) / 2, screenHeight / 2 + 40, 20, WHITE);
} 