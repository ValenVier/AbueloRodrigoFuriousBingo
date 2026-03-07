#include "PlayingState.h"
#include <cmath>

static const int SW = 800;
static const int SH = 600;

void PlayingState::Enter() {
    playerPos_ = { 400.f, 300.f };
    lastMoveDir_ = { 1.f, 0.f };

    // PATTERN: Strategy (create the starting weapon)
    currentWeapon_ = std::make_unique<CaneLaser>();

    bulletPool_.DeactivateAll(); // clear any leftover bullets from a previous run
}

void PlayingState::Update(float dt) {
    // Player movement
    float dx = 0.f, dy = 0.f;
    if (IsKeyDown(KEY_W)) dy -= 1.f;
    if (IsKeyDown(KEY_S)) dy += 1.f;
    if (IsKeyDown(KEY_A)) dx -= 1.f;
    if (IsKeyDown(KEY_D)) dx += 1.f;

    float len = sqrtf(dx * dx + dy * dy);
    if (len > 0.001f) {
        dx /= len;
        dy /= len;
        lastMoveDir_ = { dx, dy };
    }

    playerPos_.x += dx * speed_ * dt;
    playerPos_.y += dy * speed_ * dt;

    float r = 16.f;
    if (playerPos_.x < r)      playerPos_.x = r;
    if (playerPos_.x > SW - r) playerPos_.x = SW - r;
    if (playerPos_.y < r)      playerPos_.y = r;
    if (playerPos_.y > SH - r) playerPos_.y = SH - r;

    // Auto-fire
    // PATTERN: Strategy (we call Update() on the weapon, not Fire() directly)
    // The weapon internally decides when enough time has passed to fire.
    Vector2 mousePos = GetMousePosition();
    currentWeapon_->Update(dt,
        playerPos_,
        lastMoveDir_,
        mousePos,
        bulletPool_,
        1.0f,   // dmgMult — no upgrades yet
        1.0f);  // frMult  — no upgrades yet

    // Move bullets forward
    UpdateBullets(dt);
}

void PlayingState::UpdateBullets(float dt) {
    for (auto& b : bulletPool_.bullets) {
        if (!b.active) continue;

        // Accumulate lifetime (deactivate when expired)
        b.lifetime += dt;
        if (b.lifetime >= b.maxLifetime) {
            b.active = false;
            continue;
        }

        // Move in a straight line 
        b.position.x += b.velocity.x * dt;
        b.position.y += b.velocity.y * dt;

        // Deactivate if bullet leaves the screen
        if (b.position.x < -50.f || b.position.x > SW + 50.f ||
            b.position.y < -50.f || b.position.y > SH + 50.f) {
            b.active = false;
        }
    }
}

void PlayingState::Draw() const {
    ClearBackground({ 15, 15, 25, 255 });

    DrawBullets(); // draw bullets behind the player

    // Draw Rodrigo
    DrawCircleV(playerPos_, 16.f, YELLOW);
    DrawRectangle((int)playerPos_.x - 20, (int)playerPos_.y + 8, 8, 6, DARKGRAY);
    DrawRectangle((int)playerPos_.x + 12, (int)playerPos_.y + 8, 8, 6, DARKGRAY);
    Vector2 stickEnd = {
        playerPos_.x + lastMoveDir_.x * 24.f,
        playerPos_.y + lastMoveDir_.y * 24.f
    };
    DrawLineEx(playerPos_, stickEnd, 3.f, BROWN);

    // HUD (weapon name bottom left)
    DrawText(currentWeapon_->GetName(), 10, SH - 45, 18, GREEN);
    DrawText(currentWeapon_->GetDescription(), 10, SH - 25, 14, GRAY);
}

void PlayingState::DrawBullets() const {
    for (const auto& b : bulletPool_.bullets) {
        if (!b.active) continue;
        DrawCircleV(b.position, b.radius, b.color);
    }
}