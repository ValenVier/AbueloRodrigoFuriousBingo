#include "PausedState.h"
#include "StateManager.h"
#include "MenuState.h"
#include "raylib.h"

static const int SW = 800;
static const int SH = 600;

void PausedState::Update(float dt) {
    // ESC or P (resume game;pop this state, PlayingState wakes up)
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P))
        StateManager::Instance().PopState();

    // Q (quit to main menu))
    if (IsKeyPressed(KEY_Q))
        StateManager::Instance().ChangeState(std::make_unique<MenuState>());
}

void PausedState::Draw() const {
    // Note: PausedState is an overlay
    // Raylib has already cleared the screen; (overlay on top)
    DrawRectangle(0, 0, SW, SH, { 0, 0, 0, 150 }); // semi transparent
    DrawText("PAUSED", SW / 2 - 90, SH / 2 - 60, 60, YELLOW);
    DrawText("ESC / P - Resume", SW / 2 - 120, SH / 2 + 10, 22, WHITE);
    DrawText("Q       - Menu", SW / 2 - 120, SH / 2 + 40, 22, WHITE);
}