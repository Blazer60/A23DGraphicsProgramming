/**
 * @file OctreeDemoScene.h
 * @author Ryan Purse
 * @date 09/05/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "Scene.h"
#include "Tree.h"

/**
 * @author Ryan Purse
 * @date 09/05/2022
 */
class OctreeDemoScene
    : public Scene
{
public:
    OctreeDemoScene();
    ~OctreeDemoScene();
    float randomValue();
    
    void onRender() override;
    void onImguiUpdate() override;
    
protected:
    std::shared_ptr<octree::Tree<CollisionEntity>> mTree {
        std::make_shared<octree::Tree<CollisionEntity>>(octree::AABB { glm::vec3(0.f), glm::vec3(52.f) }, 2) };
    
    bool mShowBounds        { false };
    bool mShowElementBounds { false };
    
    Model<PhongVertex, BlinnPhongMaterial> mCrate {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/physics/Crate2.obj") };
    
    Model<PhongVertex, BlinnPhongMaterial> mFloor {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/thick-floor/ThickFloor.obj") };
};
