#pragma once

#include <iostream>
#include "Renderer.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		textures[i]->bind();
	}

	glm::mat4 trans = glm::mat4(1.0f);

	// Set the transform uniform
	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	IBO.bind();
	VAO.bind();
	glDrawElements(GL_TRIANGLES, IBO.getCount(), GL_UNSIGNED_INT, nullptr);


	//reset the matrix and then set a whole new set of transforms
	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
	float scaleAmount = static_cast<float>(sin(glfwGetTime()));

	trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, scaleAmount));
	unsigned int transformLoc1 = glGetUniformLocation(shader.ID, "transform");
	glUniformMatrix4fv(transformLoc1, 1, GL_FALSE, glm::value_ptr(trans));

	IBO.bind();
	VAO.bind();
	glDrawElements(GL_TRIANGLES, IBO.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::enableBlending(bool yesorno)
{
	//based on input either disable blending or enable it
	if (yesorno)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}
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
