/**
 * @file FrameBufferObject.h
 * @author Ryan Purse
 * @date 14/03/2022
 */


#pragma once

#include "Pch.h"
#include "TextureBufferObject.h"
#include "RenderBufferObject.h"

/**
 * A set of details on where to render to for OpenGL.
 * @author Ryan Purse
 * @date 14/03/2022
 */
class FramebufferObject
{
public:
    explicit FramebufferObject(const glm::ivec2 &size);
    
    /**
     * @brief Creates a framebuffer with a depth texture already attached to it.
     * Blending equation: result = fragColour * sourceFunction + bufferColour * destinationColour.
     * @param size - The size of the framebuffer.
     * @param sourceBlendFunction - The amount of frag colour that will go through.
     * @param destinationBlendFunction - The amount of buffer colour that will go through.
     * @param depthFunction - Flag on how to pass the depth test (GL_ALWAYS to disable depth testing).
     */
    FramebufferObject(
        const glm::ivec2 &size, GLenum sourceBlendFunction,
        GLenum destinationBlendFunction, GLenum depthFunction);
    
    ~FramebufferObject();
    
    void attach(std::shared_ptr<TextureBufferObject> &textureBufferObject, int bindPoint);
    void attach(const TextureBufferObject &textureBufferObject, int bindPoint);
    void detach(int bindPoint);
    void clear();
    void bind() const;
    
    unsigned int getFboName() const;

protected:
    void validate();
    
    std::vector<std::shared_ptr<TextureBufferObject>> mTextures;
    std::vector<GLenum> mAttachments;
    
    unsigned int    mFboName        { 0 };
    const float     mDepth          { 1.f };
    glm::ivec2      mSize           { 1024 };
    RenderBufferObject mRenderBufferObject;
    
    GLenum mSourceBlend         { GL_ONE };
    GLenum mDestinationBlend    { GL_ZERO };
    GLenum mDepthFunction       { GL_LESS };
};


