attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;

uniform float ratio;
uniform float time;

varying vec2 vTexCoord;

void main () {
    float scale = 10.0;
    vec2 aux = a_texCoord;
    aux.x = aux.x / ratio;
    vec2 coord = aux*2.0 - vec2(1.0);
    coord.y = -1.0*coord.y;

    coord /= scale;
    coord += 1.0/scale/2.0;// * 9.0;
    coord += 8.0/scale/2.0;
    coord.x += sin(time*12.0)*1.5/scale/ratio;

    gl_Position = vec4(vec3(coord,0.0),1.0);
    vTexCoord = a_texCoord;
}
