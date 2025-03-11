#pragma once
#include <raylib.h>
#include <string>

enum CursorStyle {
    CROSSHAIR_SIMPLE,     // Viseur simple (croix)
    CROSSHAIR_CIRCLE,     // Cercle avec point central
    CROSSHAIR_DOT,        // Simple point
    CROSSHAIR_CORNERS     // Coins de rectangle
};

class Cursor {
public:
    Cursor();
    ~Cursor();
    
    void Update();
    void Draw();
    
    // Méthodes pour modifier les propriétés du curseur
    void SetStyle(CursorStyle style);
    void SetColor(Color color);
    void SetScale(float scale);
    void SetInfoText(const std::string& text);
    void ClearInfoText();
    void ToggleInfoText(bool show);
    
    // Accesseur pour la position actuelle du curseur
    Vector2 GetPosition() const { return position; }
    
private:
    Vector2 position;       // Position du curseur
    CursorStyle style;      // Style du curseur
    Color color;            // Couleur du curseur
    float scale;            // Taille du curseur
    
    bool showInfoText;      // Afficher le texte d'info
    std::string infoText;   // Texte d'information à afficher
    
    // Méthodes privées pour dessiner chaque style
    void DrawSimpleCrosshair();
    void DrawCircleCrosshair();
    void DrawDotCrosshair();
    void DrawCornersCrosshair();
}; 