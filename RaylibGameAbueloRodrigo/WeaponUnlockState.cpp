#include "WeaponUnlockState.h"
#include "StateManager.h"
#include "GameManager.h"
#include "raylib.h"
#include <cstdio>
#include <cmath>

static const int SW = 800;
static const int SH = 600;

// Weapon metadata by slot index ,must match weapons_ order in PlayingState
static const char* WEAPON_NAMES[] = {
    "Cane Laser", "Candy Rain", "Flying Dentures",
    "Explosive Pills", "Puree Catapult", "Bingo Sound Wave", "Turbo Charge"
};
static const char* WEAPON_DESCS[] = {
    "Fast energy beam", "12 candies in 360°", "Bounces off walls x3",
    "Arc shot, explodes", "6 blobs, slows 50%", "Expanding ring", "Knockback 80px"
};
static const Color WEAPON_COLORS[] = {
    SKYBLUE, {255,150,200,255}, WHITE,
    PINK, {255,253,208,255}, GOLD, ORANGE
};

void WeaponUnlockState::Enter() { timer_ = 0.f; }

const char* WeaponUnlockState::WeaponName(int idx)  const { return WEAPON_NAMES[idx]; }
const char* WeaponUnlockState::WeaponDesc(int idx)  const { return WEAPON_DESCS[idx]; }
Color WeaponUnlockState::WeaponColor(int idx) const {
    return WEAPON_COLORS[idx];
}

void WeaponUnlockState::Update(float dt) {
    timer_ += dt;
    Vector2 mouse = GetMousePosition();
    hovered_ = -1;

    // Card 1: left side
    if (mouse.x >= 100 && mouse.x <= 320 &&
        mouse.y >= 200 && mouse.y <= 420) hovered_ = 0;
    // Card 2: right side
    if (mouse.x >= 480 && mouse.x <= 700 &&
        mouse.y >= 200 && mouse.y <= 420) hovered_ = 1;

    if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) PickWeapon(option1Idx_);
    if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) PickWeapon(option2Idx_);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (hovered_ == 0) PickWeapon(option1Idx_);
        if (hovered_ == 1) PickWeapon(option2Idx_);
    }
}

void WeaponUnlockState::PickWeapon(int idx) {
    auto& gm = GameManager::Instance();

    // If it was confiscated, recover it; otherwise unlock it fresh
    bool wasConfiscated = false;
    for (int i = 0; i < (int)gm.confiscatedSlots.size(); i++) {
        if (gm.confiscatedSlots[i] == idx) {
            gm.confiscatedSlots.erase(gm.confiscatedSlots.begin() + i);
            wasConfiscated = true;
            break;
        }
    }
    if (!wasConfiscated)
        gm.unlockedWeaponSlots.push_back(idx);

    StateManager::Instance().PopState();
}

void WeaponUnlockState::Draw() const {
    DrawRectangle(0, 0, SW, SH, { 0, 0, 0, 180 });

    // Title
    float pulse = 0.5f + 0.5f * sinf(timer_ * 3.f);
    Color titleCol = { 0, 255, (unsigned char)(100 + pulse * 155.f), 255 };
    DrawText("BINGO LINE!", SW / 2 - 120, 80, 52, titleCol);
    DrawText("Choose a weapon to unlock:", SW / 2 - 170, 148, 22, WHITE);

    int opts[2] = { option1Idx_, option2Idx_ };
    float cardX[2] = { 100.f, 480.f };

    for (int i = 0; i < 2; i++) {
        int   idx = opts[i];
        bool  hov = (hovered_ == i);
        Color col = WeaponColor(idx);
        float cx = cardX[i];
        float cy = 200.f;

        Color bg = hov ? Color{ 50,50,70,255 } : Color{ 25,25,45,255 };
        DrawRectangle((int)cx, (int)cy, 220, 220, bg);
        DrawRectangleLines((int)cx, (int)cy, 220, 220, hov ? col : DARKGRAY);
        DrawRectangle((int)cx, (int)cy, 220, 6, col);

        char buf[8];
        snprintf(buf, 8, "[%d]", i + 1);
        DrawText(buf, (int)cx + 10, (int)cy + 14, 18, GRAY);

        DrawText(WeaponName(idx), (int)cx + 10, (int)cy + 60, 20,
            hov ? col : WHITE);
        DrawText(WeaponDesc(idx), (int)cx + 10, (int)cy + 92, 15, LIGHTGRAY);
    }

    DrawText("Click or press 1 / 2", SW / 2 - 120, 450, 20, GRAY);
}