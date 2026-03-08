#include "raylib.h"
#include "StateManager.h"
#include "PlayingState.h"
#include "MenuState.h"

int main() {
    // Initialize the Raylib window
    InitWindow(800, 600, "FURIOUS BINGO");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL); //disable Raylib's default ESC-to-close behaviour

    // PATTERN: State (start directly in PlayingState)
    /*StateManager::Instance().ChangeState(std::make_unique<PlayingState>());*/
    StateManager::Instance().ChangeState(std::make_unique<MenuState>());

    // ChangeState is deferred (call Update(0) once to apply it immediately)
    StateManager::Instance().Update(0.f);

    // Main game loop
    while (!WindowShouldClose()) {
        float dt = GetFrameTime(); // time of the previous frame in seconds (~0.016)

        // Clamp dt to avoid huge jumps if the window loses focus or freezes
        if (dt > 0.1f) dt = 0.1f;

        StateManager::Instance().Update(dt);

        BeginDrawing();
        StateManager::Instance().Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}