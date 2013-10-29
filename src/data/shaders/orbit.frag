varying vec2 vTexCoord;
varying vec3 vPos;

uniform float width;
uniform float orbit;
uniform vec3 color;
uniform vec3 planetPos;
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
    if (cross(normalize(planetPos), normalize(vPos)).y < 0.0) discard;
    float dist = length(vPos - planetPos);
    if (dist > radius) discard;

    vec2 delta = dFdx(vTexCoord) + dFdy(vTexCoord);

    float aux = 0.0;
    aux += intensity(vTexCoord)*2.0;
    aux += intensity(vTexCoord + vec2(delta.x, 0.0));
    aux += intensity(vTexCoord + vec2(-delta.x, 0.0));
    aux += intensity(vTexCoord + vec2(0.0, delta.y));
    aux += intensity(vTexCoord + vec2(0.0, -delta.y));
    aux /= 6;

    if (aux < 0.01) discard;

    aux *= 1.0 - (dist - radius/2.0)/(radius/2.0);

    gl_FragColor = vec4(color, aux);
}
