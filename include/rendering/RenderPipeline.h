/**
 * @file RenderPipeline.h
 * @author Ryan Purse
 * @date 24/03/2022
 */


#pragma once

#include "Pch.h"
#include "FramebufferObject.h"
#include "TextureBufferObject.h"
#include "glm.hpp"
#include "MipmapTexture.h"
#include "WindowHelpers.h"

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
    
    void imguiMenuUpdate();
    
    glm::ivec2 mSize { window::bufferSize() };
    const int mMipmapLevels { 8 };  // Must be before frame and texture buffer creation.
    
    std::shared_ptr<FramebufferObject> mGeometry;
    std::shared_ptr<TextureBufferObject> mPosition;
    std::shared_ptr<TextureBufferObject> mNormal;
    std::shared_ptr<TextureBufferObject> mAlbedo;

    std::shared_ptr<FramebufferObject> mLightAccumulator;
    std::shared_ptr<TextureBufferObject> mDiffuse;
    std::shared_ptr<TextureBufferObject> mSpecular;
    
    std::shared_ptr<FramebufferObject> mOutput;
    std::shared_ptr<TextureBufferObject> mLightTarget;
    
    std::vector<std::shared_ptr<FramebufferObject>> mDownSampleBuffers;
    std::shared_ptr<MipmapTexture> mDownSampleTexture;
    
    std::vector<std::shared_ptr<FramebufferObject>> mUpSampleBuffers;
    std::shared_ptr<MipmapTexture> mUpSampleTexture;
    
    std::shared_ptr<FramebufferObject> mComposite;
    std::shared_ptr<TextureBufferObject> mPostProcess;
    
    bool mShowPositionBuffer    { false };
    bool mShowNormalBuffer      { false };
    bool mShowAlbedoBuffer      { false };
    bool mShowDiffuseBuffer     { false };
    bool mShowSpecularBuffer    { false };
    bool mShowLightTargetBuffer { true  };
    bool mShowDownSampleBuffers { true  };
    bool mShowUpSampleBuffers   { false };
    bool mShowPostProcessBuffer { false };
};


