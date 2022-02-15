/**
 * @file BasicUniformUpdaterSystem.cpp
 * @author Ryan Purse
 * @date 14/02/2022
 */


#include "BasicUniformUpdaterSystem.h"
#include "gtc/quaternion.hpp"

BasicUniformUpdaterSystem::BasicUniformUpdaterSystem()
{
    mEntities.forEach([](std::shared_ptr<BasicUniforms> &uniforms, const Transform &transform) {
        const glm::mat4 scale    = glm::scale(glm::mat4(1.f), transform.scale);
        const glm::mat4 rotation = glm::mat4(transform.rotation);
        const glm::mat4 position = glm::translate(glm::mat4(1.f), transform.position);
        
        uniforms->modelMat = position * rotation * scale;
    });
}
