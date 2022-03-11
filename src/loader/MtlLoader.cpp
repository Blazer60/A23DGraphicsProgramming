/**
 * @file MtlLoader.cpp
 * @author Ryan Purse
 * @date 09/03/2022
 */


#include "MtlLoader.h"

void parseMtl(std::string_view path, const std::function<void(std::string&, MtlMaterial&)> &genMaterialDelegate)
{
    MtlMaterial currentMaterial {};
    std::string currentName     { "NULL" };
    
    /**
     * @brief Converts the current material into a new material. Ignores the first newmtl.
     */
    const auto convertMaterial = [&](std::string_view arg) {
        static bool isFirst = true;
        if (isFirst)
        {
            currentName = std::string(arg);
            isFirst = false;
            return;
        }
        genMaterialDelegate(currentName, currentMaterial);
        currentMaterial = MtlMaterial();
        currentName     = std::string(arg);
    };
    
    const ArgumentList argumentList {
        {"#",       doNothing },
        {"d",       doNothing },
        {"illum",   doNothing },
        {"newmtl",  convertMaterial },
        {"Ns",      [&currentMaterial](std::string_view arg)  { currentMaterial.nS = std::stoi(std::string(arg)); } },
        {"Ka",      [&currentMaterial](std::string_view args) { currentMaterial.kA = createVec<3>(args); } },
        {"Kd",      [&currentMaterial](std::string_view args) { currentMaterial.kD = createVec<3>(args); } },
        {"Ks",      [&currentMaterial](std::string_view args) { currentMaterial.kS = createVec<3>(args); } },
    };
    
    parseFile(path, argumentList);
    convertMaterial("NULL");  // Convert the last known material when we reach the end of the file.
}
