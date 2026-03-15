#include "SpriteAnimator.h"

void SpriteAnimator::Init(Texture2D texture, int frameW, int frameH,
    int frameCount, float fps) {
    texture_ = texture;
    frameW_ = frameW;
    frameH_ = frameH;
    frameCount_ = frameCount;
    fps_ = fps;
    timer_ = 0.f;
    currentFrame_ = 0;
    currentRow_ = 0;
    frameOffsetX_ = 0;
}

void SpriteAnimator::Update(float dt) {
    // No animation — static sprite only


    /*if (frameCount_ <= 1) return;
    timer_ += dt;
    if (timer_ >= 1.f / fps_) {
        timer_ = 0.f;
        currentFrame_ = (currentFrame_ + 1) % frameCount_;
    }*/
}

void SpriteAnimator::Draw(Vector2 worldPos, float displaySize, Color tint, bool flipX) const {
    if (texture_.id == 0) return;

    // Source rectangl,select current frame from sheet
    Rectangle src = {
        (float)(currentFrame_ * frameW_) + frameOffsetX_,  // offset
        (float)(currentRow_ * frameH_),
        (float)(flipX ? -frameW_ : frameW_),
        (float)frameH_
    };

    // Destination rectangle,centered on worldPos, scaled
    /*Rectangle dst = {
        worldPos.x,
        worldPos.y,
        displaySize * 2.f,
        displaySize * 2.f
    };

    // Origin at center of destination
    Vector2 origin = { displaySize, displaySize };*/

    // Destination — keep original aspect ratio, scale by displaySize
    float aspectRatio = (float)frameW_ / (float)frameH_;
    float dstH = displaySize * 2.f;
    float dstW = dstH * aspectRatio;

    Rectangle dst = {
        worldPos.x,
        worldPos.y,
        dstW,
        dstH
    };

    // Origin at center
    Vector2 origin = { dstW / 2.f, dstH / 2.f };

    DrawTexturePro(texture_, src, dst, origin, 0.f, tint);
}