#version 400 core

layout (location = 0) in vec3 inPosition;


out vec4 clipSpace;
out vec2 textureCoords;
out vec3 toCameraVector;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform	mat4 ProjectionMatrix;
uniform vec3 cameraPosition;

const float tiling = 1.0;

void main(void) {
	vec4 worldPosition = ModelMatrix * vec4 (inPosition, 1.0);
    clipSpace = ProjectionMatrix * ViewMatrix * worldPosition;
    gl_Position = clipSpace;
    textureCoords = vec2(inPosition.x/2.0 + 0.5, inPosition.y/2.0 + 0.5) * tiling;
    toCameraVector = cameraPosition - worldPosition.xyz;
}