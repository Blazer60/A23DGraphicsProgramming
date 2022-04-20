/**
 * @file BoundingVolumeVisual.cpp
 * @author Ryan Purse
 * @date 20/04/2022
 */


#include "BoundingVolumeVisual.h"


BoundingVolumeVisual::BoundingVolumeVisual(std::shared_ptr<MainCamera> camera, const unsigned int geometryBufferId) :
    mCamera(std::move(camera)), mFbo(geometryBufferId)
{
    mSphere.renderInformation.fbo = geometryBufferId;
    mEntities.forEach([this](
        std::shared_ptr<BoundingVolume> &boundingVolume,
        std::shared_ptr<BasicUniforms> &basicUniforms)
    {
        if (const auto sphere = std::dynamic_pointer_cast<BoundingSphere>(boundingVolume))
            drawSphere(*sphere, basicUniforms->modelMat);
    });
}

void BoundingVolumeVisual::drawSphere(const BoundingSphere &boundingSphere, const glm::mat4 &modelMatrix)
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
    glBindVertexArray(mSphere.renderInformation.vao);
    
    mShader.set("u_mvp", mCamera->getVpMatrix() * modelMatrix);
    mShader.set("u_radius", boundingSphere.radius);
    mShader.set("u_camera_position_ws", mCamera->getPosition());
    
    glDrawElements(GL_LINES, mSphere.renderInformation.eboCount, GL_UNSIGNED_INT, 0);
}

void BoundingVolumeVisual::onStart()
{
    mShader.bind();
}

void BoundingVolumeVisual::onUpdate()
{
    mShader.bind();
}
