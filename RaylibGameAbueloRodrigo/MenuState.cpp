#include "MenuState.h"
#include "StateManager.h"
#include "PlayingState.h"
#include "raylib.h"
#include <cmath>

static const int SW = 800;
static const int SH = 600;

void MenuState::Enter() {
    timer_ = 0.f;
}

void MenuState::Update(float dt) {
    timer_ += dt;
    if (IsKeyPressed(KEY_ENTER))
        StateManager::Instance().ChangeState(std::make_unique<PlayingState>());

    // ESC in menu, exit the program
    
    if (IsKeyPressed(KEY_ESCAPE)) {
        SetExitKey(KEY_ESCAPE);  // reenable ESC as exit key, Raylib closes cleanly
        //CloseWindow();
    }
}

void MenuState::Draw() const {
    ClearBackground({ 20, 10, 35, 255 }); // dark purple

    // Pulsing title
    float pulse = 0.5f + 0.5f * sinf(timer_ * 2.f);
    Color titleCol = { 255, (unsigned char)(200 + pulse * 55.f), 0, 255 };
    DrawText("ABUELO RODRIGO & THE FURIOUS BINGO", 95, 140, 72, titleCol);
    DrawText("The return of Abuelo Rodrigo!", 160, 225, 26, YELLOW);

    // Controls
    DrawText("WASD       - Move", 290, 310, 20, WHITE);
    DrawText("Q / E      - Swap weapon", 290, 336, 20, WHITE);
    DrawText("ESC        - Pause", 290, 362, 20, WHITE);
    DrawText("Auto-fire  - always active", 290, 388, 20, SKYBLUE);
    DrawText("ESC - Exit", SW / 2 - 50, 510, 18, DARKGRAY);

    // Blinking ENTER prompt
    float alpha = 0.5f + 0.5f * sinf(timer_ * 3.f);
    Color enterCol = { 255, 255, 255, (unsigned char)(alpha * 255.f) };
    DrawText("Press ENTER to play", SW / 2 - 130, 470, 28, enterCol);

    DrawText("v1.0 - Raylib 5.5", 270, 570, 16, DARKGRAY);
}