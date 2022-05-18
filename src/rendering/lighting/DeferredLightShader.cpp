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
    std::shared_ptr<TextureBufferObject> specular, std::shared_ptr<TextureBufferObject> albedo,
    std::shared_ptr<TextureBufferObject> emissive)
    :
    mCamera(std::move(camera)),
    mOutput(std::move(output)), mDiffuse(std::move(diffuse)),
    mSpecular(std::move(specular)), mAlbedo(std::move(albedo)),
    mEmissive(std::move(emissive))
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
    
    mShader.set("u_diffuse",  mDiffuse->getName(),  0);
    mShader.set("u_specular", mSpecular->getName(), 1);
    mShader.set("u_albedo",   mAlbedo->getName(),   2);
    mShader.set("u_emissive", mEmissive->getName(), 3);
    
    mShader.set("u_mvp_matrix", glm::mat4(1.f));
    
    glBindVertexArray(mVao);
    glDrawElements(GL_TRIANGLES, mEboCount, GL_UNSIGNED_INT, 0);
}
