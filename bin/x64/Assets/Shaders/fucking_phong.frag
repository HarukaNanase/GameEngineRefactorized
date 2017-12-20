#version 330 core

in vec4 L;
in vec4 V;
in vec4 N;
in vec4 E;

out vec4 FragmentColor;


const vec3 ambient_color = vec3(0,0.2,0);
const vec3 diffuse_color = vec3(0,0.6,0);
const vec3 specular_color = vec3(1,1,1);
const float shininess = 128.0;



void main(){
	vec4 f_L = normalize(L);
	vec4 f_E = normalize(E);
	vec4 f_N = normalize(N);
	float diffuse = max(dot(f_L,f_N), 0);
	float specular = 0.0;
	if(diffuse > 0.0){
		vec4 halfWay = normalize(f_L + f_E);
		specular = pow(max(dot(halfWay,f_N),0.0),shininess);
	}
	FragmentColor = vec4(ambient_color + diffuse * diffuse_color + specular * specular_color, 1.0);

}