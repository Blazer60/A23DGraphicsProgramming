/**
 * @file Loader.cpp
 * @author Ryan Purse
 * @date 23/02/2022
 */


#include "Loader.h"
#include "ObjLoader.h"

#include <algorithm>

fileExtension getExtension(std::string_view path)
{
    const std::string_view objEnding = ".obj";
    if (path.size() > objEnding.size() && std::equal(objEnding.rbegin(), objEnding.rend(), path.rbegin(), path.rbegin() + objEnding.size()))
        return fileExtension::Obj;
    return fileExtension::None;
}
