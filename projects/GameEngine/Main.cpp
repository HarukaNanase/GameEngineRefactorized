#include "Mesh.h"
#include "Matrix4.h"
#include "QuaternionCamera.h"
#include "Mouse.h"
#include "SceneGraph.h"
#include "MeshManager.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "Keyboard.h"
#include "FreeImage.h"
#include "Convention.h"
#include "Water.h"

using namespace Managers;
#define CUBE_MAP_FOLDER "Assets/Textures/cubemap/"
#define VERTICES 0
std::string currentPath;
int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;
#define CAPTION "Solha's Engine Copyright @ Solha"
Shader* shader;
Shader* waterShader;
Shader* normal;
Shader* normalDebugger;
Shader* noTexDebugger;
Shader* skyBoxShader;
#define PI 3.14159265358979323846
#define SCALE(x,y,z) Matrix4::SCALE(x,y,z)
#define TRANSLATE(x,y,z) Matrix4::TRANSLATE(x,y,z)
//QuaternionCamera camera = QuaternionCamera(Vector3(0,0,-5));
Matrix4 ProjectionMatrix = Matrix4::ProjectionMatrix(PI/3, 640.0f / 480.0f, 1, 50);
Camera camera = Camera(Vector3(0, 0, 5), Vector3(0, 0, 0));
Mouse* mouse;
Water* water;
SceneNode* waterNode;
float currentTime = 0, deltaTime = 0, previousTime = 0;


SceneNode* envoirnment, *tangram;
bool Rewind = false;
bool DEBUG = false;
bool msaa = false;
SceneNode *first, *second, *third, *fourth, *fifth, *sixth, *seventh;
SceneNode *tree;
SceneNode * lightSphere;
SceneNode* rockGroup;
SceneNode* rock1;
SceneNode* rock2;
SceneNode* rock3;
SceneNode* rock4;
SceneNode* rock5;
Vector3 LightPosition(-80,80,-80);

/////////////////////////////////////////////////////////////////////// ERRORS

void TakeScreenshot() {
	BYTE* pixels = new BYTE[3 * WinX * WinY];
	glReadPixels(0, 0, WinX, WinY, GL_BGR, GL_UNSIGNED_BYTE, pixels);
	FIBITMAP* screenshot = FreeImage_ConvertFromRawBits(pixels, WinX, WinY, 3 * WinX, 24, 0x0000FF, 0x00FF00 , 0xFF0000, false);
	std::ostringstream os;
	os << "Screenshots/Screenshot_" << currentTime << ".bmp";
	std::string path = os.str();
	FreeImage_Save(FIF_BMP, screenshot, path.c_str(), 0);
	os.clear();
	FreeImage_Unload(screenshot);
	delete [] pixels;
}



bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		std::cin.get();
		exit(EXIT_FAILURE);
	}
}

