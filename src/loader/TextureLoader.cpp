/**
 * @file TextureLoader.cpp
 * @author Ryan Purse
 * @date 13/03/2022
 */


#include <filesystem>
#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace load
{
    unsigned int texture(std::string_view path)
    {
        if (path == "")
            return 0;
        std::filesystem::path systemPath(path);
        if (!std::filesystem::exists(systemPath))
        {
            debug::log("File " + systemPath.string() + " does not exist.\nAborting texture generation",
                       debug::severity::Warning);
            return 0;
        }
        
        unsigned int textureId;
        glCreateTextures(GL_TEXTURE_2D, 1, &textureId);
    
        int width;
        int height;
        int colourChannels;
        unsigned char *bytes = stbi_load(systemPath.string().c_str(), &width, &height, &colourChannels, 0);
    
        glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
        const unsigned int levels = 1;
        const int lod = 0;
        const int xOffSet = 0;
        const int yOffSet = 0;
    
        glTextureStorage2D(textureId, levels, GL_RGBA8, width, height);
    
        if (colourChannels == 3)
            glTextureSubImage2D(textureId, lod, xOffSet, yOffSet, width, height, GL_RGB, GL_UNSIGNED_BYTE, bytes);
        else if (colourChannels == 4)
            glTextureSubImage2D(textureId, lod, xOffSet, yOffSet, width, height, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
        else
        {
            debug::log("File " + systemPath.string()
                       + " does not contain the correct amount of channels. Cannot generate textures",
                       debug::severity::Warning);
            stbi_image_free(bytes);
            return 0;
        }
    
        glGenerateTextureMipmap(textureId);
    
        stbi_image_free(bytes);
        return textureId;
    }
}


