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
#include "imgui.h"
#include "EmissivePbrGeometryShader.h"


Renderer::Renderer(std::shared_ptr<MainCamera> camera, ecs::Core &EntityComponentSystem) :
    mCamera(std::move(camera)),
    mEcs(EntityComponentSystem),
    mGeometry(std::make_shared<FramebufferObject>(mSize, GL_ONE, GL_ZERO, GL_LESS)),
    mLightAccumulator(std::make_shared<FramebufferObject>(mSize, GL_ONE, GL_ONE, GL_ALWAYS)),
    mOutput(std::make_shared<FramebufferObject>(mSize, GL_ONE, GL_ONE, GL_ALWAYS)),
    mComposite(std::make_shared<FramebufferObject>(mSize, GL_ONE, GL_ONE, GL_ALWAYS)),

    mPosition(  std::make_shared<TextureBufferObject>(mSize, GL_RGB16F,      GL_NEAREST, GL_NEAREST, 1, "Position")),
    mNormal(    std::make_shared<TextureBufferObject>(mSize, GL_RGB16_SNORM, GL_NEAREST, GL_NEAREST, 1, "Normals")),
    mAlbedo(    std::make_shared<TextureBufferObject>(mSize, GL_RGB16F,      GL_NEAREST, GL_NEAREST, 1, "Albedo")),
    mEmissive(    std::make_shared<TextureBufferObject>(mSize, GL_RGB16F,      GL_NEAREST, GL_NEAREST, 1, "Emissive")),
    mDiffuse(   std::make_shared<TextureBufferObject>(mSize, GL_RGB16F,      GL_NEAREST, GL_NEAREST, 1, "Diffuse")),
    mSpecular(  std::make_shared<TextureBufferObject>(mSize, GL_RGB16F,      GL_NEAREST, GL_NEAREST, 1, "Specular")),
    mLightTarget(std::make_shared<TextureBufferObject>(mSize, GL_RGB16F,     GL_NEAREST, GL_NEAREST, 1, "Light Target")),

    mDownSampleTexture(std::make_shared<MipmapTexture>(mSize / 2, GL_RGB16F, mMipmapLevels, "Down Sample")),
    mUpSampleTexture(std::make_shared<MipmapTexture>(mSize, GL_RGB16F, mMipmapLevels, "Up Sample")),
    mPostProcess(std::make_shared<TextureBufferObject>(mSize, GL_RGB16, GL_NEAREST, GL_NEAREST, 1, "Post Process")),
    mDeferredLightingShader(camera, mOutput, mDiffuse, mSpecular, mAlbedo, mEmissive),
    mDownSamplingMipViewerShader(mDownSampleTexture, "Down Sampling"),
    mUpSamplingMipViewerShader(mUpSampleTexture, "Up Sampling")
{
    mGeometry->attach(mPosition, 0);
    mGeometry->attach(mNormal, 1);
    mGeometry->attach(mAlbedo, 2);
    mGeometry->attach(mEmissive, 3);
    
    mLightAccumulator->attach(mDiffuse, 0);
    mLightAccumulator->attach(mSpecular, 1);
    
    mOutput->attach(mLightTarget, 0);
    
    for (int i = 0; i < mMipmapLevels; ++i)
    {
        glm::ivec2 size = glm::ivec2((mSize / 2) / static_cast<int>(glm::pow(2, i)));
        auto framebuffer = std::make_shared<FramebufferObject>(size, GL_ONE, GL_ONE, GL_ALWAYS);
        framebuffer->attach(mDownSampleTexture, 0, i);
        mDownSampleBuffers.push_back(framebuffer);
    }
    
    for (int i = 0; i < mMipmapLevels; ++i)
    {
        glm::ivec2 size = glm::ivec2(mSize / static_cast<int>(glm::pow(2, i)));
        auto framebuffer = std::make_shared<FramebufferObject>(size, GL_ONE, GL_ONE, GL_ALWAYS);
        framebuffer->attach(mUpSampleTexture, 0, i);
        mUpSampleBuffers.push_back(framebuffer);
    }
    
    mComposite->attach(mPostProcess, 0);
    
    mEcs.createSystem<BlinnPhongGeometryShader> ({ geometryTag }, mCamera, mGeometry);
    mEcs.createSystem<EmissivePbrGeometryShader> ({ emissiveTag }, mCamera, mGeometry);
    mEcs.createSystem<DirectionalLightShaderSystem>(mCamera, mLightAccumulator, mPosition, mNormal, mAlbedo);
    mEcs.createSystem<PointLightShader>(mCamera, mLightAccumulator, mPosition, mNormal, mAlbedo);
    
    mGeometry->getFboName();
}

