/**
 * @file BlinnPhongGeometryShader.cpp
 * @author Ryan Purse
 * @date 21/03/2022
 */


#include "BlinnPhongGeometryShader.h"

BlinnPhongGeometryShader::BlinnPhongGeometryShader(std::shared_ptr<MainCamera> camera)
    : mCamera(std::move(camera))
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
        mShader.set("u_colour", material.diffuseColour);
        mShader.set("u_texture", 0);
        
        glDrawElements(GL_TRIANGLES, renderCoreElements.eboCount, GL_UNSIGNED_INT, 0);
    });
}

void BlinnPhongGeometryShader::onUpdate()
{
    mShader.bind();
}
