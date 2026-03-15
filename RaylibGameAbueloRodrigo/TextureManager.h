#pragma once
// PATTERN: Singleton, loads each texture once, shares it everywhere
// All textures are unloaded in Shutdown()
#include "raylib.h"
#include <map>
#include <string>

class TextureManager {
public:
    static TextureManager& Instance() {
        static TextureManager inst;
        return inst;
    }

    // Load texture from path, returns cached version if already loaded
    Texture2D Load(const std::string& path);

    // Unload all textures,call once before CloseWindow()
    void Shutdown();

private:
    TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    std::map<std::string, Texture2D> cache_;
};