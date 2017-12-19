#include "Mesh.h"



Mesh::Mesh()
{
}

Mesh::Mesh(std::string filepath)
{
	this->VerticesIdx = std::vector<unsigned int>();
	this->LoadFromFile(filepath);
	this->HasTexs = (this->TexsIdx.size() > 0);
	this->HasNormals = (this->NormalsIdx.size() > 0);
	this->ProcessMesh();
	this->CreateBufferObjects();
	this->FreeMeshData();

}

void Mesh::FreeMeshData()
{
	this->VerticesData.clear();
	this->TexsData.clear();
	this->NormalsData.clear();
	this->VerticesIdx.clear();
	this->TexsIdx.clear();
	this->NormalsIdx.clear();
}

void Mesh::ProcessMesh()
{
	for (auto i = 0; i < this->VerticesIdx.size(); i++) {
		auto vi = this->VerticesIdx[i];
		Vertex v = this->VerticesData[vi - 1];
		this->Vertices.push_back(v);
		if (this->HasTexs) {
			auto ti = this->TexsIdx[i];
			auto t = this->TexsData[ti - 1];
			this->Texs.push_back(t);
		}
		if (this->HasNormals) {
			auto ni = this->NormalsIdx[i];
			auto n = this->NormalsData[ni - 1];
			this->Normals.push_back(n);
		}
	}
}

void Mesh::Draw()
{


	//calculate Normal Matrix

	
	
	/*if(shaderProgram->GetUniform("material")>0)
	{
		glUniform3fv(shaderProgram->GetUniform("material.ambient"), 1, Vector3(0.0f, 0.05f, 0.0f).coordinates);
		glUniform3fv(shaderProgram->GetUniform("material.diffuse"), 1, Vector3(0.4f, 0.5f, 0.4f).coordinates);
		glUniform3fv(shaderProgram->GetUniform("material.specular"), 1, Vector3(0.04f, 0.7f, 0.04f).coordinates);
		glUniform1f(shaderProgram->GetUniform("material.shininess"), 0.078125);
	}
	*/
	//tex shit
	
	//tex shit
	glBindVertexArray(this->VaoId);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)this->Vertices.size());
	glBindVertexArray(0);
}


Mesh::~Mesh()
{

}

void Mesh::parseVertexs(std::stringstream & in)
{
	Vertex v;
	in >> v;
	//std::cout <<"Vertex: "<< v << std::endl;
	this->VerticesData.push_back(v);
}

void Mesh::parseTextures(std::stringstream & in)
{
	TexCoordinates t;
	in >> t;
	//std::cout << t << std::endl;
	this->TexsData.push_back(t);

}

void Mesh::parseNormals(std::stringstream & in)
{
	Normal n;
	in >> n;
	//std::cout <<"Normal: " << n << std::endl;
	this->NormalsData.push_back(n);

}

void Mesh::parseFaces(std::stringstream & in)
{
	std::string token;
	for (int i = 0; i < 3; i++) {
		std::getline(in, token, '/');
		if (token.size() > 0) 
			this->VerticesIdx.push_back(std::stoi(token));
		std::getline(in, token, '/');
		if (token.size() > 0)
			this->TexsIdx.push_back(std::stoi(token));
		std::getline(in, token, ' ');
		if (token.size() > 0) 
			this->NormalsIdx.push_back(std::stoi(token));
	}
	//std::cout << in.str() << std::endl;
}

void Mesh::parseComments(std::stringstream & in)
{
	std::cout << in.str() << std::endl;
}

bool Mesh::CreateBufferObjects()
{
	glGenVertexArrays(1, &this->VaoId);
	glBindVertexArray(this->VaoId);
	{
		glGenBuffers(1, &this->VboVertices);
		glBindBuffer(GL_ARRAY_BUFFER, this->VboVertices);
		glBufferData(GL_ARRAY_BUFFER, this->Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTEX);
		glVertexAttribPointer(VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		if(this->HasTexs)
		{
			glGenBuffers(1, &this->VboTextures);
			glBindBuffer(GL_ARRAY_BUFFER, this->VboTextures);
			glBufferData(GL_ARRAY_BUFFER, this->Texs.size() * sizeof(TexCoordinates), &Texs[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(TEXCOORDS);
			glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(TexCoordinates), 0);
		}
		if(this->HasNormals)
		{
			glGenBuffers(1, &this->VboNormals);
			glBindBuffer(GL_ARRAY_BUFFER, this->VboNormals);
			glBufferData(GL_ARRAY_BUFFER, this->Normals.size() * sizeof(Normal), &Normals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(NORMALS);
			glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), 0);
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	return true;
}

void Mesh::LoadFromFile(std::string filename)
{

	std::ifstream file = std::ifstream(filename);
	if (file.fail()) {
		std::string error = "The mesh " + filename + " seems to be missing.";
		throw std::ios_base::failure(error);
		this->Error = true;
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		this->ParseLine(std::stringstream(line));
	}
	file.close();
	
}

void Mesh::ParseLine(std::stringstream & in)
{
	std::string s;
	in >> s;
	if (s == "v")
		parseVertexs(in);
	else if (s == "vt")
		parseTextures(in);
	else if (s == "vn")
		parseNormals(in);
	else if (s == "f")
		parseFaces(in);
	else if (s == "#")
		parseComments(in);
	
}
