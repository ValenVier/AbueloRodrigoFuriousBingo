#include "PlayingState.h"
#include <cmath>
#include <cstdlib>  // srand, rand
#include <ctime>    // time
#include <cstdio>
#include <algorithm>
#include "GameManager.h"
#include "GameOverState.h" 
#include "StateManager.h"
#include "PausedState.h"
#include "LevelUpState.h"
#include "WinState.h"
#include "BingoSystem.h"

static const int SW = 800;
static const int SH = 600;

void PlayingState::Enter() {

    playerPos_ = { 400.f, 300.f };
    lastMoveDir_ = { 1.f, 0.f };
    elapsedTime_ = 0.f;

    // PATTERN: Strategy (create the starting weapon)
    /*currentWeapon_ = std::make_unique<CaneLaser>();*/

    // Build the starting weapon list
    weapons_.clear();
    /*weapons_.push_back(std::make_unique<CaneLaser>());
    weapons_.push_back(std::make_unique<FlyingDentures>());
    weapons_.push_back(std::make_unique<ExplosivePills>());
    weapons_.push_back(std::make_unique<PureeCatapult>());
    weapons_.push_back(std::make_unique<BingoSoundWave>());
    weapons_.push_back(std::make_unique<TurboCharge>());
    weapons_.push_back(std::make_unique<CandyRain>());
    currentWeaponIdx_ = 0;*/

    SyncWeaponsWithGameManager();
    currentWeaponIdx_ = 0;

    // Connect bingo line callback
    BingoSystem::Instance().OnLineCompleted = [this]() {
        // Find two options to offer: one from locked pool, one from confiscated
        auto& gm = GameManager::Instance();
        int option1 = -1, option2 = -1;

        // First option: next locked weapon
        for (int i = 0; i < 7; i++) {
            bool unlocked = false;
            for (int u : gm.unlockedWeaponSlots)
                if (u == i) { unlocked = true; break; }
            bool confiscated = false;
            for (int c : gm.confiscatedSlots)
                if (c == i) { confiscated = true; break; }
            if (!unlocked && !confiscated) { option1 = i; break; }
        }

        // Second option: confiscated weapon if any, else next locked
        if (!gm.confiscatedSlots.empty()) {
            option2 = gm.confiscatedSlots[0];
        }
        else {
            for (int i = 0; i < 7; i++) {
                bool unlocked = false;
                for (int u : gm.unlockedWeaponSlots)
                    if (u == i) { unlocked = true; break; }
                if (!unlocked && i != option1) { option2 = i; break; }
            }
        }

        // Only show if there's something to unlock
        if (option1 >= 0 || option2 >= 0) {
            if (option1 < 0) option1 = option2;
            if (option2 < 0) option2 = option1;
            StateManager::Instance().PushState(
                std::make_unique<WeaponUnlockState>(option1, option2));
        }
        };

    bulletPool_.DeactivateAll();
    enemyPool_.DeactivateAll(); // clear any leftover bullets from a previous run
    spawnSystem_.Reset();

    srand((unsigned int)time(nullptr)); // seed random number for spawn positions
    GameManager::Instance().Reset();  // fresh HP and score each game

    particlePool_.DeactivateAll();
    orbPool_.DeactivateAll();
    xpSystem_.Init(orbPool_, particlePool_);

    bossSpawned_ = false;
    BingoSystem::Instance().Init();
}

