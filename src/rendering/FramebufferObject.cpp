/**
 * @file FrameBufferObject.cpp
 * @author Ryan Purse
 * @date 14/03/2022
 */


#include "rendering/FramebufferObject.h"
#include "gtc/type_ptr.hpp"

FramebufferObject::FramebufferObject(const glm::ivec2 &size)
    : mSize(size), mRenderBufferObject(size)
{
    glCreateFramebuffers(1, &mFboName);
    glNamedFramebufferRenderbuffer(
        mFboName, mRenderBufferObject.getAttachment(),
        GL_RENDERBUFFER, mRenderBufferObject.getName());
    
    validate();
}

FramebufferObject::FramebufferObject(
    const glm::ivec2 &size, GLenum sourceBlendFunction,
    GLenum destinationBlendFunction, GLenum depthFunction) :
    mSize(size), mRenderBufferObject(size),
    mSourceBlend(sourceBlendFunction), mDestinationBlend(destinationBlendFunction),
    mDepthFunction(depthFunction)
{
    glCreateFramebuffers(1, &mFboName);
    glNamedFramebufferRenderbuffer(
        mFboName, mRenderBufferObject.getAttachment(),
        GL_RENDERBUFFER, mRenderBufferObject.getName());
    
    validate();
}

FramebufferObject::~FramebufferObject()
{
    glDeleteFramebuffers(1, &mFboName);
    mFboName = 0;
}

void FramebufferObject::attach(std::shared_ptr<TextureBufferObject> textureBufferObject, int bindPoint, int mipLevel)
{
    textureBufferObject->reattach = [&textureBufferObject, bindPoint, mipLevel, this]() {
        detach(bindPoint);
        attach(textureBufferObject, bindPoint, mipLevel);
    };
    
    if (textureBufferObject->mSize != mSize)
        debug::log("The attached texture MUST be the same size!", debug::severity::Major);
    
    glNamedFramebufferTexture(mFboName, GL_COLOR_ATTACHMENT0 + bindPoint, textureBufferObject->mName, mipLevel);
    
    mTextures.push_back(std::move(textureBufferObject));
    mAttachments.emplace_back(GL_COLOR_ATTACHMENT0 + bindPoint);
    glNamedFramebufferDrawBuffers(mFboName, mAttachments.size(), &mAttachments[0]);
    validate();
}

void FramebufferObject::detach(int bindPoint)
{
    glNamedFramebufferTexture(mFboName, GL_COLOR_ATTACHMENT0 + bindPoint, 0, 0);
    validate();
    
    const auto location = std::find(mAttachments.begin(), mAttachments.end(), GL_COLOR_ATTACHMENT0 + bindPoint);
    const auto offset = std::distance(mAttachments.begin(), location);
    mAttachments.erase(location);
    mTextures.erase(mTextures.begin() + offset);
    
    const auto *target = mAttachments.size() == 0 ? nullptr : &mAttachments[0];
    glNamedFramebufferDrawBuffers(mFboName, mAttachments.size(), target);
}

void FramebufferObject::validate()
{
    const unsigned int fboStatus = glCheckNamedFramebufferStatus(mFboName, GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        debug::log("Framebuffer error of: " + std::to_string(fboStatus), debug::severity::Major);
}

void FramebufferObject::clear()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFboName);
    for (int i = 0; i < mTextures.size(); ++i)
    {
        int drawBuffer = mAttachments[i] - GL_COLOR_ATTACHMENT0;
        glClearNamedFramebufferfv(mFboName, GL_COLOR, drawBuffer, glm::value_ptr(mTextures[i]->mClearColour));
    }
    glClearNamedFramebufferfv(mFboName, GL_DEPTH, 0, &mDepth);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FramebufferObject::getFboName() const
{
    return mFboName;
}

void FramebufferObject::bind() const
{
    glBlendFunc(mSourceBlend, mDestinationBlend);
    glDepthFunc(mDepthFunction);
    glViewport(0, 0, mSize.x, mSize.y);  // Properly sets up the NDC for this framebuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, mFboName);
}
