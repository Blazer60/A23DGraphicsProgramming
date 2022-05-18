/**
 * @file ModelDestroyer.h
 * @author Ryan Purse
 * @date 18/05/2022
 */


#pragma once

#include "Pch.h"
#include "Mesh.h"
#include "MaterialComponents.h"


namespace destroy
{
    /**
     * @brief Destroys all data and references to an opengl model. We can't use the destructor
     * as models could be referenced in multiple places.
     * @tparam TVertex - The type of vertex the mesh uses.
     * @tparam TMaterial - The type of material the mesh uses.
     * @param meshes - The mesh that you want to destroy.
     */
    template<typename TVertex, typename TMaterial>
    void model(Model<TVertex, TMaterial> &meshes)
    {
        for (auto &mesh : meshes)
        {
            glDeleteVertexArrays(1, &mesh.renderInformation.vao);
            glDeleteBuffers(1, &mesh.vbo);
            glDeleteBuffers(1, &mesh.ebo);
            mesh.renderInformation.vao = 0;
            mesh.renderInformation.eboCount = 0;
            mesh.vbo = 0;
            mesh.ebo = 0;
            
            destroy::material(mesh.material);
        }
    }
    
    /**
     * @brief Destroys a blinn-phong material.
     * @param mat - The material that you want to destroy.
     */
    void material(BlinnPhongMaterial &mat);
    
    /**
     * @brief Destorys an emissive material.
     * @param mat - The amterial that you want to destroy.
     */
    void material(EmissivePbrMaterial &mat);
    
    void material(NoMaterial &mat);
}
