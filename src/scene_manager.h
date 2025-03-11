#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "scene.h"
#include <map>
#include <memory>
#include <string>

class SceneManager {
public:
    static SceneManager& GetInstance() {
        static SceneManager instance;
        return instance;
    }

    void RegisterScene(const std::string& name, std::shared_ptr<Scene> scene) {
        scenes[name] = scene;
    }

    void ChangeScene(const std::string& name) {
        if (scenes.find(name) != scenes.end()) {
            if (currentScene) {
                currentScene->Unload();
            }
            currentScene = scenes[name];
            currentScene->Init();
        }
    }

    void Update(float deltaTime) {
        if (currentScene) {
            currentScene->ProcessInput();
            currentScene->Update(deltaTime);
        }
    }

    void Draw() {
        if (currentScene) {
            currentScene->Draw();
        }
    }

    Scene* GetCurrentScene() const {
        return currentScene.get();
    }
    
    void QuitGame() {
        shouldQuit = true;
    }
    
    bool ShouldQuit() const {
        return shouldQuit;
    }

private:
    SceneManager() : shouldQuit(false) {}
    ~SceneManager() {}
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    std::map<std::string, std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Scene> currentScene;
    bool shouldQuit;
};

#endif // SCENE_MANAGER_H 