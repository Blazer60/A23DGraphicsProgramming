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
class FrameBufferObject
{
public:
    explicit FrameBufferObject(const glm::ivec2 &size);
    FrameBufferObject(const glm::ivec2 &size, GLenum sourceBlendFunction, GLenum destinationBlendFunction);
    ~FrameBufferObject();
    
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
    
    GLenum mSourceBlend         { GL_SRC_ALPHA };
    GLenum mDestinationBlend    { GL_ONE_MINUS_SRC_ALPHA };
};


