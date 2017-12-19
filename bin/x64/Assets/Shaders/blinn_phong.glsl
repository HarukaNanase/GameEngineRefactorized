#version 330 core

uniform vec3 LightPosition;
uniform mat4 ViewMatrix;
uniform mat3 NormalMatrix;

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec3 N;
in vec3 Eye;
out vec4 FragmentColor;

const vec3 ambient_color = vec3(0.0, 0.0, 0.1);
const vec3 diffuse_color = vec3(0.0, 0.0, 0.5);
const vec3 specular_color = vec3(1.0, 1.0, 1.0);
const float shininess = 64.0;

	/*******
	/* Calc: diffuse component
	/* Requires: LightPosition, Normal, Fragment Position
	/* Outputs: diffuse factor
	/*******/
float getDiffuseComponent(vec3 LightPosition, vec3 Normal, vec3 exPosition){
	vec3 Ldir = normalize(LightPosition - exPosition);
	vec3 N = normalize(Normal);
	return max(dot(N, Ldir), 0);
}

/*******
	/* Calc: specular component
	/* Requires: LightPosition, Normal, Fragment Position, Camera Eye Position, Shininess Factor
	/* Outputs: diffuse factor
	/*******/
float getSpecularComponent(vec3 EyePosition, vec3 exPosition, vec3 LightPosition, vec3 Normal, float shininess){
	vec3 Ldir = normalize(LightPosition - exPosition);
	vec3 Eye = normalize(EyePosition - exPosition);
	vec3 H = normalize(Ldir + Eye);
	vec3 N = normalize(Normal);
	return pow(max(dot(H,N),0),shininess);
}


void main(){

	float diffuse_component, specular_component;
	diffuse_component = getDiffuseComponent(LightPosition, N, exPosition);
	vec3 Ldir = normalize(LightPosition - exPosition);
	vec3 EyePos = normalize(Eye - exPosition);
	vec3 H = normalize(Ldir + EyePos);
	vec3 N = normalize(exNormal);
	specular_component = pow(max(dot(H,N),0),shininess);
	if(specular_component == 0.0){
		FragmentColor = vec4(0.5f,0,0,1.0);
	}else{
		FragmentColor = vec4(vec3(ambient_color + diffuse_component*diffuse_color + specular_component * specular_color), 1.0);
	}
}