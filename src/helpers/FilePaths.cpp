/**
 * @file FilePaths.cpp
 * @author Ryan Purse
 * @date 16/02/2022
 */


#include "FilePaths.h"


std::string path::resources()
{
    return RESOURCE_PATH;
}

std::string path::shaders()
{
    return SHADER_PATH;
}

std::string path::textures()
{
    return TEXTURE_PATH;
}