void PlayingState::Update(float dt) {
    if (IsKeyPressed(KEY_F11))
        ToggleFullscreen();

    if (pauseCooldown_ > 0.f) pauseCooldown_ -= dt;
    // ESC (pause state on top) 
    // PATTERN: State (push/pop means PlayingState is never destroyed on pause)
    
    static int lastUnlockedCount = 0;
    int currentUnlocked = (int)GameManager::Instance().unlockedWeaponSlots.size();
    if (currentUnlocked != lastUnlockedCount) {
        lastUnlockedCount = currentUnlocked;
        SyncWeaponsWithGameManager();
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        pauseCooldown_ = 0.2f;
        StateManager::Instance().PushState(std::make_unique<PausedState>());
        return;
    }

    elapsedTime_ += dt; // track total game time

    GameManager::Instance().elapsedTime = elapsedTime_;  //keep GM in sync; to avoid problem with timing

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

    /*playerPos_.x += dx * speed_ * dt;
    playerPos_.y += dy * speed_ * dt;*/
    playerPos_.x += dx * speed_ * GameManager::Instance().speedMult * dt;
    playerPos_.y += dy * speed_ * GameManager::Instance().speedMult * dt;

    float r = 16.f;
    if (playerPos_.x < r) playerPos_.x = r;
    if (playerPos_.x > SW - r) playerPos_.x = SW - r;
    if (playerPos_.y < r) playerPos_.y = r;
    if (playerPos_.y > SH - r) playerPos_.y = SH - r;

    // PATTERN: Command (key bindings decoupled from the swap logic)
    if (IsKeyPressed(KEY_E)) NextWeapon();
    if (IsKeyPressed(KEY_Q)) PrevWeapon();

    // Auto fire
    // PATTERN: Strategy (we call Update() on the weapon, not Fire() directly)
    // The weapon internally decides when enough time has passed to fire.
    Vector2 mousePos = GetMousePosition();
    /*currentWeapon_->Update(dt, playerPos_, lastMoveDir_, mousePos, bulletPool_,
        1.0f, // dmgMult
        1.0f); // frMult*/
    if (CurrentWeapon()) {
        //CurrentWeapon()->Update(dt, playerPos_, lastMoveDir_, mousePos, bulletPool_, 1.0f, 1.0f);
        CurrentWeapon()->Update(dt, playerPos_, lastMoveDir_, mousePos, bulletPool_, 
            GameManager::Instance().dmgMult, GameManager::Instance().frMult);
    }

    // Move bullets forward
    UpdateBullets(dt);

    // Enemies (spawn new enemies based on time)
    spawnSystem_.Update(dt, elapsedTime_, playerPos_);
    // move existing enemies toward player
    UpdateEnemies(dt);

    xpSystem_.Update(dt, playerPos_);
    UpdateParticles(dt);

    // Collision detection (returns true if player just died)
    // PATTERN: Observer (CollisionSystem fires events, doesn't call systems directly)
    bool gameOver = collision_.Update(bulletPool_, enemyPool_, playerPos_, playerRadius_, dt);
    if (gameOver) {
        StateManager::Instance().ChangeState(std::make_unique<GameOverState>());
        return;
    }

    if (GameManager::Instance().upgradePoints > 0) {
        StateManager::Instance().PushState(std::make_unique<LevelUpState>());
        return;
    }

    CheckWeaponConfiscation();

    // Boss spawn at 180s
    if (!bossSpawned_ && elapsedTime_ >= 180.f) {
        bossSpawned_ = true;

        // Free a slot if pool is full — boss always spawns
        bool hasSlot = false;
        for (const auto& e : enemyPool_.enemies)
            if (!e.active) { hasSlot = true; break; }

        if (!hasSlot) {
            // Deactivate the first non boss enemy to make room
            for (auto& e : enemyPool_.enemies) {
                if (e.active && e.type != EnemyType::CEOBOSS) {
                    e.active = false;
                    break;
                }
            }
        }
        
        Enemy* boss = enemyFactory_.Spawn(EnemyType::CEOBOSS, { 400.f, 50.f }, false);
        if (boss) GameEvents::Instance().OnBossSpawned.Fire({ boss->position });
    }

    // Win condition
    if (BingoSystem::Instance().HasBingo()) {
        StateManager::Instance().ChangeState(std::make_unique<WinState>());
        return;
    }
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

        // SHOCKWAVE (expands radius, deactivates when too large)
        if (b.bulletType == BulletType::SHOCKWAVE) {
            b.radius += b.expansionSpeed * dt;
            if (b.radius > 300.f) { b.active = false; continue; }
            continue; // shockwave doesn't move
        }

        // GRAVITY (arc trajectory; used by ExplosivePills)
        if (b.bulletType == BulletType::GRAVITY) {
            b.velocity.y += 280.f * dt;
            // Detonate when hitting the bottom of the screen
            if (b.position.y >= SH - 10.f) {
                b.active = false;
                continue;
            }
        }

        // Move bullet; Move in a straight line 
        b.position.x += b.velocity.x * dt;
        b.position.y += b.velocity.y * dt;

        // BOUNCING — reflect off screen edges
        if (b.bulletType == BulletType::BOUNCING) {
            if (b.position.x < 0.f || b.position.x > SW) {
                b.velocity.x *= -1.f;
                b.bounceCount++;
            }
            if (b.position.y < 0.f || b.position.y > SH) {
                b.velocity.y *= -1.f;
                b.bounceCount++;
            }
            if (b.bounceCount >= 3) { b.active = false; continue; }
        }

        // Deactivate if bullet leaves the screen
        if (b.bulletType != BulletType::BOUNCING) {
            if (b.position.x < -50.f || b.position.x > SW + 50.f ||
                b.position.y < -50.f || b.position.y > SH + 50.f) {
                b.active = false;
            }
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

        /*e.position.x += dx * e.speed * dt;
        e.position.y += dy * e.speed * dt;*/

        switch (e.type) {

            case EnemyType::ORDERLY: {
                // Charges fast, then retreats
                e.orderlyTimer += dt;
                if (e.orderlyState == OrderlyState::IDLE && e.orderlyTimer > 2.f) {
                    e.orderlyState = OrderlyState::CHARGING;
                    e.orderlyTimer = 0.f;
                    e.speed = e.baseSpeed * 3.f;
                }
                else if (e.orderlyState == OrderlyState::CHARGING && e.orderlyTimer > 0.6f) {
                    e.orderlyState = OrderlyState::RETREATING;
                    e.orderlyTimer = 0.f;
                    e.speed = e.baseSpeed;
                }
                else if (e.orderlyState == OrderlyState::RETREATING && e.orderlyTimer > 1.f) {
                    e.orderlyState = OrderlyState::IDLE;
                    e.orderlyTimer = 0.f;
                }
                if (e.orderlyState == OrderlyState::RETREATING) {
                    dx = -dx; dy = -dy;  // move away from player
                }
                e.position.x += dx * e.speed * dt;
                e.position.y += dy * e.speed * dt;
                break;
            }

            case EnemyType::ADMIN: {
                // Slow movement, calls reinforcements every 8s
                e.reinforceTimer -= dt;
                if (e.reinforceTimer <= 0.f) {
                    e.reinforceTimer = 8.f;
                    // Spawn 2 nurses near the admin
                    for (int i = 0; i < 2; i++) {
                        float angle = ((float)rand() / RAND_MAX) * 6.2831f;
                        Vector2 pos = {
                            e.position.x + cosf(angle) * 40.f,
                            e.position.y + sinf(angle) * 40.f
                        };
                        enemyFactory_.Spawn(EnemyType::NURSE, pos);
                    }
                }
                e.position.x += dx * e.speed * dt;
                e.position.y += dy * e.speed * dt;
                break;
            }

            case EnemyType::CEOBOSS: {
                // Phase transitions based on HP percentage
                float hpPct = e.health / e.maxHealth;
                if (hpPct < 0.33f) e.bossPhase = 3;
                else if (hpPct < 0.66f) e.bossPhase = 2;
                else                    e.bossPhase = 1;

                // Speed increases each phase
                e.speed = e.baseSpeed * (1.f + (e.bossPhase - 1) * 0.4f);

                e.position.x += dx * e.speed * dt;
                e.position.y += dy * e.speed * dt;
                break;
            }

            default:
                // NURSE and DOCTOR; straight movement toward player
                e.position.x += dx * e.speed * dt;
                e.position.y += dy * e.speed * dt;
                break;
        }
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

    /*// HUD (weapon name bottom left)
    DrawText(currentWeapon_->GetName(), 10, SH - 45, 18, GREEN);
    DrawText(currentWeapon_->GetDescription(), 10, SH - 25, 14, GRAY);

    // HUD (Enemy counter; top right)
    char buf[32];
    snprintf(buf, 32, "Enemies: %d", enemyPool_.ActiveCount());
    DrawText(buf, SW - 150, 10, 20, WHITE);

    // Elapsed time; top center
    snprintf(buf, 32, "%.0fs", elapsedTime_);
    DrawText(buf, SW / 2 - 20, 10, 20, WHITE);*/

    DrawOrbs();
    DrawParticles();

    DrawBingoCard();

    DrawHUD();  // replaces the inline snprintf calls
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

        //// Enemy body
        //DrawCircleV(e.position, e.size, e.color);

        //// Health bar above the enemy
        //// Bar width = diameter of the enemy circle
        //float barWidth = e.size * 2.f;
        //float hpFrac = e.health / e.maxHealth;

        //// Background bar
        //DrawRectangle(
        //    (int)(e.position.x - barWidth / 2.f),
        //    (int)(e.position.y - e.size - 7.f),
        //    (int)barWidth, 4, DARKGRAY);

        //// Filled portion (green > yellow > red based on HP)
        //Color barColor = (hpFrac > 0.6f) ? GREEN
        //    : (hpFrac > 0.3f) ? YELLOW
        //    : RED;
        //DrawRectangle(
        //    (int)(e.position.x - barWidth / 2.f),
        //    (int)(e.position.y - e.size - 7.f),
        //    (int)(barWidth * hpFrac), 4, barColor);

        // Boss gets a special multi-ring draw

        if (e.type == EnemyType::CEOBOSS) {
            Color phaseCol = e.bossPhase == 1 ? Color{ 139,0,0,255 }
            : e.bossPhase == 2 ? ORANGE : RED;
            DrawCircleV(e.position, e.size, phaseCol);
            DrawCircleLines((int)e.position.x, (int)e.position.y,
                (int)e.size + 4, WHITE);
            DrawCircleLines((int)e.position.x, (int)e.position.y,
                (int)e.size + 8,
                e.bossPhase >= 2 ? ORANGE : DARKGRAY);

            char buf[16];
            snprintf(buf, 16, "PHASE %d", e.bossPhase);
            DrawText(buf,
                (int)e.position.x - 24,
                (int)e.position.y - e.size - 22, 14, WHITE);
        }
        else {
            DrawCircleV(e.position, e.size, e.color);
        }

        // Health bar (all enemies)
        float barWidth = e.size * 2.f;
        float hpFrac = e.health / e.maxHealth;
        DrawRectangle(
            (int)(e.position.x - barWidth / 2.f),
            (int)(e.position.y - e.size - 7.f),
            (int)barWidth, 4, DARKGRAY);
        Color barColor = hpFrac > 0.6f ? GREEN : hpFrac > 0.3f ? YELLOW : RED;
        DrawRectangle(
            (int)(e.position.x - barWidth / 2.f),
            (int)(e.position.y - e.size - 7.f),
            (int)(barWidth * hpFrac), 4, barColor);
    }
}

