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
        glBindTextureUnit(0, material.diffuseTextureId);
    
        mShader.set("u_mvp", mCamera->getVpMatrix() * uniforms->modelMat);
        mShader.set("u_model_matrix", uniforms->modelMat);
        mShader.set("u_view_matrix", mCamera->getViewMatrix());
        mShader.set("u_camera_position_ws", mCamera->getPosition());
        mShader.set("u_colour", material.diffuseColour);
        mShader.set("u_texture", 0);
        mShader.set("u_light_direction", glm::normalize(mDirectionalLight->getDirection()));
        mShader.set("u_light_colour", mDirectionalLight->getIntensity());
    
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
