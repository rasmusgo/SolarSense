#version 120
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalMatrix;

uniform vec3 lightPos;
uniform vec3 camPos;

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;

attribute vec3 vTangent;

varying vec2 vTexCoord;
varying vec3 vNormal;
varying vec3 vCam;
varying vec3 vLight;
varying vec3 vPos;

mat3 m3( mat4 m )
{
mat3 result;

result[0][0] = m[0][0]; 
result[0][1] = m[0][1]; 
result[0][2] = m[0][2]; 


result[1][0] = m[1][0]; 
result[1][1] = m[1][1]; 
result[1][2] = m[1][2]; 

result[2][0] = m[2][0]; 
result[2][1] = m[2][1]; 
result[2][2] = m[2][2]; 

return result;
}
void main () {
    gl_Position = modelViewProjectionMatrix * vec4(a_position,1.0);
    vTexCoord = a_texCoord;


    vec4 mpos = modelMatrix * vec4(a_position,1.0);

    //vec4 mnormal = modelMatrix * vec4(a_position + normalize(a_position),1.0);
    vec3 pos = mpos.xyz;
    vPos = pos;
    //vec3 norm = mnormal.xyz;
    vec3 camForw = -vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]);
    vec3 tvNormal = -normalize(vec3(normalMatrix  * vec4(a_position, 0)) ); //-normalize(norm - pos);
    vec3 tvCam = normalize(camForw);
    vec3 tvLight = normalize(pos - lightPos);
    //vNormal = vec3(0,0,1.0);

 

    vNormal = tvNormal;
    vCam = tvCam;
    vLight = tvLight;

  
}
