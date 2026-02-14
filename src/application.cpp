#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "shader.h"


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

    float offset = 0.5;
    shaderProgram.use();
    shaderProgram.setUniformFloat("offset", offset);

    //------------------------------Vertex code----------------------------
   
    //set up verticies for triangle in a terms of normalized coordinates
    float firstTriangle[] =
    {
        //vertex data        //color data
        -0.8f, -0.8f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.8f, -0.8f, 0.0f,  0.0f, 1.0f, 0.0f,
         0.0f,  0.8f, 0.0f,  0.0f, 0.0f, 1.0f
    };

    unsigned int indicies[] =
    {
        0, 1, 3, //first triangle
        1, 2, 3  //second triangle
    };

    //create vertex buffer object and vertex array object
    unsigned int VBO, VAO;
    unsigned int EBO;

    //generate both the vertex object and array
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //bind both the vertex array and then bind/set the vertex buffers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    
    //the index where the attribute is specified, the type of data stored there, if you want to normalize it
    //the offset in bytes between this data point and the next one (stride), offset from the start of the array to where the first attribute you want is at
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    //unbind the data after everything
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //this line draws the wireframe polygons
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    //------------------------------Rendering while loop------------------
    while (!glfwWindowShouldClose(window))
    {
        //process input
        processInput(window);

        //render functions
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use(); //use the shader class in order to specify which shader program we should use
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);

        //swap render buffers and poll key press events
        glfwSwapBuffers(window); 
        glfwPollEvents();        
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
}