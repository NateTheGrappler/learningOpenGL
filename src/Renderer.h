#pragma once
#include <glad/glad.h>
#include "shader.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "texture.h"
#include "Camera.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
    float m_deltaTime = 0.0f; //time between current frame and last frame
    float m_lastFrame = 0.0f; //time of last frame

public:
    void Draw(const VertexArray&, const IndexBuffer&, const Shader&, const std::vector<std::shared_ptr<Texture>>& textures, Camera& camera ) const;
    void enableBlending(bool yesorno);
    void clear();
    void clear(float r, float g, float b, float a);

    float calculateDeltaTime();



};
