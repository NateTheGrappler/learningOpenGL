#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


//struct that holds the source for the shaders
struct ShaderPrograms
{
    std::string VertexSource;
    std::string FragementSource;
    std::string YellowFragementSource;
};
static ShaderPrograms parseShader(std::string filePath)
{

    //enum class to store the shader types
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGEMENT=1, FRAGEMENTYELLOW=2
    };


    //set up filepath as well as buffer to toss string data into
    std::ifstream stream(filePath);
    std::stringstream ss[3];
    ShaderType type = ShaderType::NONE;
    

    //read file line by line
    std::string line;
    while (getline(stream, line))
    {

        //check to see if the .find() function finds the position of said string
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                //set as vertex shader
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragement") != std::string::npos)
            {
                //set as fragment shader
                type = ShaderType::FRAGEMENT;
            }
            else if (line.find("fragmentYellow") != std::string::npos)
            {
                //set as fragment shader
                type = ShaderType::FRAGEMENTYELLOW;
            }
        }
        else
        {
            //at given shader type, index into array
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str(), ss[2].str() };
}
static unsigned int compileShader(unsigned int type, const std::string& source)
{
    //create the shader and compile it given the type of shader it is and the source code of the shader
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    //use glGetShaderiv in order to check if the shader compiled correctly or not
    int result;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Failed to initialize "  << (type == GL_FRAGMENT_SHADER ? "fragement" : "vertex") << " shader " << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    //return the compiled shader if successful
    return shader;
}
static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    //create the shader program, and then also compile both your vertex and frag shaders
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //link the compiled shaders to the shader program, then compile shaderprogram
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    //delete the now uneeded vertex and fragement shaders as they are stored in program
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


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
    
    ShaderPrograms programs = parseShader("src/res/shaders/Basic.shader");
    std::cout << programs.VertexSource << std::endl;
    std::cout << programs.FragementSource << std::endl;
    std::cout << programs.YellowFragementSource << std::endl;

    unsigned int shaderProgram = createShader(programs.VertexSource, programs.FragementSource);
    unsigned int yellowShaderProgram = createShader(programs.VertexSource, programs.YellowFragementSource);


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

        glUseProgram(yellowShaderProgram);
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