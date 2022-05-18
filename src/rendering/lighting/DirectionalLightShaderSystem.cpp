/**
 * @file DirectionalLightShaderSystem.cpp
 * @author Ryan Purse
 * @date 23/03/2022
 */


#include "DirectionalLightShaderSystem.h"
#include "Vertices.h"
#include "Primitives.h"

DirectionalLightShaderSystem::DirectionalLightShaderSystem(
    std::shared_ptr<MainCamera> camera,
    std::shared_ptr<FramebufferObject> lightAccumulationBuffer,
    std::shared_ptr<TextureBufferObject> positions,
    std::shared_ptr<TextureBufferObject> normals,
    std::shared_ptr<TextureBufferObject> albedo) :
    mCamera(std::move(camera)),
    mLightAccumulationBuffer(std::move(lightAccumulationBuffer)),
    mPositions(std::move(positions)),
    mNormals(std::move(normals)),
    mAlbedo(std::move(albedo))
{
    const auto mesh = primitives::plane<UvVertex>()[0];
    mVao = mesh.renderInformation.vao;
    mEboCount = mesh.renderInformation.eboCount;
    mEbo = mesh.ebo;
    mVbo = mesh.vbo;
    
    mEntities.forEach([this](const light::DirectionalLight &directionalLight) {
        mShader.set("u_light_direction", directionalLight.direction);
        mShader.set("u_light_intensity", directionalLight.intensity);
    
        glDrawElements(GL_TRIANGLES, mEboCount, GL_UNSIGNED_INT, 0);
    });
    scheduleFor(ecs::Render);
}

DirectionalLightShaderSystem::~DirectionalLightShaderSystem()
{
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mVbo);
    glDeleteBuffers(1, &mEbo);
}

void DirectionalLightShaderSystem::onUpdate()
{
    bind();
}

void DirectionalLightShaderSystem::bind()
{
    mShader.bind();
    mLightAccumulationBuffer->bind();
    
    mShader.set("u_positions", mPositions->getName(), 0);
    mShader.set("u_normals",   mNormals->getName(),   1);
    mShader.set("u_albedo",    mAlbedo->getName(),    2);
    
    mShader.set("u_mvp_matrix", glm::mat4(1.f));
    mShader.set("u_camera_position_ws", mCamera->getPosition());
    
    glBindVertexArray(mVao);
}
