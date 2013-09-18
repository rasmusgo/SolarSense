uniform sampler2D sampler;
uniform sampler2D samplerNight;
uniform sampler2D samplerWater;

uniform float shininess;
uniform vec3 emission;
uniform vec3 specular;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

varying vec2 vTexCoord;

varying vec3 vNormal;
varying vec3 vCam;
varying vec3 vLight;

vec4 light(vec4 texColor, vec3 N, vec3 V, vec3 L)
{
        N=normalize(N); V=normalize(V); L=normalize(L);
        vec3 R = normalize( 2.0*dot(N,L)*N-L );
        float NdotL = max( 0.0, dot( N,L ) );
        float RdotV = max( 0.0, dot( R,V ) );
        float Idiff = NdotL;
        float Ispec = pow( RdotV, shininess );

        return
                vec4(emission,1.0) +
                texColor * vec4(lightAmbient,1.0) + //ambient
                texColor * vec4(lightDiffuse,1.0) * Idiff+ //difuse
                vec4(specular,1.0) * vec4(lightSpecular,1.0) * Ispec; //specular
}

void main() {
    float lightIntensity = max(0.05, min(dot(vLight, -vNormal) + 0.1, 1.0));

    vec4 lightColor = texture2D(samplerNight,vTexCoord);
    if (lightIntensity > 0.4) lightColor = vec4(0.0);
    else if (lightIntensity > 0.05) lightColor *= 1.0 - (lightIntensity-0.05)/(0.4-0.05);

    vec4 texColor = texture2D(sampler,vTexCoord);
    vec4 waterColor = texture2D(samplerWater,vTexCoord);

    vec4 color;
    if (waterColor.r < 0.5)
        color = light(texColor, vNormal, vCam, vLight)*lightIntensity+lightColor;
    else color = texColor*lightIntensity+lightColor;

    gl_FragColor = vec4(color.xyz, 1.0);
}
