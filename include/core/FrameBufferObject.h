/**
 * @file FrameBufferObject.h
 * @author Ryan Purse
 * @date 18/02/2022
 */


#pragma once


/**
 * Handles the creation of a framebuffer and renders to its texture so that it can be used by ImGui.
 * @author Ryan Purse
 * @date 18/02/2022
 */
class FrameBufferObject
{
public:
    explicit FrameBufferObject(glm::ivec2 size);
    
    void clear();
    
    void update();
    void imguiUpdate();
    
    [[nodiscard]] unsigned int getId() const;
    
    const glm::vec2 &getSize() const;
    
    [[nodiscard]] unsigned int getTexture() const;

protected:
    unsigned int mFboId { 0 };
    unsigned int mTexId { 0 };
    unsigned int mRboId { 0 };
    
    glm::vec2 mSize                 { 400 };
    const glm::vec4  mClearColour   { 0.2f, 0.2f, 0.3f, 1.f };
    const float      mDepth         { 1.f };
};


