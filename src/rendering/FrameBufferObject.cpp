/**
 * @file FrameBufferObject.cpp
 * @author Ryan Purse
 * @date 14/03/2022
 */


#include "rendering/FrameBufferObject.h"
#include "gtc/type_ptr.hpp"

FrameBufferObject::FrameBufferObject(const glm::ivec2 &size)
    : mSize(size), mRenderBufferObject(size)
{
    glCreateFramebuffers(1, &mFboName);
    glNamedFramebufferRenderbuffer(
        mFboName, mRenderBufferObject.getAttachment(),
        GL_RENDERBUFFER, mRenderBufferObject.getName());
    
    validate();
}

FrameBufferObject::FrameBufferObject(
    const glm::ivec2 &size, GLenum sourceBlendFunction, GLenum destinationBlendFunction) :
    mSize(size), mRenderBufferObject(size),
    mSourceBlend(sourceBlendFunction), mDestinationBlend(destinationBlendFunction)
{
    glCreateFramebuffers(1, &mFboName);
    glNamedFramebufferRenderbuffer(
        mFboName, mRenderBufferObject.getAttachment(),
        GL_RENDERBUFFER, mRenderBufferObject.getName());
    
    validate();
}

FrameBufferObject::~FrameBufferObject()
{
    glDeleteFramebuffers(1, &mFboName);
    mFboName = 0;
}

void FrameBufferObject::attach(std::shared_ptr<TextureBufferObject> &textureBufferObject, int bindPoint)
{
    textureBufferObject->reattach = [&textureBufferObject, bindPoint, this]() {
        detach(bindPoint);
        attach(textureBufferObject, bindPoint);
    };
    // textureBufferObject->changeSize(mSize);
    glNamedFramebufferTexture(mFboName, GL_COLOR_ATTACHMENT0 + bindPoint, textureBufferObject->mName, 0);
    validate();
    
    mTextures.push_back(textureBufferObject);
    mAttachments.emplace_back(GL_COLOR_ATTACHMENT0 + bindPoint);
    glNamedFramebufferDrawBuffers(mFboName, mAttachments.size(), &mAttachments[0]);
}

void FrameBufferObject::attach(const TextureBufferObject &textureBufferObject, int bindPoint)
{
    // textureBufferObject->changeSize(mSize);
    glNamedFramebufferTexture(mFboName, GL_COLOR_ATTACHMENT0 + bindPoint, textureBufferObject.mName, 0);
    validate();
    
    mAttachments.emplace_back(GL_COLOR_ATTACHMENT0 + bindPoint);
    glNamedFramebufferDrawBuffers(mFboName, mAttachments.size(), &mAttachments[0]);
}

void FrameBufferObject::detach(int bindPoint)
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

void FrameBufferObject::validate()
{
    const unsigned int fboStatus = glCheckNamedFramebufferStatus(mFboName, GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        debug::log("Framebuffer error of: " + std::to_string(fboStatus), debug::severity::Major);
}

void FrameBufferObject::clear()
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

unsigned int FrameBufferObject::getFboName() const
{
    return mFboName;
}

void FrameBufferObject::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFboName);
    glBlendFunc(mSourceBlend, mDestinationBlend);
}
