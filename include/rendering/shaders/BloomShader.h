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


/**
 * @author Ryan Purse
 * @date 15/05/2022
 */
class BloomShader :
    public Shader
{
public:
    BloomShader();
    ~BloomShader();
    
    void downSample(TextureBufferObject *input, const int mipLevel, FramebufferObject *output);
    
protected:
    unsigned int mVao { 0 };
    unsigned int mEbo { 0 };
    unsigned int mVbo { 0 };
    int mEboCount { 0 };
};
