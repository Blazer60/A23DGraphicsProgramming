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
    
    void onImguiUpdate() override;
    
protected:
    Model<PhongVertex, BlinnPhongMaterial> mBananaModel {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/Bole.obj") };
    
    Model<PhongVertex, BlinnPhongMaterial> mFloor {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/thick-floor/ThickFloor.obj") };
    
    Entity mBanana;
};
