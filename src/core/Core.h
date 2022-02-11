/**
 * @file Core.h
 * @author Ryan Purse
 * @date 11/02/2022
 */


#pragma once

#include "glfw3.h"

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
    
    bool initGlfw();
    bool initOpenGl();
    bool initImGui();
    //
    // void run();
    //
    // void updateScenes();
    // void updateImgui();

protected:
    // Order of declaration matters here.
    const glm::ivec2         mResolution    { 1920, 1080 };
    const std::string_view   mWindowTitle   { "3D Graphics Pipeline" };
    GLFWwindow              *mWindow        { nullptr };  // Must be declared after resolution and title.
    
    std::unique_ptr<Scene>   mScene;
    
    bool                     mIsRunning     { true };
};


