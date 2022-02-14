/**
 * @file Timers.cpp
 * @author Ryan Purse
 * @date 14/02/2022
 */


#include "Timers.h"
#include "glfw3.h"

namespace timers
{
    double deltaTime_impl    { 0.16f };
    
    static double current           { 0 };
    static double last              { 0 };
    
    void updateDeltaTime_impl();
    
    void update()
    {
        updateDeltaTime_impl();
    }
    
    void updateDeltaTime_impl()
    {
        current = glfwGetTime();
        deltaTime_impl = current - last;
        last = current;
    }
}