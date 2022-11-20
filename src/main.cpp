#include <iostream>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "asset_loader.h"
#include "window.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "glm_includes.h"
#include "input.h"
#include "transform.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include <tiny_gltf.h>

typedef std::chrono::nanoseconds delta_dur_t;

void glfw_error_callback(int error, const char *description)
{
	fprintf(stderr, "Glfw Error: %s\n", description);
}

void checkGLErr()
{
	GLenum errCode;
	while ((errCode = glGetError()) != GL_NO_ERROR)
	{
		std::string errName;
		switch (errCode)
		{
		case GL_INVALID_ENUM:
			errName = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			errName = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			errName = "INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			errName = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			errName = "STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			errName = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errName = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		default:
			errName = "UNKNOWN_ERR";
			break;
		}
		fprintf(stderr, "GL Error(%u): %s\n", errCode, errName.c_str());
	}
}

int main(int argc, char const *argv[])
{
	std::cout << "INIT" << std::endl;

	std::cout << "INIT Glfw:" << std::endl;
	glfwSetErrorCallback(glfw_error_callback);
	int glfwErr = glfwInit();
	if (!glfwErr)
	{
		std::cerr << "Failed" << std::endl;
		return 1;
	}
	std::cout << "Done" << std::endl;

	GWindow win;
	win.create();

	std::cout << "INIT Glew:" << std::endl;
	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK)
	{
		std::cerr << "Failed" << std::endl;
		fprintf(stderr, "Glew Error: %s\n", glewGetErrorString(glewStatus));
		return 1;
	}
	std::cout << "Done" << std::endl;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(win.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 450 core");

	// tinygltf::TinyGLTF loader;
	// tinygltf::Model model;
	// std::string err;
	// std::string warn;

	// bool mSuccess = loader.LoadBinaryFromFile(&model, &err, &warn, "./assets/spikeball.glb");

	// if (!warn.empty())
	// {
	// 	printf("Warn: %s\n", warn.c_str());
	// }

	// if (!err.empty())
	// {
	// 	fprintf(stderr, "Err: %s\n", err.c_str());
	// }

	// if (!mSuccess)
	// {
	// 	fprintf(stderr, "Failed to parse glTF\n");
	// }

	glEnable(GL_DEPTH_TEST);

	ShaderProgram prog;
	prog.load(
		loadFileAsString("./assets/shaders/test.vert"),
		loadFileAsString("./assets/shaders/test.frag"),
		loadFileAsString("./assets/shaders/test.tesc"),
		loadFileAsString("./assets/shaders/test.tese"));

	// Mesh mesh;
	// mesh.load({
	// 	-0.5f, -0.5f, 0.0f, /**/ 0.0f, 0.0f, 0.0f,
	// 	0.5f, -0.5f, 0.0f, /**/ 0.0f, 0.0f, 0.0f,
	// 	0.5f, 0.5f, 0.0f, /**/ 0.0f, 0.0f, 0.0f,
	// 		  },
	// 		  {
	// 			  0,
	// 			  1,
	// 			  2,
	// 		  });

	std::vector<float> terrainPosData{
		-1.0, 0.0, -1.0,
		1.0, 0.0, -1.0,
		-1.0, 0.0,  1.0,
		1.0, 0.0,  1.0,
	};

	std::vector<float> terrainUVData{
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
	};

	std::vector<unsigned int> terrainIndices{
		0,
		1,
		2,
		3,
	};

	TerrainMesh tMesh;
	tMesh.load(terrainPosData, terrainUVData, terrainIndices);

	GlTexture tex, tex2;
	loadFileAsTexture("./assets/textures/test.png", tex);
	loadFileAsTexture("./assets/textures/test2.png", tex2);

	bool optionsPanel = true;
	float perspFOV = 60.0f;
	float scale = 1.0f;
	float rotat = 22.5f;
	float tessAmount = 17.0f;
	float terrainHeight = 1.0f;
	bool moveMode = false;
	bool wireframe = false;
	bool moveTogglePressed = false;
	double lastMouseX, lastMouseY;
	Transform terrainTransform;
	Transform camTransform;


	// Loop Timing
	std::chrono::high_resolution_clock clock;

	auto timePtNow = clock.now();
	auto timePt = timePtNow;
	delta_dur_t delta(0);
	delta_dur_t stepDelta(0);
	delta_dur_t totalTime(0);
	const delta_dur_t stepDuration(1000000000LL / 60LL);
	const delta_dur_t maxDelta(1000000000LL);
	const delta_dur_t minDelta(1000000000LL / 240LL);

	// Game Loop
	printf("Starting Game Loop\n");

	while (!win.shoudClose())
	{
		glfwPollEvents();
		timePtNow = clock.now();

		stepDelta = std::chrono::duration_cast<delta_dur_t>(timePtNow - timePt);
		if (stepDelta >= minDelta)
		{
			int width, height;
			win.getFrameSize(&width, &height);
			glViewport(0, 0, width, height);
			timePt = timePtNow;
			delta += stepDelta;
			if (delta >= maxDelta)
			{
				delta = maxDelta;
			}

			for (unsigned int i = 0; i < 10 && delta.count() > 0; i++)
			{
				delta -= stepDuration;

				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once);
				ImGui::SetNextWindowSize(ImVec2(400.0f, 500.0f), ImGuiCond_Once);
				ImGui::SetNextWindowBgAlpha(0.8f);
				if (ImGui::Begin("Options", &optionsPanel))
				{
					ImGui::Text(
						"#Keys:\n"
						"Space: Toggle Movement Controls\n"
						"WASD or Arrow Keys: Move\n"
						"E and Q: Move up/down\n"
						"Mouse: Turn Camera\n");
					ImGui::NewLine();
					ImGui::SliderFloat("Cam FOV", &perspFOV, 0.01f, 179.0f);
					ImGui::SliderFloat3("Cam Pos", glm::value_ptr(camTransform.pos), -10.0f, 10.0f);
					ImGui::NewLine();
					ImGui::Checkbox("Wireframe", &wireframe);
					ImGui::NewLine();
					// ImGui::SliderFloat("Scale", &scale, 0.01f, 179.0f);
					ImGui::SliderFloat("Rotation", &rotat, -360.f, 360.f);
					ImGui::NewLine();
					ImGui::SliderFloat("Tess Amount", &tessAmount, 1.0f, 61.0f);
					ImGui::SliderFloat("Terrain Height", &terrainHeight, -3.0f, 3.0f);
					ImGui::NewLine();


				}
				ImGui::End();

				if (moveMode)
				{
					double xpos, ypos;
					glfwGetCursorPos(win.getWindow(), &xpos, &ypos);

					float dX = (float)(xpos - lastMouseX);
					float dY = (float)(ypos - lastMouseY);

					camTransform.rotation.y -= dX / 10.0f / 60.0f;
					camTransform.rotation.x -= dY / 10.0f / 60.0f;
					camTransform.rotation.x = glm::clamp(camTransform.rotation.x, -glm::half_pi<float>(), glm::half_pi<float>());

					lastMouseX = xpos;
					lastMouseY = ypos;

					glm::vec3 dir{0.0f, 0.0f, 0.0f};

					if (GInput::isPressed(GLFW_KEY_W) || GInput::isPressed(GLFW_KEY_UP))
					{
						dir.z -= 1.0f;
					}
					if (GInput::isPressed(GLFW_KEY_S) || GInput::isPressed(GLFW_KEY_DOWN))
					{
						dir.z += 1.0f;
					}
					if (GInput::isPressed(GLFW_KEY_A) || GInput::isPressed(GLFW_KEY_LEFT))
					{
						dir.x -= 1.0f;
					}
					if (GInput::isPressed(GLFW_KEY_D) || GInput::isPressed(GLFW_KEY_RIGHT))
					{
						dir.x += 1.0f;
					}
					if (GInput::isPressed(GLFW_KEY_E) /* || GInput::isPressed(GLFW_KEY_LEFT)*/)
					{
						dir.y += 1.0f;
					}
					if (GInput::isPressed(GLFW_KEY_Q) /* || GInput::isPressed(GLFW_KEY_RIGHT)*/)
					{
						dir.y -= 1.0f;
					}

					dir = glm::vec3(glm::yawPitchRoll(camTransform.rotation[1], camTransform.rotation[0], camTransform.rotation[2]) * glm::vec4(dir.x, dir.y, dir.z, 1.0f));
					const float camSpeed = 2.0f;
					dir *= camSpeed;
					dir /= 60.0f;
					camTransform.pos += dir;
				}

				
				bool toggleJustPressed = false;
				if (GInput::isPressed(GLFW_KEY_SPACE))
				{
					if (!moveTogglePressed)
					{
						moveTogglePressed = true;
						toggleJustPressed = true;
					}
				}
				else if (moveTogglePressed)
				{
					moveTogglePressed = false;
				}

				if(toggleJustPressed || (moveMode && GInput::isPressed(GLFW_KEY_ESCAPE))){
					moveMode = !moveMode;
					if (moveMode)
					{
						glfwSetInputMode(win.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
						glfwGetCursorPos(win.getWindow(), &lastMouseX, &lastMouseY);
					}
					else
					{
						glfwSetInputMode(win.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					}
				}

				terrainTransform.pos = glm::vec3(0.0f, -1.0f, -5.0f);
				terrainTransform.scale = glm::vec3(scale, scale, scale);
				terrainTransform.rotation = glm::vec3(0.0f, glm::radians(rotat), 0.0f);

				glClearColor(0.075f, 0.1f, 0.22f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				auto perspMat = glm::perspectiveFov(glm::radians(perspFOV), (float)width, (float)height, 0.01f, 100.0f);
				auto objTransMat = terrainTransform.matrixObjTransform();
				auto viewMat = camTransform.matrixView();

				tex.use(GL_TEXTURE0);
				tex2.use(GL_TEXTURE1);
				prog.use();
				tMesh.use();
				glUniform1i(glGetUniformLocation(prog.getProgramId(), "uTex"), 0);
				glUniform1i(glGetUniformLocation(prog.getProgramId(), "uTex2"), 1);
				glUniformMatrix4fv(glGetUniformLocation(prog.getProgramId(), "perspective"), 1, false, glm::value_ptr(perspMat));
				glUniformMatrix4fv(glGetUniformLocation(prog.getProgramId(), "objectTransform"), 1, false, glm::value_ptr(objTransMat));
				glUniformMatrix4fv(glGetUniformLocation(prog.getProgramId(), "viewTransform"), 1, false, glm::value_ptr(viewMat));
				glUniform2f(glGetUniformLocation(prog.getProgramId(), "tessInner"), tessAmount, tessAmount);
				glUniform4f(glGetUniformLocation(prog.getProgramId(), "tessOuter"), tessAmount, tessAmount, tessAmount, tessAmount);
				glUniform1f(glGetUniformLocation(prog.getProgramId(), "uHeightAmount"), terrainHeight);
				glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
				tMesh.draw();

				checkGLErr();

				totalTime += stepDuration;

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				win.swapBuffers();
			}
		}
	}

	// Clean up
	prog.destroy();
	tex.destroy();
	tex2.destroy();
	tMesh.destroy();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	win.destroy();

	glfwTerminate();

	return 0;
}
