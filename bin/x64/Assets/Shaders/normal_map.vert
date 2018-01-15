#version 330 core

in(layout = 0) vec3 inPosition;
in(layout = 1) vec2 inTexCoord;
in(layout = 2) vec3 inNormal;
in(layout = 3) vec4 inTangent;
in(layout = 4) vec3 inBiTangent;

out vec3 exPosition;
out vec2 exTexCoord;
out vec3 exNormal;
out Mat4 TBN;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;