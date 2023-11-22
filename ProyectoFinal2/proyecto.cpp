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

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 lightPos1(0.0f, 0.0f, 0.0f);
glm::vec3 lightDir(0.0f, 0.0f, 0.0f);
bool active;


// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(1.0f, 18.0f, -60.0f),//luz de la lambapara
	glm::vec3(0.0f, -1.37f, -60.0f), //luz de la fogata
	glm::vec3(0.0f, 18.5f, -110.0f),//luz del ventilador
	glm::vec3(-11.76f, 18.0f, -92.0f),//luz del segundo certo pimera parte
	glm::vec3(11.76f, 18.0f, -92.0f),//luz del segundo cuanrto segunda parrt
	glm::vec3(-11.76f, 18.0f, -120.0f),
	glm::vec3(11.76f, 18.0f, -120.0f)
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
glm::vec3 Light2 = glm::vec3(0);
glm::vec3 Light3 = glm::vec3(0);
glm::vec3 Light4 = glm::vec3(0);
glm::vec3 Light5 = glm::vec3(0);
glm::vec3 Light6 = glm::vec3(0);
glm::vec3 Light7 = glm::vec3(0);



// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame
float rotPIz = 0;
float rotPDe = 0;
float rotVen = 0;
float movCa = 0;
float tiempo;
float speed;
float rotAvi = 0.0f;
float rotAvnion = 0.0f;

