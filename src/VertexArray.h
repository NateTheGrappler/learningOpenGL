#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_rendererID;

public:
	//constructor and deconstructor
	VertexArray();
	~VertexArray();

	//set attribute function
	void setBufferAttribute(VertexBuffer& vertexBuffer, VertexBufferLayout& layout);
	void bind();
	void unbind();
};