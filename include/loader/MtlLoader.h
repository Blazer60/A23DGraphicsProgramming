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
struct MaterialInformation
{
    std::vector<std::string>            materialNames;
    std::vector<MaterialSpecification>  materialDetails;
};

template<typename MaterialSpecification>
MaterialInformation<MaterialSpecification> loadMtl(std::string_view path)
{
    MaterialInformation<MaterialSpecification> out;
    
    parseMtl(path, [&](const std::string &materialName, const MtlMaterial &mtlMaterial) {
        out.materialNames.emplace_back(materialName);
        out.materialDetails.emplace_back(mtlMaterial);
    });
    
    return std::move(out);
}
