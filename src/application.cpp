#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "Renderer.h"
#include <memory>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame


glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);

Camera camera(cameraPos);
float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;


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
    glViewport(0, 0, 800, 600); 

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

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
    

    //------------------------------Rendering while loop------------------
    Renderer renderer;
    renderer.enableBlending(true);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        //get delta time
        deltaTime = renderer.calculateDeltaTime();

        //process input
        processInput(window);
       
        //call the renderer class to draw
        renderer.clear(0.2f, 0.3f, 0.3f, 1.0f);
        renderer.Draw(VAO, IBO, shaderProgram, textures, camera);

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
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }

}

//the functions are called whenever the mouse is moved in the glfw window
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY; //reversed because y-coordinate go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}