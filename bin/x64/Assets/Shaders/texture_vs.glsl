#version 330 core

in vec3 inPosition;
in vec2 inTexCoord;
in vec3 inNormal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 exPosition;
out vec2 exTexCoord;
out vec3 exNormal;

void main(){
	exPosition = inPosition;
	exTexCoord = inTexCoord;
	exNormal = inNormal;

	vec4 MCPosition = vec4(inPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;

}