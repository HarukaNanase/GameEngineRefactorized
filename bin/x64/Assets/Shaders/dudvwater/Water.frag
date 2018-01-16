#version 400 core

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;

out vec4 outColor;
uniform float moveFactor;

uniform sampler2D tex; // duduvMap
uniform sampler2D ReflectionTexture;
uniform sampler2D RefractionTexture;

const float waveStrenght = 0.01;
const float reflectiveness = 2;

vec2 clipSpaceToTexCoords(vec4 c){
	return (c.xy / c.w) / 2.0 + 0.5;
}

void main(void){
	
	vec2 texCoordsReal = clipSpaceToTexCoords(clipSpace);
	
	vec2 RefractionTexCoords = texCoordsReal;
	vec2 ReflectionTexCoords = vec2(texCoordsReal.x, -texCoordsReal.y);

	vec2 distortedCoords = texture(tex, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 0.1;
	distortedCoords = textureCoords + vec2(distortedCoords.x, distortedCoords.y + moveFactor);
	vec2 distortion = (texture(tex, distortedCoords).rg * 2.0 - 1.0) * waveStrenght;
	
	RefractionTexCoords += distortion;
 	RefractionTexCoords = clamp(RefractionTexCoords, 0.001, 0.999);

	ReflectionTexCoords += distortion;
 	ReflectionTexCoords.x = clamp(ReflectionTexCoords.x, 0.001, 0.999);
 	ReflectionTexCoords.y = clamp(ReflectionTexCoords.y, -0.999, -0.001);

	vec4 refractColor = texture(RefractionTexture, RefractionTexCoords);
	vec4 reflectColor = texture(ReflectionTexture, ReflectionTexCoords);

	vec3 view = normalize(toCameraVector);
	float frenelFactor = dot(view, vec3(0.0, 1.0, 0.0));
	frenelFactor = pow(frenelFactor, reflectiveness);
	vec4 finalColor = mix(reflectColor, refractColor, frenelFactor);
	outColor = mix(finalColor, vec4(0, 0.3, 0.5, 1.0), 0.4);

}