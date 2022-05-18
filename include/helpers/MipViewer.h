/**
 * @file MipViewerShader.h
 * @author Ryan Purse
 * @date 15/05/2022
 */


#pragma once

#include "Pch.h"
#include "FramebufferObject.h"
#include "TextureBufferObject.h"
#include "Mesh.h"
#include "MaterialComponents.h"
#include "Primitives.h"
#include "FilePaths.h"
#include "Shader.h"
#include "TextureViewer.h"

/**
 * @author Ryan Purse
 * @date 15/05/2022
 */
class MipViewer
{
public:
    MipViewer(std::shared_ptr<TextureBufferObject> inputTexture, std::string_view debugName);
    ~MipViewer();
    
    void clear();
    void render();
    void imguiUpdate(bool *show);
protected:
    void init();
    
    glm::ivec2 mOriginalSize;
    glm::ivec2 mSize { 960, 540 };
    int mLevel { 0 };
    std::string mDebugName { "" };
    
    std::shared_ptr<TextureBufferObject> mInputTexture;
    std::shared_ptr<TextureBufferObject> mOutputTexture;
    std::unique_ptr<FramebufferObject>   mFramebufferObject;
    
    Shader mShader { path::shaders()  + "ScreenOverlay.vert", path::shaders() + "post-processing/MipView.frag" };
    
    bool            mFill           { false };
    bool            mResize         { false };
    unsigned int    mVao            { 0 };
    int             mEboCount       { 0 };
    unsigned int    mEbo            { 0 };
    unsigned int    mVbo            { 0 };
};
