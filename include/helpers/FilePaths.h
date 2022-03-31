/**
 * @file FilePaths.h
 * @author Ryan Purse
 * @date 16/02/2022
 */


#pragma once

#include "Pch.h"
#include <filesystem>

namespace path
{
    std::string resources();
    std::string shaders();
    std::string textures();
}
