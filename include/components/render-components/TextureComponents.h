/**
 * @file TextureComponents.h
 * @author Ryan Purse
 * @date 21/02/2022
 */


#pragma once

#include "Pch.h"
#include <filesystem>

/**
 * @brief A texture Id that is used by OpenGL
 */
struct Texture
{
    unsigned int id;
};

/**
 * @brief A file path to a texture so that it can be loaded into the GPU
 */
struct TexturePath
{
    explicit TexturePath(std::filesystem::path filePath)
            : path(std::move(filePath)) { }
    
    std::filesystem::path path;
};

