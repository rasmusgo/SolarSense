attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;

varying vec2 vTexCoord;

void main () {
    gl_Position = vec4(vec3(a_texCoord,0.0),1.0);
    vTexCoord = a_texCoord;
}
