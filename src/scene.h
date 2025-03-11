#ifndef SCENE_H
#define SCENE_H

#include "raylib.h"
#include <string>

class Scene {
public:
    Scene(const std::string& name) : name(name) {}
    virtual ~Scene() {}

    // Méthodes principales que chaque scène doit implémenter
    virtual void Init() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;
    virtual void Unload() = 0;

    // Gestionnaires d'événements
    virtual void ProcessInput() = 0;

    // Accesseurs
    std::string GetName() const { return name; }

protected:
    std::string name;
};

#endif // SCENE_H 