uniform sampler2D sampler;

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

vec4 light(vec3 N, vec3 V, vec3 L)
{
        N=normalize(N); V=normalize(V); L=normalize(L);
        vec3 R = normalize( 2.0*dot(N,L)*N-L );
        float NdotL = max( 0.0, dot( N,L ) );
        float RdotV = max( 0.0, dot( R,V ) );
        float Idiff = NdotL;
        float Ispec = pow( RdotV, shininess );

        vec4 texColor = texture2D(sampler,vTexCoord);

        return
                vec4(emission,1.0) +
                texColor * vec4(lightAmbient,1.0) + //ambient
                texColor * vec4(lightDiffuse,1.0) * Idiff+ //difuse
                vec4(specular,1.0) * vec4(lightSpecular,1.0) * Ispec; //specular
}

void main() {
    vec4 color = light(vNormal, vCam, vLight)*max(0.05, min(dot(vLight, -vNormal) + 0.1, 1.0));
    //vec4 color = texture2D(sampler,vTexCoord)*max(0.05, min(dot(vLight, -vNormal) + 0.1, 1.0));
    gl_FragColor = vec4(color.xyz, 1.0);
}
