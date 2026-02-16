#pragma once

class IndexBuffer
{
private:
	unsigned int m_rendererID;
	unsigned int m_count;

public:
	IndexBuffer(const unsigned int* dataArray, unsigned int count);
	~IndexBuffer();

	void unbind() const;
	void bind() const;

	inline unsigned int getCount() const { return m_count; }
};