varying vec2 vTexCoord;
varying vec3 vPos;
varying float vNormal;

uniform float minCoord;
uniform sampler2D texture;
uniform sampler2D alphaTexture;
uniform float orbRadius;
uniform float radius;
uniform vec3 planetCenter;

void main() {    
    vec2 dis = vTexCoord - vec2(0.5, 0.5);
    dis *= vec2(2);
    float distance = length(dis);
    vec2 texCoord = vec2(1.0 - (distance - minCoord) / (1.0 - minCoord), 0.5);

    if (distance < minCoord || distance > 1.0) discard;

    float lightIntensity = max(0.2, vNormal);

    if (length(vPos) > orbRadius) { // over the orbiting radius, could be shadowed
        vec3 sphericProj = normalize(vPos)*orbRadius;
        if (length(sphericProj - planetCenter) < radius) { // Behind the planet
            lightIntensity = 0.0;
        }
    }

    gl_FragColor = vec4(texture2D(texture, texCoord).rgb * lightIntensity, length(texture2D(alphaTexture, texCoord).rgb));
}
