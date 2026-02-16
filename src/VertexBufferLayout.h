#pragma once

#include "glad\glad.h"
#include <vector>

struct LayoutElements
{
	//the data stored in struct
	unsigned int  type;
	unsigned int  count;
	unsigned char normalized;

	//constructor
	LayoutElements(unsigned int t, unsigned int c, unsigned char n)
		: type(t), count(c), normalized(n) {}

	//function to calculate stride in buffer element
	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:	       return 4;
			case GL_UNSIGNED_INT:  return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		return 0;
	}

};

class VertexBufferLayout
{
private:
	//store the data for the layout as well as stride
    std::vector<LayoutElements> m_elements;
	unsigned int m_stride;

public:
	VertexBufferLayout()
		: m_stride(0) {}


	//create a boilerplate function that can take in several different data types
	template<typename t>
	void push(unsigned int count)
	{
		static_assert(false);
	}

	//fill in that boilerplate with other types of data
	template<>
	void push<float>(unsigned int count)
	{
		m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride += count * LayoutElements::getSizeOfType(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += count * LayoutElements::getSizeOfType(GL_UNSIGNED_INT);
	}
	template<>
	void push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += count * LayoutElements::getSizeOfType(GL_UNSIGNED_BYTE);
	}

	//the functions that return the private data pf the class
	inline const std::vector<LayoutElements> getElements() const& { return m_elements; }
	inline const unsigned int getStride() const { return m_stride; }
};
