#include "Mesh.h"
#include "Matrix4.h"
#include "QuaternionCamera.h"
#include "Mouse.h"
#include "SceneGraph.h"
#include "MeshManager.h"
#include "SceneManager.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "Keyboard.h"
#include "FreeImage.h"
#include "Convention.h"
#include "Water.h"

using namespace Managers;

#define VERTICES 0
std::string currentPath;
int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;
#define CAPTION "Hello Modern 3D World"
Shader* shader;
Shader* waterShader;
Shader* normal;
#define PI 3.14159265358979323846
#define SCALE(x,y,z) Matrix4::SCALE(x,y,z)
#define TRANSLATE(x,y,z) Matrix4::TRANSLATE(x,y,z)
//QuaternionCamera camera = QuaternionCamera(Vector3(0,0,-5));
Matrix4 ProjectionMatrix = Matrix4::ProjectionMatrix(PI/6, 640.0f / 480.0f, 1, 50);
Camera camera = Camera(Vector3(5, 10, 5), Vector3(0, 0, 0));
Mouse* mouse;
Water* water;
SceneNode* waterNode;
float currentTime = 0, deltaTime = 0, previousTime = 0;


SceneNode* envoirnment, *tangram;
bool Rewind = false;

SceneNode *first, *second, *third, *fourth, *fifth, *sixth, *seventh;
SceneNode *cube;
SceneNode * lightSphere;
Vector3 LightPosition(0.0,2.5f,5.0f);

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
	shader = new Shader();
	shader->LoadShader(GL_VERTEX_SHADER, "Assets/Shaders/blinn_phong_vert.glsl");
	shader->LoadShader(GL_FRAGMENT_SHADER, "Assets/Shaders/blinn_phong_frag.glsl");
	shader->Prepare();
	shader->Attach(GL_VERTEX_SHADER);
	shader->Attach(GL_FRAGMENT_SHADER);
	shader->BindAttribute(VERTICES, "inPosition");
	shader->BindAttribute(TEXCOORDS, "inTexCoord");
	shader->BindAttribute(NORMALS, "inNormal");
	shader->Link();
	shader->AddUniform("ModelMatrix");
	shader->AddUniform("ViewMatrix");
	shader->AddUniform("ProjectionMatrix");
	shader->AddUniform("LightPosition");
	shader->AddUniform("NormalMatrix");
	shader->AddUniform("tex");
	shader->AddUniform("material");

	waterShader = new Shader();
	waterShader->LoadShader(GL_VERTEX_SHADER, "Assets/Shaders/Water.vert");
	waterShader->LoadShader(GL_FRAGMENT_SHADER, "Assets/Shaders/Water.frag");
	waterShader->Prepare();
	waterShader->Attach(GL_VERTEX_SHADER);
	waterShader->Attach(GL_FRAGMENT_SHADER);
	waterShader->BindAttribute(VERTICES, "inPosition");
	waterShader->BindAttribute(TEXCOORDS, "inTexCoord");
	waterShader->BindAttribute(NORMALS, "inNormal");
	waterShader->Link();
	waterShader->Enable();
	GLuint reflection = waterShader->getUniform("ReflectionTexture");
	GLuint refraction = waterShader->getUniform("RefractionTexture");
	glUniform1i(reflection, 0);
	glUniform1i(refraction, 1);
	waterShader->Disable();
	normal = new Shader();
	normal->LoadShader(GL_VERTEX_SHADER, "Assets/Shaders/cube_vs.glsl");
	normal->LoadShader(GL_FRAGMENT_SHADER, "Assets/Shaders/cube_fs.glsl");
	normal->Prepare();
	normal->Attach(GL_VERTEX_SHADER);
	normal->Attach(GL_FRAGMENT_SHADER);
	normal->BindAttribute(VERTICES, "inPosition");
	normal->BindAttribute(TEXCOORDS, "inTexCoord");
	normal->BindAttribute(NORMALS, "inNormal");
	normal->Link();
	normal->AddUniform("ModelMatrix");
	normal->AddUniform("ViewMatrix");
	normal->AddUniform("ProjectionMatrix");



}
//MOUSE AND KEYBOARD INPUTS

