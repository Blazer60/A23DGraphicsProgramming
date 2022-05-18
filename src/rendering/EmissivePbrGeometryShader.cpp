/**
 * @file EmissivePbrGeometryShader.cpp
 * @author Ryan Purse
 * @date 18/05/2022
 */


#include "EmissivePbrGeometryShader.h"

EmissivePbrGeometryShader::EmissivePbrGeometryShader(
    std::shared_ptr<MainCamera> camera,
    std::shared_ptr<FramebufferObject> output)
    :
    mCamera(std::move(camera)), mOutput(std::move(output))
{
    mEntities.forEach([this](
        const RenderInformation &renderCoreElements,
        const std::shared_ptr<ModelMatrix> &uniforms,
        const EmissivePbrMaterial &material)
    {
        mShader.set("u_mvp", mCamera->getVpMatrix() * uniforms->value);
        mShader.set("u_model_matrix", uniforms->value);
        mShader.set("u_colour", material.diffuseColour);
    
        mShader.set("u_diffuse_texture", material.diffuseTextureId, 0);
        mShader.set("u_emissive_texture", material.emissiveTextureId, 1);
    
        glBindVertexArray(renderCoreElements.vao);
        glDrawElements(GL_TRIANGLES, renderCoreElements.eboCount, GL_UNSIGNED_INT, 0);
    });
    scheduleFor(ecs::Render);
}

void EmissivePbrGeometryShader::onUpdate()
{
    mShader.bind();
    mOutput->bind();
}
