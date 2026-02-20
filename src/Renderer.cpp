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
	glm::vec3 cubePositions[] = {
	glm::vec3( 0.0f,  0.0f,   0.0f),
	glm::vec3( 3.0f,  5.0f,  -15.0f),
	glm::vec3(-2.5f, -2.2f,  -2.5f),
	glm::vec3(-4.8f, -2.0f,  -12.3f),
	glm::vec3( 3.4f, -0.4f,  -3.5f),
	glm::vec3(-2.7f,  3.0f,  -7.5f),
	glm::vec3( 2.3f, -2.0f,  -2.5f),
	glm::vec3( 2.5f,  2.0f,  -2.5f),
	glm::vec3( 2.5f,  0.2f,  -1.5f),
	glm::vec3(-2.3f,  1.0f,  -1.5f),
	glm::vec3(-0.0f,  3.0f, -5.0f)
	};

	//bind all the needed things to draw in opengl
	shader.use();

	//bind all of the textures in one of the objects drawn
	shader.setUniformInt("texture1", 0);
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		textures[i]->bind();
	}


	//the math for project in 3d space
	glm::mat4 view       = glm::mat4(1.0f);
	float radius = 10.0f;
	float camX = static_cast<float>(sin(glfwGetTime()) * radius);
	float camY = static_cast<float>(cos(glfwGetTime() * 0.5f) * radius);
	float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
	view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("view", view);


	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	shader.setMat4("projection", projection);

	float time = (float)glfwGetTime();

	IBO.bind();
	VAO.bind();

	for (unsigned int i = 0; i < 11; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, cubePositions[i]);
		//model = glm::scale(model, glm::vec3(scaleAmount, scaleAmount, scaleAmount));

		// Make cubes rotate based on time
		// Each cube rotates at a different speed by using (i+1) as a multiplier
		if (i % 2 == 0)
		{
			float angle = time * 20.0f * (i + 1); // Speed multiplier based on cube index
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		}

		shader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, IBO.getCount(), GL_UNSIGNED_INT, nullptr);
	}

	
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::clear(float r, float g, float b, float a)
{
	//render functions
	glClearColor(r,g,b,a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
