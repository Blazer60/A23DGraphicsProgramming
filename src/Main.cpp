/**
 * @file Main.cpp
 * @date 06/02/2022
 * @author Ryan Purse
 */

#include "Ecs.h"
#include "glm.hpp"
#include "glew.h"
#include "glfw3.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

int main()
{
    ecs::init();
    
    if (!glfwInit())
        return 3;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // Version of OpenGl you want to use.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    
    auto *window = glfwCreateWindow(1920, 1080, "e", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return 3;
    }
    
    glfwMakeContextCurrent(window);
    
    if (glewInit() != GLEW_OK)
    {
        // No context yet.
        return 3;
    }
    
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    
    
    ecs::create<glm::vec2>(ecs::TypeDefault);
    
    ecs::Entity e = ecs::create();
    ecs::add(e, glm::vec2(14.f, 10.f));
    
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
