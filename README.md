# ABUELO RODRIGO - The furious Bingo!

> *"Abuelo" means "Grandpa" in Spanish. 
> This game is a tribute to all the elderly people who just want a 
> little freedom — and find themselves trapped in a care home instead.*

Bingo Furioso is a top-down survival shooter built with 
**C++ and Raylib 5.5**, created as a design patterns showcase project. 
You play as **Abuelo Rodrigo**, an 80-year-old man in a turbo-powered 
wheelchair who has had enough of the care home. Armed with increasingly 
creative weapons, Rodrigo must fight off the entire medical 
staff - nurses, doctors, administrators, orderlies, 
and the fearsome CEO Boss - while completing his bingo card 
to win his freedom.

---

## How to Play

### Objective
Survive the endless waves of hospital staff and **mark every square on your bingo card**. Each enemy type marks a different range of numbers. Complete the full card to win. If Rodrigo's health reaches zero, it's game over.

### Controls

| Key | Action                                 |
|-----|----------------------------------------|
| WASD | Move Rodrigo                           |
| Q / E | Cycle through weapons                  |
| ESC | Pause game                             |
| Mouse | Aim (some weapons fire toward cursor)  |
| Auto-fire | Always active - no shoot button needed |

### Bingo Mechanic
Each enemy type marks a specific range on the bingo card when killed:

| Enemy | Number Range |
|-------|-------------|
| Nurse (white) | 1 – 15 |
| Orderly (blue) | 16 – 30 |
| Doctor (green) | 31 – 45 |
| Administrator (gray) | 46 – 60 |
| CEO Boss (dark red) | 61 – 75 (one number only) |

The center square is always FREE. Fill every square to trigger victory.

### Weapons
Rodrigo starts with **2 weapons**. Complete a bingo line to 
unlock more - choose between 2 options each time. 
If Rodrigo takes heavy damage, his weapon may get **confiscated** by 
the medical staff. Recover it by completing another bingo line.

| Weapon | Description |
|--------|-------------|
| Cane Laser | Fast cyan beam in movement direction |
| Candy Rain | 12 candy projectiles in all directions |
| Flying Dentures | Bounces off walls up to 3 times |
| Explosive Pills | Arc shot with gravity, explodes on impact |
| Puree Catapult | 6 spread blobs that slow enemies |
| Bingo Sound Wave | Expanding gold ring that hits all enemies |
| Turbo Charge | High-speed projectile with knockback |

### Weapon Confiscation
At **75%, 50%, and 25% HP**, the medical staff confiscates 
Rodrigo's active weapon for "disturbing the patients". 
A confiscation card is displayed. The weapon can be recovered 
by completing a bingo line. Rodrigo is never left with zero weapons.

### Difficulty Modes
Select before starting from the main menu:

| Mode | Enemy Speed | CandyRain Rate |
|------|-------------|----------------|
| Easy | Slow | Slow (3.5s) |
| Medium | Moderate | Medium (1.8s) |
| Hard | Fast | Fast (0.6s) |

### The CEO Boss
At **180 seconds**, the CEO Boss appears - a large, 
slow-moving enemy with 2000 HP and 3 phases. 
Each phase increases speed and changes his 
ring colour (red → orange → yellow). 
He always spawns enemies.

---

## What's in the Game

The furious Bingo! ships with WASD movement and normalized 
diagonal speed, auto-fire that adapts to the active weapon - some shoot 
in the movement direction, others aim toward the mouse. 
Rodrigo has a visible HP bar and takes continuous damage on 
contact with enemies.

There are 7 weapons with distinct firing styles: 
a straight laser beam, bouncing dentures, an arc pill with gravity, 
a spread of slowing blobs, an expanding shockwave ring, 
a knockback projectile, and a 360-degree candy burst. 
Weapons are swapped with Q/E and unlocked progressively through 
the bingo system rather than handed out all at once.

Five enemy types populate the waves - nurses, doctors, administrators, 
orderlies and the CEO Boss - each with different health, speed, 
and behaviour. Nurses and doctors move straight toward Rodrigo. 
Orderlies charge at triple speed then retreat. 
Administrators call nurse reinforcements every 8 seconds. 
The CEO Boss has three phases that increase speed and change 
his visual appearance. Enemy spawning accelerates over time, 
with new types unlocking at 20, 45 and 80 seconds. Wave size grows 
every 20 seconds and is capped to prevent pool exhaustion.

The bingo card drives the win condition - every square must be 
filled, not just one line. Each enemy type marks a specific 
number range, and the boss guarantees exactly one number in 
the 61–75 range. Completing a line opens a weapon choice screen. 
Losing 25%, 50% or 75% of HP triggers a confiscation event that 
takes away the active weapon until the next line is completed.

On top of that: an XP and levelling system with upgrade cards, 
magnetic XP orbs dropped by enemies, a particle system for deaths 
and collections, a full state machine with pause and overlay screens, 
three difficulty modes, sprite-based characters with directional flip, 
fullscreen support, and 8 design patterns woven throughout the codebase.

---

## Design Patterns Used

### 1. State (StateManager)
All game screens are states managed by a stack-based StateManager. Push/pop allows overlays like Pause and LevelUp without destroying the game state underneath. States: MenuState, PlayingState, PausedState, LevelUpState, GameOverState, WinState, ConfiscationState, WeaponUnlockState.

### 2. Object Pool
Four pools with fixed arrays — zero heap allocation during gameplay:
- **BulletPool** — 500 slots
- **EnemyPool** — 200 slots
- **ParticlePool** — 600 slots
- **OrbPool** — 80 slots

### 3. Strategy
**WeaponStrategy** is the abstract base. 
Each of the 7 weapons implements **Fire()** independently. **PlayingState** 
calls **Update()** on the active weapon without knowing which one it holds.

### 4. Factory + Dependency Injection
**EnemyFactory** receives **EnemyPool** via constructor injection 
and handles both spawning and configuration. **SpawnSystem** 
receives **EnemyFactory** via constructor. 
Neither class knows about the other's internals.

### 5. Observer
**GameEvents** singleton holds typed **Event<T>** instances. 
Systems subscribe and unsubscribe without coupling. 
Events fired: **OnEnemyKilled**, **OnPlayerDamaged**, **OnLevelUp**, 
**OnOrbCollected**, **OnBingoNumber**, **OnBossSpawned**.

### 6. Singleton
**GameManager**, **GameEvents**, **BingoSystem**, **StateManager**, 
**TextureManager** - each exists as a single global instance 
with private constructors.

### 7. Command
Weapon swap actions (**NextWeapon**, **PrevWeapon**) are 
decoupled from the keys that trigger them. 
Key bindings can be changed without touching weapon logic.

### 8. Component (TextureManager + SpriteAnimator)
**TextureManager** caches and shares textures across all enemies. 
**SpriteAnimator** is a reusable component attached to each Enemy 
and Player instance, handling frame selection, directional flip, 
and scaling independently.

---

*Rodrigo just wants to play bingo. Let him.*
