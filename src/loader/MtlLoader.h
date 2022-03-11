/**
 * @file MtlLoader.h
 * @author Ryan Purse
 * @date 09/03/2022
 */


#pragma once

#include "CommonLoader.h"
#include "MaterialComponents.h"

void parseMtl(std::string_view path, const std::function<void(std::string&, MtlMaterial&)> &genMaterialDelegate);

template<typename MaterialSpecification>
std::unordered_map<std::string, MaterialSpecification> loadMtl(std::string_view path)
{
    std::unordered_map<std::string, MaterialSpecification> outMaterials;
    
    parseMtl(path, [&](const std::string &materialName, const MtlMaterial &mtlMaterial) {
        outMaterials.insert({ materialName, mtlMaterial });
    });
    
    return std::move(outMaterials);
}
