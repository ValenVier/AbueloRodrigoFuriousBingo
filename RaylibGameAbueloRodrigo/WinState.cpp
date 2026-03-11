#include "WinState.h"
#include "StateManager.h"
#include "GameManager.h"
#include "MenuState.h"
#include "raylib.h"
#include <cstdio>
#include <cmath>

static const int SW = 800;
static const int SH = 600;

void WinState::Enter() {
    auto& gm = GameManager::Instance();
    finalScore_ = gm.score;
    finalKills_ = gm.enemiesKilled;
    finalTime_ = gm.elapsedTime;
    finalLevel_ = gm.level;
    timer_ = 0.f;
    gm.Reset();
}

void WinState::Update(float dt) {
    timer_ += dt;
    if (timer_ > 1.f && IsKeyPressed(KEY_ENTER))
        StateManager::Instance().ChangeState(std::make_unique<MenuState>());
}

void WinState::Draw() const {
    ClearBackground({ 0, 30, 0, 255 });

    float pulse = 0.5f + 0.5f * sinf(timer_ * 3.f);
    Color titleCol = { (unsigned char)(pulse * 255.f), 255, 0, 255 };
    DrawText("BINGO!", SW / 2 - 120, 80, 80, titleCol);
    DrawText("Rodrigo wins the bingo!", SW / 2 - 175, 175, 26, YELLOW);

    char buf[64];
    int mins = (int)finalTime_ / 60;
    int secs = (int)finalTime_ % 60;

    snprintf(buf, 64, "Time:           %02d:%02d", mins, secs);
    DrawText(buf, SW / 2 - 160, 260, 24, WHITE);

    snprintf(buf, 64, "Score:          %d", finalScore_);
    DrawText(buf, SW / 2 - 160, 296, 24, YELLOW);

    snprintf(buf, 64, "Enemies killed: %d", finalKills_);
    DrawText(buf, SW / 2 - 160, 332, 24, RED);

    snprintf(buf, 64, "Level reached:  %d", finalLevel_);
    DrawText(buf, SW / 2 - 160, 368, 24, SKYBLUE);

    float alpha = 0.5f + 0.5f * sinf(timer_ * 2.f);
    Color promptCol = { 255, 255, 255, (unsigned char)(alpha * 255.f) };
    DrawText("Press ENTER to play again", SW / 2 - 170, 470, 24, promptCol);
}