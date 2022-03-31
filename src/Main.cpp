/**
 * @file Main.cpp
 * @date 06/02/2022
 * @author Ryan Purse
 */

#include "Pch.h"
#include "core/Core.h"

int main()
{
    Core core(glm::ivec2(1920, 1080));
    core.run();
}
