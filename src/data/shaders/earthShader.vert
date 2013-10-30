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

varying vec2 vTexCoord;
varying vec3 vNormal;
varying vec3 vCam;
varying vec3 vLight;
varying vec3 vPos;

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
