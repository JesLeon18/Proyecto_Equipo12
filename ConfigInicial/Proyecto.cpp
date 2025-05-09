/*Autores:          Yesenia / Gbriela
* Num. Cuenta: 317248683 / 317313521
* Proyecto Equipo 12
* Fecha: 07 de mayo de 2025
*/
#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 1200, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 4.5f, 20.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

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



glm::vec3 Light1 = glm::vec3(0);
//Anim
bool activateAnimation = false;
float rotationAngle = 0.0f;
float scaleOldDesks = 1.0f;
float newDesksYPos = 10.0f; // Posici�n inicial en el cielo
float animationTime = 0.0f;
const float animationDuration = 5.0f; // Duraci�n total de la animaci�n en segundos
float column_2 = 14.0f; //No quitar, es para el acomodo de modelos
float column_2n = 15.7f;
float altura = 0.5f;
float showComputersAndKeyboards = 0;

//piso bool
bool aparecePiso = true;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Equipo 12: Yesenia y Gabriela", nullptr, nullptr);

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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	//models
	Model Escritorios((char*)"Models/all_desk.obj");
	Model NewEscri((char*)"Models/all_new_desk.obj");
	Model Compu((char*)"Models/computadora.obj");
	Model Teclado((char*)"Models/teclado.obj");
	Model Silla((char*)"Models/silla.obj");
	Model SillaNueva((char*)"Models/chairNew.obj");
	Model Piso((char*)"Models/suelo.obj");
	Model Paredes((char*)"Models/Paredes.obj");
	Model Techo((char*)"Models/techo.obj");
	Model luz((char*)"Models/leds.obj");
	Model Ventana((char*)"Models/all_windows.obj");
	Model Pizza((char*)"Models/pizzaron.obj");
	Model bodega((char*)"Models/bodega.obj");
	Model emerg((char*)"Models/emerg.obj"); 
	Model cosoMadera((char*)"Models/cosoMadera.obj"); 
	Model puertaDer((char*)"Models/puertaDer.obj");
	Model puertaIzq((char*)"Models/puertaIzq.obj"); 
	Model VentanasNuevas((char*)"Models/VentanasNuevas.obj");
	Model sueloNuevo((char*)"Models/sueloNuevo.obj"); 
    Model columnaIzq((char*)"Models/columnaIzq.obj");
	Model columnaDer((char*)"Models/columnaDer.obj");


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

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp



		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);



		//Cargas de modelos 
		//Lamparas
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		luz.Draw(lightingShader);

		//Ventanas
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ventana.Draw(lightingShader);
		
		//Techo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Techo.Draw(lightingShader);

		//Paredes
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Paredes.Draw(lightingShader);

		if (scaleOldDesks > 0.0f) {
			model = glm::mat4(1);
			model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(scaleOldDesks));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Escritorios.Draw(lightingShader);
		}

		// Dibujar nuevos escritorios (si la animaci�n est� activa o completada)
		if (activateAnimation || scaleOldDesks == 0.0f) {
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, newDesksYPos, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			NewEscri.Draw(lightingShader);
		} 

		//Pizzaron
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pizza.Draw(lightingShader);

		if (showComputersAndKeyboards == 0) {
			//Compus
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			//Segunda columna
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);
			//Teclados
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			//Segunda columna
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);



		}
		else {
			if (showComputersAndKeyboards == 1)
			{
				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				////Segunda columna
				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				//Teclados nuevos

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f + altura, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f + altura, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f + altura, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f + altura, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f + altura, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f + altura, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f + altura, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f + altura, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f + altura, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f + altura, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f + altura, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f + altura, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f + altura, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f + altura, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f + altura, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f + altura, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f + altura, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f + altura, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f + altura, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f + altura, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f + altura, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f + altura, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f + altura, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f + altura, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f + altura, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f + altura, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f + altura, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f + altura, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f + altura, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f + altura, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f + altura, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f + altura, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				////Segunda columna
				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f + altura, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f + altura, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f + altura, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f + altura, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f + altura, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f + altura, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f + altura, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f + altura, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f + altura, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f + altura, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f + altura, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f + altura, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f + altura, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f + altura, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f + altura, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f + altura, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f + altura, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f + altura, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f + altura, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f + altura, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f + altura, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f + altura, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f + altura, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f + altura, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f + altura, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f + altura, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f + altura, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f + altura, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f + altura, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f + altura, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f + altura, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f + altura, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);
			}
			}

		
		//Sillaaaaaa
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 24.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 24.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 24.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 24.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.3f, 0.0f, -6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.45f, 0.0f, -6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.3f, 0.0f, -12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.45f, 0.0f, -12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.3f, 0.0f, -18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.45f, 0.0f, -18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		//Segunda columna
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 24.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 24.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 24.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 24.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, -6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, -6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, -6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, -6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, -12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, -12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, -12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, -12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, -18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, -18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, -18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, -18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);
		
		//SillaNuevaaaaaa
