/**
 * @file MipmapTexture.cpp
 * @author Ryan Purse
 * @date 14/05/2022
 */


#include "MipmapTexture.h"

MipmapTexture::MipmapTexture(const glm::ivec2 &size)
    : TextureBufferObject(size, GL_RGB16, GL_LINEAR, 1, std::string())
{
    glGenerateTextureMipmap(mName);
}

MipmapTexture::MipmapTexture(const glm::ivec2 &size, GLenum format, const int layers, const std::string &debugName)
    : TextureBufferObject(size, format, GL_LINEAR, layers, debugName)
{
    glGenerateTextureMipmap(mName);
}
