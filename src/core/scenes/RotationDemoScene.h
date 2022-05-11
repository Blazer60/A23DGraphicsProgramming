/**
 * @file RotationDemoScene.h
 * @author Ryan Purse
 * @date 11/05/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "Scene.h"
#include "Tree.h"

/**
 * @author Ryan Purse
 * @date 11/05/2022
 */
class RotationDemoScene
    : public Scene
{
public:
    RotationDemoScene();
    
    void onUpdate() override;
    void onRender() override;
    void onImguiUpdate() override;
    
protected:
    Model<PhongVertex, BlinnPhongMaterial> mStoneCladding {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/pbr-spheres/StoneCladding.obj") };
    
    Model<PhongVertex, BlinnPhongMaterial> mFloor {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/thick-floor/ThickFloor.obj") };
    
    Entity mAlpha;
    
    std::shared_ptr<octree::Tree<CollisionEntity>> mTree {
        std::make_shared<octree::Tree<CollisionEntity>>(octree::AABB { glm::vec3(0.f), glm::vec3(52.f) }, 2) };
    
    bool mShowBounds        { false };
    bool mShowElementBounds { false };
    bool mSetup             { false };
};
