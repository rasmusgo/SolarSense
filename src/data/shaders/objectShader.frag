uniform sampler2D sampler;
varying vec2 vTexCoord;
varying float LightIntensity;

varying float Diffuse;
varying vec3 Specular;

void main() {
    vec3 color = (texture2D(sampler, vTexCoord).rgb) * max((Diffuse+ Specular), 0.1);

    gl_FragColor =  vec4(color, 1.0);
}

