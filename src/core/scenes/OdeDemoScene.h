/**
 * @file OdeDemoScene.h
 * @author Ryan Purse
 * @date 10/05/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "Scene.h"
#include "Tree.h"

/**
 * @author Ryan Purse
 * @date 10/05/2022
 */
class OdeDemoScene
    : public Scene
{
public:
    OdeDemoScene();
    ~OdeDemoScene();
    
    void onUpdate() override;
    void onRender() override;
    void onImguiUpdate() override;
    
protected:
    void setupBall(Entity ball, Component type, const glm::vec3 position, Model<PhongVertex, BlinnPhongMaterial> &model);
    
    Component mEulerTag { mEcs.create<DynamicObject>() };
    Component mRk2Tag   { mEcs.create<DynamicObject>() };
    Component mRk4Tag   { mEcs.create<DynamicObject>() };
    Component mRkNTag   { mEcs.create<DynamicObject>() };
    
    Entity mRedBall;
    Entity mGreenBall;
    Entity mBlueBall;
    Entity mYellowBall;
    
    uint32_t mRkNValue { 4 };
    
    std::shared_ptr<octree::Tree<CollisionEntity>> mTree {
        std::make_shared<octree::Tree<CollisionEntity>>(octree::AABB { glm::vec3(0.f), glm::vec3(52.f) }, 2) };
    
    bool mShowBounds        { false };
    bool mShowElementBounds { false };
    bool mSetup             { false };
    
    Model<PhongVertex, BlinnPhongMaterial> mRedSphere {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/physics/RedSphere.obj") };
    
    Model<PhongVertex, BlinnPhongMaterial> mGreenSphere {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/physics/GreenSphere.obj") };
    
    Model<PhongVertex, BlinnPhongMaterial> mBlueSphere {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/physics/BlueSphere.obj") };
    
    Model<PhongVertex, BlinnPhongMaterial> mYellowSphere {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/physics/YellowSphere.obj") };
    
    Model<PhongVertex, BlinnPhongMaterial> mFloor {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/thick-floor/ThickFloor.obj") };
};
