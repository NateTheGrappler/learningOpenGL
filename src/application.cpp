#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


//vertex shader code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//fragment shader code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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
    //create a var and then use that far to store the source code for the given shader created before, passing in ssaid code, and the compiling it
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //check to see if your shader compiled correctly
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Failed to initialize vertex shader " << infoLog << std::endl;
    }

    //now do the same for the fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Failed to initialize fragment shader " << infoLog <<  std::endl;
    }

    //lastly create a shader program that is used for the actual rendering, and then link the complied shaders into it
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //check to see if shader program was linked correctly
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "shader program linking failed: " << infoLog << std::endl;
    }
    
    //delete the shader objects given that they arent needed after program is formed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    //------------------------------Vertex code----------------------------
   
    //set up verticies for triangle in a terms of normalized coordinates
    float firstTriangle[] =
    {
        //first triangle
        -0.9f, -0.5f, 0.0f,  // left 
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f  // top
    };
    float secondTriangle[] =
    {
        //second triangle
        -0.0f, -0.5f, 0.0f, //left
         0.9f, -0.5f, 0.0f, //right
         0.45f,  0.5f, 0.0f //top
    };

    unsigned int indicies[] =
    {
        0, 1, 3, //first triangle
        1, 2, 3  //second triangle
    };

    //create vertex buffer object and vertex array object
    unsigned int VBO[2], VAO[2];
    unsigned int EBO;

    //generate both the vertex object and array
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(1, &EBO);

    //bind both the vertex array and then bind/set the vertex buffers
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //second triangle set up
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //unbind the data after everything
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

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

        glUseProgram(shaderProgram); //specify open gl to use the fragement and vertex shaders we defined earlier
        
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(VAO[1]);
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