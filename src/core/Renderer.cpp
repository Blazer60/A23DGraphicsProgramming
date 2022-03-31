/**
 * @file Renderer.cpp
 * @author Ryan Purse
 * @date 30/03/2022
 */


#include "Renderer.h"
#include "BasicShaderSystem.h"
#include "BlinnPhongGeometryShader.h"
#include "DirectionalLightShaderSystem.h"
#include "PointLightShader.h"


Renderer::Renderer(std::weak_ptr<MainCamera> camera) :
    mCamera(std::move(camera)),
    mMainTexture(std::make_shared<TextureBufferObject>(mScreenSize)),
    mDeferredLightingShader(
        camera.lock(), mRenderPipeline.mOutput,
        mRenderPipeline.mDiffuse, mRenderPipeline.mSpecular,
        mRenderPipeline.mAlbedo),
    mDirectionalLight(std::make_shared<DirectionalLight>())
{
    ecs::createSystem<BlinnPhongGeometryShader> ({ geometryTag },   mCamera.lock(), mDirectionalLight);
    ecs::createSystem<DirectionalLightShaderSystem>(
        mCamera.lock(), mRenderPipeline.mLightAccumulator,
        mRenderPipeline.mPosition, mRenderPipeline.mNormal,
        mRenderPipeline.mAlbedo);
    ecs::createSystem<PointLightShader>(
        mCamera.lock(), mRenderPipeline.mLightAccumulator,
        mRenderPipeline.mPosition, mRenderPipeline.mNormal,
        mRenderPipeline.mAlbedo);
    
    geometryFboName = mRenderPipeline.mGeometry->getFboName();
}

void Renderer::clear()
{
    // todo: Blending doesn't work?
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // todo: Make the geometry shader take in the geometry buffer.
    mRenderPipeline.mGeometry->clear();
    mRenderPipeline.mLightAccumulator->clear();
    mRenderPipeline.mOutput->clear();
}

void Renderer::update()
{
    mDeferredLightingShader.render();
}

void Renderer::imguiUpdate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(0);
    
    mDirectionalLight->onImguiUpdate();
    mMainTexture->imguiUpdate();
    mRenderPipeline.imguiUpdate();
}
