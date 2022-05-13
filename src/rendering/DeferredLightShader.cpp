/**
 * @file DeferredLightShader.cpp
 * @author Ryan Purse
 * @date 24/03/2022
 */


#include "DeferredLightShader.h"
#include "Vertices.h"
#include "Primitives.h"
#include "FramebufferObject.h"

DeferredLightShader::DeferredLightShader(
    std::shared_ptr<MainCamera> camera,
    std::shared_ptr<FramebufferObject> output, std::shared_ptr<TextureBufferObject> diffuse,
    std::shared_ptr<TextureBufferObject> specular, std::shared_ptr<TextureBufferObject> albedo)
    :
    mCamera(std::move(camera)),
    mOutput(std::move(output)), mDiffuse(std::move(diffuse)),
    mSpecular(std::move(specular)), mAlbedo(std::move(albedo))
{
    const auto mesh = primitives::plane<UvVertex>()[0];
    mVao      = mesh.renderInformation.vao;
    mEboCount = mesh.renderInformation.eboCount;
    mEbo      = mesh.ebo;
    mVbo      = mesh.vbo;
}

void DeferredLightShader::render()
{
    mShader.bind();
    mOutput->bind();
    glBindVertexArray(mVao);
    
    glBindTextureUnit(0, mDiffuse->getName());
    glBindTextureUnit(1, mSpecular->getName());
    glBindTextureUnit(2, mAlbedo->getName());
    
    mShader.set("u_diffuse",  0);
    mShader.set("u_specular", 1);
    mShader.set("u_albedo",   2);
    
    mShader.set("u_model_matrix", glm::mat4(1.f));
    // mSphereShader.set("u_camera_position_ws", mCamera->getPosition());
    
    glDrawElements(GL_TRIANGLES, mEboCount, GL_UNSIGNED_INT, 0);
}
