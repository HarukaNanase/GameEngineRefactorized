#version 330 core



in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

uniform vec3 LightPosition;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

out vec4 L;
out vec4 V;
out vec4 N;
out vec4 E;
void main(){
	vec4 MCPosition = vec4(inPosition,1.0);
	vec4 Lpos = ViewMatrix * vec4(LightPosition, 1.0);
	V = ViewMatrix * ModelMatrix * MCPosition;
	L = Lpos - V;
	E = -V;
	N = vec4(NormalMatrix * inNormal,1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;

}