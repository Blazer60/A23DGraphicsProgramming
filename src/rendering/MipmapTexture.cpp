/**
 * @file MipmapTexture.cpp
 * @author Ryan Purse
 * @date 14/05/2022
 */


#include "MipmapTexture.h"

MipmapTexture::MipmapTexture(const glm::ivec2 &size)
    : TextureBufferObject(size, GL_RGB16, GL_LINEAR)
{
    glGenerateTextureMipmap(mName);
}

MipmapTexture::MipmapTexture(const glm::ivec2 &size, GLenum format, const std::string &debugName)
    : TextureBufferObject(size, format, GL_LINEAR, debugName)
{
    glGenerateTextureMipmap(mName);
}
