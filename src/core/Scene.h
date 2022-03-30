/**
 * @file Scene.h
 * @author Ryan Purse
 * @date 11/02/2022
 */


#pragma once

#include "Components.h"
#include "Shader.h"
#include "MainCamera.h"
#include "Common.h"
#include "FrameBufferObject.h"
#include "PostProcessingShader.h"
#include "FilePaths.h"
#include "DirectionalLight.h"
#include "ModelLoader.h"
#include "TextureBufferObject.h"
#include "Core.h"
#include "RenderPipeline.h"
#include "DeferredLightShader.h"

#include "Renderer.h"
#include "Primitives.h"

#include <memory>

/**
 * An abstraction to differentiate from core functionality and ECS Systems
 * @author Ryan Purse
 * @date 11/02/2022
 */
class Scene
{
public:
    Scene();
    ~Scene() = default;
    
    void onUpdate();
    
    void onImguiUpdate();

protected:
    glm::ivec2 mSreenSize { 1920, 1080 };  // Must be at the top.
    
    Model<UvVertex, NoMaterial> mCube { primitives::cube<UvVertex>() };

    std::shared_ptr<MainCamera> mMainCamera { std::make_shared<MainCamera>(glm::vec3(0.f, 1.f, 5.f)) };
    Renderer mRenderer { mMainCamera };
    
    ecs::Entity createPhongModel(glm::vec3 position, std::string_view path);
};


