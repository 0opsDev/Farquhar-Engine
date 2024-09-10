#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include<iostream>
#include<fstream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"src/video/Texture.h"
#include"src/video/shaderClass.h"
#include"fileClass.h"
#include"src/video/EBO.h"
#include"src/video/VAO.h"
#include"src/video/VBO.h"
#include"src/Camera.h"

const unsigned int width = 2560;
const unsigned int height = 1440;
//https://discord.gg/fd6REHgBus
//prism
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
	-1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
};
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};
GLfloat lightVertices[] =
{
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

int main() 
{
	//fileread > file names
	//https://www.youtube.com/watch?v=rQhBECyA6ew - need to watch this for global vars
	//settings/glb vars

	//start glfw
	glfwInit(); 

	//give glfw hints on open gl version (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, 1);



	//size, name, fullscreen
	GLFWwindow* window = glfwCreateWindow(width, height, "Farquhar Engine", NULL, NULL);

	//error checking
	if (window == NULL) 
	{
		std::cout << "failed to create window" << std::endl; 
		glfwTerminate(); 
		return -1; 
	}
	//make window current context
	glfwMakeContextCurrent(window); 

	//load open gl config
	gladLoadGL(); 


	//area of open gl we want to render in
	glViewport(0, 0, width, height);

	Shader shaderProgram("Shaders/Default.vert","Shaders/Default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));
	// array size (8) and where to plug into vbo
	//shape
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	//color
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	//texture
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	//normals
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));


	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	Shader lightShader("Shaders/light.vert", "Shaders/light.frag");

	VAO lightVAO;
	lightVAO.Bind();

	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));

	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	//glm::vec4 lightColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	//glm::vec3 InnerLight = glm::vec3(0.90f, 0.95f, 0.0f);
	//glm::vec3 spotLightRot = glm::vec3(0.0f, -1.0f, 0.0f);
	//glm::vec4 skylightSpread = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	//model transform
	//glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	//glm::mat4 lightModel = glm::mat4(1.0f);
	//lightModel = glm::translate(lightModel, lightPos);

	//glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	//glm::mat4 pyramidModel = glm::mat4(1.0f);
	//pyramidModel = glm::translate(pyramidModel, pyramidPos);

	//glUniform3f(glGetUniformLocation(shaderProgram.ID, "InnerLight1"), InnerLight.x, InnerLight.y, InnerLight.z);
	//glUniform3f(glGetUniformLocation(shaderProgram.ID, "spotLightRot"), InnerLight.x, InnerLight.y, InnerLight.z);
	//glUniform4f(glGetUniformLocation(shaderProgram.ID, "skylightSpread"), skylightSpread.x, skylightSpread.y, skylightSpread.z, skylightSpread.w);

	//lightShader.Activate();
	//glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	//glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	//shaderProgram.Activate();
	//glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	//glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	//glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	//texture

	Texture tileTex("assets/Textures/Model/wood_floor_deck_diff_4k.jpg", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	tileTex.texUnit(shaderProgram, "tex0", 0);
	Texture tileTexspec("assets/Textures/Model/wood_floor_deck_ao_4k.jpg", GL_TEXTURE_2D, 1, GL_RGBA, GL_UNSIGNED_BYTE);
	tileTexspec.texUnit(shaderProgram, "tex1", 1);


	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//imgui vars
	bool save = false;
	bool DoDefaultAnimation = false;
	float rotation = 0.0f;
	double prevTime = glfwGetTime();
	bool doVsync = true;
	bool drawTriangles = true;
	bool ResetTrans = false;
	GLfloat ConeSI[3] = { 0.05f, 0.95f , 1.0f };
	GLfloat ConeRot[3] = { 0.0f, -1.0f , 0.0f };
	GLfloat varFOV = 60.0f;
	GLfloat lightRGBA[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat skyRGBA[4] = { 0.778f, 0.853f, 0.956f, 1.0f };
	GLfloat LightTransform1[3] = { 0.0f, 1.0f, 0.0f };
	GLfloat ObTransform1[3] = { 0.0f, 0.0f, 0.0f };
	//
	std::fstream TestFile2;
	TestFile2.open("Settings/DoDefaultAnimation.txt", std::ios::in);//read
	if (TestFile2.is_open()) {
		//writes in lines
		std::string lineT;
		while (getline(TestFile2, lineT))
		{
			if (lineT == "true") {
				DoDefaultAnimation = true;
				std::cout << DoDefaultAnimation << std::endl;
			}
			else {
				DoDefaultAnimation = false;
				std::cout << DoDefaultAnimation << std::endl;
			}
		}
		TestFile2.close();
	}

	glEnable(GL_DEPTH_TEST);

	// camera ratio and pos
	Camera camera(width, height, glm::vec3(0.0f, 1.0f, 2.0f));

	//icon creation
	int iconW, iconH;
	int iconChannels;
	//STBI_rgb_alpha

	unsigned char* pixelsIcon = stbi_load("assets/Icons/IconF.png", &iconW, &iconH, &iconChannels, STBI_rgb_alpha);

	//change window icon
	GLFWimage Iconinages[1];
	//give glfw pixels, width and height
	Iconinages[0].width = iconW;
	Iconinages[0].height = iconH;
	Iconinages[0].pixels = pixelsIcon;

	//change to icon (what window, how many images, what image)
	glfwSetWindowIcon(window, 1, Iconinages);
	glfwCreateCursor(Iconinages, iconW, iconH);

	//makes sure window stays open
	while (!glfwWindowShouldClose(window))  
	{
		if (save) {
			std::fstream TestFile;
			TestFile.open("Settings/DoDefaultAnimation.txt", std::ios::out);//write
			if (TestFile.is_open()) {
				if (DoDefaultAnimation) {
					TestFile << "true\n";
					std::cout << "true\n";
				}
				else {
					TestFile << "false\n";
					std::cout << "false\n";
				}
			}

			save = false;
		}
		switch (doVsync) {
		case true:
			glfwSwapInterval(1);
			break;
		case false:
			glfwSwapInterval(0);
			break;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_5) == GLFW_PRESS)
		{
			varFOV += 0.2f;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_4) == GLFW_PRESS)
		{
			varFOV -= 0.2f;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		{
			varFOV = 60.0f;
		}
		if (varFOV <= 0.00f)
		{
			varFOV = 0.1f;
		}
		if (varFOV >= 160.1f)
		{
			varFOV = 160.0f;
		}
		
		
		//RGB ALPHA
		glClearColor(skyRGBA[0], skyRGBA[1], skyRGBA[2], skyRGBA[3]);
		// SEND TO COLOR BUFFER (DRAWS COLOUR TO SCREEN)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		// Tell OpenGL a new frame is about to begin
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//inputs
		camera.Inputs(window);
		//camera fov, near and far plane
		camera.updateMatrix(varFOV, 0.1f, 100.0f);

		double crntTime = glfwGetTime();
		if (DoDefaultAnimation) {
			if (crntTime - prevTime >= 1 / 60)
			{
				rotation += 0.5f;
				prevTime = crntTime;
			}
		}
		else {
			rotation = 0.0f;
		}
		
		//i added these
		//transform light but not model
		glm::vec4 lightColor = glm::vec4(lightRGBA[0], lightRGBA[1], lightRGBA[2], lightRGBA[3]);
		glm::vec3 InnerLight = glm::vec3(ConeSI[1] - ConeSI[0], ConeSI[1], ConeSI[2]);
		glm::vec3 spotLightRot = glm::vec3(ConeRot[0], ConeRot[1], ConeRot[2]);
		glm::vec4 skylightSpread = glm::vec4(skyRGBA[0], skyRGBA[1], skyRGBA[2], skyRGBA[3]);

		glm::vec3 lightPos = glm::vec3(LightTransform1[0], LightTransform1[1], LightTransform1[2]);
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);

		lightShader.Activate();

		//update light transform
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		//update light color on the model (not the world)
		glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

		glm::vec3 pyramidPos = glm::vec3(ObTransform1[0], ObTransform1[1], ObTransform1[2]);
		glm::mat4 pyramidModel = glm::mat4(1.0f);
		pyramidModel = glm::translate(pyramidModel, pyramidPos);
		pyramidModel = glm::rotate(pyramidModel, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

		//shaderprog can stay
		//activate shader program
		shaderProgram.Activate();
		//i wrote innerlight
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "InnerLight1"), InnerLight.x, InnerLight.y, InnerLight.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "spotLightRot"), spotLightRot.x, spotLightRot.y, spotLightRot.z);
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "skylightSpread"), skylightSpread.x, skylightSpread.y, skylightSpread.z, skylightSpread.w);

		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		//
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
		//update light color seprate from the model
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		// update light pos
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		//
		camera.Matrix(shaderProgram, "camMatrix");

		if (ResetTrans) {
			camera.Position = glm::vec3(0, 0, 0);
			ResetTrans = false;
		}


		//i added these
		//


		tileTex.Bind();
		tileTexspec.Bind();
		//bind vao
		VAO1.Bind();
		//primative type
		if (drawTriangles) {
			glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
			lightShader.Activate();
			camera.Matrix(lightShader, "camMatrix");
			lightVAO.Bind();
			glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}
		
		//glDrawArrays(GL_TRIANGLES, 0, 3); 
			// ImGUI window creation
		ImGui::Begin("Settings Window");
		// Text that appears in the window
		ImGui::Text("Settings (Press escape to use mouse)");
		ImGui::Checkbox("save changes?", &save);
		// Checkbox that appears in the window
		ImGui::Text("Rendering");
		ImGui::Checkbox("Vsync (GAMETIME IS SYNCED ON THIS SHIT)", &doVsync);
		ImGui::Checkbox("Draw Triangles", &drawTriangles);
		ImGui::SliderFloat("FOV", &varFOV, 0.1f, 160.0f);
		ImGui::Text("Animation");
		ImGui::Checkbox("DoDefaultAnimation", &DoDefaultAnimation);

		ImGui::Text("Transform");
		ImGui::Checkbox("move camera to 0,0,0", &ResetTrans);
		ImGui::DragFloat3("Light Yransform", LightTransform1);
		ImGui::DragFloat3("Object 1 Transform", ObTransform1);

		ImGui::Text("Lighting");
		ImGui::Text("Light color and intens");
		ImGui::ColorEdit4("sky RGBA", skyRGBA);
		ImGui::ColorEdit4("light RGBA", lightRGBA);
		ImGui::DragFloat("light I", &ConeSI[2]);
		ImGui::Text("cone size");
		ImGui::SliderFloat("cone Size (D: 0.95)", &ConeSI[1], 0.0f, 1.0f);
		ImGui::SliderFloat("cone Strength (D: 0.05)", &ConeSI[0], 0.0f, 0.90f);
		ImGui::Text("Light Angle");
		ImGui::DragFloat3("Cone Angle", ConeRot);
		//ImGui::SliderFloat("Animation speed", &TimeT, 0.0f, 50.0f);
		// Slider that appears in the window
		//ImGui::SliderFloat("Size", &size, 0.5f, 2.0f);
		
		//
		// Ends the window
		ImGui::End();
		 
		// Renders the ImGUI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		
		//swap back buffer with front buffer
		glfwSwapBuffers(window); 
		//tells open gl to proccess all events like window resizing and all otheer events
		glfwPollEvents(); 



	}
	//delete all objects on close
	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	tileTex.Delete();
	tileTexspec.Delete();
	shaderProgram.Delete();
	//end opengl
	glfwDestroyWindow(window); 
	glfwTerminate(); 
	return 0; 
}