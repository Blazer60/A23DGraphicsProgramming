/**
 * @file Core.cpp
 * @author Ryan Purse
 * @date 11/02/2022
 */


#include "glew.h"
#include "Core.h"
#include "scenes/BloomSceneDemo.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glfw3.h"
#include "Timers.h"
#include "scenes/ImpulseScene.h"
#include "scenes/OctreeDemoScene.h"
#include "scenes/PlatformDemoScene.h"
#include "scenes/DynamicImpulseDemoScene.h"
#include "scenes/OdeDemoScene.h"
#include "scenes/RotationDemoScene.h"
#include "WindowHelpers.h"

Core::Core(const glm::ivec2 &resolution)
    : mResolution(resolution)
{
    if (!(initGlfw() && initOpenGl() && initImGui()))
    {
        mIsRunning = false;
        debug::log("Unable to initialise everything.", debug::severity::Warning);
        return;
    }
    window::setBufferSize(resolution);
    mScene = std::make_unique<BloomSceneDemo>();  // Scenes must be made after the initialisation of underlying architectures.
}

Core::~Core()
{
    mScene.reset();
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    glfwTerminate();
}

void Core::run()
{
    double nextUpdateTick = 0.0;
    
    while (mIsRunning)
    {
        unsigned int loopAmount = 0;
        
        while (timers::getTicks<double>() > nextUpdateTick && loopAmount < mMaxLoopCount)
        {
            mScene->onFixedUpdate();
    
            glfwPollEvents();
            mIsRunning = !glfwWindowShouldClose(mWindow);
            
            nextUpdateTick += timers::fixedTime<double>();
            ++loopAmount;
        }
        
        mScene->onUpdate();
        mScene->onRender();
        updateImgui();
        
        glfwSwapBuffers(mWindow);
        timers::update();
    }
}

bool Core::initGlfw()
{
    if (!glfwInit())
        return false;
    
    glfwSetErrorCallback([](int errorCode, const char *description) {
       debug::log(description);  // The program will bail out after this.
    });
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // Version of opengl you want to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (mEnableDebugging)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    
    mWindow = glfwCreateWindow(mResolution.x, mResolution.y, mWindowTitle.data(), nullptr, nullptr);
    if (!mWindow)
        return false;
    
    glfwMakeContextCurrent(mWindow);
    
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
    
    return true;
}

bool Core::initOpenGl()
{
    if (glewInit() != GLEW_OK)
        return false;
    
    // Debug Messaging.
    if (mEnableDebugging)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debug::openglCallBack, nullptr);
    
        int flags { 0 };  // Check to see if OpenGL debug context was set up correctly.
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT))
            return false;
    }
    
    debug::log(glGetString(GL_VERSION), debug::severity::Notification);
    
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
    
    mGuiIo = &ImGui::GetIO();
    (void)mGuiIo;
    mGuiIo->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    mGuiIo->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    mGuiIo->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    
    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    if (mGuiIo->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.f;
        style.Colors[ImGuiCol_WindowBg].w = 1.f;
    }
    
    if (!ImGui_ImplGlfw_InitForOpenGL(mWindow, true))
        return false;
    
    if (!ImGui_ImplOpenGL3_Init())
        return false;
    
    return true;
}

void Core::updateImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(0);
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    
    enum class Level { None, Impulse, Octree, Platforms, Dynamic, Ode, Rotation, Bloom };
    static Level type = Level::None;
    
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Scenes"))
        {
            if (ImGui::MenuItem("Impulse Demo"))
                type = Level::Impulse;
            
            if (ImGui::MenuItem("Octree Demo"))
                type = Level::Octree;
                
            if (ImGui::MenuItem("Platforms Demo"))
                type = Level::Platforms;
            
            if (ImGui::MenuItem("Dynamic Impulse Demo"))
                type = Level::Dynamic;
    
            if (ImGui::MenuItem("ODE Demo"))
                type = Level::Ode;
            
            if (ImGui::MenuItem("Rotation Demo"))
                type = Level::Rotation;
            
            if (ImGui::MenuItem("Bloom Scene"))
                type = Level::Bloom;
                
            ImGui::EndMenu();
        }
        mScene->onImguiMenuUpdate();
        const std::string text = "TPS: %.0f | Frame Rate: %.3f s/frame (%.1f FPS)";
        ImGui::SetCursorPosX(
            ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(text.c_str()).x
            - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
        ImGui::Text(text.c_str(), 1.f / timers::fixedTime<float>(), timers::deltaTime<float>(), ImGui::GetIO().Framerate);
        ImGui::EndMainMenuBar();
    }
    
    ImGui::Begin("Scene Settings");
    mScene->onImguiUpdate();
    ImGui::End();
    
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(mWindow, &display_w, &display_h);
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    if (mGuiIo->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    
    switch (type)
    {
        case Level::None:
            break;
        case Level::Impulse:
            mScene = std::make_unique<ImpulseScene>();
            break;
        case Level::Octree:
            mScene = std::make_unique<OctreeDemoScene>();
            break;
        case Level::Platforms:
            mScene = std::make_unique<PlatformDemoScene>();
            break;
        case Level::Dynamic:
            mScene = std::make_unique<DynamicImpulseDemoScene>();
            break;
        case Level::Ode:
            mScene = std::make_unique<OdeDemoScene>();
            break;
        case Level::Rotation:
            mScene = std::make_unique<RotationDemoScene>();
            break;
        case Level::Bloom:
            mScene = std::make_unique<BloomSceneDemo>();
            break;
    }
    type = Level::None;
}
