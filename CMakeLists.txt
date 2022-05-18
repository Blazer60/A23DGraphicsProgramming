cmake_minimum_required(VERSION 3.21)
set(PROJECT_NAME A23DGraphicsProgramming)

project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)

string(REPLACE ${CMAKE_SOURCE_DIR} "" RELATIVE_PATH ${CMAKE_BINARY_DIR})
string(REGEX MATCHALL / FILE_DIFF ${RELATIVE_PATH})
string(REPLACE "/" "../" BIN_PATH ${FILE_DIFF})
set(BINARY_TO_ROOT  ${BIN_PATH})
set(RESOURCE_PATH   "res/")
set(SHADER_PATH     "res/shaders")
set(TEXTURE_PATH    "res/textures")

set(USE_PRE_BUILT_LIBS ON)
set(USE_PSEUDO_PCH ON)
set(ADD_ECS_TO_EXECUTABLE ON)

message(STATUS "Using Cmake:    " ${CMAKE_VERSION})
message(STATUS "Compiling with: " ${CMAKE_CXX_COMPILER_ID})
message(STATUS "CXX Standard:   " ${CMAKE_CXX_STANDARD})

if (${ADD_ECS_TO_EXECUTABLE})
    set(ECS_INCLUDE_FILES
        entity-component-system/include/systems/BaseSystem.h
        entity-component-system/include/systems/Entities.h
        entity-component-system/include/Common.h
        entity-component-system/include/Core.h
        entity-component-system/include/Ecs.h

        entity-component-system/src/components/Archetype.h
        entity-component-system/src/components/ArchetypeManager.h
        entity-component-system/src/components/ComponentArray.h
        entity-component-system/src/systems/SystemManager.h
        entity-component-system/src/EntityManager.h)

    set(ECS_SOURCE_FILES
        entity-component-system/src/components/Archetype.cpp
        entity-component-system/src/components/ArchetypeManager.cpp
        entity-component-system/src/systems/SystemManager.cpp
        entity-component-system/src/Common.cpp
        entity-component-system/src/Core.cpp
        entity-component-system/src/EntityManager.cpp)
else()
    add_subdirectory(entity-component-system)
endif ()

add_subdirectory(vendor/glm)
add_subdirectory(vendor/glew/build/cmake)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor)


add_executable(${PROJECT_NAME}
        ${ECS_INCLUDE_FILES} ${ECS_SOURCE_FILES}

        include/components/render-components/Vertices.h
        include/components/render-components/RenderComponents.h
        include/components/render-components/UniformComponents.h
        include/components/render-components/TextureComponents.h
        include/components/render-components/MaterialComponents.h
        include/components/render-components/LightingComponents.h
        include/components/Components.h
        src/components/render-components/Mesh.cpp               include/components/render-components/Mesh.h

        src/core/Core.cpp                                       src/core/Core.h
        src/core/Shader.cpp                                     include/core/Shader.h
        src/core/DebugLogger.cpp                                include/core/DebugLogger.h
        src/core/MainCamera.cpp                                 include/core/MainCamera.h
        src/core/Renderer.cpp                                   include/core/Renderer.h

        src/core/scenes/Scene.cpp                               src/core/scenes/Scene.h
        src/core/scenes/BloomSceneDemo.cpp src/core/scenes/BloomSceneDemo.h
        src/core/scenes/ImpulseScene.cpp                        src/core/scenes/ImpulseScene.h
        src/core/scenes/OctreeDemoScene.cpp                     src/core/scenes/OctreeDemoScene.h
        src/core/scenes/PlatformDemoScene.cpp                   src/core/scenes/PlatformDemoScene.h
        src/core/scenes/DynamicImpulseDemoScene.cpp             src/core/scenes/DynamicImpulseDemoScene.h
        src/core/scenes/OdeDemoScene.cpp                        src/core/scenes/OdeDemoScene.h
        src/core/scenes/RotationDemoScene.cpp                   src/core/scenes/RotationDemoScene.h

        src/helpers/Primitives.cpp                              include/helpers/Primitives.h
        src/helpers/Timers.cpp                                  include/helpers/Timers.h
        src/helpers/FilePaths.cpp                               include/helpers/FilePaths.h
        src/helpers/WindowHelpers.cpp                           include/helpers/WindowHelpers.h

        src/loader/CommonLoader.cpp                             include/loader/CommonLoader.h
        src/loader/MaterialLoader.cpp                           include/loader/MaterialLoader.h
        src/loader/ModelLoader.cpp                              include/loader/ModelLoader.h
        src/loader/RawMesh.cpp                                  include/loader/RawMesh.h
        src/loader/TextureLoader.cpp                            include/loader/TextureLoader.h
        include/helpers/Callback.h

        include/physics/components/Physics.h
        include/physics/octree/Node.h
        include/physics/octree/Tree.h
        src/physics/PhysicsSystems.cpp                          include/physics/PhysicsSystems.h
        src/physics/PhysicsHelpers.cpp                          include/physics/PhysicsHelpers.h
        src/physics/CollisionDetection.cpp                      include/physics/CollisionDetection.h
        src/components/BoundingVolumes.cpp                      include/physics/components/BoundingVolumes.h
        src/physics/BoundingVolumeVisual.cpp                    include/physics/BoundingVolumeVisual.h
        src/physics/CollisionResponse.cpp                       include/physics/CollisionResponse.h
        src/physics/octree/OctreeHelpers.cpp                    include/physics/octree/OctreeHelpers.h
        src/physics/TreeBuilder.cpp                             include/physics/TreeBuilder.h

        src/rendering/lighting/DirectionalLightShaderSystem.cpp include/rendering/lighting/DirectionalLightShaderSystem.h
        src/rendering/lighting/PointLightShader.cpp             include/rendering/lighting/PointLightShader.h
        src/rendering/TextureBufferObject.cpp                   include/rendering/TextureBufferObject.h
        src/rendering/FramebufferObject.cpp                     include/rendering/FramebufferObject.h
        src/rendering/RenderBufferObject.cpp                    include/rendering/RenderBufferObject.h
        src/rendering/lighting/DeferredLightShader.cpp          include/rendering/lighting/DeferredLightShader.h
        src/rendering/MipmapTexture.cpp                         include/rendering/MipmapTexture.h
        src/helpers/MipViewer.cpp include/helpers/MipViewer.h
        src/rendering/post-processing/Bloom.cpp                 include/rendering/post-processing/Bloom.h
        src/rendering/BlinnPhongGeometryShader.cpp              include/rendering/BlinnPhongGeometryShader.h

        src/systems/ModelMatrixUpdater.cpp include/systems/ModelMatrixUpdater.h
        src/systems/RotatorSystem.cpp                           include/systems/RotatorSystem.h

        src/Main.cpp
        include/Pch.h
        src/rendering/EmissivePbrGeometryShader.cpp include/rendering/EmissivePbrGeometryShader.h src/loader/ModelDestroyer.cpp include/loader/ModelDestroyer.h src/helpers/TextureViewer.cpp include/helpers/TextureViewer.h)

