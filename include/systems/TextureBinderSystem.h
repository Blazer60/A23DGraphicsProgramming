/**
 * @file TextureBinderSystem.h
 * @author Ryan Purse
 * @date 16/02/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "TextureComponents.h"

/**
 * Binds a texture path to a texture that can be used by OpenGl.
 * @author Ryan Purse
 * @date 16/02/2022
 */
class TextureBinderSystem
        : public ecs::BaseSystem<Texture, TexturePath>
{
public:
    TextureBinderSystem();
    
    void onUpdate() override;
};


