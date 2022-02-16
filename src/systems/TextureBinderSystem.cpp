/**
 * @file TextureBinderSystem.cpp
 * @author Ryan Purse
 * @date 16/02/2022
 */


#include "TextureBinderSystem.h"
#include "stb_image.h"

TextureBinderSystem::TextureBinderSystem()
{
    stbi_set_flip_vertically_on_load(true);
}

void TextureBinderSystem::onStart()
{
    mEntities.forEach([](Texture &texture, TexturePath &texturePath) {
        glCreateTextures(GL_TEXTURE_2D, 1, &texture.id);
    
        int width;
        int height;
        int colourChannels;
        if (!std::filesystem::exists(texturePath.path))
        {
            debug::log("File " + texturePath.path.string() + " does not exist.\nAborting texture generation",
                       debug::severity::Warning);
            return;
        }
        
        unsigned char *bytes = stbi_load(texturePath.path.string().c_str(), &width, &height, &colourChannels, 0);
        
        
        glTextureParameteri(texture.id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(texture.id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(texture.id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(texture.id, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        const unsigned int levels = 1;
        const int lod = 0;
        const int xOffSet = 0;
        const int yOffSet = 0;
    
        glTextureStorage2D(texture.id, levels, GL_RGBA8, width, height);
    
        if (colourChannels == 3)
            glTextureSubImage2D(texture.id, lod, xOffSet, yOffSet, width, height, GL_RGB, GL_UNSIGNED_BYTE, bytes);
        else if (colourChannels == 4)
            glTextureSubImage2D(texture.id, lod, xOffSet, yOffSet, width, height, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
        else
        {
            debug::log("File " + texturePath.path.string()
                + " does not contain the correct amount of channels. Cannot generate textures",
                debug::severity::Warning);
            stbi_image_free(bytes);
            return;
        }
        
        glGenerateTextureMipmap(texture.id);
    
        stbi_image_free(bytes);
    });
}

void TextureBinderSystem::onUpdate()
{
    mEntities.forEach([](Texture &texture, TexturePath &texturePath) {
    
    });
}
