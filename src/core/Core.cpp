/**
 * @file Core.cpp
 * @author Ryan Purse
 * @date 11/02/2022
 */


#include "glew.h"
#include "Core.h"
#include "scenes/TestingScene.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glfw3.h"
#include "Timers.h"
#include "scenes/ImpulseScene.h"

Core::Core(const glm::ivec2 &resolution)
{
    if (!(initGlfw() && initOpenGl() && initImGui()))
    {
        mIsRunning = false;
        debug::log("Unable to initialise everything.", debug::severity::Warning);
        return;
    }
    
    mScene = std::make_unique<ImpulseScene>();  // Scenes must be made after the initialisation of underlying architectures.
}

Core::~Core()
{
    mScene.reset();
    glfwTerminate();
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
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
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // Version of opengl you want to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);  // For debugging
    
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
    
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Scenes"))
        {
            if (ImGui::MenuItem("Impulse Demo"))
                mScene = std::make_unique<ImpulseScene>();
            
            ImGui::MenuItem("Scene B");
            ImGui::MenuItem("Scene C");
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
}
