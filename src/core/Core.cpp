/**
 * @file Core.cpp
 * @author Ryan Purse
 * @date 11/02/2022
 */


#include "glew.h"
#include "Core.h"
#include "Scene.h"
#include "Ecs.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glfw3.h"

Core::Core(const glm::ivec2 &resolution)
{
    if (!(initGlfw() && initOpenGl() && initImGui()))
    {
        mIsRunning = false;
        debug::log("Unable to initialise everything.", debug::severity::Warning);
        return;
    }
    
    ecs::init();  // Never crashes.
    
    mScene = std::make_unique<Scene>();  // Scenes must be made after the initialisation of underlying architectures.
}

Core::~Core()
{
    glfwTerminate();
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
}

void Core::run()
{
    while (mIsRunning)
    {
        updateScene();
        updateImgui();
        
        glfwSwapBuffers(mWindow);
        
        glfwPollEvents();
        mIsRunning = !glfwWindowShouldClose(mWindow);
    }
}

bool Core::initGlfw()
{
    if (!glfwInit())
        return false;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // Version of opengl you want to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);  // For debugging
    
    mWindow = glfwCreateWindow(mResolution.x, mResolution.y, mWindowTitle.data(), nullptr, nullptr);
    if (!mWindow)
        return false;
    
    glfwMakeContextCurrent(mWindow);
    
    return true;
}

bool Core::initOpenGl()
{
    if (glewInit() != GLEW_OK)
        return false;
    
    // Debug Messaging.
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debug::openglCallBack, nullptr);

    debug::log(glGetString(GL_VERSION), debug::severity::Notification);
    
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT))
        return false;
    
    // Blending texture data / enabling lerping.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    return true;
}

bool Core::initImGui()
{
    ImGui::CreateContext();
    
    if (!ImGui_ImplGlfw_InitForOpenGL(mWindow, true))
        return false;
    
    if (!ImGui_ImplOpenGL3_Init())
        return false;
    
    ImGui::StyleColorsDark();
    
    return true;
}

void Core::updateScene()
{
    mScene->onUpdate();
}

void Core::updateImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    mScene->onImguiUpdate();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
