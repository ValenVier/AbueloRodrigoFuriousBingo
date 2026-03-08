#include "PlayingState.h"
#include <cmath>
#include <cstdlib>  // srand, rand
#include <ctime>    // time
#include <cstdio>
#include <algorithm>

static const int SW = 800;
static const int SH = 600;

void PlayingState::Enter() {
    playerPos_ = { 400.f, 300.f };
    lastMoveDir_ = { 1.f, 0.f };
    elapsedTime_ = 0.f;

    // PATTERN: Strategy (create the starting weapon)
    currentWeapon_ = std::make_unique<CaneLaser>();

    bulletPool_.DeactivateAll();
    enemyPool_.DeactivateAll(); // clear any leftover bullets from a previous run
    spawnSystem_.Reset();

    srand((unsigned int)time(nullptr)); // seed random number for spawn positions
}

void PlayingState::Update(float dt) {
    elapsedTime_ += dt; // track total game time

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
    currentWeapon_->Update(dt, playerPos_, lastMoveDir_, mousePos, bulletPool_,
        1.0f,   // dmgMult
        1.0f);  // frMult

    // Move bullets forward
    UpdateBullets(dt);

    // Enemies (spawn new enemies based on time)
    spawnSystem_.Update(dt, elapsedTime_, playerPos_, enemyPool_);
    // move existing enemies toward player
    UpdateEnemies(dt);
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

void PlayingState::UpdateEnemies(float dt) {
    for (auto& e : enemyPool_.enemies) {
        if (!e.active) continue;

        // Move toward the player (normalise direction first)
        float dx = playerPos_.x - e.position.x;
        float dy = playerPos_.y - e.position.y;
        float len = sqrtf(dx * dx + dy * dy);

        if (len > 0.001f) {
            dx /= len;
            dy /= len;
        }

        e.position.x += dx * e.speed * dt;
        e.position.y += dy * e.speed * dt;
    }
}

void PlayingState::Draw() const {
    ClearBackground({ 15, 15, 25, 255 });

    DrawEnemies();  // draw behind player
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

    // HUD (Enemy counter; top right)
    char buf[32];
    snprintf(buf, 32, "Enemies: %d", enemyPool_.ActiveCount());
    DrawText(buf, SW - 150, 10, 20, WHITE);

    // Elapsed time; top center
    snprintf(buf, 32, "%.0fs", elapsedTime_);
    DrawText(buf, SW / 2 - 20, 10, 20, WHITE);
}

void PlayingState::DrawBullets() const {
    for (const auto& b : bulletPool_.bullets) {
        if (!b.active) continue;
        DrawCircleV(b.position, b.radius, b.color);
    }
}

void PlayingState::DrawEnemies() const {
    for (const auto& e : enemyPool_.enemies) {
        if (!e.active) continue;

        // Enemy body
        DrawCircleV(e.position, e.size, e.color);

        // Health bar above the enemy
        // Bar width = diameter of the enemy circle
        float barWidth = e.size * 2.f;
        float hpFrac = e.health / e.maxHealth;

        // Background bar
        DrawRectangle(
            (int)(e.position.x - barWidth / 2.f),
            (int)(e.position.y - e.size - 7.f),
            (int)barWidth, 4, DARKGRAY);

        // Filled portion (green > yellow > red based on HP)
        Color barColor = (hpFrac > 0.6f) ? GREEN
            : (hpFrac > 0.3f) ? YELLOW
            : RED;
        DrawRectangle(
            (int)(e.position.x - barWidth / 2.f),
            (int)(e.position.y - e.size - 7.f),
            (int)(barWidth * hpFrac), 4, barColor);
    }
}