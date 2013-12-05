uniform sampler2D sampler;
uniform sampler2D samplerNight;
uniform sampler2D samplerWater;
uniform sampler2D samplerNormal;
uniform sampler2D samplerWaterTex;
uniform sampler2D samplerCloud;

uniform float shininess;
uniform vec3 emission;
uniform vec3 specular;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
uniform float globaltime;


varying vec3 c0;
varying vec3 c1;

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

mat3 tbn(void){
// compute derivations of the texture coordinate
vec2 tc_dx = dFdx(vTexCoord);
vec2 tc_dy = dFdy(vTexCoord);
vec3 p_dx  = dFdx(vPos);
vec3 p_dy  = dFdy(vPos);
// compute initial tangent and bi-tangent
vec3 t = normalize( tc_dy.y * p_dx - tc_dx.y * p_dy );
vec3 b = normalize( tc_dy.x * p_dx - tc_dx.x * p_dy ); // sign inversion
// get new tangent from a given mesh normal
vec3 n = normalize(vNormal);
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
    mat3 TBN = tbn();
    vec3 tanLight = vLight * TBN;

    float lightIntensity = max(0.1, min(dot(vLight, vNormal) + 0.1, 1.0));

    vec4 texColor = texture2D(sampler,vTexCoord);

    vec4 waterColor = texture2D(samplerWater,vTexCoord);


    vec4 lightColor = 2.f*texture2D(samplerNight,vTexCoord);
    vec2 tc_dx = dFdx(vTexCoord);
    vec2 tc_dy = dFdy(vTexCoord);
    lightColor += texture2D(samplerNight,vTexCoord+ tc_dx);
    lightColor += texture2D(samplerNight,vTexCoord- tc_dx);
    lightColor += texture2D(samplerNight,vTexCoord+ tc_dy);
    lightColor += texture2D(samplerNight,vTexCoord- tc_dy);
    lightColor /= 6.0f;

    lightColor = pow(lightColor, vec4(5.f));
    lightColor *= 5.f;
    //lightColor *= pow(max(0.f, min(1.0 - lightIntensity, 1.0f)), 5.f);
    if (lightIntensity > 0.4) lightColor *=  vec4(0.0);
    else if (lightIntensity > 0.05) {
        lightColor *= 1.0 - (lightIntensity-0.05)/(0.4-0.05);
        lightColor *= max(0.1, min(dot(-tanLight, detailNormal) + 0.1, 1.0));
    }

    vec4 color;
   // float levelW = max(dot(waterColor, waterColor), 0.1);
    vec4 specularC = vec4(1.0) - waterColor;
   
    //TO use another texture for water highlights
    if(specularC.r <= 0.3) specularC = vec4(0.4, 0.4, 0.4, 1.0);
    if(specularC.r > 0.3){
        float frac = min(specularC.r, 0.7);
        vec4 blueWater = texture2D(samplerWaterTex, vTexCoord);
        texColor = (1.0-frac)*texColor + frac * blueWater;
    }
    specularC *= texColor;

    float time      = globaltime*0.001;
    vec2 uv         = vTexCoord.xy;

    uv += vec2( 0, 0 );
    
    // vec3 texSample  = texture2D( samplerCloud, newUv ).rgb;
    // // float uOff      = ( texSample.r *  time );
    // // vec2 warpedUV     = newUv + vec2( uOff, 0.0 );
    vec4 cloudColor = texture2D(samplerCloud,uv)*max(0.0,dot(vLight, vNormal));

    color = light(texColor, detailNormal, vCam*TBN, tanLight, specularC )+lightColor; //*lightIntensity

    gl_FragColor = vec4(c0*0.75+mix(mix(cloudColor.xyz,color.xyz,0.5), vec3(1.0)-vec3(c1), min(dot(-vLight, vNormal), 0.0)), 1.0);
}