void createShaderProgram()
{
	//shader = new Shader();
	
	//shader->LoadShader(GL_VERTEX_SHADER, "Assets/Shaders/NormalMapping/normal_vert_refact.glsl");
	//shader->LoadShader(GL_FRAGMENT_SHADER, "Assets/Shaders/NormalMapping/normal_frag_refact.glsl");
	shader = ShaderManager::getInstance()->create("shader",
		"Assets/Shaders/NormalMapping/normal_vert_refact.glsl",
		"Assets/Shaders/NormalMapping/normal_frag_refact.glsl");
	
	//shader->LoadShader(GL_VERTEX_SHADER, "Assets/Shaders/BlinnPhong/blinn_phong_vert.glsl");
	//shader->LoadShader(GL_FRAGMENT_SHADER, "Assets/Shaders/BlinnPhong/blinn_phong_frag.glsl");
	//shader->Prepare();
	//shader->Attach(GL_VERTEX_SHADER);
	//shader->Attach(GL_FRAGMENT_SHADER);
	//shader->BindAttribute(VERTICES, "inPosition");
	//shader->BindAttribute(TEXCOORDS, "inTexCoord");
	//shader->BindAttribute(NORMALS, "inNormal");
	//shader->BindAttribute(TANGENTS, "inTangent");
	//shader->BindAttribute(BITANGENTS, "inBiTangent");
	//shader->Link();
	//shader->AddUniform("ModelMatrix");
	//shader->AddUniform("ViewMatrix");
	//shader->AddUniform("ProjectionMatrix");
	//shader->AddUniform("LightPosition");
	//shader->AddUniform("NormalMatrix");
	//shader->AddUniform("tex");
	//shader->AddUniform("normalMap");
	//shader->AddUniform("material");

	//waterShader = new Shader();
	//waterShader->LoadShader(GL_VERTEX_SHADER, "Assets/Shaders/dudvwater/Water.vert");
	//waterShader->LoadShader(GL_FRAGMENT_SHADER, "Assets/Shaders/dudvwater/Water.frag");
	//waterShader->Prepare();
	//waterShader->Attach(GL_VERTEX_SHADER);
	//waterShader->Attach(GL_FRAGMENT_SHADER);
	//waterShader->BindAttribute(VERTICES, "inPosition");
	//waterShader->BindAttribute(TEXCOORDS, "inTexCoord");
	//waterShader->BindAttribute(NORMALS, "inNormal");
	//waterShader->Link();
	//waterShader->AddUniform("LightPosition");
	waterShader = ShaderManager::getInstance()->create("water",
		"Assets/Shaders/dudvwater/Water.vert",
		"Assets/Shaders/dudvwater/Water.frag");
	waterShader->Enable();
	GLuint reflection = waterShader->getUniform("ReflectionTexture");
	GLuint refraction = waterShader->getUniform("RefractionTexture");
	glUniform1i(reflection, 0);
	glUniform1i(refraction, 1);
	
	waterShader->Disable();
	
	normal = ShaderManager::getInstance()->create("normal",
		"Assets/Shaders/cube_vs.glsl",
		"Assets/Shaders/cube_fs.glsl");
	//normal = new Shader();
	//normal->LoadShader(GL_VERTEX_SHADER, "Assets/Shaders/cube_vs.glsl");
	//normal->LoadShader(GL_FRAGMENT_SHADER, "Assets/Shaders/cube_fs.glsl");
	//normal->Prepare();
	//normal->Attach(GL_VERTEX_SHADER);
	//normal->Attach(GL_FRAGMENT_SHADER);
	//normal->BindAttribute(VERTICES, "inPosition");
	//normal->BindAttribute(TEXCOORDS, "inTexCoord");
	//normal->BindAttribute(NORMALS, "inNormal");
	//normal->Link();
	//normal->AddUniform("ModelMatrix");
	//normal->AddUniform("ViewMatrix");
	//normal->AddUniform("ProjectionMatrix");

	//normalDebugger = new Shader();
	normalDebugger = ShaderManager::getInstance()->create("normalDebugger",
		"Assets/Shaders/NormalMapping/normal_vert_refact.glsl",
		"Assets/Shaders/NormalMapping/normal_frag_refact_debug.glsl");
	//normalDebugger->LoadShader(GL_VERTEX_SHADER, "Assets/Shaders/NormalMapping/normal_vert_refact.glsl");
	//normalDebugger->LoadShader(GL_FRAGMENT_SHADER, "Assets/Shaders/NormalMapping/normal_frag_refact_debug.glsl");
	//normalDebugger->LoadShader(GL_VERTEX_SHADER, "Assets/Shaders/BlinnPhong/blinn_phong_vert.glsl");
	//normalDebugger->LoadShader(GL_FRAGMENT_SHADER, "Assets/Shaders/BlinnPhong/blinn_phong_frag.glsl");
	//normalDebugger->Prepare();
	//normalDebugger->Attach(GL_VERTEX_SHADER);
	//normalDebugger->Attach(GL_FRAGMENT_SHADER);
	//normalDebugger->BindAttribute(VERTICES, "inPosition");
	//normalDebugger->BindAttribute(TEXCOORDS, "inTexCoord");
	//normalDebugger->BindAttribute(NORMALS, "inNormal");
	//normalDebugger->BindAttribute(TANGENTS, "inTangent");
	//normalDebugger->BindAttribute(BITANGENTS, "inBiTangent");
	//normalDebugger->Link();
	//normalDebugger->AddUniform("ModelMatrix");
	//normalDebugger->AddUniform("ViewMatrix");
	//normalDebugger->AddUniform("ProjectionMatrix");
	//normalDebugger->AddUniform("LightPosition");
	//normalDebugger->AddUniform("NormalMatrix");
	//normalDebugger->AddUniform("tex");
	//normalDebugger->AddUniform("normalMap");
	//normalDebugger->AddUniform("material");

	//noTexDebugger = new Shader();
	noTexDebugger = ShaderManager::getInstance()->create("noTexDebugger",
		"Assets/Shaders/NormalMapping/normal_vert_refact.glsl",
		"Assets/Shaders/NormalMapping/normal_frag_refact_debug_notex.glsl");
	//noTexDebugger->LoadShader(GL_VERTEX_SHADER, "Assets/Shaders/NormalMapping/normal_vert_refact.glsl");
	//noTexDebugger->LoadShader(GL_FRAGMENT_SHADER, "Assets/Shaders/NormalMapping/normal_frag_refact_debug_notex.glsl");
	//shader->LoadShader(GL_VERTEX_SHADER, "Assets/Shaders/BlinnPhong/blinn_phong_vert.glsl");
	//shader->LoadShader(GL_FRAGMENT_SHADER, "Assets/Shaders/BlinnPhong/blinn_phong_frag.glsl");
	//noTexDebugger->Prepare();
	//noTexDebugger->Attach(GL_VERTEX_SHADER);
	//noTexDebugger->Attach(GL_FRAGMENT_SHADER);
	//noTexDebugger->BindAttribute(VERTICES, "inPosition");
	//noTexDebugger->BindAttribute(TEXCOORDS, "inTexCoord");
	//noTexDebugger->BindAttribute(NORMALS, "inNormal");
	//noTexDebugger->BindAttribute(TANGENTS, "inTangent");
	//noTexDebugger->BindAttribute(BITANGENTS, "inBiTangent");
	//noTexDebugger->Link();
	//noTexDebugger->AddUniform("ModelMatrix");
	//noTexDebugger->AddUniform("ViewMatrix");
	//noTexDebugger->AddUniform("ProjectionMatrix");
	//noTexDebugger->AddUniform("LightPosition");
	//noTexDebugger->AddUniform("NormalMatrix");
	//noTexDebugger->AddUniform("tex");
	//noTexDebugger->AddUniform("normalMap");
	//noTexDebugger->AddUniform("material");

	//skyBoxShader = new Shader();
	//skyBoxShader->LoadShader(GL_VERTEX_SHADER, "Assets/Shaders/skybox/skybox_vert.glsl");
	//skyBoxShader->LoadShader(GL_FRAGMENT_SHADER, "Assets/Shaders/skybox/skybox_frag.glsl");
	//skyBoxShader->Prepare();
	//skyBoxShader->Attach(GL_VERTEX_SHADER);
	//skyBoxShader->Attach(GL_FRAGMENT_SHADER);
	//skyBoxShader->BindAttribute(VERTICES, "inPosition");
	//skyBoxShader->Link();
	//skyBoxShader->AddUniform("ViewMatrix");
	//skyBoxShader->AddUniform("ProjectionMatrix");
	//skyBoxShader->AddUniform("skybox");

	skyBoxShader = ShaderManager::getInstance()->create("skyboxshader",
		"Assets/Shaders/skybox/skybox_vert.glsl",
		"Assets/Shaders/skybox/skybox_frag.glsl");
}
//MOUSE AND KEYBOARD INPUTS

