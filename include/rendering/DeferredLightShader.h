/**
 * @file DeferredLightShader.h
 * @author Ryan Purse
 * @date 24/03/2022
 */


#pragma once


#include "Shader.h"
#include "FilePaths.h"
#include "TextureBufferObject.h"
#include "MainCamera.h"

/**
 * Gets both the diffuse and specular and creates the final image before post-processing.
 * @author Ryan Purse
 * @date 24/03/2022
 */
class DeferredLightShader
{
public:
    DeferredLightShader(std::shared_ptr<MainCamera> camera, std::shared_ptr<FrameBufferObject> output,
                        std::shared_ptr<TextureBufferObject> diffuse, std::shared_ptr<TextureBufferObject> specular,
                        std::shared_ptr<TextureBufferObject> albedo);
    
    void render();
    
protected:
    Shader mShader { path::resources() + "shaders/lighting/Lighting.vert", path::resources() + "shaders/lighting/Output.frag" };
    unsigned int mVao { 0 };
    unsigned int mEbo { 0 };
    unsigned int mVbo { 0 };
    int          mEboCount { 0 };
    
    std::shared_ptr<MainCamera>          mCamera;
    std::shared_ptr<FrameBufferObject>   mOutput;
    std::shared_ptr<TextureBufferObject> mDiffuse;
    std::shared_ptr<TextureBufferObject> mSpecular;
    std::shared_ptr<TextureBufferObject> mAlbedo;
};

