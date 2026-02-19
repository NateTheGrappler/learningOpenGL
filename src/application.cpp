#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "Renderer.h"
#include <memory>


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

    //------------------------------Texture code----------------------------
    std::vector<std::shared_ptr<Texture>> textures;
    auto texture1 = std::make_shared<Texture>("src/res/textures/VenusDirt.png");
    textures.push_back(texture1);


    //------------------------------Vertex code----------------------------
    {
    //set up verticies for triangle in a terms of normalized coordinates
    float firstTriangle[] =
    {
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 0
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 1
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 2
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 3

        // Back face
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 4
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 5
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 6
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 7

        // Left face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 8
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 9
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 10
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 11

        // Right face
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 12
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 13
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 14
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 15

         // Bottom face
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 16
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // 17
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 18
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 19

         // Top face
         -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, // 20
          0.5f,  0.5f, -0.5f,  1.0f, 0.0f, // 21
          0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 22
         -0.5f,  0.5f,  0.5f,  0.0f, 1.0f  // 23
    };

    unsigned int indicies[] =
    {
        // Front face
        0, 1, 2,
        2, 3, 0,
        // Back face
        4, 5, 6,
        6, 7, 4,
        // Left face
        8, 9, 10,
        10, 11, 8,
        // Right face
        12, 13, 14,
        14, 15, 12,
        // Bottom face
        16, 17, 18,
        18, 19, 16,
        // Top face
        20, 21, 22,
        22, 23, 20
    };


    //create the vertex buffer object, the index buffer object, and the vertex array object
    VertexArray VAO;
    VertexBuffer VBO(firstTriangle, sizeof(firstTriangle));
    IndexBuffer IBO(indicies, 36);
    VertexBufferLayout layout;


    layout.push<float>(3); //vertex positions
    layout.push<float>(2); //texture coords
    VAO.setBufferAttribute(VBO, layout);
    

    //------------------------------Vertex and Matrix code----------------------------
    

    ////create a new uniform matrix and set the given diagonal values of it to one
    //glm::mat4 trans = glm::mat4(1.0f);
    ////now set up that matrix to rotate, using that uniform matrix, and then the angle for the sin and cos, and then the axis about which to rotate which is z
    //trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ////now take that same matrix which is rotated, and scale it down by half in all axis
    //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    //
    //unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
    //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    //------------------------------Rendering while loop------------------
    Renderer renderer;
    renderer.enableBlending(true);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        //process input
        processInput(window);
       
        //call the renderer class to draw
        renderer.clear(0.2f, 0.3f, 0.3f, 1.0f);


        renderer.Draw(VAO, IBO, shaderProgram, textures);

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