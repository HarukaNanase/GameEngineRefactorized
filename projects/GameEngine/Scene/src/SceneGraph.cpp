#include "SceneGraph.h"



SceneGraph::SceneGraph()
{
	this->root = new SceneNode();
}


SceneGraph::~SceneGraph()
{

}

SceneNode* SceneGraph::getRoot()
{
	return (this->root);
}

SceneNode* SceneGraph::createNode()
{
	SceneNode* node = this->root->createNode();
	return node;
}

QuaternionCamera* SceneGraph::getCamera()
{
	return this->camera;
}

void SceneGraph::setCamera(QuaternionCamera* camera)
{
	this->camera = camera;
}

void SceneGraph::Draw(Vector3 LightPosition)
{
	//Matrix3 NormalMatrix = Matrix3::ConvertFromMatrix4(DrawMatrix).inverse().transpose();
	this->root->shaderProgram->Enable();
	if (this->root->shaderProgram->getUniform("cameraPos") != -1)
		glUniform3fv(this->root->shaderProgram->getUniform("cameraPos"),1, this->FreeCamera->position.coordinates);
		//glUniformMatrix4fv(this->root->shaderProgram->getUniform("ViewMatrix"), 1, GL_FALSE, this->FreeCamera->GetCamera().data);
		//glUniformMatrix4fv(this->root->shaderProgram->getUniform("ProjectionMatrix"), 1, GL_FALSE, FreeCamera->GetProjectionMatrix().data);
	if (this->root->shaderProgram->getUniform("LightPosition") != -1)
		glUniform3fv(this->root->shaderProgram->getUniform("LightPosition"), 1, LightPosition.coordinates);

	//	std::cout << this->root->shaderProgram->getUniform("LightPosition") << std::endl;

	this->root->Draw();
	this->root->shaderProgram->Disable();
}

void SceneGraph::Release()
{
	this->root->Release();
	this->Destroy();
}

void SceneGraph::Destroy()
{
	delete(this);
}