void createMesh()
{
	MeshManager::getInstance()->create("Assets/Models/treeuv.obj", "tree");
	MeshManager::getInstance()->create("Assets/Models/waterPlane.obj", "water");
	MeshManager::getInstance()->create("Assets/Models/barrel.obj", "barrel");
	MeshManager::getInstance()->create("Assets/Models/landscape.obj", "landscape");
	MeshManager::getInstance()->create("Assets/Models/rock1.obj", "rock1");
	MeshManager::getInstance()->create("Assets/Models/rock2.obj", "rock2");
	MeshManager::getInstance()->create("Assets/Models/rock3.obj", "rock3");
	MeshManager::getInstance()->create("Assets/Models/rock4.obj", "rock4");
	MeshManager::getInstance()->create("Assets/Models/rock5.obj", "rock5");

}

void createTextures() {
	TextureManager::getInstance()->create("Assets/Textures/BarkBurned001_COL_1K.jpg", "treediffuse");
	TextureManager::getInstance()->create("Assets/Textures/BarkBurned001_NRM_1K.jpg", "treenormal");
	TextureManager::getInstance()->create("Assets/Textures/waterDUDV.png", "waterdudv");
	TextureManager::getInstance()->create("Assets/Textures/waterNormalMap.png", "waternormal");
	TextureManager::getInstance()->create("Assets/Textures/landscape.jpg", "landscapetex");
	TextureManager::getInstance()->create("Assets/Textures/rocks_01_dif.tga", "rockdiffuse");
	TextureManager::getInstance()->create("Assets/Textures/rocks_01_nm.tga", "rocknormal");
}

