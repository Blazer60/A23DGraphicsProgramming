/**
 * @file MainCamera.h
 * @author Ryan Purse
 * @date 13/02/2022
 */


#pragma once


#include "gtc/quaternion.hpp"
#include "glfw3.h"

/**
 * A basic main camera so that we can move around the world.
 * @author Ryan Purse
 * @date 13/02/2022
 */
class MainCamera
{
public:
    MainCamera();
    
    void update();
    
    void move();
    
    void setProjectionMatrix(glm::vec2 viewSize);
    
    [[nodiscard]] const glm::mat4 &getVpMatrix() const;

protected:
    glm::mat4       mVpMatrix           { 1.f };
    glm::mat4       mProjectionMatrix   { 1.f };
    
    glm::vec3       mPosition           { 0.f, 0.f, 3.f };
    glm::quat       mRotation           { glm::vec3(0.f) };
    glm::dvec2      mPanAngles          { 0.f };
    
    float           mSpeed              { 10.f };
    double          mMouseSpeed         { 0.5f };
    
    float           mFovY               { glm::radians(45.f) };
    float           mNearClip           { 0.001f };
    float           mFarClip            { 100.f };
    
    GLFWwindow *    mWindow             { };
};


