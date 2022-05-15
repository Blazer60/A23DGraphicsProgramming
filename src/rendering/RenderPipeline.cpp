/**
 * @file RenderPipeline.cpp
 * @author Ryan Purse
 * @date 24/03/2022
 */


#include "RenderPipeline.h"
#include "imgui.h"

RenderPipeline::RenderPipeline() :
    mGeometry(std::make_shared<FramebufferObject>(mSize, GL_ONE, GL_ZERO, GL_LESS)),
    mLightAccumulator(std::make_shared<FramebufferObject>(mSize, GL_ONE, GL_ONE, GL_ALWAYS)),
    mOutput(std::make_shared<FramebufferObject>(mSize, GL_ONE, GL_ONE, GL_ALWAYS)),
    mDownSample(std::make_shared<FramebufferObject>(mSize / 2, GL_ONE, GL_ONE, GL_ALWAYS)),
    mUpSample(std::make_shared<FramebufferObject>(mSize / 2, GL_ONE, GL_ONE, GL_ALWAYS)),
    mComposite(std::make_shared<FramebufferObject>(mSize, GL_ONE, GL_ONE, GL_ALWAYS)),
    
    mPosition(  std::make_shared<TextureBufferObject>(mSize, GL_RGB16F,      GL_NEAREST, "Position")),
    mNormal(    std::make_shared<TextureBufferObject>(mSize, GL_RGB16_SNORM, GL_NEAREST, "Normals")),
    mAlbedo(    std::make_shared<TextureBufferObject>(mSize, GL_RGB16F,      GL_NEAREST, "Albedo")),
    mDiffuse(   std::make_shared<TextureBufferObject>(mSize, GL_RGB16F,      GL_NEAREST, "Diffuse")),
    mSpecular(  std::make_shared<TextureBufferObject>(mSize, GL_RGB16F,      GL_NEAREST, "Specular")),
    mLightTarget(std::make_shared<TextureBufferObject>(mSize, GL_RGB16F, GL_NEAREST, "Light Target")),
    
    mDownSampleTextures(    std::make_shared<MipmapTexture>(        mSize / 2,  GL_RGB16F, "Down Sample")),
    mUpSampleTextures(      std::make_shared<MipmapTexture>(        mSize / 2,  GL_RGB16F, "Up Sample")),
    mPostProcess(           std::make_shared<TextureBufferObject>(  mSize,      GL_RGB16F, GL_NEAREST, "Post Process"))
{
    mGeometry->attach(mPosition, 0);
    mGeometry->attach(mNormal, 1);
    mGeometry->attach(mAlbedo, 2);
    
    mLightAccumulator->attach(mDiffuse, 0);
    mLightAccumulator->attach(mSpecular, 1);
    
    mOutput->attach(mLightTarget, 0);
    
    mDownSample->attach(mDownSampleTextures, 0);
    
    mUpSample->attach(mUpSampleTextures, 0);
    
    mComposite->attach(mPostProcess, 0);
}

void RenderPipeline::imguiUpdate()
{
    if (mShowPositionBuffer)
        mPosition->imguiUpdate(&mShowPositionBuffer, true);
    if (mShowNormalBuffer)
        mNormal->imguiUpdate(&mShowNormalBuffer, true);
    if (mShowAlbedoBuffer)
        mAlbedo->imguiUpdate(&mShowAlbedoBuffer, true);
    if (mShowDiffuseBuffer)
        mDiffuse->imguiUpdate(&mShowDiffuseBuffer, true);
    if (mShowSpecularBuffer)
        mSpecular->imguiUpdate(&mShowSpecularBuffer, true);
    if (mShowLightTargetBuffer)
        mLightTarget->imguiUpdate(&mShowLightTargetBuffer, true);
    if (mShowDownSampleBuffers)
        mDownSampleTextures->imguiUpdate(&mShowDownSampleBuffers, true);
    if (mShowUpSampleBuffers)
        mUpSampleTextures->imguiUpdate(&mShowUpSampleBuffers, true);
    if (mShowPostProcessBuffer)
        mPostProcess->imguiUpdate(&mShowPostProcessBuffer, true);
}

void RenderPipeline::imguiMenuUpdate()
{
    if (ImGui::BeginMenu("Viewports"))
    {
        if (ImGui::MenuItem("Show Position Buffer"))
            mShowPositionBuffer = true;
        if (ImGui::MenuItem("Show Normal Buffer"))
            mShowNormalBuffer = true;
        if (ImGui::MenuItem("Show Albedo Buffer"))
            mShowAlbedoBuffer = true;
        if (ImGui::MenuItem("Show Diffuse Buffer"))
            mShowDiffuseBuffer = true;
        if (ImGui::MenuItem("Show Specular Buffer"))
            mShowSpecularBuffer = true;
        if (ImGui::MenuItem("Show Light Target Buffer"))
            mShowLightTargetBuffer = true;
        if (ImGui::MenuItem("Show Down Sample Buffer"))
            mShowDownSampleBuffers = true;
        if (ImGui::MenuItem("Show Up Sample Buffer"))
            mShowUpSampleBuffers = true;
        if (ImGui::MenuItem("Show Post Process Buffer"))
            mShowPostProcessBuffer = true;
        ImGui::EndMenu();
    }
}
