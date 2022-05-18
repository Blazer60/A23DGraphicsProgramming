/**
 * @file TextureBufferObject.cpp
 * @author Ryan Purse
 * @date 14/03/2022
 */


#include "rendering/TextureBufferObject.h"
#include "imgui.h"

TextureBufferObject::TextureBufferObject(const glm::ivec2 &size)
    : mSize(size)
{
    init(GL_NEAREST, GL_LINEAR, 1);
}

TextureBufferObject::TextureBufferObject(
    const glm::ivec2 &size, GLenum format, GLenum minFilter, GLenum magFilter, const int layers,
    std::string debugName)
    :
    mSize(size), mFormat(format), mDebugName(std::move(debugName))
{
    init(minFilter, magFilter, layers);
}

TextureBufferObject::~TextureBufferObject()
{
    deInit();
}

void TextureBufferObject::init(GLenum minFilter, GLenum magFilter, const int layers)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &mName);
    glTextureParameteri(mName, GL_TEXTURE_MIN_FILTER, minFilter);
    glTextureParameteri(mName, GL_TEXTURE_MAG_FILTER, magFilter);
    glTextureParameteri(mName, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mName, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(mName, layers, mFormat, mSize.x, mSize.y);
}

void TextureBufferObject::deInit()
{
    glDeleteTextures(1, &mName);
}

void TextureBufferObject::setClearColour(const glm::vec4 &clearColour)
{
    mClearColour = clearColour;
}

unsigned int TextureBufferObject::getName() const
{
    return mName;
}

const glm::ivec2 &TextureBufferObject::getSize() const
{
    return mSize;
}