void createCubeScene() {
	SceneGraph* main = SceneManager::getInstance()->create("main");
	SkyBox* box = new SkyBox({ 
		"Assets/Textures/cubemap/day_right.png",    //right
		"Assets/Textures/cubemap/day_left.png",     //left
		"Assets/Textures/cubemap/day_down.png",	//bottom
		"Assets/Textures/cubemap/day_up.png",		//top
		"Assets/Textures/cubemap/day_back.png",		//back
		"Assets/Textures/cubemap/day_front.png"		//front
	});
	box->setSkyBoxShader(skyBoxShader);
	main->setSkyBox(box);

	main->getRoot()->setShaderProgram(shader);
	(&camera)->setProjectionMatrix(ProjectionMatrix);
	main->FreeCamera = &camera;
	Material* material = new Material();
	material->LoadMaterial("Assets/Models/untitled.mtl");
	tree = main->createNode();
	tree->material = material;

	tree->setMesh(MeshManager::getInstance()->get("tree"));
	tree->setMatrix(TRANSLATE(0,0.5,0)*tree->GetModelMatrix());
	tree->addTexture("tex", TextureManager::getInstance()->get("treediffuse"));
	tree->addTexture("normalMap", TextureManager::getInstance()->get("treenormal"));

	tree->setActive(true);
	waterNode = main->createNode();
	waterNode->setMesh(MeshManager::getInstance()->get("water"));
	waterNode->setMatrix(Matrix4::TRANSLATE(0, 0, 0) * Matrix4::SCALE(15, 1, 15) * waterNode->GetModelMatrix());
	waterNode->addTexture("tex", TextureManager::getInstance()->get("waterdudv"));
	waterNode->addTexture("normalMap", TextureManager::getInstance()->get("waternormal"));


	waterNode->ChangeDirection(0, Vector4(1, 0, 0, 1));
	waterNode->setShaderProgram(waterShader);
	waterNode->setActive(false);
	SceneNode* underWater = main->createNode();
	underWater->setMesh(MeshManager::getInstance()->get("landscape"));
	underWater->setMatrix(Matrix4::TRANSLATE(0, 0, 0) * Matrix4::SCALE(0.025, 0.025, 0.025) * underWater->GetModelMatrix());
	underWater->setActive(true);
	underWater->addTexture("tex", TextureManager::getInstance()->get("landscapetex"));
	lightSphere = main->createNode();
	lightSphere->setMesh(MeshManager::getInstance()->get("sphere"));
	lightSphere->setMatrix(Matrix4::TRANSLATE(LightPosition.coordinates[0], LightPosition.coordinates[1], LightPosition.coordinates[2])*Matrix4::SCALE(0.2, 0.2, 0.2)*lightSphere->GetModelMatrix());

	rock1 = main->createNode();
	rock1->setShaderProgram(shader);
	rock1->setMesh(MeshManager::getInstance()->get("rock1"));
	rock1->addTexture("tex", TextureManager::getInstance()->get("rockdiffuse"));
	rock1->addTexture("normalMap", TextureManager::getInstance()->get("rocknormal"));
	rock1->setActive(true);
	rock1->setMatrix(TRANSLATE(1, 0, 0) * rock1->GetModelMatrix());

	rock2 = main->createNode();
	rock2->setShaderProgram(shader);
	rock2->setMesh(MeshManager::getInstance()->get("rock2"));
	rock2->addTexture("tex", TextureManager::getInstance()->get("rockdiffuse"));
	rock2->addTexture("normalMap", TextureManager::getInstance()->get("rocknormal"));
	rock2->setActive(true);
	rock2->setMatrix(TRANSLATE(1, 0, -1) * rock2->GetModelMatrix());

	rock3 = main->createNode();
	rock3->setShaderProgram(shader);
	rock3->setMesh(MeshManager::getInstance()->get("rock3"));
	rock3->addTexture("tex", TextureManager::getInstance()->get("rockdiffuse"));
	rock3->addTexture("normalMap", TextureManager::getInstance()->get("rocknormal"));
	rock3->setActive(true);
	rock3->setMatrix(TRANSLATE(-1, 0, -1) * rock3->GetModelMatrix());
	
	rock4 = main->createNode();
	rock4->setShaderProgram(shader);
	rock4->setMesh(MeshManager::getInstance()->get("rock4"));
	rock4->addTexture("tex", TextureManager::getInstance()->get("rockdiffuse"));
	rock4->addTexture("normalMap", TextureManager::getInstance()->get("rocknormal"));
	rock4->setActive(true);
	rock4->setMatrix(TRANSLATE(0, 0, -1) * rock4->GetModelMatrix());

	rock5 = main->createNode();
	rock5->setShaderProgram(shader);
	rock5->setMesh(MeshManager::getInstance()->get("rock5"));
	rock5->addTexture("tex", TextureManager::getInstance()->get("rockdiffuse"));
	rock5->addTexture("normalMap", TextureManager::getInstance()->get("rocknormal"));
	rock5->setActive(true);
	rock5->setMatrix(TRANSLATE(-1, 0, 0) * rock5->GetModelMatrix());
}

