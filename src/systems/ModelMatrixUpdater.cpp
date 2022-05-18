/**
 * @file BasicUniformUpdaterSystem.cpp
 * @author Ryan Purse
 * @date 14/02/2022
 */


#include "ModelMatrixUpdater.h"
#include "gtc/quaternion.hpp"

ModelMatrixUpdater::ModelMatrixUpdater()
{
    mEntities.forEach([](std::shared_ptr<ModelMatrix> &modelMatrix, const Transform &transform) {
        const glm::mat4 scale    = glm::scale(glm::mat4(1.f), transform.scale);
        const glm::mat4 rotation = glm::mat4(transform.rotation);
        const glm::mat4 position = glm::translate(glm::mat4(1.f), transform.position);
    
        modelMatrix->value = position * rotation * scale;
    });
    scheduleFor(ecs::Update);
}
