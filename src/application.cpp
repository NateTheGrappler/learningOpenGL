#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "Renderer.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


//settings
const unsigned int screenWidth  = 800;
const unsigned int screenHeight = 600;

int main(void)
{
    glfwInit();                                                                                           //init the window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                                                       //configure whatever is specified, and then choosen option from given enum
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                                                       //configure whatever is specified, and then choosen option from given enum
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);         //intialize the window, and then make sure that it was created successfully
    if (window == NULL)
    {
        std::cout << "Failed to create the window you goober" << std::endl;
        glfwTerminate();
        return -1;
    }
    //set up changing the viewport whenever you want to resize
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))                                             //Initialize the glad library
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);                                                                         //set the viewport of the window so that way open gl knows the size and position of the render


    //------------------------------shader code---------------------------

    Shader shaderProgram("src/res/shaders/Basic.shader");

    //------------------------------Vertex code----------------------------
    {
    //set up verticies for triangle in a terms of normalized coordinates
    float firstTriangle[] =
    {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indicies[] =
    {
        0, 1, 2, //first triangle
        2, 3, 0, //second triangle
    };


    //create the vertex buffer object, the index buffer object, and the vertex array object
    VertexArray VAO;
    VertexBuffer VBO(firstTriangle, 32 * sizeof(float));
    IndexBuffer IBO(indicies, 6);
    VertexBufferLayout layout;


    layout.push<float>(3); //vertex positions
    layout.push<float>(3); //vertex color
    layout.push<float>(2); //texture coords
    VAO.setBufferAttribute(VBO, layout);
    
    //------------------------------Texture code----------------------------

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load("src/res/textures/brick.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum imageFormat = GL_RGB;
        if (nrChannels == 1) { imageFormat = GL_RED; }
        else if (nrChannels == 3) { imageFormat = GL_RGB; }
        else if (nrChannels == 4) { imageFormat = GL_RGBA; }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Unable to get the image data" << std::endl;
    }
    stbi_image_free(data);



    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width2, height2, nrChannels2;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data2 = stbi_load("src/res/textures/VenusDirt.png", &width2, &height2, &nrChannels2, 0);
    if (data2)
    {
        GLenum imageFormat2 = GL_RGB;
        if (nrChannels2 == 1) { imageFormat2 = GL_RED; }
        else if (nrChannels2 == 3) { imageFormat2 = GL_RGB; }
        else if (nrChannels2 == 4) { imageFormat2 = GL_RGBA; }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, imageFormat2, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Unable to get the image data" << std::endl;
    }
    stbi_image_free(data2);


    //------------------------------Rendering while loop------------------
    Renderer renderer;
    shaderProgram.use();
    shaderProgram.setUniformInt("texture1", 0);
    shaderProgram.setUniformInt("texture2", 1);
    while (!glfwWindowShouldClose(window))
    {
        //process input
        processInput(window);



        //call the renderer class to draw
        renderer.clear(0.2f, 0.3f, 0.3f, 1.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        renderer.Draw(VAO, IBO, shaderProgram);

        //swap render buffers and poll key press events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    }

    //close glfw cleanly
    glfwTerminate();
    return 0;
}


//function to handle viewport resizing with window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//handle input detected from user
void processInput(GLFWwindow* window)
{
    //if escape key pressed, close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        //get the polygon mode
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, &polygonMode[0]);

        if (polygonMode[0] == GL_LINE)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else if (polygonMode[0] == GL_FILL)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    }
}