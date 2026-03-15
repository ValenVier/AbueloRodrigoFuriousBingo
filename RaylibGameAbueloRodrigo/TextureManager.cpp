#include "TextureManager.h"
#include <iostream>

Texture2D TextureManager::Load(const std::string& path) {
    auto it = cache_.find(path);
    if (it != cache_.end()) return it->second;

    Texture2D tex = LoadTexture(path.c_str());
    if (tex.id == 0)
        std::cout << "[TextureManager] Failed to load: " << path << "\n";
    else
        std::cout << "[TextureManager] Loaded: " << path << "\n";

    cache_[path] = tex;
    return tex;
}

void TextureManager::Shutdown() {
    for (auto& pair : cache_)
        UnloadTexture(pair.second);
    cache_.clear();
}