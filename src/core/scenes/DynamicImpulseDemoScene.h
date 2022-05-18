/**
 * @file DynamicImpulseDemoScene.h
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
class DynamicImpulseDemoScene
    : public Scene
{
public:
    DynamicImpulseDemoScene();
    ~DynamicImpulseDemoScene();
    
    void onUpdate() override;
    void onRender() override;
    void onImguiUpdate() override;
    
protected:
    void showBallSettings(Entity entity);
    
    Entity mRedBall;
    Entity mGreenBall;
    
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
};
