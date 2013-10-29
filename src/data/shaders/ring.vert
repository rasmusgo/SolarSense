uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;

varying vec2 vTexCoord;
varying float vNormal;

void main () {
    gl_Position = modelViewProjectionMatrix * vec4(a_position,1.0);
    vTexCoord = a_texCoord;
    vec4 pos = (modelMatrix * vec4(a_position,1.0));
    float n1 = dot( normalize(vec3((modelMatrix * vec4(a_position + a_normal, 1.0)) - pos)), normalize(vec3(pos)) );
    float n2 = dot( normalize(vec3((modelMatrix * vec4(a_position - a_normal, 1.0)) - pos)), normalize(vec3(pos)) );
    vNormal = min(1.0, max(n1,n2));
}
