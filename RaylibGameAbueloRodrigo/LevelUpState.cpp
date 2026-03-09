#include "LevelUpState.h"
#include "StateManager.h"
#include "GameManager.h"
#include "raylib.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>

static const int SW = 800;
static const int SH = 600;

static const UpgradeCard ALL_CARDS[] = {
    {UpgradeType::DAMAGE_UP, "Power Pills", "+25% bullet damage", RED},
    {UpgradeType::FIRE_RATE_UP, "Rapid Dentures", "+20% fire rate", ORANGE},
    {UpgradeType::SPEED_UP, "Turbo Wheels", "+15% move speed", GREEN},
    {UpgradeType::MAX_HP_UP, "Bingo Biscuits", "+20 max HP, heal 20", PINK},
    {UpgradeType::XP_MAGNET, "XP Magnet", "Orb pickup radius +40", SKYBLUE},
};
static const int CARD_COUNT = 5;

void LevelUpState::Enter() {
    timer_ = 0.f;
    BuildCards();
}

void LevelUpState::BuildCards() {
    // Fisher-Yates shuffle to pick 3 unique random cards
    int indices[CARD_COUNT] = { 0, 1, 2, 3, 4 };
    for (int i = CARD_COUNT - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;
    }
    for (int i = 0; i < 3; i++)
        cards_[i] = ALL_CARDS[indices[i]];
}

void LevelUpState::Update(float dt) {
    timer_ += dt;

    Vector2 mouse = GetMousePosition();
    hoveredCard_ = -1;

    for (int i = 0; i < 3; i++) {
        float cardX = 80.f + i * 220.f;
        float cardY = 180.f;
        if (mouse.x >= cardX && mouse.x <= cardX + 180.f &&
            mouse.y >= cardY && mouse.y <= cardY + 260.f)
            hoveredCard_ = i;
    }

    if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) ApplyCard(0);
    if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) ApplyCard(1);
    if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) ApplyCard(2);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hoveredCard_ >= 0)
        ApplyCard(hoveredCard_);
}

void LevelUpState::ApplyCard(int idx) {
    auto& gm = GameManager::Instance();

    switch (cards_[idx].type) {
    case UpgradeType::DAMAGE_UP: gm.dmgMult *= 1.25f; break;
    case UpgradeType::FIRE_RATE_UP: gm.frMult *= 0.80f; break;
    case UpgradeType::SPEED_UP: gm.speedMult *= 1.15f; break;
    case UpgradeType::MAX_HP_UP:
        gm.maxHealth += 20.f;
        gm.health += 20.f;
        if (gm.health > gm.maxHealth) gm.health = gm.maxHealth;
        break;
    case UpgradeType::XP_MAGNET: gm.orbPickupRadius += 40.f; break;
    default: break;
    }

    gm.upgradePoints--;
    if (gm.upgradePoints > 0) {
        BuildCards();
        timer_ = 0.f;
    }
    else {
        StateManager::Instance().PopState();
    }
}

void LevelUpState::Draw() const {
    DrawRectangle(0, 0, SW, SH, { 0, 0, 0, 180 });

    float pulse = 0.5f + 0.5f * sinf(timer_ * 3.f);
    Color titleCol = { 255, (unsigned char)(200 + pulse * 55.f), 0, 255 };
    DrawText("LEVEL UP!", SW / 2 - 100, 80, 52, titleCol);

    char buf[32];
    snprintf(buf, 32, "Level %d", GameManager::Instance().level);
    DrawText(buf, SW / 2 - 45, 140, 24, YELLOW);

    for (int i = 0; i < 3; i++) {
        float cardX = 80.f + i * 220.f;
        float cardY = 180.f;
        bool  hov = (hoveredCard_ == i);

        Color bg = hov ? Color{ 60, 60, 80, 255 } : Color{ 30, 30, 50, 255 };
        DrawRectangle((int)cardX, (int)cardY, 180, 260, bg);
        DrawRectangleLines((int)cardX, (int)cardY, 180, 260,
            hov ? cards_[i].color : DARKGRAY);
        DrawRectangle((int)cardX, (int)cardY, 180, 6, cards_[i].color);

        snprintf(buf, 32, "[%d]", i + 1);
        DrawText(buf, (int)cardX + 10, (int)cardY + 10, 18, GRAY);

        DrawText(cards_[i].title,
            (int)cardX + 10, (int)cardY + 50, 20,
            hov ? cards_[i].color : WHITE);

        DrawText(cards_[i].description,
            (int)cardX + 10, (int)cardY + 90, 15, LIGHTGRAY);
    }

    DrawText("Click a card or press 1, 2, 3", SW / 2 - 165, 480, 20, GRAY);
}