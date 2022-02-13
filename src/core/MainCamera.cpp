/**
 * @file MainCamera.cpp
 * @author Ryan Purse
 * @date 13/02/2022
 */


#include "MainCamera.h"
#include "glfw3.h"

MainCamera::MainCamera()
    : mWindow(glfwGetCurrentContext())
{
    glm::ivec2 size;
    glfwGetWindowSize(mWindow, &size.x, &size.y);
    
    const glm::vec2 fSize { size };
    
    mProjectionMatrix = glm::perspective(mFovY, fSize.x / fSize.y, mNearClip, mFarClip);
    
    glfwSetMouseButtonCallback(mWindow, [](GLFWwindow *window, int button, int action, int mods){
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(window, 0.0, 0.0);
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    });
}

void MainCamera::update()
{
    move();
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.f), mPosition)
                         * glm::mat4(mRotation);
    
    mVpMatrix = mProjectionMatrix * glm::inverse(viewMatrix);
}

void MainCamera::move()
{
    if (!glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_RIGHT))
        return;
    
    // const float timeStep = static_cast<float>(glfwGetTime());
    const float timeStep = 0.001f;
    
    glm::dvec2 mouseOffset;
    glfwGetCursorPos(mWindow, &mouseOffset.x, &mouseOffset.y);
    glfwSetCursorPos(mWindow, 0.0, 0.0);
    
    // mRotation = glm::rotate(mRotation, static_cast<float>(glm::radians(mouseOffset.x)), glm::vec3(0.f, 1.f, 0.f));
    mRotation = mRotation * glm::quat(glm::vec3(0.f, -glm::radians(mouseOffset.x), 0.f));
    
    glm::vec3 input(0.f);
    if (glfwGetKey(mWindow, GLFW_KEY_W))
        input.z -= 1.f;
    if (glfwGetKey(mWindow, GLFW_KEY_S))
        input.z += 1.f;
    if (glfwGetKey(mWindow, GLFW_KEY_A))
        input.x -= 1.f;
    if (glfwGetKey(mWindow, GLFW_KEY_D))
        input.x += 1.f;
    
    if (glfwGetKey(mWindow, GLFW_KEY_LEFT_CONTROL))
        mPosition.y -= mSpeed * timeStep;
    if (glfwGetKey(mWindow, GLFW_KEY_SPACE))
        mPosition.y += mSpeed * timeStep;
    
    if (glm::length(input) > 1)
        input = glm::normalize(input);
    
    mPosition += mRotation * (mSpeed * timeStep * input);
    
    
    if (glfwGetKey(mWindow, GLFW_KEY_E))
        debug::log(std::to_string(mPosition.x) + ", "
                   + std::to_string(mPosition.y) + ", "
                   + std::to_string(mPosition.z));
}

const glm::mat4 &MainCamera::getVpMatrix() const
{
    return mVpMatrix;
}
