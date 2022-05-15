/**
 * @file MipViewerShader.cpp
 * @author Ryan Purse
 * @date 15/05/2022
 */


#include "MipViewerShader.h"
#include "FilePaths.h"
#include "imgui.h"

MipViewerShader::MipViewerShader(std::shared_ptr<TextureBufferObject> inputTexture)
    : PostProcessShader(path::shaders()  + "ScreenOverlay.vert", path::shaders() + "post-processing/MipView.frag"),
    mInputTexture(std::move(inputTexture))
{
    mOriginalSize = mInputTexture->getSize();
    init();
}

void MipViewerShader::init()
{
    mSize = mOriginalSize / static_cast<int>(glm::pow(2.f, mLevel));
    mFramebufferObject = std::make_unique<FramebufferObject>(mSize, GL_ONE, GL_ONE, GL_ALWAYS);
    mOutputTexture = std::make_shared<TextureBufferObject>(mSize, GL_RGB16, GL_LINEAR, 1, "Mip Viewer");
    mFramebufferObject->attach(mOutputTexture, 0);
}

void MipViewerShader::onRender()
{
    mFramebufferObject->bind();
    set("u_mip_level", mLevel);
    set("u_mip_size", glm::vec2(mSize));
    
    glBindTextureUnit(2, mInputTexture->getName());
    set("u_texture", 2);
    
    set("u_mvp_matrix", glm::mat4(1.f));
}

void MipViewerShader::clear()
{
    mFramebufferObject->clear();
}

void MipViewerShader::imguiUpdate()
{
    if (mShowOutput)
        mOutputTexture->imguiUpdate(&mShowOutput, mFill);
    
    ImGui::Begin("Mip Settings");
    auto temp = mLevel;
    ImGui::SliderInt("Mip Level", &mLevel, 0, 7);
    if (temp != mLevel)
        init();
    ImGui::Checkbox("Fill?", &mFill);
    
    ImGui::End();
}