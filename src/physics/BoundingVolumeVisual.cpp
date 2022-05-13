/**
 * @file BoundingVolumeVisual.cpp
 * @author Ryan Purse
 * @date 20/04/2022
 */


#include "BoundingVolumeVisual.h"


BoundingVolumeVisual::BoundingVolumeVisual(std::shared_ptr<MainCamera> camera, const unsigned int geometryBufferId) :
    mCamera(std::move(camera)), mFbo(geometryBufferId)
{
    mEntities.forEach([this](
        std::shared_ptr<BoundingVolume> &boundingVolume,
        std::shared_ptr<BasicUniforms> &basicUniforms)
    {
        if (const auto sphere = std::dynamic_pointer_cast<BoundingSphere>(boundingVolume))
            drawSphere(*sphere, basicUniforms->modelMat);
        else if (const auto box = std::dynamic_pointer_cast<BoundingBox>(boundingVolume))
            drawBox(*box, basicUniforms->modelMat);
    });
    scheduleFor(ecs::Render);
}

void BoundingVolumeVisual::drawSphere(const BoundingSphere &boundingSphere, const glm::mat4 &modelMatrix)
{
    mSphereShader.bind();
    glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
    glBindVertexArray(mSphere.renderInformation.vao);
    
    mSphereShader.set("u_mvp", mCamera->getVpMatrix() * modelMatrix);
    mSphereShader.set("u_model_matrix", modelMatrix);
    mSphereShader.set("u_radius", boundingSphere.radius);
    mSphereShader.set("u_camera_position_ws", mCamera->getPosition());
    
    glDrawElements(GL_LINES, mSphere.renderInformation.eboCount, GL_UNSIGNED_INT, 0);
}

void BoundingVolumeVisual::drawBox(const BoundingBox &boundingBox, const glm::mat4 &modelMatrix)
{
    mBoxShader.bind();
    glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
    glBindVertexArray(mBox.renderInformation.vao);
    
    mBoxShader.set("u_mvp", mCamera->getVpMatrix() * modelMatrix);
    mBoxShader.set("u_model_matrix", modelMatrix);
    mBoxShader.set("u_halfsize", boundingBox.halfSize);
    mBoxShader.set("u_camera_position_ws", mCamera->getPosition());
    
    glDrawElements(GL_LINES, mBox.renderInformation.eboCount, GL_UNSIGNED_INT, 0);
}
