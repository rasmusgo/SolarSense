varying vec2 vTexCoord;

uniform float width;
uniform float orbit;
uniform vec3 color;

void main() {
    vec2 dist = vTexCoord - vec2(0.5, 0.5);
    dist *= vec2(2);

    float d = length(dist);
    if (d > orbit+width || d < orbit-width) discard;

    float orbD = width - abs(d - orbit);
    gl_FragColor = vec4(color, ((orbD*orbD)/(width*width)));
}
