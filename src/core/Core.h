/**
 * @file Core.h
 * @author Ryan Purse
 * @date 11/02/2022
 */


#pragma once

#include "Pch.h"
#include "glfw3.h"
#include "imgui.h"

class Scene;

/**
 * The main loop of the program. Handles the updates of scenes and refreshes the screen.
 * @author Ryan Purse
 * @date 11/02/2022
 */
class Core
{
public:
    explicit Core(const glm::ivec2 &resolution);
    ~Core();
    
    void run();
protected:
    bool initGlfw();
    bool initOpenGl();
    bool initImGui();
    
    void updateImgui();

protected:
    // Order of declaration matters here.
    const glm::ivec2         mResolution        { 1920, 1080 };
    const std::string_view   mWindowTitle       { "3D Graphics Pipeline" };
    GLFWwindow              *mWindow            { nullptr };  // Must be declared after resolution and title.
    
    std::unique_ptr<Scene>   mScene;
    
    ImGuiIO                 *mGuiIo;
    
    bool                     mIsRunning         { true };
    const unsigned int       mMaxLoopCount      { 10 };
    const bool               mEnableDebugging   { true };
};


