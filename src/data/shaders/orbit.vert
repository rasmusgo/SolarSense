uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform vec3 planetPos;

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;

varying vec2 vTexCoord;
varying vec3 vPos;

void main () {
    vPos = vec3(modelMatrix * vec4(a_position,1.0));
    gl_Position = modelViewProjectionMatrix * vec4(a_position,1.0);
    vTexCoord = a_texCoord;
}
