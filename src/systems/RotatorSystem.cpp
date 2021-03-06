/**
 * @file RotatorSystem.cpp
 * @author Ryan Purse
 * @date 15/02/2022
 */


#include "RotatorSystem.h"
#include "Timers.h"

RotatorSystem::RotatorSystem()
{
    mEntities.forEach([](Rotator &rotator, Transform &transform) {
        rotator.time += timers::deltaTime<float>();
        const auto timeSin = glm::sin(rotator.time);
        
        transform.rotation = glm::quat(glm::vec3(rotator.time));
        transform.position.y = timeSin * rotator.height + rotator.originalHeight.y;
    });
}