void ControlCamera() {

	if (Keyboard::getInstance()->isKeyPressed('w'))
	{
		SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Forward, deltaTime);
		//std::cout << SceneManager::getInstance()->get("main")->FreeCamera->GetCamera() << std::endl;
	}
	if (Keyboard::getInstance()->isKeyPressed('s')) {
		SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Backward, deltaTime);
		//std::cout << SceneManager::getInstance()->get("main")->FreeCamera->GetCamera() << std::endl;

	}
	if (Keyboard::getInstance()->isKeyPressed('a')) {
		SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Left, deltaTime);
		//std::cout << SceneManager::getInstance()->get("main")->FreeCamera->GetCamera() << std::endl;
	}
	if (Keyboard::getInstance()->isKeyPressed('d')) {
		SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Right, deltaTime);
		//std::cout << SceneManager::getInstance()->get("main")->FreeCamera->GetCamera() << std::endl;

	}
	if (Keyboard::getInstance()->isKeyPressed('q')) {

		SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Down, deltaTime);
		//std::cout << SceneManager::getInstance()->get("main")->FreeCamera->GetCamera() << std::endl;
	}
	if (Keyboard::getInstance()->isKeyPressed('e')) {
		SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Up, deltaTime);
		//std::cout << SceneManager::getInstance()->get("main")->FreeCamera->GetCamera() << std::endl;
	}
	if (Keyboard::getInstance()->isKeyPressed('m')) {
	//	envoirnment->ChangeDirection(deltaTime* 360.0f, Vector4(0, 1, 0, 1));
	}
	if (Keyboard::getInstance()->isKeyPressed('k')) {

		//tangram->ChangeDirection(deltaTime* 360.0f, Vector4(0, 1, 0, 1));
		tree->setMatrix(TRANSLATE(0, -1 * deltaTime, 0) * tree->GetModelMatrix());
	}
	if (Keyboard::getInstance()->isKeyPressed('j')) {
		tree->setMatrix(TRANSLATE(-1 * deltaTime, 0, 0) * tree->GetModelMatrix());
	}
	if (Keyboard::getInstance()->isKeyPressed('i')) {

		tree->setMatrix(TRANSLATE(0, 1 * deltaTime, 0)*tree->GetModelMatrix());
	}
	if (Keyboard::getInstance()->isKeyPressed('u')) {
		tree->setMatrix(TRANSLATE(1 * deltaTime, 0, 0) * tree->GetModelMatrix());
	}
	if (Keyboard::getInstance()->isKeyPressed('l')) {
		tree->ChangeDirection(deltaTime*360.0f, Vector4(0, 1, 0, 1));
	}
	if (Keyboard::getInstance()->isKeyPressed('z')) {
		LightPosition += Vector3(-1*deltaTime, 0, 0);
		std::cout << "LightPosition: " << LightPosition << std::endl;
	}
	if (Keyboard::getInstance()->isKeyPressed('x')) {
		LightPosition += Vector3(1*deltaTime, 0, 0);
		std::cout << "LightPosition: " << LightPosition << std::endl;
	}
	if (Keyboard::getInstance()->isKeyPressed('c')) {
		LightPosition += Vector3(0, 1 * deltaTime, 0);
		std::cout << "LightPosition: " << LightPosition << std::endl;
	}
	if (Keyboard::getInstance()->isKeyPressed('v')) {
		LightPosition += Vector3(0, -1 * deltaTime, 0);
		std::cout << "LightPosition: " << LightPosition << std::endl;
	}
	if (Keyboard::getInstance()->isKeyPressed('b')) {
		LightPosition += Vector3(0, 0, -1 * deltaTime);
		std::cout << "LightPosition: " << LightPosition << std::endl;
	}
	if (Keyboard::getInstance()->isKeyPressed('n')) {
		LightPosition += Vector3(0, 0, 1 * deltaTime);
		std::cout << "LightPosition: " << LightPosition << std::endl;
	}
	if (Keyboard::getInstance()->isKeyPressed('i')) {
		tree->setMatrix(TRANSLATE(0, 1 * deltaTime, 0)*tree->GetModelMatrix());
	}
	
}



