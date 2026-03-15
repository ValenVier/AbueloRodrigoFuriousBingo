#pragma once
// Handles sprite sheet animation,frame selection and timing
// Used by all enemy types and the player
#include "raylib.h"

class SpriteAnimator {
    public:
        // texture    : loaded sprite sheet
        // frameW/H   : pixel size of one frame
        // frameCount : how many frames in this animation
        // fps        : animation speed
        void Init(Texture2D texture, int frameW, int frameH, int frameCount, float fps);

        void Update(float dt);

        // Draw centered on worldPos, scaled to displaySize, tinted with color
        //void Draw(Vector2 worldPos, float displaySize, Color tint, bool flipX = false) const;
        void Draw(Vector2 worldPos, float displaySize, 
            Color tint, bool flipX = false, bool flipY = false) const;

        // Change which row of the sheet to use (for directional sprites)
        void SetRow(int row) { currentRow_ = row; }
        void SetFrameCount(int count) { frameCount_ = count; }

        int frameOffsetX_ = 0;
        void SetFrameOffset(int offsetX) { frameOffsetX_ = offsetX; }

    private:
        Texture2D texture_ = {};
        int frameW_ = 0;
        int frameH_ = 0;
        int frameCount_ = 1;
        int currentRow_ = 0;
        float fps_ = 8.f;
        float timer_ = 0.f;
        int currentFrame_ = 0;
};