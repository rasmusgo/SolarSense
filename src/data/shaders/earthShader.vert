uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

uniform vec3 lightPos;
uniform vec3 camPos;

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;

varying vec2 vTexCoord;
varying vec3 vNormal;
varying vec3 vCam;
varying vec3 vLight;

void main () {
    gl_Position = modelViewProjectionMatrix * vec4(a_position,1.0);
    vTexCoord = a_texCoord;

    vec4 mpos = modelMatrix * vec4(a_position,1.0);
    vec4 mnormal = modelMatrix * vec4(a_position + normalize(a_position),1.0);
    vec3 pos = mpos.xyz;
    vec3 norm = mnormal.xyz;
    vec3 camForw = -vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]);
    vNormal = normalize(norm - pos);
    vCam = normalize(camForw);
    vLight = normalize(pos - lightPos);
}
