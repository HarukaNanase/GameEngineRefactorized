#pragma once
#ifndef SCENENODE_H
#define SCENENODE_H
#include "Mesh.h"
#include "Matrix4.h"
#include "QuaternionCamera.h"
#include <unordered_map>
#include "Material.h"

class SceneNode
{
	bool isActive = true;

public:
	SceneNode();
	~SceneNode();
	// attributes
	SceneNode* parent;
	Shader* shaderProgram;
	Mesh* mesh;
	Quaternion quat;
	Matrix4 ModelMatrix;
	Matrix4 DrawMatrix;
	//children pointers
	std::vector <SceneNode*> children;
	//methods
	//Changes the direction of this node according to an Angle (In degrees) and a vec4 (the axis)
	void ChangeDirection(float angle, Vector4 axis);
	//Texture of this node
	Texture* tex;
	//Material of this node
	Material* material;
	//Set Active - False->Won't be drawn; True->Will be drawn;
	void setActive(bool b);
	//Set Mesh -> Set Mesh Pointer for this node
	void setMesh(Mesh* newMesh);
	//Retrieve the pointer to the mesh assigned to this node.
	Mesh* GetMesh();
	//Retrieve the ModelMatrix of this node.
	Matrix4 GetModelMatrix();
	//Set the ModelMatrix of this node and updates children.
	void setMatrix(Matrix4 newMatrix);
	//Create a new SceneNode attached to this node.
	SceneNode* createNode();
	//Sets a pointer to the shader to be used by this node.
	void setShaderProgram(Shader* shader);
	//Draws this node.
	void Draw();
	//Updates children nodes with changes done to this done.
	void Update();
	//Releases the current node and all its children.
	void Release();
	//Destroy this node.
	void Destroy();

};
#endif

