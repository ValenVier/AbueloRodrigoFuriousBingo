#include "GameOverState.h"
#include "StateManager.h"
#include "GameManager.h"
#include "raylib.h"
#include <cstdio>
#include <cmath>
#include "MenuState.h"

static const int SW = 800;
static const int SH = 600;

void GameOverState::Enter() {
    // Snapshot stats at the moment of death
    auto& gm = GameManager::Instance();
    finalScore_ = gm.score;
    finalKills_ = gm.enemiesKilled;
    finalTime_ = gm.elapsedTime;
    timer_ = 0.f;

    // Reset GameManager so next game starts clean
    gm.Reset();
}

void GameOverState::Update(float dt) {
    timer_ += dt;
    // Any key after 1 second restarts — avoids accidental skip
    if (timer_ > 1.f && IsKeyPressed(KEY_ENTER))
        /*StateManager::Instance().ChangeState(std::make_unique<GameOverState>());*/ // placeholder
        StateManager::Instance().ChangeState(std::make_unique<MenuState>());
}

void GameOverState::Draw() const {
    ClearBackground({ 30, 0, 0, 255 });

    // Pulsing title
    float pulse = 0.5f + 0.5f * sinf(timer_ * 3.f);
    Color titleCol = { 255, (unsigned char)(pulse * 80.f), (unsigned char)(pulse * 80.f), 255 };
    DrawText("GAME OVER", SW / 2 - 230, 100, 72, titleCol);
    DrawText("Rodrigo didn't make it to bingo...", SW / 2 - 220, 190, 22, GRAY);

    // Stats
    char buf[64];
    int mins = (int)finalTime_ / 60;
    int secs = (int)finalTime_ % 60;

    snprintf(buf, 64, "Time survived:    %02d:%02d", mins, secs);
    DrawText(buf, SW / 2 - 160, 270, 24, WHITE);

    snprintf(buf, 64, "Score:             %d", finalScore_);
    DrawText(buf, SW / 2 - 160, 306, 24, YELLOW);

    snprintf(buf, 64, "Enemies killed:    %d", finalKills_);
    DrawText(buf, SW / 2 - 160, 342, 24, RED);

    // Blinking prompt
    float alpha = 0.5f + 0.5f * sinf(timer_ * 2.f);
    Color promptCol = { 255, 255, 255, (unsigned char)(alpha * 255.f) };
    DrawText("Press ENTER to play again", SW / 2 - 170, 460, 24, promptCol);
}