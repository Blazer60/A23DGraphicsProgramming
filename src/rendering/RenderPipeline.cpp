/**
 * @file RenderPipeline.cpp
 * @author Ryan Purse
 * @date 24/03/2022
 */


#include "RenderPipeline.h"
#include "imgui.h"

RenderPipeline::RenderPipeline() :
    mGeometry(std::make_shared<FrameBufferObject>(mSize)),
    mLightAccumulator(std::make_shared<FrameBufferObject>(mSize, GL_ONE, GL_ONE)),
    mOutput(std::make_shared<FrameBufferObject>(mSize, GL_ONE, GL_ONE)),
    
    mPosition(  std::make_shared<TextureBufferObject>(mSize, GL_RGB16F,       "Position")),
    mNormal(    std::make_shared<TextureBufferObject>(mSize, GL_RGB16_SNORM,  "Normals")),
    mAlbedo(    std::make_shared<TextureBufferObject>(mSize, GL_RGB16,        "Albedo")),
    mDiffuse(   std::make_shared<TextureBufferObject>(mSize, GL_RGB16,        "Diffuse")),
    mSpecular(  std::make_shared<TextureBufferObject>(mSize, GL_RGB16,        "Specular")),
    mTarget(    std::make_shared<TextureBufferObject>(mSize, GL_RGB8,         "Target"))
{
    mGeometry->attach(mPosition, 0);
    mGeometry->attach(mNormal,   1);
    mGeometry->attach(mAlbedo,   2);
    
    mLightAccumulator->attach(mDiffuse,  0);
    mLightAccumulator->attach(mSpecular, 1);
    
    mOutput->attach(mTarget, 0);
}

void RenderPipeline::imguiUpdate()
{
    if (ImGui::Begin("ViewPorts", nullptr))
    {
        if (ImGui::Button("Show Position Buffer"))
            mShowPositionBuffer = true;
        if (ImGui::Button("Show Normal Buffer"))
            mShowNormalBuffer = true;
        if (ImGui::Button("Show Albedo Buffer"))
            mShowAlbedoBuffer = true;
        if (ImGui::Button("Show Diffuse Buffer"))
            mShowDiffuseBuffer = true;
        if (ImGui::Button("Show Specular Buffer"))
            mShowSpecularBuffer = true;
        if (ImGui::Button("Show Target Buffer"))
            mShowTargetBuffer = true;
    }
    ImGui::End();
    
    if (mShowPositionBuffer)
        mPosition->imguiUpdate(&mShowPositionBuffer);
    if (mShowNormalBuffer)
        mNormal->imguiUpdate(&mShowNormalBuffer);
    if (mShowAlbedoBuffer)
        mAlbedo->imguiUpdate(&mShowAlbedoBuffer);
    if (mShowDiffuseBuffer)
        mDiffuse->imguiUpdate(&mShowDiffuseBuffer);
    if (mShowSpecularBuffer)
        mSpecular->imguiUpdate(&mShowSpecularBuffer);
    if (mShowTargetBuffer)
        mTarget->imguiUpdate(&mShowTargetBuffer);
}
