varying vec2 vTexCoord;

uniform float width;
uniform float orbit;
uniform vec3 color;

float intensity (vec2 tex) {
    vec2 dist = tex - vec2(0.5, 0.5);
    dist *= vec2(2);

    float d = length(dist);
    if (d > orbit+width || d < orbit-width) return 0.0;

    float orbD = width - abs(d - orbit);
    return (orbD*orbD)/(width*width);
}

void main() {
    vec2 delta = dFdx(vTexCoord) + dFdy(vTexCoord);

    float aux = 0.0;
    for (int i = -1; i <= 1; ++i)
        for (int j = -1; j <= 1; ++j)
            aux += intensity(vTexCoord + vec2(float(i)*delta.x, float(j)*delta.y)) / 9.0;

    gl_FragColor = vec4(color, aux);
}
