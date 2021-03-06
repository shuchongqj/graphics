#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec4 frontColor;
out vec2 vtexCoord;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;
uniform vec4 lightPosition; // Eye space

uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;
uniform float matShininess;

vec4 calculate_phong() {
  vec3 P = (modelViewMatrix * vec4(vertex, 1)).xyz;
	vec3 V = normalize(-P);
  vec3 N = normalize(normalMatrix * normal);
	vec3 L = normalize(lightPosition.xyz - P);
	float NL = max(0.0, dot(N,L));
	vec3 R = normalize(2*NL*N-L);
	float RV = max(0.0, dot(R,V));

	vec4 KaIa = matAmbient * lightAmbient;
	vec4 KdId = matDiffuse * lightDiffuse;
	vec4 KsIs = matSpecular * lightSpecular;
	
	return KaIa + KdId*NL + KsIs*pow(RV,matShininess);
}

void main()
{
    frontColor = calculate_phong();
    vtexCoord = texCoord;
    gl_Position = modelViewProjectionMatrix * vec4(vertex.xyz, 1.0);
}
