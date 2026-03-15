#include "ConfiscationState.h"
#include "StateManager.h"
#include "raylib.h"
#include <cmath>

static const int SW = 800;
static const int SH = 600;

void ConfiscationState::Enter() {
    timer_ = 0.f;
}

void ConfiscationState::Update(float dt) {
    timer_ += dt;
    // Autodismiss after 2.5 seconds or on any key press after 1s
    if (timer_ >= 5.f || (timer_ > 1.f && GetKeyPressed()))
        StateManager::Instance().PopState();
}

void ConfiscationState::Draw() const {
    // Dark red overlay
    DrawRectangle(0, 0, SW, SH, { 80, 0, 0, 160 });

    // Card background
    DrawRectangle(SW / 2 - 200, SH / 2 - 120, 400, 240, { 20, 0, 0, 240 });
    DrawRectangleLines(SW / 2 - 200, SH / 2 - 120, 400, 240, RED);

    // Stamp-style title
    float pulse = 0.5f + 0.5f * sinf(timer_ * 8.f);
    Color stampCol = { 255, (unsigned char)(pulse * 50.f), (unsigned char)(pulse * 50.f), 255 };
    DrawText("CONFISCATED", SW / 2 - 155, SH / 2 - 100, 36, stampCol);
    DrawLine(SW / 2 - 200, SH / 2 - 58, SW / 2 + 200, SH / 2 - 58, RED);

    // Weapon taken
    DrawText("Weapon seized:", SW / 2 - 160, SH / 2 - 40, 18, GRAY);
    DrawText(confiscatedName_, SW / 2 - 160, SH / 2 - 16, 24, WHITE);

    DrawText("Reason: disturbing medical staff", SW / 2 - 160, SH / 2 + 20, 14, DARKGRAY);

    // New active weapon
    DrawLine(SW / 2 - 200, SH / 2 + 48, SW / 2 + 200, SH / 2 + 48, DARKGRAY);
    DrawText("Now equipped:", SW / 2 - 160, SH / 2 + 58, 18, GRAY);
    DrawText(replacementName_, SW / 2 - 160, SH / 2 + 80, 22, GREEN);

    DrawText("Make a BINGO line to recover it", SW / 2 - 175, SH / 2 + 112, 15, YELLOW);
}