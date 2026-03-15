#include "EnemyFactory.h"
#include "GameManager.h"
#include "TextureManager.h"

Enemy* EnemyFactory::Spawn(EnemyType type, Vector2 position, bool isFinalBoss) {
    Enemy* e = pool_.Spawn();   // ask the injected pool for a free slot
    if (!e) return nullptr;     // pool exhausted — caller gets nullptr
    Configure(*e, type, isFinalBoss);
    e->position = position;
    return e;
}

void EnemyFactory::Configure(Enemy& e, EnemyType type, bool isFinalBoss) {
    e.type = type;
    e.isFinalBoss = isFinalBoss;

    // Final boss multiplier applied to HP only
    const float finalMult = isFinalBoss ? 2.5f : 1.0f;

    switch (type) {
    case EnemyType::NURSE:
        e.health = e.maxHealth = 50.f * finalMult;
        //e.speed = e.baseSpeed = 10.f;//85.f;
        switch (GameManager::Instance().difficulty) {
        case Difficulty::EASY:   e.speed = e.baseSpeed = 10.f;  break;
        case Difficulty::MEDIUM: e.speed = e.baseSpeed = 45.f;  break;
        case Difficulty::HARD:   e.speed = e.baseSpeed = 85.f;  break;
        }
        e.damage = 1.f;
        e.size = 16.f;
        e.color = WHITE;
        e.pointValue = 10;
        e.xpValue = 12;
        e.attackCooldown = 3.f;

        Texture2D tex = TextureManager::Instance().Load("assets/sprites/Sally.png");
        //e.animator.Init(tex, 32, 32, 4, 8.f);
        e.animator.Init(tex, 32, 32, 1, 1.f);  // frameCount = 1
        e.animator.SetRow(4);

        break;

        
        case EnemyType::DOCTOR: {
            e.health = e.maxHealth = 80.f * finalMult;
            //e.speed = e.baseSpeed = 10.f;//60.f;
            switch (GameManager::Instance().difficulty) {
            case Difficulty::EASY:   e.speed = e.baseSpeed = 10.f;  break;
            case Difficulty::MEDIUM: e.speed = e.baseSpeed = 35.f;  break;
            case Difficulty::HARD:   e.speed = e.baseSpeed = 60.f;  break;
            }
            e.damage = 2.f;
            e.size = 20.f;
            e.color = GREEN;
            e.pointValue = 20;
            e.xpValue = 18;
            e.clipboardActive = true;
            e.clipboardCooldown = 4.f;

            Texture2D tex = TextureManager::Instance().Load("assets/sprites/spr_NPCWOMEN_0.png");
            //e.animator.Init(tex, 96, 60, 2, 4.f);
            e.animator.Init(tex, 96, 60, 1, 1.f);  // frameCount = 1
            e.animator.SetRow(0);
            break;
        }

        case EnemyType::ADMIN: {
            e.health = e.maxHealth = 120.f * finalMult;
            //e.speed = e.baseSpeed = 10.f;//45.f;
            switch (GameManager::Instance().difficulty) {
            case Difficulty::EASY:   e.speed = e.baseSpeed = 10.f;  break;
            case Difficulty::MEDIUM: e.speed = e.baseSpeed = 28.f;  break;
            case Difficulty::HARD:   e.speed = e.baseSpeed = 45.f;  break;
            }
            e.damage = 1.f;
            e.size = 22.f;
            e.color = GRAY;
            e.pointValue = 35;
            e.xpValue = 25;
            e.reinforceTimer = 8.f;

            Texture2D tex = TextureManager::Instance().Load("assets/sprites/spr_NPCWOMEN_0.png");
            //e.animator.Init(tex, 96, 60, 2, 4.f);
            e.animator.Init(tex, 96, 60, 1, 1.f);  // frameCount = 1
            e.animator.SetRow(4);
            break;
        }
        case EnemyType::ORDERLY: {
            e.health = e.maxHealth = 40.f * finalMult;
            //e.speed = e.baseSpeed = 10.f;//140.f;
            switch (GameManager::Instance().difficulty) {
            case Difficulty::EASY:   e.speed = e.baseSpeed = 10.f;   break;
            case Difficulty::MEDIUM: e.speed = e.baseSpeed = 80.f;   break;
            case Difficulty::HARD:   e.speed = e.baseSpeed = 140.f;  break;
            }
            e.damage = 3.f;
            e.size = 13.f;
            e.color = BLUE;
            e.pointValue = 25;
            e.xpValue = 20;
            e.orderlyState = OrderlyState::IDLE;

            Texture2D tex = TextureManager::Instance().Load("assets/sprites/spr_NPCWOMEN_0.png");
            //e.animator.Init(tex, 96, 60, 2, 6.f);
            e.animator.Init(tex, 96, 60, 1, 1.f);  // frameCount = 1
            e.animator.SetRow(2);
            break;
        }
    case EnemyType::CEOBOSS: {
        e.health = e.maxHealth = 2000.f * finalMult;
        //e.speed = e.baseSpeed = 10.f;//55.f;
        switch (GameManager::Instance().difficulty) {
        case Difficulty::EASY:   e.speed = e.baseSpeed = 10.f;  break;
        case Difficulty::MEDIUM: e.speed = e.baseSpeed = 35.f;  break;
        case Difficulty::HARD:   e.speed = e.baseSpeed = 55.f;  break;
        }
        e.damage = 5.f;
        e.size = 55.f;
        e.color = { 139, 0, 0, 255 }; // dark red
        e.pointValue = 500;
        e.xpValue = 200;
        e.bossPhase = 1;
        e.attackCooldown = 2.f;

        Texture2D tex = TextureManager::Instance().Load("assets/sprites/drdoktor.png");
        //e.animator.Init(tex, 85, 64, 3, 6.f);
        e.animator.Init(tex, 70, 64, 1, 1.f);  // frameCount = 1 — just first frame
        e.animator.SetFrameOffset(2);  // only boss
        e.animator.SetRow(0);
        break;
        }
    }
}