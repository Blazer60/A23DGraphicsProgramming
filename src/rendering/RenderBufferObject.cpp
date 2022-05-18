/**
 * @file RenderBufferObject.cpp
 * @author Ryan Purse
 * @date 14/03/2022
 */


#include "RenderBufferObject.h"

RenderBufferObject::RenderBufferObject(const glm::ivec2 &size)
    : mSize(size)
{
    init();
}

RenderBufferObject::~RenderBufferObject()
{
    deInit();
}

void RenderBufferObject::init()
{
    glCreateRenderbuffers(1, &mName);
    glNamedRenderbufferStorage(mName, mFormat, mSize.x, mSize.y);
}

void RenderBufferObject::deInit()
{
    glDeleteRenderbuffers(1, &mName);
}

GLenum RenderBufferObject::getAttachment() const
{
    return mAttachment;
}

unsigned int RenderBufferObject::getName() const
{
    return mName;
}