void Renderer::clear()
{
    mGeometry->clear();
    mLightAccumulator->clear();
    mOutput->clear();
    for (const auto &item : mDownSampleBuffers)
        item->clear();
    
    for (const auto &item : mUpSampleBuffers)
        item->clear();
    
    mComposite->clear();
    
    mDownSamplingMipViewerShader.clear();
    mUpSamplingMipViewerShader.clear();
    
    mGeometry->bind();  // For safety as this is typically the first buffer drawn to.
}

void Renderer::update()
{
    mDeferredLightingShader.render();
    
    mBloomShader.preFilter(mLightTarget.get(), mDownSampleBuffers[0].get());
    
    auto *downSampleTexture = mDownSampleTexture.get();
    for (int i = 1; i < mDownSampleBuffers.size(); ++i)
    {
        auto *output = mDownSampleBuffers[i].get();
        mBloomShader.downSample(downSampleTexture, i - 1, output);
    }
    
    auto *upSampleTexture = mUpSampleTexture.get();
    
    const auto count = mUpSampleBuffers.size();
    mBloomShader.upSample(
        downSampleTexture, count - 1, downSampleTexture, count - 2, mUpSampleBuffers[count - 1].get());
    
    for (int i = count - 2; i > 0; --i)
    {
        auto *output = mUpSampleBuffers[i].get();
        mBloomShader.upSample(upSampleTexture, i + 1, downSampleTexture, i - 1, output);
    }
    
    auto *original = mLightTarget.get();
    mBloomShader.composite(original, upSampleTexture, mComposite.get());
    
    mDownSamplingMipViewerShader.render();
    mUpSamplingMipViewerShader.render();
}

void Renderer::imguiUpdate()
{
    if (mShow.positionTexture)
        mPosition->imguiUpdate(&mShow.positionTexture, true);
    if (mShow.normalTexture)
        mNormal->imguiUpdate(&mShow.normalTexture, true);
    if (mShow.albedoTexture)
        mAlbedo->imguiUpdate(&mShow.albedoTexture, true);
    if (mShow.emissiveTexture)
        mEmissive->imguiUpdate(&mShow.emissiveTexture, true);
    if (mShow.diffuseTexture)
        mDiffuse->imguiUpdate(&mShow.diffuseTexture, true);
    if (mShow.specularTexture)
        mSpecular->imguiUpdate(&mShow.specularTexture, true);
    if (mShow.lightTargetTexture)
        mLightTarget->imguiUpdate(&mShow.lightTargetTexture, true);
    if (mShow.downSampleTextures)
        mDownSampleTexture->imguiUpdate(&mShow.downSampleTextures, true);
    if (mShow.upSampleTextures)
        mUpSampleTexture->imguiUpdate(&mShow.upSampleTextures, true);
    if (mShow.postProcessTexture)
        mPostProcess->imguiUpdate(&mShow.postProcessTexture, true);
    
    mDownSamplingMipViewerShader.imguiUpdate();
    mUpSamplingMipViewerShader.imguiUpdate();
    mBloomShader.imGuiUpdate();
}

void Renderer::drawBox(const glm::mat4 &modelMatrix, const glm::vec3 &halfSize)
{
    mBoxShader.bind();
    mGeometry->bind();
    glBindVertexArray(mBox.renderInformation.vao);
    
    mBoxShader.set("u_mvp", mCamera->getVpMatrix() * modelMatrix);
    mBoxShader.set("u_model_matrix", modelMatrix);
    mBoxShader.set("u_halfsize", halfSize);
    mBoxShader.set("u_camera_position_ws", mCamera->getPosition());
    
    glDrawElements(GL_LINES, mBox.renderInformation.eboCount, GL_UNSIGNED_INT, 0);
}

void Renderer::imguiMenuUpdate()
{
    
    if (ImGui::BeginMenu("Viewports"))
    {
        if (ImGui::MenuItem("Show Position Buffer"))
            mShow.positionTexture = true;
        if (ImGui::MenuItem("Show Normal Buffer"))
            mShow.normalTexture = true;
        if (ImGui::MenuItem("Show Albedo Buffer"))
            mShow.albedoTexture = true;
        if (ImGui::MenuItem("Show Emissive Buffer"))
            mShow.emissiveTexture = true;
        if (ImGui::MenuItem("Show Diffuse Buffer"))
            mShow.diffuseTexture = true;
        if (ImGui::MenuItem("Show Specular Buffer"))
            mShow.specularTexture = true;
        if (ImGui::MenuItem("Show Light Target Buffer"))
            mShow.lightTargetTexture = true;
        if (ImGui::MenuItem("Show Down Sample Buffer"))
            mShow.downSampleTextures = true;
        if (ImGui::MenuItem("Show Up Sample Buffer"))
            mShow.upSampleTextures = true;
        if (ImGui::MenuItem("Show Post Process Buffer"))
            mShow.postProcessTexture = true;
        ImGui::EndMenu();
    }
}
