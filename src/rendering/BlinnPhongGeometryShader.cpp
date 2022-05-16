/**
 * @file BlinnPhongGeometryShader.cpp
 * @author Ryan Purse
 * @date 21/03/2022
 */


#include "BlinnPhongGeometryShader.h"

BlinnPhongGeometryShader::BlinnPhongGeometryShader(
    std::shared_ptr<MainCamera> camera, std::shared_ptr<FramebufferObject> framebuffer)
    :
    mCamera(std::move(camera)), mFrameBufferObject(std::move(framebuffer))
{
    mEntities.forEach([this](
        const RenderInformation &renderCoreElements,
        const std::shared_ptr<ModelMatrix> &uniforms,
        const BlinnPhongMaterial &material) {
        glBindVertexArray(renderCoreElements.vao);
        glBindTextureUnit(0, material.diffuseTextureId);
        
        mShader.set("u_mvp", mCamera->getVpMatrix() * uniforms->value);
        mShader.set("u_model_matrix", uniforms->value);
        mShader.set("u_colour", material.diffuseColour);
        mShader.set("u_texture", 0);
        
        glDrawElements(GL_TRIANGLES, renderCoreElements.eboCount, GL_UNSIGNED_INT, 0);
    });
    scheduleFor(ecs::Render);
}

void BlinnPhongGeometryShader::onUpdate()
{
    mShader.bind();
    mFrameBufferObject->bind();
}
