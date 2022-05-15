/**
 * @file PostProcessShader.cpp
 * @author Ryan Purse
 * @date 14/05/2022
 */


#include "PostProcessShader.h"
#include "Primitives.h"
#include "Vertices.h"

PostProcessShader::PostProcessShader(
    const std::filesystem::path &vertexPath,
    const std::filesystem::path &fragmentPath)
    : Shader(vertexPath, fragmentPath)
{
    const auto mesh = primitives::plane<UvVertex>()[0];
    mVao      = mesh.renderInformation.vao;
    mEboCount = mesh.renderInformation.eboCount;
    mEbo      = mesh.ebo;
    mVbo      = mesh.vbo;
}

PostProcessShader::~PostProcessShader()
{
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mEbo);
    glDeleteBuffers(1, &mVbo);
}

void PostProcessShader::render()
{
    bind();
    glBindVertexArray(mVao);
    onRender();
    glDrawElements(GL_TRIANGLES, mEboCount, GL_UNSIGNED_INT, 0);
}
