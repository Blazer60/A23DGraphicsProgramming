/**
 * @file BoundingVolumeVisual.h
 * @author Ryan Purse
 * @date 20/04/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "BoundingVolumes.h"
#include "UniformComponents.h"
#include "Vertices.h"
#include "ModelLoader.h"
#include "FilePaths.h"
#include "Shader.h"
#include "MainCamera.h"

/**
 * @author Ryan Purse
 * @date 20/04/2022
 */
class BoundingVolumeVisual
    : public ecs::BaseSystem<std::shared_ptr<BoundingVolume>, std::shared_ptr<BasicUniforms>>
{
public:
    BoundingVolumeVisual(std::shared_ptr<MainCamera> camera, const unsigned int geometryBufferId);
    void drawSphere(const BoundingSphere &boundingSphere, const glm::mat4 &modelMatrix);
    
    void onStart() override;
    
    void onUpdate() override;

protected:
    Mesh<UvVertex, NoMaterial> mSphere { load::model<UvVertex, NoMaterial>(path::resources() + "models/physics/Sphere.obj")[0] };
    Shader mShader { path::shaders() + "physics/SphereVolume.vert", path::shaders() + "physics/SphereVolume.frag" };
    std::shared_ptr<MainCamera> mCamera;
    unsigned int mFbo { 0 };
};
