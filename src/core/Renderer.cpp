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


Renderer::Renderer(std::shared_ptr<MainCamera> camera, ecs::Core &EntityComponentSystem) :
    mCamera(std::move(camera)),
    mEcs(EntityComponentSystem),
    mDeferredLightingShader(
        camera, mRenderPipeline.mOutput,
        mRenderPipeline.mDiffuse, mRenderPipeline.mSpecular,
        mRenderPipeline.mAlbedo)
{
    mEcs.createSystem<BlinnPhongGeometryShader> ({ geometryTag },   mCamera);
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
    mRenderPipeline.imguiUpdate();
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
