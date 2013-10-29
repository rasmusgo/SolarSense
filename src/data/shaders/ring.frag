varying vec2 vTexCoord;
varying float vNormal;

uniform float minCoord;
uniform sampler2D texture;
uniform sampler2D alphaTexture;

void main() {    
    vec2 dis = vTexCoord - vec2(0.5, 0.5);
    dis *= vec2(2);
    float distance = length(dis);
    vec2 texCoord = vec2(1.0 - (distance - minCoord) / (1.0 - minCoord), 0.5);

    if (distance < minCoord || distance > 1.0) discard;

    gl_FragColor = vec4(texture2D(texture, texCoord).rgb * max(0.2, vNormal), length(texture2D(alphaTexture, texCoord).rgb));
}
