/**
 * @file MipmapTexture.h
 * @author Ryan Purse
 * @date 14/05/2022
 */


#pragma once

#include "Pch.h"
#include "TextureBufferObject.h"

/**
 * A Texture Buffer with mip-mapping enabled.
 * @author Ryan Purse
 * @date 14/05/2022
 */
class MipmapTexture
    : public TextureBufferObject
{
public:
    explicit MipmapTexture(const glm::ivec2 &size);
    
    MipmapTexture(const glm::ivec2 &size, GLenum format, const int layers=8, const std::string &debugName="");
};
