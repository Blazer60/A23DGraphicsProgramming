/**
 * @file Renderer.cpp
 * @author Ryan Purse
 * @date 30/03/2022
 */


#include "Renderer.h"
#include "BlinnPhongGeometryShader.h"
#include "DirectionalLightShaderSystem.h"
#include "PointLightShader.h"
#include "BoundingVolumeVisual.h"


Renderer::Renderer(std::shared_ptr<MainCamera> camera, ecs::Core &EntityComponentSystem) :
    mCamera(std::move(camera)),
    mEcs(EntityComponentSystem),
    mDeferredLightingShader(
        camera, mRenderPipeline.mOutput,
        mRenderPipeline.mDiffuse, mRenderPipeline.mSpecular,
        mRenderPipeline.mAlbedo),
    mPreFilterShader(mRenderPipeline.mDownSampleBuffers[0], mRenderPipeline.mLightTarget),
    mDownSamplingMipViewerShader(mRenderPipeline.mDownSampleTexture, "Down Sampling"),
    mUpSamplingMipViewerShader(mRenderPipeline.mUpSampleTexture, "Up Sampling")
{
    mEcs.createSystem<BlinnPhongGeometryShader> ({ geometryTag }, mCamera, mRenderPipeline.mGeometry);
    mEcs.createSystem<DirectionalLightShaderSystem>(
        mCamera, mRenderPipeline.mLightAccumulator,
        mRenderPipeline.mPosition, mRenderPipeline.mNormal,
        mRenderPipeline.mAlbedo);
    mEcs.createSystem<PointLightShader>(
        mCamera, mRenderPipeline.mLightAccumulator,
        mRenderPipeline.mPosition, mRenderPipeline.mNormal,
        mRenderPipeline.mAlbedo);
    
    geometryFboName = mRenderPipeline.mGeometry->getFboName();
    // mEcs.createSystem<BoundingVolumeVisual>(mCamera, geometryFboName);
}

void Renderer::clear()
{
    mRenderPipeline.mGeometry->clear();
    mRenderPipeline.mLightAccumulator->clear();
    mRenderPipeline.mOutput->clear();
    for (const auto &item : mRenderPipeline.mDownSampleBuffers)
        item->clear();
    
    for (const auto &item : mRenderPipeline.mUpSampleBuffers)
        item->clear();
    
    mRenderPipeline.mComposite->clear();
    
    mDownSamplingMipViewerShader.clear();
    mUpSamplingMipViewerShader.clear();
    
    mRenderPipeline.mGeometry->bind();  // For safety as this is typically the first buffer drawn to.
}

void Renderer::update()
{
    mDeferredLightingShader.render();
    mPreFilterShader.render();
    auto *downSampleTexture = mRenderPipeline.mDownSampleTexture.get();
    for (int i = 1; i < mRenderPipeline.mDownSampleBuffers.size(); ++i)
    {
        auto *output = mRenderPipeline.mDownSampleBuffers[i].get();
        mBloomShader.downSample(downSampleTexture, i - 1, output);
    }
    
    auto *upSampleTexture = mRenderPipeline.mUpSampleTexture.get();
    
    const auto count = mRenderPipeline.mUpSampleBuffers.size();
    mBloomShader.upSample(
        downSampleTexture, count - 1, downSampleTexture, count - 2, mRenderPipeline.mUpSampleBuffers[count - 1].get());
    
    for (int i = count - 2; i > 0; --i)
    {
        auto *output = mRenderPipeline.mUpSampleBuffers[i].get();
        mBloomShader.upSample(upSampleTexture, i + 1, downSampleTexture, i - 1, output);
    }
    
    auto *original = mRenderPipeline.mLightTarget.get();
    mBloomShader.composite(original, upSampleTexture, mRenderPipeline.mComposite.get());
    
    mDownSamplingMipViewerShader.render();
    mUpSamplingMipViewerShader.render();
}

void Renderer::imguiUpdate()
{
    mRenderPipeline.imguiUpdate();
    mDownSamplingMipViewerShader.imguiUpdate();
    mUpSamplingMipViewerShader.imguiUpdate();
    mBloomShader.imGuiUpdate();
}

void Renderer::drawBox(const glm::mat4 &modelMatrix, const glm::vec3 &halfSize)
{
    mBoxShader.bind();
    glBindFramebuffer(GL_FRAMEBUFFER, geometryFboName);
    glBindVertexArray(mBox.renderInformation.vao);
    
    mBoxShader.set("u_mvp", mCamera->getVpMatrix() * modelMatrix);
    mBoxShader.set("u_model_matrix", modelMatrix);
    mBoxShader.set("u_halfsize", halfSize);
    mBoxShader.set("u_camera_position_ws", mCamera->getPosition());
    
    glDrawElements(GL_LINES, mBox.renderInformation.eboCount, GL_UNSIGNED_INT, 0);
}

void Renderer::imguiMenuUpdate()
{
    mRenderPipeline.imguiMenuUpdate();
}
