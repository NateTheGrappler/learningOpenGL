#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
    //bind the vertex array, for either drawing or for ebo to use to draw
    glGenVertexArrays(1, &m_rendererID);
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_rendererID));
}

void VertexArray::bind()
{
    glBindVertexArray(m_rendererID);
}
void VertexArray::unbind()
{
    glBindVertexArray(0);
}

void VertexArray::setBufferAttribute(VertexBuffer& vertexBuffer, VertexBufferLayout& layout)
{
    //bind both the array and the buffer
    vertexBuffer.bind();
    bind();

    //get the custom class that holds all of the layout data
    const std::vector<LayoutElements>& elements = layout.getElements();
    unsigned int offset = 0;

    //loop over the vector holding the data and then set it all properly
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const LayoutElements& element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset));
        offset += element.count *  LayoutElements::getSizeOfType(element.type);

    }

}