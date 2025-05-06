/*Autor: Lopez Hernandez Yesenia Sarahi
* Num. Cuenta: 317248683
* Practica 6: Carga de modelos
* Objetivo: Realizar la carga creacion de un escenario donde situemos al perro rojo en 3D
* Fecha: 16 de Marzo de 2025
*/

// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Yesenia Lopez: Practica 6: Carga de modelos y camara sintetica", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");

    // Load models
    Model dog((char*)"Models/RedDog.obj");
    Model arbol((char*)"Modelos/Arbol.obj");
    Model pino((char*)"Modelos/pino.obj");
    Model pino2((char*)"Modelos/pino2.obj");
    Model pino3((char*)"Modelos/pino3.obj");
    Model suelo((char*)"Modelos/Suelo.obj");
    Model roca((char*)"Modelos/roca.obj");
    Model roca2((char*)"Modelos/Roca2.obj");
    Model tronco((char*)"Modelos/Tronco.obj");
    Model planta((char*)"Modelos/Planta.obj");
    Model planta2((char*)"Modelos/Suegra.obj");
    Model arbusto((char*)"Modelos/Arbusto.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.35f, 0.55f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f)); // Posición del perro
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(shader);

        // Draw the arbol model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.5f, -0.15f, 1.2f)); // Posición del árbol
        model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f)); // Escala del árbol
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(shader);

        // Draw the arbol model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.0f, -0.1f, -0.2f)); // Posición del árbol
        model = glm::scale(model, glm::vec3(0.95f, 0.95f, 0.95f)); // Escala del árbol
        model = glm::rotate(model, 3.14f, glm::vec3(0.0f, 1.0f, 0.0f)); //Rotacion del árbol
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(shader);

        // Draw the arbol model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.1f, -0.1f, 0.7f)); // Posición del árbol
        model = glm::scale(model, glm::vec3(0.83f, 0.83f, 0.83f)); // Escala del árbol
        model = glm::rotate(model, 3.14f, glm::vec3(0.0f, 0.2f, 0.0f)); //Rotacion del árbol
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(shader);

        // Draw the arbol model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.3f, -1.0f)); // Posición del árbol
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f)); // Escala del árbol
        model = glm::rotate(model, 3.14f, glm::vec3(0.0f, 0.8f, 0.0f)); //Rotacion del árbol
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(shader);

        // Draw the pino model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.4f, -0.3f, -0.8f)); 
        model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f)); 
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pino2.Draw(shader);
        
        //Draw the pino model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2.2f, -0.3f, -1.4f)); 
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pino.Draw(shader);

        //Draw the pino model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.4f, -0.3f, 1.8f)); 
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pino2.Draw(shader);

        //Draw the pino model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.4f, -0.3f, 0.8f)); 
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pino3.Draw(shader);

        //Draw the pino model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(2.4f, -0.3f, 0.6f)); 
        model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f)); 
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pino.Draw(shader);

        // Draw the suelo model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f)); 
        model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f)); 
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        suelo.Draw(shader);

        // Draw the roca model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.1f, -0.4f, 0.2f));
        model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f)); // Escala de la roca
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        roca.Draw(shader);

        // Draw the roca2 model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.2f));
        model = glm::scale(model, glm::vec3(0.65f, 0.65f, 0.65f)); // Escala de la segunda roca
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        roca2.Draw(shader);

        // Draw the planta model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.0f, -0.3f, 0.0f));
        model = glm::scale(model, glm::vec3(0.95f, 1.2f, 0.95f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        planta.Draw(shader);

        // Draw the planta model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 3.0f));
        model = glm::scale(model, glm::vec3(0.95f, 0.95f, 0.95f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        planta2.Draw(shader);

        // Draw the tronco model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.90f, 0.90f, 0.90f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tronco.Draw(shader);

        // Draw the arbusto model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.15f, -2.5f));
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbusto.Draw(shader);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }


}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }




}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}