bool activar;
bool AnimPIA = false;
bool AnimPDA = false;
bool AnimPIC = false;
bool AnimPDC = false;
bool AnimVen = false;
bool AnimCaA = false;
bool animcaC = false;
bool animAvi = false;
bool animAvion = false;



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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto final ", nullptr, nullptr);

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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



	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader anim("Shaders/anim.vs", "Shaders/anim.frag");





	Model museo((char*)"Models/ModelosN/museo.obj");
	Model piso((char*)"Models/ModelosN/Piso.obj");
	Model arbol((char*)"Models/ModelosN/arbol.obj");
	Model MarcoP((char*)"Models/ModelosN/MarcoPuerta.obj");
	Model PuertaIz((char*)"Models/ModelosN/PuertaIz.obj");
	Model PuertaD((char*)"Models/ModelosN/PuertaD.obj");
	//interior del mueso 
	//primera sala
	Model lampara((char*)"Models/ModelosN/Lampara1.obj");
	Model letrero((char*)"Models/ModelosN/Letrero.obj");
	Model ArmaHueso((char*)"Models/ModelosN/ArmaHueso.obj");
	Model armacurva((char*)"Models/ModelosN/armacurv.obj");
	Model piedras((char*)"Models/ModelosN/piedras.obj");
	Model antorchas((char*)"Models/ModelosN/antorchas.obj");
	Model leña((char*)"Models/ModelosN/madera.obj");
	Model fuego((char*)"Models/ModelosN/fuego.obj");
	Model flechas((char*)"Models/ModelosN/Flechas3.obj");
	Model urna1((char*)"Models/ModelosN/urna1.obj");
	Model urna2((char*)"Models/ModelosN/urna2.obj");
	Model urna3((char*)"Models/ModelosN/urna3.obj");
	Model lanzas((char*)"Models/ModelosN/lanzas.obj");
	Model MazoPicos((char*)"Models/ModelosN/mazoPicos.obj");
	//segudna sala 
	Model vidrio((char*)"Models/ModelosN/vidrio.obj");
	Model vidrio2((char*)"Models/ModelosN/vidrio2.obj");
	Model misil((char*)"Models/ModelosN/misil.obj");
	Model pistola((char*)"Models/ModelosN/Pistola.obj");
	Model rifle((char*)"Models/ModelosN/rifle.obj");
	Model ventiB((char*)"Models/ModelosN/ventiladorBase.obj");
	Model ventiA((char*)"Models/ModelosN/ventiladorAspas.obj");
	Model casco((char*)"Models/ModelosN/casco.obj");
	Model bazuca((char*)"Models/ModelosN/bazuca.obj");
	Model ametralladora((char*)"Models/ModelosN/ametralladora.obj");
	Model Mesa((char*)"Models/ModelosN/Mesa.obj");
	Model Herramienta((char*)"Models/ModelosN/Herramienta.obj");
	Model Monitor((char*)"Models/ModelosN/Monitor.obj");
	Model letrero2((char*)"Models/ModelosN/letrero2.obj");
	Model tanque((char*)"Models/ModelosN/tanque.obj");
	Model avion((char*)"Models/ModelosN/avion.obj");


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

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = glfwGetTime() * Light1.x;
		lightColor.y = glfwGetTime() * Light1.y;
		lightColor.z = glfwGetTime() * Light1.z;


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.05f, 0.05f, 0.05f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 1.8f);

		glm::vec3 lightColor1;
		lightColor1.x = glfwGetTime() * Light2.x;
		lightColor1.y = glfwGetTime() * Light2.y;
		lightColor1.z = glfwGetTime() * Light2.z;

		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lightColor1.x, lightColor1.y, lightColor1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lightColor1.x, lightColor1.y, lightColor1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.06f, 0.00f, 0.00f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 1.8f);


		glm::vec3 lightColor2;
		lightColor2.x = glfwGetTime() * Light3.x;
		lightColor2.y = glfwGetTime() * Light3.y;
		lightColor2.z = glfwGetTime() * Light3.z;
		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.05f, 0.05f, 0.05f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 1.8f);


		glm::vec3 lightColor3;
		lightColor3.x = glfwGetTime() * Light4.x;
		lightColor3.y = glfwGetTime() * Light4.y;
		lightColor3.z = glfwGetTime() * Light4.z;
		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), lightColor3.x, lightColor3.y, lightColor3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), lightColor3.x, lightColor3.y, lightColor3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.05f, 0.05f, 0.05f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 1.8f);

		glm::vec3 lightColor4;
		lightColor4.x = glfwGetTime() * Light5.x;
		lightColor4.y = glfwGetTime() * Light5.y;
		lightColor4.z = glfwGetTime() * Light5.z;
		// Point light 5
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), lightColor4.x, lightColor4.y, lightColor4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), lightColor4.x, lightColor4.y, lightColor4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), 0.05f, 0.05f, 0.05f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].quadratic"), 1.8f);

		glm::vec3 lightColor5;
		lightColor5.x = glfwGetTime() * Light6.x;
		lightColor5.y = glfwGetTime() * Light6.y;
		lightColor5.z = glfwGetTime() * Light6.z;
		// Point light 6
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].position"), pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].ambient"), lightColor5.x, lightColor5.y, lightColor5.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].diffuse"), lightColor5.x, lightColor5.y, lightColor5.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].specular"), 0.05f, 0.05f, 0.05f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].quadratic"), 1.8f);

		glm::vec3 lightColor6;
		lightColor6.x = glfwGetTime() * Light7.x;
		lightColor6.y = glfwGetTime() * Light7.y;
		lightColor6.z = glfwGetTime() * Light7.z;
		// Point light 6
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].position"), pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].ambient"), lightColor6.x, lightColor6.y, lightColor6.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].diffuse"), lightColor6.x, lightColor6.y, lightColor6.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].specular"), 0.05f, 0.05f, 0.05f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].quadratic"), 1.8f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.8f, 0.8f, 0.8f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.22f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.20f);


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



		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);


		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//museo 
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -40.0f));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		museo.Draw(lightingShader);

		//Puerta
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		MarcoP.Draw(lightingShader);
		//PuertaI
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-5.0f, 4.0f, -47.0f));
		model = glm::rotate(model, glm::radians(rotPIz), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		PuertaIz.Draw(lightingShader);
		//PuertaD
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.0f, 4.0f, -47.0f));
		model = glm::rotate(model, glm::radians(rotPDe), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		PuertaD.Draw(lightingShader);
		//Vidrio 
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		vidrio.Draw(lightingShader);
		//Vidrio 
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		vidrio2.Draw(lightingShader);

		//piso 
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -40.0f));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		piso.Draw(lightingShader);
		//arbol
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-20.0f, -5.0f, -20.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		arbol.Draw(lightingShader);
		//arbol
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-16.0f, -5.0f, -20.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		arbol.Draw(lightingShader);
		//arbol
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.0f, -5.0f, -20.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		arbol.Draw(lightingShader);
		//arbol
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(20.0f, -5.0f, -20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		arbol.Draw(lightingShader);
		//arbol
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(16.0f, -5.0f, -20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		arbol.Draw(lightingShader);
		//arbol
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.0f, -5.0f, -20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		arbol.Draw(lightingShader);
		//lampara
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 12.5f, -60.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		lampara.Draw(lightingShader);
		//letrero
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 12.0f, -75.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		letrero.Draw(lightingShader);
		//arma hueso 
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-9.0f, 8.0f, -48.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		ArmaHueso.Draw(lightingShader);
		//arma hueso 
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.0f, 8.0f, -48.1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		ArmaHueso.Draw(lightingShader);
		//Flechas
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.0f, 6.0f, -48.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		flechas.Draw(lightingShader);
		//Flechas
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 6.0f, -48.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		flechas.Draw(lightingShader);
		//arma con picos
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.5f, -1.37f, -75.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		armacurva.Draw(lightingShader);
		//urna de picos
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.5f, -1.37f, -75.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		urna3.Draw(lightingShader);
		//piedras
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.2f, 1.0f, -70.2f));
		model = glm::rotate(model, glm::radians(rotAvi), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		piedras.Draw(lightingShader);
		//urna de piedras
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.2f, -2.0f, -70.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		urna1.Draw(lightingShader);
		//lanzas
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.2f, -1.5f, -60.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		lanzas.Draw(lightingShader);
		//urna de lanzas
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.2f, -1.5f, -60.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		urna2.Draw(lightingShader);
		//Antorcha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.0f, -1.37f, -75.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		antorchas.Draw(lightingShader);
		//urna de antorcha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.0f, -1.37f, -75.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		urna3.Draw(lightingShader);

		//urna de la fogata
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, -1.37f, -60.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		urna3.Draw(lightingShader);
		//mazoPicos
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.8f, -1.8f, -60.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		MazoPicos.Draw(lightingShader);
		//urna de picos
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.8f, -1.8f, -60.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		urna2.Draw(lightingShader);
		//misil 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-11.3f, 5.9f, -93.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		misil.Draw(lightingShader);
		//misil  2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-11.3f, 5.9f, -96.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		misil.Draw(lightingShader);
		//Pistola 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(14.7f, 7.0f, -95.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		pistola.Draw(lightingShader);
		//Pistola 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(14.7f, 4.0f, -95.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		pistola.Draw(lightingShader);
		//Pistola 3
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(14.7f, 10.0f, -95.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		pistola.Draw(lightingShader);
		//Pistola 4
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(14.7f, 13.0f, -95.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		pistola.Draw(lightingShader);
		//Pistola 5
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(14.7f, 7.0f, -99.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		pistola.Draw(lightingShader);
		//Pistola 6
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(14.7f, 4.0f, -99.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		pistola.Draw(lightingShader);
		//Pistola 7
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(14.7f, 10.0f, -99.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		pistola.Draw(lightingShader);
		//Pistola 8
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(14.7f, 13.0f, -99.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		pistola.Draw(lightingShader);
		//rifle 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(14.4f, 7.0f, -116.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		rifle.Draw(lightingShader);
		//rifle 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(14.4f, 4.0f, -116.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		rifle.Draw(lightingShader);
		//rifle 3
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(14.4f, 10.0f, -116.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		rifle.Draw(lightingShader);
		//rifle 4
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(14.4f, 13.0f, -116.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		rifle.Draw(lightingShader);
		//ventilador Base
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 13.59f, -106.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		ventiB.Draw(lightingShader);
		//ventilador Base
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 13.59f, -106.0f));
		model = glm::rotate(model, glm::radians(rotVen), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		ventiA.Draw(lightingShader);

		//casco 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(13.5f, 7.0f, -107.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		casco.Draw(lightingShader);
		//casco 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(13.5f, 4.0f, -107.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		casco.Draw(lightingShader);
		//casco 3
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(13.5f, 10.0f, -107.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		casco.Draw(lightingShader);
		//casco 4
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(13.5f, 13.0f, -107.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		casco.Draw(lightingShader);
		//casco 5
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(13.5f, 7.0f, -104.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		casco.Draw(lightingShader);
		//casco 6
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(13.5f, 4.0f, -104.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		casco.Draw(lightingShader);
		//casco 7
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(13.5f, 10.0f, -104.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		casco.Draw(lightingShader);
		//casco 8
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(13.5f, 13.0f, -104.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		casco.Draw(lightingShader);
		//bazuca1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.3f, 4.0f, -114.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		bazuca.Draw(lightingShader);
		//bazuca2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.3f, 8.0f, -114.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		bazuca.Draw(lightingShader);
		//bazuca2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.3f, 12.0f, -114.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		bazuca.Draw(lightingShader);

		//Ametralladora 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.5f, 3.0f, -104.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		ametralladora.Draw(lightingShader);
		//Ametralladora 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.5f, 7.0f, -104.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		ametralladora.Draw(lightingShader);
		//Ametralladora 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.5f, 11.0f, -104.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		ametralladora.Draw(lightingShader);

		//mesa
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-18.0f, -2.6139f, -111.8f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 100.0);
		Mesa.Draw(lightingShader);
		//herramientas
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-18.0f, -2.6139f, -111.8f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, movCa));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Herramienta.Draw(lightingShader);
		//monitor
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 1.88f, -128.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Monitor.Draw(lightingShader);
		//letrero 
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 9.0f, -128.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		letrero2.Draw(lightingShader);
		//tanque
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.76f, -0.2f, -85.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tanque.Draw(lightingShader);
		//avion 
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, -79.6f));
		model = glm::rotate(model, glm::radians(rotAvnion), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		avion.Draw(lightingShader);

		//Fogata (madera)
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, -1.37f, -60.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 50.0);
		leña.Draw(lightingShader);
		//Fogata(fuego)
		anim.Use();
		model = glm::mat4(1);
		tiempo = glfwGetTime() * speed;
		modelLoc = glGetUniformLocation(anim.Program, "model");
		viewLoc = glGetUniformLocation(anim.Program, "view");
		projLoc = glGetUniformLocation(anim.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, -1.37f, -60.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(anim.Program, "time"), tiempo);
		fuego.Draw(anim);


		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "coloralpha"), 1.0, 1.0, 1.0, 1.0);
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
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
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
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])//acercamiento de la camara 
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);


	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])//alejamiento de la camara
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])//movemiento de la camara en -x
	{
		camera.ProcessKeyboard(LEFT, deltaTime);

	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])//moviemiento de la camara en +x
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);

	}

	if (AnimPIA)
	{
		if (rotPIz <= 90) {
			rotPIz += 0.5;
			if (rotPIz >= 90)
			{
				AnimPIA = false;
				AnimPIC = false;
			}
		}
	}
	if (AnimPDA)
	{
		if (rotPDe >= -90)
		{
			rotPDe -= 0.5;
			if (rotPDe <= -90)
			{
				AnimPDA = false;
				AnimPIA = false;
			}
		}

	}
	if (AnimPIC)
	{
		if (rotPIz >= 0)
		{
			rotPIz -= 0.5;
			if (rotPIz <= 0)
			{
				AnimPIA = false;
				AnimPIC = false;
			}
		}

	}
	if (AnimPDC)
	{
		if (rotPDe <= 0)
		{
			rotPDe += 0.5;
			if (rotPDe >= 0)
			{
				AnimPDA = false;
				AnimPIA = false;
			}
		}

	}
	if (AnimVen) {
		rotVen += 10.0;

	}

	if (AnimCaA)
	{
		if (movCa <= 0.4f) {
			movCa += 0.01;
			if (movCa >= 0.4f) {
				AnimCaA = false;
			}
		}

	}
	if (animcaC) {
		if (movCa >= 0.0f) {
			movCa -= 0.01;
			if (movCa <= 0.0f) {
				animcaC = false;
			}
		}

	}
	if (animAvi)
	{
		if (rotAvi < 30.0f)
		{
			rotAvi += 0.5f;
		}
		else
		{
			animAvi = false;
		}

	}

	if (animAvi == false)
	{
		if (rotAvi > -30.0f)
		{
			rotAvi -= 0.5f;
		}
		else
		{
			animAvi = true;
		}

	}

	if (animAvion)
	{

		rotAvnion += 1.0f;

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
			Light1 = glm::vec3(0.08f, 0.08f, 0.08f);
			Light3 = glm::vec3(0.08f, 0.08f, 0.08f);
			Light4 = glm::vec3(0.08f, 0.08f, 0.08f);
			Light5 = glm::vec3(0.06f, 0.06f, 0.06f);
			Light6 = glm::vec3(0.08f, 0.08f, 0.08f);
			Light7 = glm::vec3(0.08f, 0.08f, 0.08f);

		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
			Light3 = glm::vec3(0);
			Light4 = glm::vec3(0);
			Light5 = glm::vec3(0);
			Light6 = glm::vec3(0);
			Light7 = glm::vec3(0);


		}
	}
	if (keys[GLFW_KEY_Q]) {
		AnimVen = !AnimVen;
		if (AnimVen) {
			AnimVen = true;
		}
		else {
			AnimVen = false;
		}
	}
	if (keys[GLFW_KEY_O])
	{
		if (rotPIz >= -1.0 and rotPIz <= 1.0f)
		{
			AnimPIA = true;
			AnimPIC = false;
		}
		else if (rotPIz <= 91.0f and rotPIz >= 89.0)
		{
			AnimPIA = false;
			AnimPIC = true;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (rotPDe >= -1.0 and rotPDe <= 1.0f)
		{
			AnimPDA = true;
			AnimPDC = false;
		}
		else if (rotPDe >= -91.0f and rotPDe <= -89.0)
		{
			AnimPDA = false;
			AnimPDC = true;
		}
	}
	if (keys[GLFW_KEY_I])
	{
		if (rotPDe >= -1.0 and rotPDe <= 1.0f)
		{
			AnimPIA = true;
			AnimPIC = false;
			AnimPDA = true;
			AnimPDC = false;
		}
		else if (rotPDe >= -91.0f and rotPDe <= -89.0)
		{
			AnimPIA = false;
			AnimPIC = true;
			AnimPDA = false;
			AnimPDC = true;
		}
	}
	if (keys[GLFW_KEY_Z])
	{
		if (movCa >= -0.1 and movCa <= 0.1)
		{
			animcaC = false;
			AnimCaA = true;
		}
		else if (movCa >= 0.3 and movCa <= 0.5) {
			animcaC = true;
			AnimCaA = false;

		}
	}
	if (keys[GLFW_KEY_X])
	{
		activar = true;
		if (activar)
		{
			Light2 = glm::vec3(0.1f, 0.00f, 0.00f);
			speed += +0.5;
		}
	}

	if (keys[GLFW_KEY_R])
	{
		animAvi = true;
	}
	if (keys[GLFW_KEY_V]) {
		animAvion = !animAvion;
		if (animAvion) {
			animAvion = true;
		}
		else {
			animAvion = false;
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