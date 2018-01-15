#version 330 core

in vec3 texCoords;
out vec4 FragmentColor;
uniform samplerCube skybox;

void main(){
	FragmentColor = texture(skybox, texCoords);
	//FragmentColor = vec4(1,0,0,1);
}