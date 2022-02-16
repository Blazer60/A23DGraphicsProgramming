/**
 * @file UvShaderSystem.cpp
 * @author Ryan Purse
 * @date 15/02/2022
 */


#include "UvShaderSystem.h"

#include <utility>


UvShaderSystem::UvShaderSystem(std::shared_ptr<MainCamera> camera)
    : mCamera(std::move(camera))
{
    mEntities.forEach([this](
            const RenderCoreElements &renderCoreElements,
            const std::shared_ptr<BasicUniforms> &uniforms,
            const UvUniforms &uvUniforms,
            const Texture &texture) {
        glBindFramebuffer(GL_FRAMEBUFFER, renderCoreElements.fbo);
        glBindVertexArray(renderCoreElements.vao);
        glBindTextureUnit(0, texture.id);
        
        mShader.set("u_mvp", mCamera->getVpMatrix() * uniforms->modelMat);
        mShader.set("u_colour", uvUniforms.colour);
        mShader.set("u_texture", 0);
        
        glDrawElements(GL_TRIANGLES, renderCoreElements.eboCount, GL_UNSIGNED_INT, 0);
    });
}

void UvShaderSystem::onStart()
{
    mShader.bind();
}

void UvShaderSystem::onUpdate()
{
    mShader.bind();
}
