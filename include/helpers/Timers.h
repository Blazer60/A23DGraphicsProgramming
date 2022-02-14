/**
 * @file Timers.h
 * @author Ryan Purse
 * @date 14/02/2022
 */


#pragma once

namespace timers
{
    extern double deltaTime_impl;
    
    void update();
    
    template<typename T>
    T deltaTime()
    {
        return static_cast<T>(deltaTime_impl);
    }
}