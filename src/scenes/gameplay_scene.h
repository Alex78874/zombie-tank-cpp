#ifndef GAMEPLAY_SCENE_H
#define GAMEPLAY_SCENE_H

#include "scene.h"
#include "../tank.h"
#include "../cursor.h"
#include "../spawner.h"
#include "../zombie.h"
#include <vector>

class GameplayScene : public Scene {
public:
    GameplayScene() : 
        Scene("Gameplay"), 
        playerTank(0, 0),  // Initialisation temporaire, sera remplacée dans Init()
        zombieSpawner(0, 0),
        isPaused(false), 
        gameOver(false) {}
    ~GameplayScene() override {}

    void Init() override;
    void Update(float deltaTime) override;
    void Draw() override;
    void Unload() override;
    void ProcessInput() override;

    // Méthodes spécifiques au gameplay
    void TogglePause() { isPaused = !isPaused; }
    bool IsPaused() const { return isPaused; }
    bool IsGameOver() const { return gameOver; }
    void SetGameOver(bool value) { gameOver = value; }

private:
    // Entités du jeu
    Tank playerTank;
    Cursor gameCursor;
    Spawner zombieSpawner;
    std::vector<Zombie> zombies;

    // États du jeu
    bool isPaused;
    bool gameOver;
    
    // Méthodes internes
    void DrawPauseOverlay();
    void DrawGameOverScreen();
};

#endif // GAMEPLAY_SCENE_H 