/**
 * @file RenderPipeline.cpp
 * @author Ryan Purse
 * @date 24/03/2022
 */


#include "RenderPipeline.h"

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
    mPosition->imguiUpdate();
    mNormal->imguiUpdate();
    mAlbedo->imguiUpdate();
    mDiffuse->imguiUpdate();
    mSpecular->imguiUpdate();
    mTarget->imguiUpdate();
}
