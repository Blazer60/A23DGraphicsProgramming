/**
 * @file BloomShader.h
 * @author Ryan Purse
 * @date 15/05/2022
 */


#pragma once

#include "Pch.h"
#include "Shader.h"
#include "FramebufferObject.h"
#include "TextureBufferObject.h"
#include "FilePaths.h"


/**
 * @author Ryan Purse
 * @date 15/05/2022
 */
class Bloom
{
public:
    Bloom();
    ~Bloom();
    
    void preFilter(TextureBufferObject *input, FramebufferObject *output);
    
    void downSample(TextureBufferObject *input, const int mipLevel, FramebufferObject *output);
    
    void upSample(
        TextureBufferObject *lastUpSample, const int upSampleMipLevel, TextureBufferObject *downSample,
        const int downSampleMipLevel, FramebufferObject *output);
    
    void composite(TextureBufferObject *original, TextureBufferObject *bloom, FramebufferObject *output);
    
    void imGuiUpdate();
    
protected:
    Shader mPreFilter  { path::shaders() + "ScreenOverlay.vert", path::shaders() + "post-processing/pre-filter/PreFilter.frag" };
    Shader mDownSample { path::shaders() + "ScreenOverlay.vert", path::shaders() + "post-processing/BloomDownSample.frag" };
    Shader mUpSample   { path::shaders() + "ScreenOverlay.vert", path::shaders() + "post-processing/BloomUpSample.frag"  };
    Shader mComposite  { path::shaders() + "ScreenOverlay.vert", path::shaders() + "post-processing/BloomComposite.frag" };
    
    glm::vec3 mBloomThreshold { 1.f };
    float mBloomScale { 1.f };
    float mExposure { 1.f };
    
    unsigned int mVao { 0 };
    unsigned int mEbo { 0 };
    unsigned int mVbo { 0 };
    int mEboCount { 0 };
};
