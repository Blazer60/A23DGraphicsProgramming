/**
 * @file Scene.h
 * @author Ryan Purse
 * @date 11/02/2022
 */


#pragma once

#include "Components.h"
#include "Shader.h"

/**
 * An abstraction to differentiate from core functionality and ECS Systems
 * @author Ryan Purse
 * @date 11/02/2022
 */
class Scene
{
public:
    Scene();
    ~Scene() = default;
    
    void onUpdate();
    
    void onImguiUpdate();

protected:
    std::shared_ptr<BasicMesh> mCube;
    std::shared_ptr<BasicMesh> mTri;
    
    GLuint VAO, VBO, EBO;
    Shader shader { "../res/shaders/basic/Basic.vert", "../res/shaders/basic/Basic.frag" };
};