view = camera.GetViewMatrix();
model = glm::mat4(1);
model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-3.4f, 0.0f, 0.0f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-6.7f, 0.0f, 0.0f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);


view = camera.GetViewMatrix();
model = glm::mat4(1);
model = glm::translate(model, glm::vec3(0.0f, 0.0f, 6.2f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-3.4f, 0.0f, 6.2f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-6.7f, 0.0f, 6.2f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 6.2f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);


view = camera.GetViewMatrix();
model = glm::mat4(1);
model = glm::translate(model, glm::vec3(0.0f, 0.0f, 12.4f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-3.4f, 0.0f, 12.4f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-6.7f, 0.0f, 12.4f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 12.4f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);


view = camera.GetViewMatrix();
model = glm::mat4(1);
model = glm::translate(model, glm::vec3(0.0f, 0.0f, 18.6f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-3.4f, 0.0f, 18.6f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-6.7f, 0.0f, 18.6f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 18.6f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);


view = camera.GetViewMatrix();
model = glm::mat4(1);
model = glm::translate(model, glm::vec3(0.0f, 0.0f, 24.8f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-3.4f, 0.0f, 24.8f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-6.7f, 0.0f, 24.8f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 24.8f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);


view = camera.GetViewMatrix();
model = glm::mat4(1);
model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.2f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-3.4f, 0.0f, -6.2f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-6.7f, 0.0f, -6.2f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -6.2f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);


view = camera.GetViewMatrix();
model = glm::mat4(1);
model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.4f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-3.4f, 0.0f, -12.4f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-6.7f, 0.0f, -12.4f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -12.4f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);


view = camera.GetViewMatrix();
model = glm::mat4(1);
model = glm::translate(model, glm::vec3(0.0f, 0.0f, -18.6f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-3.4f, 0.0f, -18.6f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-6.7f, 0.0f, -18.6f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -18.6f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

//Segunda columna


view = camera.GetViewMatrix();
model = glm::mat4(1);
model = glm::translate(model, glm::vec3(0.0f + column_2n, 0.0f, 0.0f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-3.4f + column_2n, 0.0f, 0.0f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-6.7f + column_2n, 0.0f, 0.0f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-10.0f + column_2n, 0.0f, 0.0f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);


view = camera.GetViewMatrix();
model = glm::mat4(1);
model = glm::translate(model, glm::vec3(0.0f + column_2n, 0.0f, 6.2f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-3.4f + column_2n, 0.0f, 6.2f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-6.7f + column_2n, 0.0f, 6.2f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-10.0f + column_2n, 0.0f, 6.2f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);


view = camera.GetViewMatrix();
model = glm::mat4(1);
model = glm::translate(model, glm::vec3(0.0f + column_2n, 0.0f, 12.4f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-3.4f + column_2n, 0.0f, 12.4f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-6.7f + column_2n, 0.0f, 12.4f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-10.0f + column_2n, 0.0f, 12.4f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);


view = camera.GetViewMatrix();
model = glm::mat4(1);
model = glm::translate(model, glm::vec3(0.0f + column_2n, 0.0f, 18.6f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-3.4f + column_2n, 0.0f, 18.6f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-6.7f + column_2n, 0.0f, 18.6f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-10.0f + column_2n, 0.0f, 18.6f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);


view = camera.GetViewMatrix();
model = glm::mat4(1);
model = glm::translate(model, glm::vec3(0.0f + column_2n, 0.0f, 24.8f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-3.4f + column_2n, 0.0f, 24.8f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-6.7f + column_2n, 0.0f, 24.8f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-10.0f + column_2n, 0.0f, 24.8f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);


view = camera.GetViewMatrix();
model = glm::mat4(1);
model = glm::translate(model, glm::vec3(0.0f + column_2n, 0.0f, -6.2f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-3.4f + column_2n, 0.0f, -6.2f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-6.7f + column_2n, 0.0f, -6.2f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-10.0f + column_2n, 0.0f, -6.2f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);


view = camera.GetViewMatrix();
model = glm::mat4(1);
model = glm::translate(model, glm::vec3(0.0f + column_2n, 0.0f, -12.4f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-3.4f + column_2n, 0.0f, -12.4f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-6.7f + column_2n, 0.0f, -12.4f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-10.0f + column_2n, 0.0f, -12.4f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);


view = camera.GetViewMatrix();
model = glm::mat4(1);
model = glm::translate(model, glm::vec3(0.0f + column_2n, 0.0f, -18.6f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-3.4f + column_2n, 0.0f, -18.6f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-6.7f + column_2n, 0.0f, -18.6f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);

model = glm::mat4(1);
model = glm::translate(model, glm::vec3(-10.0f + column_2n, 0.0f, -18.6f));
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
SillaNueva.Draw(lightingShader);



		//bodega
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.15f + column_2, 0.0f, -0.3f));
		//model = glm::scale(model, glm::vec3(1.25f, 0.75f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		bodega.Draw(lightingShader);

		//emergencia
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.0f + column_2, 0.0f, -0.02f));
		//model = glm::scale(model, glm::vec3(1.25f, 0.75f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		emerg.Draw(lightingShader);

		//coso madera
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.0f + column_2, 0.0f, -0.02f));
		//model = glm::scale(model, glm::vec3(1.25f, 0.75f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cosoMadera.Draw(lightingShader);

		//puerta Izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.0f + column_2, 0.0f, -0.02f));
		//model = glm::scale(model, glm::vec3(1.25f, 0.75f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puertaIzq.Draw(lightingShader);

		//puerta Dere
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.0f + column_2, 0.0f, -0.02f));
		//model = glm::scale(model, glm::vec3(1.25f, 0.75f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puertaDer.Draw(lightingShader);

		//Ventanas nuevas
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.0f + column_2, 0.0f, -0.02f));
		//model = glm::scale(model, glm::vec3(1.25f, 0.75f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		VentanasNuevas.Draw(lightingShader); 

		

		if (aparecePiso == true) {
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Piso.Draw(lightingShader);
		}
		else {
			//Suelo nuevo
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-14.0f + column_2, 0.0f, -0.02f));
			//model = glm::scale(model, glm::vec3(1.25f, 0.75f, 0.0f)); 
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			sueloNuevo.Draw(lightingShader);
		}

		//Columna derecha 
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.0f + column_2, 0.0f, -0.02f));
		//model = glm::scale(model, glm::vec3(1.25f, 0.75f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		columnaDer.Draw(lightingShader);

		//Columna izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.0f + column_2, 0.0f, -0.02f));
		//model = glm::scale(model, glm::vec3(1.25f, 0.75f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		columnaIzq.Draw(lightingShader);

		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
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

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
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

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);

		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
	if (keys[GLFW_KEY_R] && !activateAnimation) {
		activateAnimation = true;
		animationTime = 0.0f;
	}
	if (keys[GLFW_KEY_T]) {
		aparecePiso = !aparecePiso;
	}
}
void Animation() {
	if (activateAnimation) {
		animationTime += deltaTime;
		float progress = animationTime / animationDuration;
		showComputersAndKeyboards = 2;

		if (progress < 0.5f) {
			// Fase 1: Rotaci�n y reducci�n de escala de los escritorios antiguos
			rotationAngle += 20.0f; // Giro r�pido
			scaleOldDesks = 1.0f - progress * 2.0f; // Reduce la escala hasta 0
		}
		else if (progress < 1.0f) {
			// Fase 2: Ca�da de los nuevos escritorios
			newDesksYPos = 10.0f - (progress - 0.5f) * 20.0f; // Cae desde Y=10 a Y=0
		}
		else {
			// Finalizar animaci�n
			activateAnimation = false;
			rotationAngle = 0.0f;
			scaleOldDesks = 0.0f;
			newDesksYPos = 0.0f;
			showComputersAndKeyboards = 1;
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

