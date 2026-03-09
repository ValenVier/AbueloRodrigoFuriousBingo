#pragma once
// Data struct for a single upgrade card shown in LevelUpState
// No logic here (LevelUpState reads these and PlayingState applies them)
#include "raylib.h"

enum class UpgradeType {
    DAMAGE_UP, // +25% bullet damage
    FIRE_RATE_UP, // +20% fire rate
    SPEED_UP, // +15% move speed
    MAX_HP_UP, // +20 max HP, heal 20
    XP_MAGNET, // orb pickup radius +40px
};

struct UpgradeCard {
    UpgradeType type;
    const char* title;
    const char* description;
    Color color;
};