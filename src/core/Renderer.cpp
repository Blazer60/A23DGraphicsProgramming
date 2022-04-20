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
#include "BoundingVolumeVisual.h"


Renderer::Renderer(std::weak_ptr<MainCamera> camera, ecs::Core &EntityComponentSystem) :
    mCamera(std::move(camera)),
    mEcs(EntityComponentSystem),
    mMainTexture(std::make_shared<TextureBufferObject>(mScreenSize)),
    mDeferredLightingShader(
        camera.lock(), mRenderPipeline.mOutput,
        mRenderPipeline.mDiffuse, mRenderPipeline.mSpecular,
        mRenderPipeline.mAlbedo),
    mDirectionalLight(std::make_shared<DirectionalLight>())
{
    mEcs.createSystem<BlinnPhongGeometryShader> ({ geometryTag },   mCamera.lock(), mDirectionalLight);
    mEcs.createSystem<DirectionalLightShaderSystem>(
        mCamera.lock(), mRenderPipeline.mLightAccumulator,
        mRenderPipeline.mPosition, mRenderPipeline.mNormal,
        mRenderPipeline.mAlbedo);
    mEcs.createSystem<PointLightShader>(
        mCamera.lock(), mRenderPipeline.mLightAccumulator,
        mRenderPipeline.mPosition, mRenderPipeline.mNormal,
        mRenderPipeline.mAlbedo);
    
    geometryFboName = mRenderPipeline.mGeometry->getFboName();
    mEcs.createSystem<BoundingVolumeVisual>(mCamera.lock(), geometryFboName);
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
