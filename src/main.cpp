#include <raylib.h>
#include <raymath.h>
#include "view.h"
#include "scenes/scene_manager.h"
#include "scenes/menu_scene.h"
#include "scenes/gameplay_scene.h"
#include <memory>

int main()
{
    // Initial window viewport setup. Check View for scaling
    View Viewport;                          
    InitWindow(Viewport.Width, Viewport.Height, "Tank Game");
    SetTargetFPS(60);
    
    // Initialiser notre gestionnaire de scènes
    auto& sceneManager = SceneManager::GetInstance();
    
    // Créer et enregistrer nos scènes
    sceneManager.RegisterScene("Menu", std::make_shared<MenuScene>());
    sceneManager.RegisterScene("Gameplay", std::make_shared<GameplayScene>());
    
    // Commencer par le menu
    sceneManager.ChangeScene("Menu");
    
    // Boucle principale
    while (!WindowShouldClose() && !sceneManager.ShouldQuit()) {
        // Mettre à jour avec le temps écoulé
        float deltaTime = GetFrameTime();
        
        // Mettre à jour la scène active
        sceneManager.Update(deltaTime);
        
        // Dessiner
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            // Dessiner la scène active
            sceneManager.Draw();
            
        EndDrawing();
    }
    
    // Fermer la fenêtre et décharger les ressources
    CloseWindow();
    
    return 0;
}