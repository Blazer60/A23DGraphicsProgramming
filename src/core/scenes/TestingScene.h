/**
 * @file TestingScene.h
 * @author Ryan Purse
 * @date 05/05/2022
 */


#pragma once

#include "Pch.h"
#include "Scene.h"

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
};
