#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
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
        //vertex data        //color data
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 1.0f
    };

    unsigned int indicies[] =
    {
        0, 1, 2, //first triangle
        2, 3, 0, //second triangle
    };


    //create the vertex buffer object, the index buffer object, and the vertex array object
    VertexArray VAO;
    VertexBuffer VBO(firstTriangle, 24 * sizeof(float));
    IndexBuffer IBO(indicies, 6);
    VertexBufferLayout layout;


    layout.push<float>(3); //vertex positions
    layout.push<float>(3); //vertex color
    VAO.setBufferAttribute(VBO, layout);
    

    //------------------------------Rendering while loop------------------
    Renderer renderer;
    while (!glfwWindowShouldClose(window))
    {
        //process input
        processInput(window);



        //call the renderer class to draw
        renderer.clear();
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