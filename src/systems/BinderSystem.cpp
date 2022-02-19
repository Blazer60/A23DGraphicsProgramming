/**
 * @file BinderSystem.cpp
 * @author Ryan Purse
 * @date 19/02/2022
 */


#include "BinderSystem.h"

namespace vaoImpl
{
    void layout(unsigned int vao, const Instructions &instructions)
    {
        const std::unordered_map<GLenum, int> typeToSize {
                { GL_FLOAT, 4 },
                { GL_DOUBLE, 8 }
        };
    
        const unsigned int bindingIndex = 0;
        unsigned int attributeIndex     = 0;
        unsigned int offset             = 0;
    
        for (const auto &[type, count] : instructions)
        {
            glEnableVertexArrayAttrib(vao, attributeIndex);
            glVertexArrayAttribFormat(vao, attributeIndex, count, type, GL_FALSE, offset);
            glVertexArrayAttribBinding(vao, attributeIndex, bindingIndex);
        
            offset += typeToSize.at(type) * count;  // Floats are always four bytes.
            ++attributeIndex;
        }
    }
}


