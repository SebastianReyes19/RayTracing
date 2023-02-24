#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <chrono>
#include <iostream>
#include "Camera.h"
#include "Scene.h"
#include "BVH.h"


#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1200

int bunny = 0;
int rays = 4;
std::string filepath;

float frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
GLFWwindow *window;

std::vector<Shape*> shapes;
std::vector<Light*> lights;

void ClearFrameBuffer()
{
	memset(&frameBuffer, 0, WINDOW_HEIGHT * WINDOW_WIDTH * 3 * sizeof(float));
}

void Display()
{	
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer);
}

void Init()
{
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Sebastian Reyes Ray Tracing", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearFrameBuffer();

	Scene scene = Scene();

	if (bunny == 1) {
		std::cout << "rendering bunny" << std::endl;
		material b_mat({ .1f, .1f, .1f }, { 0, 0, 1.0f }, { 1.0f, 1.0f, .5f }, { 0, 0, 0 }, 100.0f);
		Shape* bunny = new BVH((char*)filepath.c_str(), b_mat);
		scene.addShape(bunny);
	}
	else {
		std::cout << "rendering scene " << std::endl;
		material sp1_mat({ .1, .1, .1 }, { .2, 1.0f, .2 }, { 1.0f, 1.0f, 1.0f }, { 0, 0, 0 }, 100.0);
		Shape* sphere1 = new Sphere({ -1.0, -.7, 3.0 }, .3f, sp1_mat);
		scene.addShape(sphere1);

		material sp2_mat({ .1, .1, .1 }, { 0, 0, 1.0 }, { 1.f, 1.f, 1.f }, { 0.f, 0.f, 0.f, }, 10.0);
		Shape* sphere2 = new Sphere({ 1.0f, -.5f, 3.0f }, .5f, sp2_mat);
		scene.addShape(sphere2);

		material sp3_mat({ 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 1.0, 1.0, 1.0 }, 0.0);
		Shape* sphere3 = new Sphere({ -1.0, 0.0, -0.0 }, 1.0, sp3_mat);
		scene.addShape(sphere3);

		material sp4_mat({ 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0.8, 0.8, 0.8 }, 0.0);
		Shape* sphere4 = new Sphere({ 1.0, 0.0, -1.0 }, 1.0, sp4_mat);
		scene.addShape(sphere4);

		material pl_mat({ 0.1, 0.1, 0.1 }, { 1, 1, 1 }, { 0, 0, 0 }, { 0, 0, 0 }, 0.0);
		Shape* plane1 = new Plane({ 0.0, -1.0f, 0.0 }, { 0.0, 1.0f, 0.0 }, pl_mat);
		scene.addShape(plane1);

		Shape* plane2 = new Plane({ 0.0, 0.0, -3.0 }, { 0.0, 0.0, 1.0 }, pl_mat);
		scene.addShape(plane2);
	}

	Light* light1 = new Light({ 0.0, 3.0, -2.0 }, { .2, .2, .2 });
	scene.addLight(light1);

	Light* light2 = new Light({ -2.0, 1.0, 4.0 }, { .5, .5, .5 });
	scene.addLight(light2);

	lights = scene.giveLights();
	shapes = scene.giveShapes();

	Camera camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT, { 0.0f, 0.0f, 7.0f }, { 0, 0, 0, }, { 0, 1.0f, 0 }, 45.f, 1.0f, rays);

	auto start = std::chrono::high_resolution_clock::now();
	camera.TakePicture(&scene);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Time passed (ms): " << duration.count() << std::endl;
	
	float *renderedImage = camera.GetRenderedImage();
	memcpy(frameBuffer, renderedImage, sizeof(float) * WINDOW_HEIGHT * WINDOW_WIDTH * 3);
}


int main()
{	
	/*std::cout << "Show bunny? enter '1' if yes, enter '0' if no: ";
	//std::cin >> bunny;
	if (bunny) {
		std::cout << "What is the bunny.obj file path? ";
		do
		{
			std::cin >> filepath;
			if (FILE* file = fopen(filepath.c_str(), "r")) {
				fclose(file);
				break;
			}
			else {
				std::cout << "invalid name\nEnter a valid filename: ";
			}

		} while (true);
	}
	//std::cout << "\n";
	//std::cout << "How many rays (depth of recusion)? Default is 4: ";
	std::cin >> rays;
	*/
	

	Init();
	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	for (int i = 0; i < shapes.size(); i++) {
		delete shapes[i];
	}
	for (int i = 0; i < lights.size(); i++) {
		delete lights[i];
	}

	glfwTerminate();
	return 0;
}