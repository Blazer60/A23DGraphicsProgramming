/**
 * @file MipmapTexture.cpp
 * @author Ryan Purse
 * @date 14/05/2022
 */


#include "MipmapTexture.h"

MipmapTexture::MipmapTexture(const glm::ivec2 &size)
    : TextureBufferObject(size)
{

}

MipmapTexture::MipmapTexture(const glm::ivec2 &size, GLenum format, const std::string &debugName)
    : TextureBufferObject(size, format, debugName)
{

}

void MipmapTexture::init2()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &mName);
    glTextureParameteri(mName, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mName, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(mName, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mName, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(mName, 1, mFormat, mSize.x, mSize.y);
    glGenerateTextureMipmap(mName);
}
