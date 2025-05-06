/*Autor: Lopez Hernandez Yesenia Sarahi
* Num. Cuenta: 317248683
* Practica 8: Materiales e Iluminacion
* Objetivo: Añadir un "sol" y una "luna" con sus respectiva iluminacion
* añadir un movimiento oscilatorio que tenga un limite de movimiento hasta el otro extremo 
* Fecha: 30 de Marzo de 2025
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
const GLint WIDTH = 1200, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

//Apagador de luces
bool light1n = true;
bool light2n = true;

// Light attributes
glm::vec3 lightPos(5.0f, 0.0f, 0.0f);
glm::vec3 lightPos2(6.0f, 0.0f, 0.0f);
float movelightPos = 0.0f;

//Movimiento
float	lunaM = 0.0f,
        solM = 0.0f;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;

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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Yesenia Lopez, Practica 8: Materiales e Iluminacion", nullptr, nullptr);

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
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");



    // Load models
    Model red_dog((char*)"Models/RedDog.obj");
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
    Model luna((char*)"Modelos/luna.obj");
    Model sol((char*)"Modelos/sol.obj");
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load textures

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);


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
        glClearColor(0.0f, 0.0f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.Use();
        float angle = glm::radians(lunaM); // el ángulo de movimiento de la luna
        float angle2 = glm::radians(solM); // ángulo de movimiento del sol
        lightPos.x = 4.2f * cos(angle);
        lightPos.y= 4.2f * sin(angle);
        lightPos2.x = 5.0f * cos(angle2);
        lightPos2.y = 5.0f * sin(angle2);
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint lightPosLoc2 = glGetUniformLocation(lightingShader.Program, "light2.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        //glUniform3f(lightPosLoc, lightPos.x + movelightPos + lunaM, lightPos.y + movelightPos, lightPos.z + movelightPos);
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(lightPosLoc2, lightPos2.x, lightPos2.y, lightPos2.z);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        
        // Set lights properties
        if (light1n) {
            //Luna
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.2f, 0.2f, 0.6f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.1f, 0.1f, 0.8f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.3f, 0.8f, 1.0f);
        }
        else {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.0f, 0.0f, 0.0f);
        }
        
        //Segunda luz
        //Sol
        if (light2n) {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.4f, 0.2f, 0.1f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.9f, 0.3f, 0.2f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.6f, 1.0f, 1.0f);
        }
        else {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.0f, 0.0f, 0.0f);
        }
        

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.5f);


        // Draw the loaded model
        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f)); // Posición del perro
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        red_dog.Draw(lightingShader);

        // Draw the arbol model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.5f, -0.15f, 1.2f)); // Posición del árbol
        model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f)); // Escala del árbol
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(lightingShader);

        // Draw the arbol model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.0f, -0.1f, -0.2f)); // Posición del árbol
        model = glm::scale(model, glm::vec3(0.95f, 0.95f, 0.95f)); // Escala del árbol
        model = glm::rotate(model, 3.14f, glm::vec3(0.0f, 1.0f, 0.0f)); //Rotacion del árbol
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(lightingShader);

        // Draw the arbol model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.1f, -0.1f, 0.7f)); // Posición del árbol
        model = glm::scale(model, glm::vec3(0.83f, 0.83f, 0.83f)); // Escala del árbol
        model = glm::rotate(model, 3.14f, glm::vec3(0.0f, 0.2f, 0.0f)); //Rotacion del árbol
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(lightingShader);

        // Draw the arbol model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.3f, -1.0f)); // Posición del árbol
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f)); // Escala del árbol
        model = glm::rotate(model, 3.14f, glm::vec3(0.0f, 0.8f, 0.0f)); //Rotacion del árbol
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(lightingShader);

        // Draw the pino model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.4f, -0.3f, -0.8f));
        model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pino2.Draw(lightingShader);

        //Draw the pino model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2.2f, -0.3f, -1.4f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pino.Draw(lightingShader);

        //Draw the pino model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.4f, -0.3f, 1.8f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pino2.Draw(lightingShader);

        //Draw the pino model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.4f, -0.3f, 0.8f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pino3.Draw(lightingShader);

        //Draw the pino model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(2.4f, -0.3f, 0.6f));
        model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pino.Draw(lightingShader);

        // Draw the suelo model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        suelo.Draw(lightingShader);

        // Draw the roca model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.1f, -0.4f, 0.2f));
        model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f)); // Escala de la roca
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        roca.Draw(lightingShader);

        // Draw the roca2 model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.2f));
        model = glm::scale(model, glm::vec3(0.65f, 0.65f, 0.65f)); // Escala de la segunda roca
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        roca2.Draw(lightingShader);

        // Draw the planta model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.0f, -0.3f, 0.0f));
        model = glm::scale(model, glm::vec3(0.95f, 1.2f, 0.95f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        planta.Draw(lightingShader);

        // Draw the planta model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 3.0f));
        model = glm::scale(model, glm::vec3(0.95f, 0.95f, 0.95f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        planta2.Draw(lightingShader);

        // Draw the tronco model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.90f, 0.90f, 0.90f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tronco.Draw(lightingShader);

        // Draw the arbusto model
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.15f, -2.5f));
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbusto.Draw(lightingShader);


        glBindVertexArray(0);

        //Creacion de nuestra iluminacion
        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        //Luna
        model = glm::mat4(1.0f);
       // model = glm::translate(model, lightPos + movelightPos);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.7f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        luna.Draw(lightingShader);
        glBindVertexArray(0);

        //Sol
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos2);
        model = glm::scale(model, glm::vec3(0.8f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        sol.Draw(lightingShader);
        glBindVertexArray(0);



        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
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

    if (keys[GLFW_KEY_O])
    {

        movelightPos += 0.1f;
    }

    if (keys[GLFW_KEY_L])
    {

        movelightPos -= 0.1f;
    }

    //Controlar luces
    if (action == GLFW_PRESS) {
        //Cotrol de la luna
        if (key == GLFW_KEY_K) // Alternar la luz 1
            light1n = !light1n;
        //Control de el sol
        if (key == GLFW_KEY_I) // Alternar la luz 2
            light2n = !light2n;
    }

    //Movimiento oscilatorio 
    if (keys[GLFW_KEY_R])
        if (lunaM < 190.0f)
            lunaM += 0.6f;
    if (keys[GLFW_KEY_F])
        if (lunaM > -10.0f)
            lunaM -= 0.6f;
    if (keys[GLFW_KEY_T])
        if (solM < 190.0f)
            solM += 0.6f;
    if (keys[GLFW_KEY_G])
        if (solM > -10.0f)
            solM -= 0.6f;
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