void PlayingState::DrawHUD() const {
    auto& gm = GameManager::Instance();

    // HP bar; top left
    DrawRectangle(10, 10, 200, 18, DARKGRAY);
    float hpFrac = gm.health / gm.maxHealth;
    DrawRectangle(10, 10, (int)(200.f * hpFrac), 18, RED);
    DrawRectangleLines(10, 10, 200, 18, WHITE);

    char buf[32];
    snprintf(buf, 32, "HP: %.0f/%.0f", gm.health, gm.maxHealth);
    DrawText(buf, 14, 12, 14, WHITE);

    // XP bar
    DrawXPBar();

    // Score; top right
    snprintf(buf, 32, "Score: %d", gm.score);
    DrawText(buf, SW - 160, 10, 20, WHITE);

    snprintf(buf, 32, "Kills: %d", gm.enemiesKilled);
    DrawText(buf, SW - 140, 34, 16, LIGHTGRAY);

    // Elapsed time; top center
    snprintf(buf, 32, "%.0fs", elapsedTime_);
    DrawText(buf, SW / 2 - 20, 10, 20, WHITE);

    // Weapon; bottom left
    /*DrawText(currentWeapon_->GetName(), 10, SH - 45, 18, GREEN);
    DrawText(currentWeapon_->GetDescription(), 10, SH - 25, 14, GRAY);*/
    if (CurrentWeapon()) {
        DrawText(CurrentWeapon()->GetName(), 10, SH - 45, 18, GREEN);
        DrawText(CurrentWeapon()->GetDescription(), 10, SH - 25, 14, GRAY);
    }
    DrawText("[Q] Prev   [E] Next", 10, SH - 65, 13, DARKGRAY);
}

