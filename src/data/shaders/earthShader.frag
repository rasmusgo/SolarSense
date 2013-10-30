uniform sampler2D sampler;
uniform sampler2D samplerNight;
uniform sampler2D samplerWater;
uniform sampler2D samplerNormal;
uniform sampler2D samplerWaterTex;

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
varying mat3 localSurface2World;
varying vec3 vPos;

vec4 light(vec4 texColor, vec3 N, vec3 V, vec3 L, vec4 uSpecular)
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
                texColor * vec4(lightDiffuse,1.0) * Idiff + //difuse
                vec4(specular,1.0) * uSpecular * Ispec; //specular
}

mat3 tbn(vec2 texCoord, vec3 meshNormal, vec3 pos){
// compute derivations of the texture coordinate
vec2 tc_dx = dFdx(texCoord);
vec2 tc_dy = dFdy(texCoord);
vec3 p_dx  = dFdx(pos);
vec3 p_dy  = dFdy(pos);
// compute initial tangent and bi-tangent
vec3 t = normalize( tc_dy.y * p_dx - tc_dx.y * p_dy );
vec3 b = normalize( tc_dy.x * p_dx - tc_dx.x * p_dy ); // sign inversion
// get new tangent from a given mesh normal
vec3 n = normalize(meshNormal);
vec3 x = cross(n, t);
t = cross(x, n);
t = normalize(t);
// get updated bi-tangent
x = cross(b, n);
b = cross(n, x);
b = normalize(b);
return mat3(t, b, n);
}

void main() {
    vec3 detailNormal = normalize(texture2D(samplerNormal, vTexCoord).xyz * 2.0 - 1.0);
    mat3 TBN = tbn(vTexCoord, vNormal, vPos);
    vec3 tanLight = vLight * TBN;

    float lightIntensity = max(0.1, min(dot(tanLight, detailNormal) + 0.1, 1.0));

    vec4 lightColor = texture2D(samplerNight,vTexCoord);
    if (lightIntensity > 0.4) lightColor = vec4(0.0);
    else if (lightIntensity > 0.05) {
        lightColor *= 1.0 - (lightIntensity-0.05)/(0.4-0.05);
        lightColor *= max(0.1, min(dot(-tanLight, detailNormal) + 0.1, 1.0));
    }


    vec4 texColor = texture2D(sampler,vTexCoord);
    vec4 waterColor = texture2D(samplerWater,vTexCoord);

    vec4 color;
   // float levelW = max(dot(waterColor, waterColor), 0.1);
    vec4 specularC = waterColor;
    if(specularC.r <= 0.4) specularC = vec4(0.4, 0.4, 0.4, 1.0);
    if(specularC.r > 0.5){
        float frac = specularC.r;
        vec4 blueWater = texture2D(samplerWaterTex, vTexCoord);
        texColor = (1.0-frac)*texColor + frac * blueWater;
    }
    //if (waterColor.r < 0.5)
     //   color = min(light(texColor, vNormal, vCam, vLight, specular), 0.7)*lightIntensity+lightColor;
    //else color = texColor*lightIntensity+lightColor;
    color = light(texColor, detailNormal, vCam*TBN, tanLight, specularC )*lightIntensity+lightColor;
    //if (waterColor.r < 0.9)
     //   color = min(light(texColor, vNormal, vCam, vLight, lightSpecular * levelW ), 0.5)*lightIntensity+lightColor;
    //else color = texColor*lightIntensity+lightColor;


    gl_FragColor = vec4(color.xyz, 1.0);
}
