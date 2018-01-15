#version 330 core

in vec3 inPosition;
in vec2 inTexCoord;
in vec3 inNormal;
in vec4 inTangent;
in vec3 inBiTangent;


out vec3 exPosition;
out vec2 exTexCoord;
out vec3 exNormal;
out vec4 exTangent;
out vec3 exBiTangent;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;
uniform vec4 plane;
uniform vec3 cameraPos;


out vec3 lightPos;
out vec3 viewPos;
out vec3 fragPos;


void main(void)
{
	exTexCoord = inTexCoord;

	vec4 MCPosition = vec4(inPosition, 1.0);
	vec4 worldPosition = ModelMatrix * MCPosition;
	mat4 modelViewMatrix = ViewMatrix * ModelMatrix;
	vec4 positionRelativeToCam = modelViewMatrix * MCPosition;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;

	mat4 normMat = transpose(inverse(ViewMatrix*ModelMatrix));
	exNormal = normalize((normMat * vec4(inNormal,1.0)).xyz);
	vec3 tang = normalize(normMat * inTangent).xyz;
	vec3 bitang = normalize((normMat * vec4(inBiTangent,1.0)).xyz);

	mat3 TBN = transpose(mat3(tang,bitang,exNormal));
	lightPos = (normMat * vec4(LightPosition,1.0)).xyz;
	viewPos = cameraPos;
	fragPos = (modelViewMatrix * MCPosition).xyz;
	lightPos = normalize(TBN * lightPos);
	viewPos = TBN * cameraPos;
	fragPos = TBN * fragPos;


	//directionToLight = TBN * ((ViewMatrix*vec4(LightPosition,0.0)).xyz - positionRelativeToCam.xyz);

	//directionToCamera = TBN * (-positionRelativeToCam.xyz);



	//
	//exNormal = normalize(mat3(normMat) * inNormal);
	//exTangent = normalize(normMat * inTangent);
	//exBiTangent = normalize(cross(exNormal, exTangent.xyz));

	

	//exPosition = (ViewMatrix* ModelMatrix*MCPosition).xyz;
	//WorldModelLight = vec3(ViewMatrix * vec4(LightPosition,1.0));

	//directionToLight = TBN*normalize(WorldModelLight - exPosition);

	//directionToCamera = TBN* (-exPosition);





	vec4 clipPosition = ModelMatrix * MCPosition;
	gl_ClipDistance[0] = dot(clipPosition, plane);
}