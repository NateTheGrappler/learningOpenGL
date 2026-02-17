#pragma once

#include <iostream>
#include "Renderer.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
		" " << file << ": " << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Draw(const VertexArray& VAO, const IndexBuffer& IBO, const Shader& shader, const std::vector<std::shared_ptr<Texture>>& textures) const
{
	//bind all the needed things to draw in opengl
	shader.use();

	//bind all of the textures in one of the objects drawn
	shader.setUniformInt("texture1", 0);
	shader.setUniformInt("texture2", 1);
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		textures[i]->bind();
	}

	IBO.bind();
	VAO.bind();
	glDrawElements(GL_TRIANGLES, IBO.getCount(), GL_UNSIGNED_INT, nullptr);
}
void Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}
void Renderer::clear(float r, float g, float b, float a)
{
	//render functions
	glClearColor(r,g,b,a);
	glClear(GL_COLOR_BUFFER_BIT);
}
