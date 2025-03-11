#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include "scene.h"
#include <vector>
#include <string>

class MenuItem {
public:
    MenuItem(const std::string& text, float x, float y, int fontSize, Color color) 
        : text(text), x(x), y(y), fontSize(fontSize), color(color), isSelected(false) {}
    
    void Draw() {
        Color textColor = isSelected ? RED : color;
        DrawText(text.c_str(), x, y, fontSize, textColor);
    }
    
    void SetSelected(bool selected) { isSelected = selected; }
    bool IsSelected() const { return isSelected; }
    
    std::string GetText() const { return text; }
    
private:
    std::string text;
    float x, y;
    int fontSize;
    Color color;
    bool isSelected;
};

class MenuScene : public Scene {
public:
    MenuScene() : Scene("Menu"), selectedIndex(0) {}
    ~MenuScene() override {}
    
    void Init() override;
    void Update(float deltaTime) override;
    void Draw() override;
    void Unload() override;
    void ProcessInput() override;
    
private:
    std::vector<MenuItem> menuItems;
    int selectedIndex;
    
    void SelectNextItem();
    void SelectPreviousItem();
    void ExecuteSelectedItem();
};

#endif // MENU_SCENE_H 