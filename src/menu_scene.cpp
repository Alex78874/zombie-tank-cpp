#include "menu_scene.h"
#include "scene_manager.h"

void MenuScene::Init() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Créer les éléments du menu
    menuItems.clear();
    
    // Titre du jeu
    menuItems.push_back(MenuItem("ZOMBIE TANK", screenWidth / 2 - MeasureText("ZOMBIE TANK", 60) / 2, screenHeight / 4, 60, DARKGREEN));
    
    // Options du menu
    menuItems.push_back(MenuItem("Jouer", screenWidth / 2 - MeasureText("Jouer", 30) / 2, screenHeight / 2, 30, WHITE));
    menuItems.push_back(MenuItem("Options", screenWidth / 2 - MeasureText("Options", 30) / 2, screenHeight / 2 + 50, 30, WHITE));
    menuItems.push_back(MenuItem("Quitter", screenWidth / 2 - MeasureText("Quitter", 30) / 2, screenHeight / 2 + 100, 30, WHITE));
    
    // Sélectionner le premier élément (Jouer)
    selectedIndex = 1;
    menuItems[selectedIndex].SetSelected(true);
}

void MenuScene::ProcessInput() {
    // Navigation dans le menu
    if (IsKeyPressed(KEY_UP)) {
        SelectPreviousItem();
    } else if (IsKeyPressed(KEY_DOWN)) {
        SelectNextItem();
    }
    
    // Sélection d'une option
    if (IsKeyPressed(KEY_ENTER)) {
        ExecuteSelectedItem();
    }
}

void MenuScene::Update(float deltaTime) {
    // La logique du menu est principalement gérée par ProcessInput
    // Nous pourrions ajouter des animations ici si nécessaire
}

void MenuScene::Draw() {
    // Dessiner un fond
    ClearBackground(BLACK);
    
    // Dessiner tous les éléments du menu
    for (auto& item : menuItems) {
        item.Draw();
    }
    
    // Ajouter des instructions
    DrawText("Utilisez les flèches pour naviguer et ENTER pour sélectionner", 
             GetScreenWidth() / 2 - MeasureText("Utilisez les flèches pour naviguer et ENTER pour sélectionner", 15) / 2, 
             GetScreenHeight() - 50, 15, GRAY);
}

void MenuScene::Unload() {
    // Libérer les ressources si nécessaire
}

void MenuScene::SelectNextItem() {
    // Désélectionner l'élément actuel
    menuItems[selectedIndex].SetSelected(false);
    
    // Passer à l'élément suivant (en ignorant le titre)
    selectedIndex++;
    if (selectedIndex >= (int)menuItems.size()) {
        selectedIndex = 1; // Boucler au premier élément sélectionnable
    }
    
    // Sélectionner le nouvel élément
    menuItems[selectedIndex].SetSelected(true);
}

void MenuScene::SelectPreviousItem() {
    // Désélectionner l'élément actuel
    menuItems[selectedIndex].SetSelected(false);
    
    // Passer à l'élément précédent (en ignorant le titre)
    selectedIndex--;
    if (selectedIndex < 1) {
        selectedIndex = menuItems.size() - 1; // Boucler au dernier élément
    }
    
    // Sélectionner le nouvel élément
    menuItems[selectedIndex].SetSelected(true);
}

void MenuScene::ExecuteSelectedItem() {
    std::string selectedItem = menuItems[selectedIndex].GetText();
    
    if (selectedItem == "Jouer") {
        // Passer à la scène de gameplay
        SceneManager::GetInstance().ChangeScene("Gameplay");
    } else if (selectedItem == "Options") {
        // À implémenter plus tard si nécessaire
    } else if (selectedItem == "Quitter") {
        // Signaler que le jeu doit se terminer
        SceneManager::GetInstance().QuitGame();
    }
} 