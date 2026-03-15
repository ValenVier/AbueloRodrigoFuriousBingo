#include "MenuState.h"
#include "StateManager.h"
#include "PlayingState.h"
#include "raylib.h"
#include <cmath>
#include "GameManager.h"

//static const int SW = 800;
//static const int SH = 600;

void MenuState::Enter() {
    timer_ = 0.f;
    selectedDiff_ = 1;  // default Medium
}

void MenuState::Update(float dt) {
    timer_ += dt;
    if (IsKeyPressed(KEY_ENTER))
        StateManager::Instance().ChangeState(std::make_unique<PlayingState>());

    // Navigate difficulty with arrow keys
    if (IsKeyPressed(KEY_LEFT) && selectedDiff_ > 0) selectedDiff_--;
    if (IsKeyPressed(KEY_RIGHT) && selectedDiff_ < 2) selectedDiff_++;

    // Direct selection with number keys
    if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) selectedDiff_ = 0;
    if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) selectedDiff_ = 1;
    if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) selectedDiff_ = 2;

    // Screen mode toggle with Tab or F
    if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_F))
        selectedMode_ = (selectedMode_ == 0) ? 1 : 0;

    if (IsKeyPressed(KEY_ENTER)) {
        // Apply difficulty before starting
        switch (selectedDiff_) {
        case 0: GameManager::Instance().difficulty = Difficulty::EASY;   break;
        case 1: GameManager::Instance().difficulty = Difficulty::MEDIUM; break;
        case 2: GameManager::Instance().difficulty = Difficulty::HARD;   break;
        }

        // Apply screen mode
        bool isFullscreen = IsWindowFullscreen();
        if (selectedMode_ == 1 && !isFullscreen) ToggleFullscreen();
        if (selectedMode_ == 0 && isFullscreen) ToggleFullscreen();

        StateManager::Instance().ChangeState(std::make_unique<PlayingState>());
    }

    // ESC in menu, exit the program
    if (IsKeyPressed(KEY_ESCAPE)) {
        SetExitKey(KEY_ESCAPE);  // reenable ESC as exit key, Raylib closes cleanly
        //CloseWindow();
    }
}

void MenuState::Draw() const {
    ClearBackground({ 20, 10, 35, 255 }); // dark purple

    // Instead of static SW/SH, use Raylib functions:
    int SW = GetScreenWidth();
    int SH = GetScreenHeight();

    // Pulsing title
    float pulse = 0.5f + 0.5f * sinf(timer_ * 2.f);
    Color titleCol = { 255, (unsigned char)(200 + pulse * 55.f), 0, 255 };
    DrawText("ABUELO RODRIGO", SW / 2 - 200, SH / 10, 56, titleCol);
    DrawText("The furious Bingo!", SW / 2 - 140, SH / 10 + 65, 20, YELLOW);

    // Controls
    int ctrlY = SH / 10 + 110;
    DrawText("WASD      - Move", SW / 2 - 130, ctrlY, 17, WHITE);
    DrawText("Q / E     - Swap weapon", SW / 2 - 130, ctrlY + 20, 17, WHITE);
    DrawText("ESC       - Pause", SW / 2 - 130, ctrlY + 40, 17, WHITE);

    // Difficulty + Screen mode side by side
    int selectorsY = ctrlY + 90;
    DrawText("DIFFICULTY:", SW / 2 - 250, selectorsY, 18, WHITE);
    DrawText("< 1  2  3 >", SW / 2 - 250, selectorsY + 22, 13, DARKGRAY);

    DrawText("SCREEN MODE:", SW / 2 + 20, selectorsY, 18, WHITE);
    DrawText("< Tab / F >", SW / 2 + 20, selectorsY + 22, 13, DARKGRAY);

    static const char* DIFF_NAMES[] = { "EASY",  "MEDIUM", "HARD" };
    static const Color DIFF_COLORS[] = { GREEN,    YELLOW,   RED };
    static const char* DIFF_DESCS[] = {
        "Slow enemies",
        "Balanced",
        "Fast enemies"
    };

    static const char* MODE_NAMES[] = { "WINDOWED", "FULLSCREEN" };
    static const Color MODE_COLORS[] = { SKYBLUE, GOLD };

    float rowY = (float)(selectorsY + 48);

    // Difficulty boxes, left side
    for (int i = 0; i < 3; i++) {
        float cx = SW / 2 - 250.f + i * 82.f;
        bool  sel = (selectedDiff_ == i);
        Color col = DIFF_COLORS[i];
        Color bg = sel ? Color{ 50,50,70,255 } : Color{ 20,20,40,255 };

        DrawRectangle((int)cx, (int)rowY, 74, 44, bg);
        DrawRectangleLines((int)cx, (int)rowY, 74, 44, sel ? col : DARKGRAY);
        if (sel) DrawRectangle((int)cx, (int)rowY, 74, 4, col);
        DrawText(DIFF_NAMES[i],
            (int)cx + (sel ? 6 : 8), (int)rowY + 13,
            sel ? 17 : 15,
            sel ? col : GRAY);
    }

    // Mode boxes, right side
    for (int i = 0; i < 2; i++) {
        float cx = SW / 2 + 20.f + i * 115.f;
        bool  sel = (selectedMode_ == i);
        Color col = MODE_COLORS[i];
        Color bg = sel ? Color{ 50,50,70,255 } : Color{ 20,20,40,255 };

        DrawRectangle((int)cx, (int)rowY, 105, 44, bg);
        DrawRectangleLines((int)cx, (int)rowY, 105, 44, sel ? col : DARKGRAY);
        if (sel) DrawRectangle((int)cx, (int)rowY, 105, 4, col);
        DrawText(MODE_NAMES[i], (int)cx + (sel ? 6 : 8), (int)rowY + 13, sel ? 14 : 13, sel ? col : GRAY);
       
    }

    // Description of selected difficulty
    DrawText(DIFF_DESCS[selectedDiff_], SW / 2 - 250, (int)rowY + 54, 15, LIGHTGRAY);

    // Bingo hint
    DrawText("Complete ALL squares to win!", SW / 2 - 145, (int)rowY + 80, 17, GOLD);

    // Blinking ENTER prompt
    float alpha = 0.5f + 0.5f * sinf(timer_ * 3.f);
    Color enterCol = { 255, 255, 255, (unsigned char)(alpha * 255.f) };
    DrawText("Press ENTER to play", SW / 2 - 125, (int)rowY + 115, 24, enterCol);

    // Footer
    DrawText("F11 - Toggle fullscreen", SW / 2 - 105, SH - 55, 15, DARKGRAY);
    DrawText("ESC - Exit", SW / 2 - 50, SH - 35, 15, DARKGRAY);
    DrawText("v1.0 - Raylib 5.5", SW / 2 - 65, SH - 15, 14, DARKGRAY);
}