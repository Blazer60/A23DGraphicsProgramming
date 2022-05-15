/**
 * @file PointLightShader.cpp
 * @author Ryan Purse
 * @date 31/03/2022
 */


#include "systems/lighting/PointLightShader.h"
#include "Primitives.h"

PointLightShader::PointLightShader(
    std::shared_ptr<MainCamera> camera,
    std::shared_ptr<FramebufferObject> lightAccumulationBuffer,
    std::shared_ptr<TextureBufferObject> positions,
    std::shared_ptr<TextureBufferObject> normals,
    std::shared_ptr<TextureBufferObject> albedo)
    :
    mCamera(std::move(camera)),
    mLightAccumulationBuffer(std::move(lightAccumulationBuffer)),
    mPositions(std::move(positions)),
    mNormals(std::move(normals)),
    mAlbedo(std::move(albedo))
{
    const auto mesh = load::model<UvVertex, NoMaterial>(path::resources() + "models/lighting/PointLightVolume.obj")[0];
    mVao = mesh.renderInformation.vao;
    mEboCount = mesh.renderInformation.eboCount;
    mEbo = mesh.ebo;
    mVbo = mesh.vbo;
    
    mEntities.forEach([this](const light::PointLight &pointLight, const Position &position) {
        const glm::mat4 positionMatrix = glm::translate(glm::mat4(1.f), position.value);
        const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), glm::vec3(pointLight.distance));
        const glm::mat4 mvp = mCamera->getVpMatrix() * positionMatrix * scaleMatrix;
        
        mShader.set("u_mvp_matrix", mCamera->getVpMatrix() * positionMatrix * scaleMatrix);
        mShader.set("u_light_position",  position.value);
        mShader.set("u_light_intensity", pointLight.intensity);
        mShader.set("u_light_distance",  pointLight.distance);
        mShader.set("u_light_power",     pointLight.power);
    
        // The model has inverted faces!
        // Point lights are always rendered backwards (so you can go inside the mesh).
        glDrawElements(GL_TRIANGLES, mEboCount, GL_UNSIGNED_INT, 0);
    });
    scheduleFor(ecs::Render);
}

PointLightShader::~PointLightShader()
{

}

void PointLightShader::onUpdate()
{
    bind();
}

void PointLightShader::bind()
{
    mShader.bind();
    mLightAccumulationBuffer->bind();
    glBindVertexArray(mVao);
    
    glBindTextureUnit(0, mPositions->getName());
    glBindTextureUnit(1, mNormals->getName());
    glBindTextureUnit(2, mAlbedo->getName());
    
    mShader.set("u_positions", 0);
    mShader.set("u_normals",   1);
    mShader.set("u_albedo",    2);
    
    mShader.set("u_mvp_matrix", glm::mat4(1.f));
    mShader.set("u_camera_position_ws", mCamera->getPosition());
}
