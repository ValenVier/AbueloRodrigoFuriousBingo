#include "PlayingState.h"
#include <cmath> // sqrtf

// Screen dimensions as local constants
static const int SW = 800;
static const int SH = 600;

void PlayingState::Enter() {
    // Reset position every time we enter this state
    playerPos_ = { 400.f, 300.f };
    lastMoveDir_ = { 1.f, 0.f };
}

void PlayingState::Update(float dt) {
    // Read WASD input
    // PATTERN: Command (each key represents a movemente command)
    float dx = 0.f, dy = 0.f;
    if (IsKeyDown(KEY_W)) dy -= 1.f;
    if (IsKeyDown(KEY_S)) dy += 1.f;
    if (IsKeyDown(KEY_A)) dx -= 1.f;
    if (IsKeyDown(KEY_D)) dx += 1.f;

    // Normalise the movement vector
    // After normalising, speed is always "speed_" regardless of how many keys are held down.
    float len = sqrtf(dx * dx + dy * dy);
    if (len > 0.001f) { // avoid dividing by zero
        dx /= len;
        dy /= len;
        lastMoveDir_ = { dx, dy }; // store direction for bullets
    }

    // Move the player
    playerPos_.x += dx * speed_ * dt;
    playerPos_.y += dy * speed_ * dt;

    // Clamp: keep player inside the screen
    // 16.f is the visual radius of the player circle
    float r = 16.f;
    if (playerPos_.x < r)      playerPos_.x = r;
    if (playerPos_.x > SW - r) playerPos_.x = SW - r;
    if (playerPos_.y < r)      playerPos_.y = r;
    if (playerPos_.y > SH - r) playerPos_.y = SH - r;
}

void PlayingState::Draw() const {
    ClearBackground({ 15, 15, 25, 255 }); // black background with a blue tint

    // Draw Rodrigo

    // Body: yellow circle (the wheelchair)
    DrawCircleV(playerPos_, 16.f, YELLOW);

    // Wheels: two grey rectangles on each side
    DrawRectangle((int)playerPos_.x - 20, (int)playerPos_.y + 8, 8, 6, DARKGRAY);
    DrawRectangle((int)playerPos_.x + 12, (int)playerPos_.y + 8, 8, 6, DARKGRAY);

    // Walking stick: brown line pointing in the current movement direction
    Vector2 stickEnd = {
        playerPos_.x + lastMoveDir_.x * 24.f,
        playerPos_.y + lastMoveDir_.y * 24.f
    };
    DrawLineEx(playerPos_, stickEnd, 3.f, BROWN);

    // Help text
    DrawText("WASD to move Rodrigo", 10, SH - 25, 16, DARKGRAY);
}