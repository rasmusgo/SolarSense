varying vec2 vTexCoord;
varying vec3 vPos;

uniform float width;
uniform float orbit;
uniform vec3 color;
uniform vec3 planetPos;
uniform vec3 parentPos;
uniform float fadeFactor;
uniform float radius;

float intensity (vec2 tex) {
    vec2 dis = tex - vec2(0.5, 0.5);
    dis *= vec2(2);

    float d = length(dis);
    if (d > orbit+width || d < orbit-width) return 0.0;

    float orbD = width - abs(d - orbit);
    return (orbD*orbD)/(width*width);
}

void main() {
    if (cross(normalize(planetPos - parentPos), normalize(vPos - parentPos)).y < 0.0) discard;
    float dist = length(vPos - planetPos);
    if (dist > radius) discard;

    vec2 deltaX = dFdx(vTexCoord);
    vec2 deltaY = dFdy(vTexCoord);
    float aux = 0.0;
    aux += intensity(vTexCoord);
    aux += intensity(vTexCoord + deltaX);
    aux += intensity(vTexCoord - deltaX);
    aux += intensity(vTexCoord + deltaY);
    aux += intensity(vTexCoord - deltaY);
    aux /= 5.0;
    /*aux += intensity(vTexCoord + deltaX + deltaY);
    aux += intensity(vTexCoord - deltaX + deltaY);
    aux += intensity(vTexCoord + deltaX - deltaY);
    aux += intensity(vTexCoord - deltaX - deltaY);
    aux /= 9.0;*/
    aux *= aux;

    aux *= 1.0 - (dist - radius/2.0)/(radius/2.0);
    aux *= fadeFactor;


    if (aux < 0.01) discard;

    gl_FragColor = vec4(color, aux);
}