void createMesh()
{
	MeshManager::getInstance()->create("Assets/Models/kled.obj", "sphere");
	MeshManager::getInstance()->create("Assets/Models/treeuv.obj", "tree");
	MeshManager::getInstance()->create("Assets/Models/jinx.obj", "jinx");
	MeshManager::getInstance()->create("Assets/Models/waterPlane.obj", "water");
	
}
void createCubeScene() {
	SceneGraph* main = SceneManager::getInstance()->create("main");
	main->getRoot()->setShaderProgram(shader);
	camera.setProjectionMatrix(ProjectionMatrix);
	main->FreeCamera = &camera;
	Material* material = new Material();
	material->LoadMaterial("Assets/Models/treeuv_real.mtl");
	//Texture* tex1 = new Texture();
	//tex1->LoadTexture("Assets/Textures/kled.tga");

	cube = main->createNode();
	cube->material = material;
	//cube->tex = tex1;
	cube->setMesh(MeshManager::getInstance()->get("jinx"));
	cube->setMatrix(Matrix4::TRANSLATE(0,0,0)*Matrix4::SCALE(0.005, 0.005, 0.005) * cube->GetModelMatrix());
	Texture* tex2 = new Texture();
	tex2->LoadTexture("Assets/Textures/b_jinx.tga");
	//cube->tex = tex2;
	//SceneNode* jinx = cube->createNode();
	//jinx->tex = tex2;
	//jinx->setMesh(MeshManager::getInstance()->get("jinx"));
	
	waterNode = main->createNode();
	waterNode->setMesh(MeshManager::getInstance()->get("water"));
	waterNode->setMatrix(Matrix4::TRANSLATE(0, 0, 0) *Matrix4::SCALE(5, 1, 5) * waterNode->GetModelMatrix());

	waterNode->ChangeDirection(0, Vector4(1, 0, 0, 1));
//	waterNode->setColor(Vec4(0.08, 0.05, 0.9, 1));
	waterNode->setShaderProgram(waterShader);
	waterNode->setActive(false);


	SceneNode* underWater = main->createNode();
	underWater->setMesh(MeshManager::getInstance()->get("water"));
	underWater->setMatrix(Matrix4::TRANSLATE(0, -5, 0) * Matrix4::SCALE(5, 1, 5) * underWater->GetModelMatrix());
	underWater->setActive(true);
	underWater->setShaderProgram(normal);
	//lightSphere = main->createNode();
	//lightSphere->setMesh(MeshManager::getInstance()->get("sphere"));
	//lightSphere->setMatrix(Matrix4::TRANSLATE(LightPosition.coordinates[0], LightPosition.coordinates[1], LightPosition.coordinates[2])*Matrix4::SCALE(0.2, 0.2, 0.2)*lightSphere->GetModelMatrix());

}

void ControlCamera() {

	if (Keyboard::getInstance()->isKeyPressed('w'))
	{
		SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Forward, deltaTime);
	}
	if (Keyboard::getInstance()->isKeyPressed('s')) {
		SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Backward, deltaTime);

	}
	if (Keyboard::getInstance()->isKeyPressed('a')) {
		SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Left, deltaTime);
	}
	if (Keyboard::getInstance()->isKeyPressed('d')) {
		SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Right, deltaTime);

	}
	if (Keyboard::getInstance()->isKeyPressed('q')) {

		float AngleZ = 360.0f*0.25f;
		//SceneManager::getInstance()->get("main")->getCamera()->ChangeOrientation(AngleZ, Vector4(0, 0, 1, 1));
	}
	if (Keyboard::getInstance()->isKeyPressed('e')) {

		float AngleZ = -360.0f*0.25f;
		//SceneManager::getInstance()->get("main")->getCamera()->ChangeOrientation(AngleZ, Vector4(0, 0, 1, 1));
	}
	if (Keyboard::getInstance()->isKeyPressed('m')) {
	//	envoirnment->ChangeDirection(deltaTime* 360.0f, Vector4(0, 1, 0, 1));
	}
	if (Keyboard::getInstance()->isKeyPressed('n')) {
	//	envoirnment->ChangeDirection(-deltaTime*360.0f, Vector4(0, 1, 0, 1));
	}
	if (Keyboard::getInstance()->isKeyPressed('k')) {

		//tangram->ChangeDirection(deltaTime* 360.0f, Vector4(0, 1, 0, 1));
		cube->setMatrix(TRANSLATE(0, -1 * deltaTime, 0) * cube->GetModelMatrix());
	}
	if (Keyboard::getInstance()->isKeyPressed('j')) {
		cube->setMatrix(TRANSLATE(-1 * deltaTime, 0, 0) * cube->GetModelMatrix());
	}
	if (Keyboard::getInstance()->isKeyPressed('i')) {

		cube->setMatrix(TRANSLATE(0, 1 * deltaTime, 0)*cube->GetModelMatrix());
	}
	if (Keyboard::getInstance()->isKeyPressed('u')) {
		cube->setMatrix(TRANSLATE(1 * deltaTime, 0, 0) * cube->GetModelMatrix());
	}
	if (Keyboard::getInstance()->isKeyPressed('l')) {
		cube->ChangeDirection(deltaTime*360.0f, Vector4(0, 1, 0, 1));
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
}


void updateMatrixes() {
	waterShader->Enable();
	glUniformMatrix4fv(waterShader->getUniform("ViewMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetCamera().data);
	glUniformMatrix4fv(waterShader->getUniform("ProjectionMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetProjectionMatrix().data);
	shader->Enable();
	glUniformMatrix4fv(shader->getUniform("ViewMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetCamera().data);
	glUniformMatrix4fv(shader->getUniform("ProjectionMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetProjectionMatrix().data);
	normal->Enable();
	glUniformMatrix4fv(normal->getUniform("ViewMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetCamera().data);
	glUniformMatrix4fv(normal->getUniform("ProjectionMatrix"), 1, GL_FALSE, SceneManager::getInstance()->get("main")->FreeCamera->GetProjectionMatrix().data);
	normal->Disable();

}

void drawScene()
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
	glDisable(GL_CLIP_DISTANCE0);
	water->unbindCurrentFrameBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, water->getReflectionTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, water->getRefractionTexture());
	sceneGraph->Draw(LightPosition);
	
}

void display()
{
		++FrameCount;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawScene();
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
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
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
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	setupCallbacks();
	createShaderProgram();
	createMesh();
	//createScene();
	//createAnimation();
	createCubeScene();
	previousTime = glutGet(GLUT_ELAPSED_TIME);
	mouse = new Mouse();
	currentPath = argv[0];
	std::cout << currentPath << std::endl;
	vec3 vec = vec3(0, 0, 0);
	water = new Water();
}
using namespace Managers;
int main(int argc, char** argv) {
	init(argc, argv);
	glutMainLoop();

	exit(EXIT_SUCCESS);
}
