#pragma once
#include <vector>

class VertexArray
{
private:
	unsigned int m_rendererID;

public:
	//constructor and deconstructor
	VertexArray();
	~VertexArray();

	//set attribute function
	void setAttribute();
	void bind();
	void unbind();
};