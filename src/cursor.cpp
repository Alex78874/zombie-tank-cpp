#include "cursor.h"

Cursor::Cursor() {
    position = { 0, 0 };
    style = CROSSHAIR_SIMPLE;
    color = RED;
    scale = 1.0f;
    showInfoText = true;
    infoText = "";
    
    // Désactiver le curseur système
    HideCursor();
}

Cursor::~Cursor() {
    // Rétablir le curseur système à la fermeture
    ShowCursor();
}

void Cursor::Update() {
    // Mettre à jour la position du curseur
    position = GetMousePosition();
}

void Cursor::Draw() {
    // Dessiner le curseur selon le style sélectionné
    switch (style) {
        case CROSSHAIR_SIMPLE:
            DrawSimpleCrosshair();
            break;
        case CROSSHAIR_CIRCLE:
            DrawCircleCrosshair();
            break;
        case CROSSHAIR_DOT:
            DrawDotCrosshair();
            break;
        case CROSSHAIR_CORNERS:
            DrawCornersCrosshair();
            break;
    }
    
    // Afficher le texte d'info si activé
    if (showInfoText && !infoText.empty()) {
        int fontSize = 16;
        int padding = 8;
        int textWidth = MeasureText(infoText.c_str(), fontSize);
        
        // Position dans le coin inférieur droit du curseur
        float textX = position.x + 10;
        float textY = position.y + 10;
        
        // Fond semi-transparent pour le texte
        DrawRectangle(textX - padding/2, textY - padding/2, 
                      textWidth + padding, fontSize + padding, 
                      Fade(BLACK, 0.7f));
        
        // Texte
        DrawText(infoText.c_str(), textX, textY, fontSize, WHITE);
    }
}

void Cursor::SetStyle(CursorStyle newStyle) {
    style = newStyle;
}

void Cursor::SetColor(Color newColor) {
    color = newColor;
}

void Cursor::SetScale(float newScale) {
    scale = newScale;
}

void Cursor::SetInfoText(const std::string& text) {
    infoText = text;
}

void Cursor::ClearInfoText() {
    infoText = "";
}

void Cursor::ToggleInfoText(bool show) {
    showInfoText = show;
}

void Cursor::DrawSimpleCrosshair() {
    // Dessiner une croix simple
    int size = 10 * scale;
    int thickness = 2 * scale;
    
    // Ligne horizontale
    DrawLineEx(
        { position.x - size, position.y },
        { position.x + size, position.y },
        thickness, color
    );
    
    // Ligne verticale
    DrawLineEx(
        { position.x, position.y - size },
        { position.x, position.y + size },
        thickness, color
    );
    
    // Point central
    DrawCircle(position.x, position.y, 2 * scale, color);
}

void Cursor::DrawCircleCrosshair() {
    // Dessiner un cercle avec point central
    int size = 8 * scale;
    int thickness = 2 * scale;
    
    // Cercle extérieur
    DrawCircleLines(position.x, position.y, size, color);
    
    // Point central
    DrawCircle(position.x, position.y, 2 * scale, color);
}

void Cursor::DrawDotCrosshair() {
    // Dessiner juste un point
    DrawCircle(position.x, position.y, 3 * scale, color);
}

void Cursor::DrawCornersCrosshair() {
    // Dessiner les coins d'un rectangle
    int size = 12 * scale;
    int lineLength = 6 * scale;
    int thickness = 2 * scale;
    
    // Coin supérieur gauche
    DrawLineEx(
        { position.x - size, position.y - size },
        { position.x - size + lineLength, position.y - size },
        thickness, color
    );
    DrawLineEx(
        { position.x - size, position.y - size },
        { position.x - size, position.y - size + lineLength },
        thickness, color
    );
    
    // Coin supérieur droit
    DrawLineEx(
        { position.x + size - lineLength, position.y - size },
        { position.x + size, position.y - size },
        thickness, color
    );
    DrawLineEx(
        { position.x + size, position.y - size },
        { position.x + size, position.y - size + lineLength },
        thickness, color
    );
    
    // Coin inférieur gauche
    DrawLineEx(
        { position.x - size, position.y + size - lineLength },
        { position.x - size, position.y + size },
        thickness, color
    );
    DrawLineEx(
        { position.x - size, position.y + size },
        { position.x - size + lineLength, position.y + size },
        thickness, color
    );
    
    // Coin inférieur droit
    DrawLineEx(
        { position.x + size, position.y + size - lineLength },
        { position.x + size, position.y + size },
        thickness, color
    );
    DrawLineEx(
        { position.x + size - lineLength, position.y + size },
        { position.x + size, position.y + size },
        thickness, color
    );
    
    // Point central (optionnel)
    DrawCircle(position.x, position.y, 1 * scale, color);
} 