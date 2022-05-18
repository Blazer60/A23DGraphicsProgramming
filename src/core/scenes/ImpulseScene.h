/**
 * @file ImpulseScene.h
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
class ImpulseScene
    : public Scene
{
public:
    ImpulseScene();
    ~ImpulseScene();
    
    void onUpdate() override;
    void onRender() override;
    void onImguiUpdate() override;
    
protected:
    void showSettings(Entity entity);
    
    Entity mRedBall;
    Entity mGreenBall;
    Entity mBlueBall;
    Entity mYellowBall;
    
    std::shared_ptr<octree::Tree<CollisionEntity>> mTree {
        std::make_shared<octree::Tree<CollisionEntity>>(octree::AABB { glm::vec3(0.f), glm::vec3(52.f) }, 2) };
    
    bool mShowBounds        { true };
    bool mShowElementBounds { true };
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
