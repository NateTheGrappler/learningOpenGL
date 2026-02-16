#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
    //bind the vertex array, for either drawing or for ebo to use to draw
    glGenVertexArrays(1, &m_rendererID);
    glBindVertexArray(m_rendererID);
}