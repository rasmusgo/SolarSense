uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat4 NormalMatrix;

uniform vec3 LightPosition; 

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;

varying float Diffuse;
varying vec3 Specular;
varying vec2 TexCoord;

// const float specularContribution = 0.1; 
// const float diffuseContribution = 1.0 - specularContribution; 

varying vec2 vTexCoord;

void main() 
{ 
 vec3 ecPosition = vec3(MVMatrix * vec4(a_position,1.0)); 
 //vec3 tnorm = normalize(vec3(NormalMatrix * vec4(a_normal, 0.0)));
 vec3 tnorm = normalize(vec3(NormalMatrix * vec4(normalize(a_position), 0.0)));
 vec3 lightVec = normalize(LightPosition - ecPosition); 
 vec3 reflectVec = reflect(lightVec, tnorm);
 vec3 viewVec = normalize(-ecPosition); 

 float spec = clamp(dot(reflectVec, viewVec), 0.0, 1.0); 
 spec = pow(spec, 8.0); 

 Specular = spec * vec3(1.0, 0.941, 0.898) * 0.3;
 Diffuse = max(dot(lightVec, tnorm), 0.0);

 // LightIntensity = diffuseContribution 
 //                 * max(dot(lightVec, tnorm), 0.0)
 //                 + specularContribution * spec; 

 vTexCoord = a_texCoord;
 gl_Position = MVPMatrix * vec4(a_position,1.0);
}
