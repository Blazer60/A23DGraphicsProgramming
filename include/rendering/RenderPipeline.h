/**
 * @file RenderPipeline.h
 * @author Ryan Purse
 * @date 24/03/2022
 */


#pragma once

#include "FrameBufferObject.h"
#include "TextureBufferObject.h"
#include "glm.hpp"

/**
 * [Description Goes Here]
 * @author Ryan Purse
 * @date 24/03/2022
 */
class RenderPipeline
{
public:
    RenderPipeline();
    
    void imguiUpdate();
    
    glm::ivec2 mSize { 1920, 1080 };
    
    std::shared_ptr<FrameBufferObject> mGeometry;

    std::shared_ptr<TextureBufferObject> mPosition;
    std::shared_ptr<TextureBufferObject> mNormal;
    std::shared_ptr<TextureBufferObject> mAlbedo;

    std::shared_ptr<FrameBufferObject> mLightAccumulator;
    
    std::shared_ptr<TextureBufferObject> mDiffuse;
    std::shared_ptr<TextureBufferObject> mSpecular;
    
    std::shared_ptr<FrameBufferObject> mOutput;
    
    std::shared_ptr<TextureBufferObject> mTarget;
};


