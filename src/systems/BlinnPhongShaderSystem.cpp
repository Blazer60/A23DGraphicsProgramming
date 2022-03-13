/**
 * @file BlinnPhongShaderSystem.cpp
 * @author Ryan Purse
 * @date 07/03/2022
 */


#include "BlinnPhongShaderSystem.h"

BlinnPhongShaderSystem::BlinnPhongShaderSystem(
        std::shared_ptr<MainCamera> camera, std::shared_ptr<DirectionalLight> directionalLight
        )
    : mCamera(std::move(camera)), mDirectionalLight(std::move(directionalLight))
{
    mEntities.forEach([this](
            const RenderInformation &renderCoreElements,
            const std::shared_ptr<BasicUniforms> &uniforms,
            const BlinnPhongMaterial &material) {
        glBindFramebuffer(GL_FRAMEBUFFER, renderCoreElements.fbo);
        glBindVertexArray(renderCoreElements.vao);
        // glBindTextureUnit(0, texture.id);
    
        mShader.set("u_mvp", mCamera->getVpMatrix() * uniforms->modelMat);
        mShader.set("u_modelMat", uniforms->modelMat);
        mShader.set("u_viewMat", mCamera->getViewMatrix());
        mShader.set("u_cameraPosWS", mCamera->getPosition());
        mShader.set("u_colour", material.diffuseColour);
        mShader.set("u_texture", 0);
        mShader.set("u_lightDir", glm::normalize(mDirectionalLight->getDirection()));
        mShader.set("u_lightColour", mDirectionalLight->getIntensity());
    
        glDrawElements(GL_TRIANGLES, renderCoreElements.eboCount, GL_UNSIGNED_INT, 0);
    });
}

void BlinnPhongShaderSystem::onStart()
{
    mShader.bind();
}

void BlinnPhongShaderSystem::onUpdate()
{
    mShader.bind();
}