void mouseMovement(int x, int y) {
		mouse->Movement(x, y);
		SceneManager::getInstance()->get("main")->FreeCamera->LookAt(mouse->MouseX, mouse->MouseY);
}
void cleanup()
{
	MeshManager::freeInstance();
	SceneManager::freeInstance();
	AnimationManager::freeInstance();
	TextureManager::freeInstance();
	ShaderManager::freeInstance();
}


void updateMatrixes() {

	auto shadersMap = ShaderManager::getInstance()->getShaders();
	if (shadersMap.size() > 0) {
		for (auto lt = shadersMap.begin(); lt != shadersMap.end(); ++lt) {
			lt->second->Enable();
			if (lt->second->getUniform("ViewMatrix") != -1)
				glUniformMatrix4fv(lt->second->getUniform("ViewMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetCamera().data);
			if (lt->second->getUniform("ProjectionMatrix") != -1)
				glUniformMatrix4fv(lt->second->getUniform("ProjectionMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetProjectionMatrix().data);
			if(lt->second->getUniform("cameraPos") != -1)
				glUniform3fv(lt->second->getUniform("cameraPos"), 1,
					SceneManager::getInstance()->get("main")->FreeCamera->position.coordinates);
			if(lt->second->getUniform("LightPosition") != -1)
				glUniform3fv(lt->second->getUniform("LightPosition"), 1, LightPosition.coordinates);

			lt->second->Disable();
		}
	}



/*
	waterShader->Enable();
	glUniformMatrix4fv(waterShader->getUniform("ViewMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetCamera().data);
	glUniformMatrix4fv(waterShader->getUniform("ProjectionMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetProjectionMatrix().data);
	//glUniform3fv(waterShader->getUniform("LightPosition"),1,LightPosition.coordinates);
	shader->Enable();
	glUniformMatrix4fv(shader->getUniform("ViewMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetCamera().data);
	glUniformMatrix4fv(shader->getUniform("ProjectionMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetProjectionMatrix().data);
	normal->Enable();
	glUniformMatrix4fv(normal->getUniform("ViewMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetCamera().data);
	glUniformMatrix4fv(normal->getUniform("ProjectionMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetProjectionMatrix().data);
	normal->Disable();
	normalDebugger->Enable();
	glUniformMatrix4fv(normalDebugger->getUniform("ViewMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetCamera().data);
	glUniformMatrix4fv(normalDebugger->getUniform("ProjectionMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetProjectionMatrix().data);
	normalDebugger->Disable();
	noTexDebugger->Enable();
	glUniformMatrix4fv(noTexDebugger->getUniform("ViewMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetCamera().data);
	glUniformMatrix4fv(noTexDebugger->getUniform("ProjectionMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetProjectionMatrix().data);
	noTexDebugger->Disable();
	skyBoxShader->Enable();
	glUniformMatrix4fv(skyBoxShader->getUniform("ViewMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetCamera().data);
	glUniformMatrix4fv(skyBoxShader->getUniform("ProjectionMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetProjectionMatrix().data);
	//glUniformMatrix4fv(skyBoxShader->getUniform("ProjectionMatrix"), 1, GL_FALSE, Matrix4::ProjectionMatrix(PI/2, (float)WinX/(float)WinY, 1, 50).data);
	skyBoxShader->Disable();
	*/
}

void drawSceneWithReflections()
{
	SceneGraph* sceneGraph = SceneManager::getInstance()->get("main");
	glEnable(GL_CLIP_DISTANCE0);
	waterNode->setActive(false);
	water->bindReflectionBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const float distance = 2 * SceneManager::getInstance()->get("main")->FreeCamera->position.coordinates[1];
	SceneManager::getInstance()->get("main")->FreeCamera->position.coordinates[1] -= distance;
	SceneManager::getInstance()->get("main")->FreeCamera->invertPitch();
	updateMatrixes();
	shader->Enable();
	glUniform4f(shader->getUniform("plane"), 0, 1, 0, -0);
	sceneGraph->Draw(LightPosition);
	SceneManager::getInstance()->get("main")->FreeCamera->position.coordinates[1] += distance;
	SceneManager::getInstance()->get("main")->FreeCamera->invertPitch();
	updateMatrixes();

	//render refraction texture
	water->bindRefractionBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//normalShader->use();
	//glUniform4f(normalShader->getUniform("plane"), 0, 0, -1, 0);
	shader->Enable();
	glUniform4f(shader->getUniform("plane"), 0, -1, 0, -0);
	sceneGraph->Draw(LightPosition);

	//render to screen
	waterNode->setActive(true);
	waterShader->Enable();
	water->calculateMoveFactor(waterShader->getUniform("moveFactor"), deltaTime);
	water->sendCameraPosition(waterShader->getUniform("cameraPosition"), SceneManager::getInstance()->get("main")->FreeCamera->position);
	//glUniform3fv(waterShader->GetUniform("cona"), 1, SceneManager::getInstance()->get("main")->FreeCamera->position.coordinates);
	glDisable(GL_CLIP_DISTANCE0);
	water->unbindCurrentFrameBuffer();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, water->getReflectionTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, water->getRefractionTexture());
	sceneGraph->Draw(LightPosition);

}
void drawSceneWithoutReflections()
{
	updateMatrixes();
	SceneManager::getInstance()->get("main")->Draw(LightPosition);
}

void drawScene()
{
//	drawSceneWithoutReflections();

	drawSceneWithReflections();
}

void debugMode() {
	updateMatrixes();
	waterNode->setActive(false);
	glViewport(0, 0, WinX*0.5, WinY*0.5);
	tree->setShaderProgram(noTexDebugger);
	rock1->setShaderProgram(noTexDebugger);
	rock2->setShaderProgram(noTexDebugger);
	rock3->setShaderProgram(noTexDebugger);
	rock4->setShaderProgram(noTexDebugger);
	rock5->setShaderProgram(noTexDebugger);
	SceneManager::getInstance()->get("main")->Draw(LightPosition);
	glViewport(WinX*0.5, 0, WinX*0.5, WinY*0.5);
	tree->setShaderProgram(normalDebugger);
	rock1->setShaderProgram(normalDebugger);
	rock2->setShaderProgram(normalDebugger);
	rock3->setShaderProgram(normalDebugger);
	rock4->setShaderProgram(normalDebugger);
	rock5->setShaderProgram(normalDebugger);
	SceneManager::getInstance()->get("main")->Draw(LightPosition);
	//drawSceneWithReflections();
	tree->setShaderProgram(shader);
	rock1->setShaderProgram(shader);
	rock2->setShaderProgram(shader);
	rock3->setShaderProgram(shader);
	rock4->setShaderProgram(shader);
	rock5->setShaderProgram(shader);
	//left top
	glViewport(0, WinY*0.5, WinX*0.5, WinY*0.5);
	
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH, GL_NICEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//tree->setShaderProgram(noTexDebugger);
	SceneManager::getInstance()->get("main")->Draw(LightPosition);
	//drawSceneWithoutReflections();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_LINE_SMOOTH);
	//glDisable(GL_BLEND);

	//tree->setShaderProgram(shader);
	//wireframe mode
	//right top
	glViewport(WinX*0.5, WinY*0.5, WinX*0.5, WinY*0.5);
	//glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_MULTISAMPLE);
	SceneManager::getInstance()->get("main")->Draw(LightPosition);;
	//drawSceneWithReflections();
	glDisable(GL_MULTISAMPLE);
	//glDisable(GL_POLYGON_SMOOTH);
	glViewport(0, 0, WinX, WinY); //restore default
	waterNode->setActive(true);
}

void display()
{
		++FrameCount;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (DEBUG) {
			debugMode();
		}else{
			if (msaa)
			{
				glEnable(GL_MULTISAMPLE_ARB);
			}
			else
			{
				glDisable(GL_MULTISAMPLE_ARB);
			}

			drawScene();
		}
		glutSwapBuffers();
}



void idle()
{
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (currentTime - previousTime) / 1000.0f;
	previousTime = currentTime;
	
	if (!Rewind)
		AnimationManager::getInstance()->play(deltaTime);
	else
		AnimationManager::getInstance()->rewind(deltaTime);
	ControlCamera();
	waterShader->Enable();
	GLuint v = waterShader->getUniform("ViewMatrix");
	GLuint p = waterShader->getUniform("ProjectionMatrix");
	Camera* a = SceneManager::getInstance()->get("main")->FreeCamera;
	glUniformMatrix4fv(v, 1, GL_FALSE, a->GetCamera().data);
	glUniformMatrix4fv(p, 1, GL_FALSE, a->GetProjectionMatrix().data);

	glutPostRedisplay();
}

void reshape(int w, int h)
{
	Texture* t = new Texture();
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
	//ProjectionMatrix = Matrix4::ProjectionMatrix(PI/2, (float)WinX / (float)WinY, 1, 300);
	ProjectionMatrix = Matrix4::ProjectionMatrix(PI / 4, (float)WinX / (float)WinY, 1, 50);
	SceneManager::getInstance()->get("main")->FreeCamera->setProjectionMatrix(ProjectionMatrix);
	water->setDimensions(WinX , WinY);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}


void handleKeyUp(unsigned char Key, int x, int y) {
	Keyboard::getInstance()->releaseKey(Key);
	switch (Key) {	
	case '7':
		if (msaa) {
			std::cout << "MSAA: off" << std::endl;
		}
		else {
			std::cout << "MSAA: on" << std::endl;
		}
		msaa = !msaa;
		break;
	case '<':
		DEBUG = !DEBUG;
		std::cout << "Debug Mode: " << DEBUG << std::endl;
		break;
	case '0':
		AnimationManager::getInstance()->toggle();
		break;
	case '1':
		Rewind = !Rewind;
		break;
	case 0x2c:
		TakeScreenshot();
		break;
	case 'r':
		const float distance = 2 * SceneManager::getInstance()->get("main")->FreeCamera->position.coordinates[1];
		SceneManager::getInstance()->get("main")->FreeCamera->position.coordinates[1] -= distance;
		SceneManager::getInstance()->get("main")->FreeCamera->invertPitch();
		//std::cout << SceneManager::getInstance()->get("main")->FreeCamera->GetCamera() << std::endl;
		std::cout << "**********CAMERA*************" << std::endl;
		std::cout << "Position: " << SceneManager::getInstance()->get("main")->FreeCamera->position << std::endl;
		std::cout << "Front: " << SceneManager::getInstance()->get("main")->FreeCamera->front << std::endl;
		std::cout << "Up: " << SceneManager::getInstance()->get("main")->FreeCamera->currentUp << std::endl;
		std::cout << "*****************************" << std::endl;
		break;
	}

	
}

void mouseHandle(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouse->MouseState = 1;

		}
		if (state == GLUT_UP) {
			mouse->MouseState = 0;
			mouse->FirstClick = true;
		}
	}
}


void handleKeyDown(unsigned char Key, int x, int y) {
	Keyboard::getInstance()->pressKey(Key);
}

void mouseWheel(int button, int dir, int x, int y)
{
	SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Forward, dir);
}
/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutKeyboardUpFunc(handleKeyUp);
	glutKeyboardFunc(handleKeyDown);
	glutMotionFunc(mouseMovement);
	glutMouseFunc(mouseHandle);
	glutMouseWheelFunc(mouseWheel);

}

void setupOpenGL() {
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);

	
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
	GLint data;
	glGetIntegerv(GL_MAX_SAMPLES, &data);
	std::cout << "Max MSAA: MSAA(x " << data << ")" << std::endl;
	GLint maxTextureUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
	std::cout << "Max Number Of Texture Units: " << maxTextureUnits << std::endl;
	glutSetOption(GLUT_MULTISAMPLE, maxTextureUnits);
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	setupCallbacks();
	createShaderProgram();
	createTextures();
	createMesh();
	createCubeScene();

	
	//createScene();
	//createAnimation();
	previousTime = glutGet(GLUT_ELAPSED_TIME);
	mouse = new Mouse();
	currentPath = argv[0];
	std::cout << currentPath << std::endl;
	vec3 vec = vec3(0, 0, 0);
	water = new Water(WinX, WinY);
}
using namespace Managers;
int main(int argc, char** argv) {
	init(argc, argv);
	glutMainLoop();

	exit(EXIT_SUCCESS);
}