void PlayingState::NextWeapon() {
    if (weapons_.empty()) return;
    currentWeaponIdx_ = (currentWeaponIdx_ + 1) % (int)weapons_.size();
}

void PlayingState::PrevWeapon() {
    if (weapons_.empty()) return;
    currentWeaponIdx_ = (currentWeaponIdx_ - 1 + (int)weapons_.size())
        % (int)weapons_.size();
}

WeaponStrategy* PlayingState::CurrentWeapon() const {
    if (weapons_.empty()) return nullptr;
    return weapons_[currentWeaponIdx_].get();
}

void PlayingState::Exit() {
    xpSystem_.Shutdown();
    BingoSystem::Instance().Shutdown();
}

void PlayingState::UpdateParticles(float dt) {
    for (auto& p : particlePool_.particles) {
        if (!p.active) continue;
        p.lifetime += dt;
        if (p.lifetime >= p.maxLifetime) { p.active = false; continue; }
        p.position.x += p.velocity.x * dt;
        p.position.y += p.velocity.y * dt;
        p.velocity.x *= 0.92f;  // slow down over time
        p.velocity.y *= 0.92f;
    }
}

void PlayingState::DrawParticles() const {
    for (const auto& p : particlePool_.particles) {
        if (!p.active) continue;
        float alpha = 1.f - (p.lifetime / p.maxLifetime);
        Color c = p.color;
        c.a = (unsigned char)(alpha * 255.f);
        DrawCircleV(p.position, p.radius, c);
    }
}