target_include_directories(${PROJECT_NAME} PUBLIC
# Project Includes
        include           include/core
        include/systems   include/components
        include/helpers   include/components/render-components
        include/loader    include/lighting
        include/rendering include/systems/lighting
        include/systems/physics include/octree
        include/physics     include/physics/components
        include/physics/octree
        include/rendering/post-processing
        include/rendering/lighting

# ECS Includes
        entity-component-system                 entity-component-system/include
        entity-component-system/include/systems entity-component-system/src

# Vendor Includes
        vendor/glm/glm           vendor/glew/include/GL
        vendor/glfw/include/GLFW vendor/imgui
        vendor/stb/include)

if (${USE_PSEUDO_PCH})
    message(STATUS "Using pseudo PCH")
    add_compile_definitions(PCH=1)
else()
    message(STATUS "Using CMake PCH")
    target_precompile_headers(${PROJECT_NAME} PUBLIC
            <iostream> <vector> <unordered_map>
            <string> <string_view> <algorithm>
            <memory> <numeric> <cstdint>
            <glm.hpp> [["DebugLogger.h"]])
endif ()

# STB_IMAGE_IMPLEMENTATION is set within a source file (Texture Loader).
add_compile_definitions(
        GLEW_STATIC
        RESOURCE_PATH="${BINARY_TO_ROOT}/${RESOURCE_PATH}/"
        SHADER_PATH="${BINARY_TO_ROOT}/${SHADER_PATH}/"
        TEXTURE_PATH="${BINARY_TO_ROOT}/${TEXTURE_PATH}/")


find_package(OpenGL)  # Glew Requires OpenGL to be added.

if (NOT ${ADD_ECS_TO_EXECUTABLE})
    find_library(ECS NAMES EntityComponentSystem2022 PATHS ${CMAKE_CURRENT_BINARY_DIR}/entity-component-system REQUIRED)
    target_link_libraries(${ECS})
endif ()

if (${USE_PRE_BUILT_LIBS} EQUAL OFF)
    find_library(GLEW  NAMES libglew32d PATHS ${CMAKE_CURRENT_BINARY_DIR}/lib             REQUIRED)
    find_library(GLFW  NAMES glfw3      PATHS ${CMAKE_CURRENT_BINARY_DIR}/vendor/glfw/src REQUIRED)
    find_library(IMGUI NAMES imgui      PATHS ${CMAKE_CURRENT_BINARY_DIR}/vendor          REQUIRED)
else()
    find_library(GLEW  NAMES libglew32d PATHS ${CMAKE_SOURCE_DIR}/vendor/lib REQUIRED)
    find_library(GLFW  NAMES glfw3      PATHS ${CMAKE_SOURCE_DIR}/vendor/lib REQUIRED)
    find_library(IMGUI NAMES imgui      PATHS ${CMAKE_SOURCE_DIR}/vendor/lib REQUIRED)
endif ()

target_link_libraries(${PROJECT_NAME}
        ${GLEW} ${GLFW} ${IMGUI} OpenGL::GL)
