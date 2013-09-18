#version 130
uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat4 NormalMatrix;

uniform vec3 LightPosition; 
varying float LightIntensity; 

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;
attribute vec2 a_texCoord_test;

const float specularContribution = 0.1; 
const float diffuseContribution = 1.0 - specularContribution; 

varying vec2 vTexCoord0;
varying vec2 vTexCoord1;

void main() 
{ 
 vec3 ecPosition = vec3(MVMatrix * vec4(a_position,1.0)); 
 vec3 tnorm = normalize(vec3(NormalMatrix * vec4(a_normal, 0.0))); 
 vec3 lightVec = normalize(LightPosition - ecPosition); 
 vec3 reflectVec = reflect(-lightVec, tnorm); 
 vec3 viewVec = normalize(-ecPosition); 

 float spec = clamp(dot(reflectVec, viewVec), 0.0, 1.0); 
 spec = pow(spec, 16.0); 

 LightIntensity = diffuseContribution 
 				* max(dot(lightVec, tnorm), 0.0)
 				+ specularContribution * spec; 

 vTexCoord0 = a_texCoord;
 vTexCoord1 = a_texCoord_test;

 gl_Position = MVPMatrix * vec4(a_position,1.0);
}