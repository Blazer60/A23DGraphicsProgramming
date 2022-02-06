/**
 * @file Main.cpp
 * @date 06/02/2022
 * @author Ryan Purse
 */

#include "Ecs.h"
#include "glm.hpp"


int main()
{
    ecs::init();
    
    ecs::create<glm::vec2>(ecs::TypeDefault);
    
    ecs::Entity e = ecs::create();
    ecs::add(e, glm::vec2(14.f, 10.f));
    
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
