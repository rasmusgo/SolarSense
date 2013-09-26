uniform sampler2D sampler;
uniform float time;
uniform float lastTime;

varying vec2 vTexCoord;

void main() {
    vec4 color = texture2D(sampler,vTexCoord);
    if (color.a < 0.1) discard;

    float alpha = 1.0;
    float handTime = time-lastTime;
    if (handTime > 2.5) {
        alpha = max(0.25, 1.0 -(handTime-2.5)/3.0);
    }

    gl_FragColor = vec4(color.xyz, alpha);
}