void PlayingState::DrawOrbs() const {
    for (const auto& o : orbPool_.orbs) {
        if (!o.active) continue;
        float pulse = 0.7f + 0.3f * sinf(o.lifetime * 6.f);
        DrawCircleV(o.position, o.radius * pulse, o.color);
        DrawCircleLines((int)o.position.x, (int)o.position.y,
            (int)(o.radius * pulse) + 2, WHITE);
    }
}

void PlayingState::DrawXPBar() const {
    auto& gm = GameManager::Instance();
    float xpFrac = gm.xpToNextLevel > 0.f ? gm.xp / gm.xpToNextLevel : 0.f;

    DrawRectangle(10, 32, 200, 8, DARKGRAY);
    DrawRectangle(10, 32, (int)(200.f * xpFrac), 8, SKYBLUE);
    DrawRectangleLines(10, 32, 200, 8, WHITE);

    char buf[32];
    snprintf(buf, 32, "LVL %d", gm.level);
    DrawText(buf, 215, 30, 14, SKYBLUE);
}

void PlayingState::DrawBingoCard() const {
    const auto& nums = BingoSystem::Instance().Numbers();
    const auto& marked = BingoSystem::Instance().Marked();

    // Small card in bottom right corner
    int startX = SW - 145;
    int startY = SH - 155;
    int cellW = 26;
    int cellH = 26;

    // Background
    DrawRectangle(startX - 4, startY - 18, 135, 160, { 0, 0, 0, 160 });
    DrawText("BINGO", startX, startY - 16, 14, GOLD);

    char buf[8];
    for (int i = 0; i < 25; i++) {
        int col = i % 5;
        int row = i / 5;
        int x = startX + col * cellW;
        int y = startY + row * cellH;

        Color bg = marked[i] ? Color{ 0, 180, 0, 200 } : Color{ 20, 20, 40, 200 };
        DrawRectangle(x, y, cellW - 2, cellH - 2, bg);

        if (i == 12) {
            DrawText("*", x + 8, y + 4, 14, GOLD);  // FREE square
        }
        else {
            snprintf(buf, 8, "%d", nums[i]);
            DrawText(buf, x + 2, y + 5, 11,
                marked[i] ? WHITE : LIGHTGRAY);
        }
    }
}

void PlayingState::SyncWeaponsWithGameManager() {
    auto& gm = GameManager::Instance();
    weapons_.clear();

    // Rebuild weapon list from unlocked slots in order
    // Full weapon list order must match slot indices 0-6
    for (int slot : gm.unlockedWeaponSlots) {
        switch (slot) {
        case 0: weapons_.push_back(std::make_unique<CaneLaser>()); break;
        case 1: weapons_.push_back(std::make_unique<CandyRain>()); break;
        case 2: weapons_.push_back(std::make_unique<FlyingDentures>()); break;
        case 3: weapons_.push_back(std::make_unique<ExplosivePills>()); break;
        case 4: weapons_.push_back(std::make_unique<PureeCatapult>()); break;
        case 5: weapons_.push_back(std::make_unique<BingoSoundWave>()); break;
        case 6: weapons_.push_back(std::make_unique<TurboCharge>()); break;
        }
    }

    if (currentWeaponIdx_ >= (int)weapons_.size())
        currentWeaponIdx_ = 0;
}

void PlayingState::CheckWeaponConfiscation() {
    auto& gm = GameManager::Instance();

    // Only confiscate if player has more than 1 weapon
    if (weapons_.size() <= 1) return;

    if (gm.CheckConfiscation()) {
        // Get name of current weapon before confiscating
        const char* taken = CurrentWeapon() ? CurrentWeapon()->GetName() : "Unknown";
        int takenIdx = gm.unlockedWeaponSlots[currentWeaponIdx_];

        // Remove from unlocked, add to confiscated
        gm.unlockedWeaponSlots.erase(gm.unlockedWeaponSlots.begin() + currentWeaponIdx_);
        gm.confiscatedSlots.push_back(takenIdx);

        // Rebuild weapon list and clamp index
        SyncWeaponsWithGameManager();

        // Get name of new active weapon
        const char* replacement = CurrentWeapon() ? CurrentWeapon()->GetName() : "None";

        // Show confiscation card as overlay
        StateManager::Instance().PushState(std::make_unique<ConfiscationState>(taken, replacement));
    }
}