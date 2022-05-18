/**
 * @file MipViewerShader.h
 * @author Ryan Purse
 * @date 15/05/2022
 */


#pragma once

#include "Pch.h"
#include "PostProcessShader.h"
#include "FramebufferObject.h"
#include "TextureBufferObject.h"

/**
 * @author Ryan Purse
 * @date 15/05/2022
 */
class MipViewerShader
    : public PostProcessShader
{
public:
    MipViewerShader(std::shared_ptr<TextureBufferObject> inputTexture, std::string_view debugName);
    
    void clear();
    void imguiUpdate();
protected:
    void init();
    void onRender() override;
    glm::ivec2 mOriginalSize;
    glm::ivec2 mSize { 960, 540 };
    int mLevel { 0 };
    std::string mDebugName { "" };
    
    std::unique_ptr<FramebufferObject> mFramebufferObject;
    
    std::shared_ptr<TextureBufferObject> mOutputTexture;
    
    bool mShowOutput { true };
    bool mFill { false };
    bool mResize { false };
    
    std::shared_ptr<TextureBufferObject> mInputTexture;
};
