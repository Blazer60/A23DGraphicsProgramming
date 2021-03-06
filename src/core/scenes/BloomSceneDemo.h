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
class BloomSceneDemo
    : public Scene
{
public:
    BloomSceneDemo();
    ~BloomSceneDemo();
    
    void onImguiUpdate() override;
    
protected:
    Model<PhongVertex, BlinnPhongMaterial> mBananaModel {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/Bole.obj") };
    
    Model<PhongVertex, EmissivePbrMaterial> mJapanModel {
        load::model<PhongVertex, EmissivePbrMaterial>(
            path::resources() + "models/japan/Japan.obj") };
    
    Entity mBanana;
    Entity mPointLight;
};
