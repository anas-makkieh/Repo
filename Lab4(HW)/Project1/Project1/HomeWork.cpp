
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// ================== SHADERS ==================

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform float offsetX;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x + offsetX, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0);\n"
"}\n\0";

// ================== CALLBACKS ==================

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// ================== MAIN ==================

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2D Game Scene", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    glewInit();

    // ================== SHADER PROGRAM ==================
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ================== OBJECT DATA ==================

    // Player (square)
    float playerVertices[] = {
        -0.1f, -0.1f, 0.0f,
         0.1f, -0.1f, 0.0f,
         0.1f,  0.1f, 0.0f,

         0.1f,  0.1f, 0.0f,
        -0.1f,  0.1f, 0.0f,
        -0.1f, -0.1f, 0.0f
    };

    // Ground
    float groundVertices[] = {
        -1.0f, -0.7f, 0.0f,
         1.0f, -0.7f, 0.0f,
         1.0f, -1.0f, 0.0f,

         1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        -1.0f, -0.7f, 0.0f
    };

    unsigned int VAO_player, VBO_player;
    glGenVertexArrays(1, &VAO_player);
    glGenBuffers(1, &VBO_player);

    glBindVertexArray(VAO_player);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_player);
    glBufferData(GL_ARRAY_BUFFER, sizeof(playerVertices), playerVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int VAO_ground, VBO_ground;
    glGenVertexArrays(1, &VAO_ground);
    glGenBuffers(1, &VBO_ground);

    glBindVertexArray(VAO_ground);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ground);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float playerX = 0.0f;

    // ================== RENDER LOOP ==================

    while (!glfwWindowShouldClose(window))
    {
        // Input
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            playerX -= 0.001f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            playerX += 0.001f;

        glClearColor(0.5f, 0.8f, 1.0f, 1.0f); // sky
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        int offsetLoc = glGetUniformLocation(shaderProgram, "offsetX");
        int colorLoc = glGetUniformLocation(shaderProgram, "ourColor");

            // Draw ground (green)
            glUniform1f(offsetLoc, 0.0f);
        glUniform3f(colorLoc, 0.2f, 0.8f, 0.3f);
        glBindVertexArray(VAO_ground);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Draw player (red)
        glUniform1f(offsetLoc, playerX);
        glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f);
        glBindVertexArray(VAO_player);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO_player);
    glDeleteBuffers(1, &VBO_player);
    glDeleteVertexArrays(1, &VAO_ground);
    glDeleteBuffers(1, &VBO_ground);

    glfwTerminate();
    return 0;
}