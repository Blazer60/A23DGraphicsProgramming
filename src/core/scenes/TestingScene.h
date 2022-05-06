/**
 * @file TestingScene.h
 * @author Ryan Purse
 * @date 05/05/2022
 */


#pragma once

#include "Pch.h"
#include "Scene.h"
#include "Tree.h"

/**
 * @author Ryan Purse
 * @date 05/05/2022
 */
class TestingScene
    : public Scene
{
public:
    TestingScene();
    ~TestingScene() = default;
    
    void onUpdate() override;
    void onRender() override;
    void onImguiUpdate() override;
    
protected:
    Model<PhongVertex, BlinnPhongMaterial> mBanana {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/Bole.obj") };
    
    Model<PhongVertex, BlinnPhongMaterial> mStoneCladding {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/pbr-spheres/StoneCladding.obj") };
    
    Model<PhongVertex, BlinnPhongMaterial> mFloor {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/floor/Floor.obj") };
    
    Entity mAlpha;
    Entity mBeta;
    
    std::shared_ptr<octree::Tree<CollisionEntity>> mTree {
        std::make_shared<octree::Tree<CollisionEntity>>(octree::AABB { glm::vec3(0.f), glm::vec3(52.f) }, 2) };
    
    bool mShowBounds        { false };
    bool mShowElementBounds { false };
};
