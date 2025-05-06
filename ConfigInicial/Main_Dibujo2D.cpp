/*Autor: Lopez Hernandez Yesenia Sarahi
* Num. Cuenta: 317248683
* Practica 2: Dibujo de Primitivas
* Objetivo: Realizar el dibujo de un elefante de origami con primitivas
* Fecha: 13 de Febrero de 2025
*/

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 2: Dibujo de primitivas", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	//Se puso los vertices basado en el analisis del dibujo echo en geogebra
	float vertices[] = {
		0.2f,  0.8f, 0.0f,		0.5f,0.5f,0.5f,  // c 0
		0.36f, 0.69f, 0.0f,		0.40f,0.40f,0.40f,//g 1
		0.47f, 0.62f, 0.0f,		0.5f,0.5f,0.5f,  // d 2
		0.47f, 0.27f, 0.0f,		0.5f,0.5f,0.5f,  // e 3
		0.23f, 0.0f, 0.0f,		0.5f,0.5f,0.5f, // f 4
		0.51f, 0.81f, 0.0f,		0.5f,0.5f,0.5f, //h 5
		0.72f, 0.81f, 0.0f,		0.5f,0.5f,0.5f, //i 6
		1.0f, 0.55f, 0.0f,		0.5f,0.5f,0.5f, //j 7
		1.0f, 0.31f, 0.0f,		0.5f,0.5f,0.5f, //k 8
		0.69f, 0.0f, 0.0f,		0.5f,0.5f,0.5f,//l 9
		0.51f, 0.03f, 0.0f,		0.5f,0.5f,0.5f,//m 10
		0.37f, 0.15f, 0.0f,		0.35f,0.35f,0.35f,//n 11
		0.16f, 0.0f,0.0f,		0.45f,0.45f,0.45f,//o 12
		0.16f, -0.18f, 0.0f,	0.52f,0.52f,0.52f, //p 13
		0.07f,-0.3f, 0.0f,		0.5f,0.5f,0.5f, //q 14
		0.55f, -0.3f, 0.0f,		0.5f,0.5f,0.5f,//r 15
		0.55, -0.37f, 0.0f,		0.5f,0.5f,0.5f, //s 16
		0.26f, -0.7f, 0.7f,		0.5f,0.5f,0.5f, //t 17
		0.15f, -0.7f, 0.0f,		0.5f,0.5f,0.5f, //u 18
		0.0f, -0.6f, 0.0f,		0.35f,0.35f,0.35f,//v 19
		0.0f, -0.3f, 0.0f,		0.5f,0.5f,0.5f,//w 20
		0.73f,-0.6f, 0.0f,		0.5f,0.5f,0.5f,//z 21
		0.32f, -0.6f,0.0f,		0.5f,0.5f,0.5f,//a1	22
		-0.55, -0.37f, 0.0f,	0.5f,0.5f,0.5f, //-s 23
		-0.73f, -0.6f, 0.0f,	0.5f,0.5f,0.5f,//-z 24
		-0.32f, -0.6f,0.0f,		0.40f,0.40f,0.40f,//-a1	25
		-0.2f, 0.8f, 0.0f,		0.5f,0.5f,0.5f, //-c 26
		-0.47f, 0.62f, 0.0f,	0.5f,0.5f,0.5f,  // d 27
		-0.47f, 0.27f, 0.0f,	0.5f,0.5f,0.5f,  // e 28
		-0.23f, 0.0f, 0.0f,		0.5f,0.5f,0.5f, // f 29
		-0.51f, 0.81f, 0.0f,	0.5f,0.5f,0.5f, //h 30
		-0.72f, 0.81f, 0.0f,	0.5f,0.5f,0.5f, //i 31
		-1.0f, 0.55f, 0.0f,		0.5f,0.5f,0.5f, //j 32
		-1.0f, 0.31f, 0.0f,		0.5f,0.5f,0.5f, //k 33
		-0.69f, 0.0f, 0.0f,		0.5f,0.5f,0.5f,//l 34
		-0.51f, 0.03f, 0.0f,	0.5f,0.5f,0.5f,//m 35
		-0.37f, 0.15f, 0.0f,	0.35f,0.35f,0.35f,//n 36
		-0.16f, 0.0f,0.0f,		0.45f,0.45f,0.45f,//o 37
		-0.16f, -0.18f, 0.0f,	0.52f,0.52f,0.52f, //p 38
		-0.07f,-0.3f, 0.0f,		0.5f,0.5f,0.5f, //q 39
		-0.55f, -0.3f, 0.0f,	0.5f,0.5f,0.5f,//r 40
		-0.26f, -0.7f, 0.7f,	0.5f,0.5f,0.5f, //t 41
		-0.15f, -0.7f, 0.0f,	0.5f,0.5f,0.5f, //u 42
		0.0f, -0.6f, 0.0f,		0.5f,0.5f,0.5f,//b 43
		 0.0f, 0.66f, 0.0f,		0.35f,0.35f,0.35f,//b1	44
		-0.36f, 0.69f, 0.0f,	0.40f,0.40f,0.40f,//g 45
		0.55, -0.37f, 0.0f,		0.35f,0.35f,0.35f,// 46


	};
	unsigned int indices[] = {  // note that we start from 0!
		46,21,22, //Pata trasera derecha
		23,24,25, //Pata trasera izquierda

		1,11,7, //oreja derecha
		1,7,5,
		5,7,6,
		11,7,8,
		11,8,9,
		11,9,10,

		36,45,32, //oreja izquierda
		32,45,30,
		30,32,31,
		32,36,33,
		36,33,34,
		34,36,35,

		11,44,19, //Pata derecha
		11,19,15,
		19,15,16,
		19,16,17,
		19,17,18,

		36,44,43, //Pata Izquierda
		36,40,43,
		40,43,23,
		43,23,41,
		41,43,42,

		0,26,2, //cabeza
		2,26,27,
		27,2,3,
		3,27,28,
		3,28,4,
		28,29,4,

		12,37,20,//Trompa
		12,20,13,
		13,20,14,
		37,20,38,
		38,39,20,

	};



	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
        ourShader.Use();
        glBindVertexArray(VAO);

		//Solo se uso la realizacion de triangulos para crear la figura
        glDrawElements(GL_TRIANGLES, 105,	GL_UNSIGNED_INT, 0);

		

        
        
        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}