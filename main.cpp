#include "./lib/GLFW/glfw3.h"
#include "./lib/GL/glu.h"
#include <iostream>
#include <vector>
#include <cstdlib>

#include "cores.h"
#include "primitivas.h"
#include "camera.h"

Camera camera(vec3(0, 0, 0));

static float angulo = 5.0;
static float lastMousePos = 0.0;
static bool firstTimeMouse = true;

GLFWwindow* window;

int count = 1;

color* cores[7] = {&preto, &azul_dodger, &verde_escuro, &azul_dodger, &verde_escuro, &vermelho_sadao, &amarelo_gold};

// Display List
struct Objeto {
	GLuint id;
	vec3 pos;
	Objeto():id(0),pos(vec3(0.0,0.0,0.0)){}
};

const GLuint total_objetos = 7;
std::vector<Objeto> objetos;


void teclado_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key==GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key==GLFW_KEY_W &&(action == GLFW_PRESS || action==GLFW_REPEAT)) {
		// Forward
		camera.forward();
	}
	else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		// Back
		camera.back();

	}
	else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		// Left
		camera.left();

	}
	else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		// Right
		camera.right();
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	float dx;

	if (firstTimeMouse) {
		dx = 0;
		lastMousePos = xpos;
		firstTimeMouse = false;
	}
	dx = xpos - lastMousePos;
	lastMousePos = xpos;
	camera.updateYaw(dx/16);
	camera.update();
}

void initLight(GLFWwindow* window) {
	desenhaArestas(objetos[0].id, branco);

	glfwMakeContextCurrent(window);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	float globalAmb[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

	float light0[4][4] = {
		{0.1f, 0.1f, 0.1f, 1.0f}, 	// ambient
		{0.8f, 0.8f, 0.8f, 1.0f}, 	// diffuse
		{1.0f, 1.0f, 1.0f, 1.0f}, 	// specular
		{0.0f, 0.0f, 1.0f, 1.0f}    // position
	};

	glLightfv(GL_LIGHT0, GL_AMBIENT,  &light0[0][0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  &light0[1][0]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, &light0[2][0]);
	glLightfv(GL_LIGHT0, GL_POSITION, &light0[3][0]);
}

void redimensiona(int w, int h) {
	glViewport(0, 0, w, h);

	float aspect = (float)w / (float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 500.0);

	glMatrixMode(GL_MODELVIEW);
}

void desenha(float dt) {	
	float veloc_ang =  60.0*dt; // 60 graus por segundo
	
	glLoadIdentity();

	// Ativar a camera
	camera.ativar();

	glCallList(objetos[0].id);

	for (int i = 1; i < total_objetos; i++) {
		float x = objetos[i].pos.x;
		float y = objetos[i].pos.y;
		float z = objetos[i].pos.z;

		glPushMatrix();
		glTranslatef(x,y,z);
		glRotatef(angulo, 1.0, 1.0, 0.0);
		glCallList(objetos[i].id);
		glPopMatrix();
	}

	if (angulo == 0) {		
		desenhaEsfera(objetos[count].id, *cores[count], 5.0, 20.0, 20.0);
		count ++;
	}

	if (count >= total_objetos) {
		count = 1;
		initLight(window);
	}

	angulo += veloc_ang;
	if (angulo >= 360.0)
		angulo = 0.0;
		
}

void init(GLFWwindow* window) {
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, teclado_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwMaximizeWindow(window);

	glClearColor(0.88, 0.91, 0.89, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < total_objetos;i++) {
		Objeto p;
		objetos.push_back(p);
	}

	objetos[0].id = glGenLists(total_objetos);		
	desenhaArestas(objetos[0].id, preto);

	// Vertex A
	objetos[1].id = objetos[0].id + 1;
	objetos[1].pos = vec3(-50/3.0,0,-150/2.0);

	// Vertex B
	objetos[2].id = objetos[0].id + 2;
	objetos[2].pos = vec3(-25/3.0,50/3.0,-125/2.0);

	// Vertex C
	objetos[3].id = objetos[0].id + 3;
	objetos[3].pos = vec3(25/3.0,50/3.0,-175/2.0);

	// Vertex D
	objetos[4].id = objetos[0].id + 4;
	objetos[4].pos = vec3(50/3.0,0,-150/2.0);

	// Vertex E
	objetos[5].id = objetos[0].id + 5;
	objetos[5].pos = vec3(25/3.0,-50/3.0,-125/2.0);

	// Vertex F
	objetos[6].id = objetos[0].id + 6;
	objetos[6].pos = vec3(-25/3.0,-50/3.0,-175/2.0);

	for (int i = 1; i < total_objetos; i++) {
		desenhaEsfera(objetos[i].id, *cores[0], 6.0, 20.0, 20.0);	
	}
}

int main(void) {

	const int LARGURA = 800;
	const int ALTURA = 600;

	/* Initialize the library */
	glfwInit();

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(LARGURA, ALTURA, "Coloracao de Grafos", NULL, NULL);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// -- inicio
	init(window);

	float valor = 0.0;
	float lastTime = 0.0;
	int index = 1;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		
		float currentTime = (float)glfwGetTime();
		float dt = currentTime - lastTime;
		lastTime = currentTime;

		/* Poll for and process events */
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		int largura, altura;
		glfwGetFramebufferSize(window, &largura, &altura);

		redimensiona(largura, altura);

		desenha(dt);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
	}

	glDeleteLists(objetos[0].id, total_objetos);
	glfwTerminate();
	return 0;
}