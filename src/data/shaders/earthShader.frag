uniform sampler2D sampler;
uniform sampler2D samplerNight;
uniform sampler2D samplerWater;
uniform sampler2D samplerNormal;

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

/*mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

vec3 perturb_normal( vec3 N, vec3 V, vec2 texcoord )
{
    // assume N, the interpolated vertex normal and 
    // V, the view vector (vertex to eye)
   vec3 map = texture(tex1, texcoord ).xyz;
   map = map * 255./127. - 128./127.;
    mat3 TBN = cotangent_frame(N, -V, texcoord);
    return normalize(TBN * map);
}*/
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
    //vec3 detailNormal = normalize( vNormal + N );
    vec3 tanLight = vLight * tbn(vTexCoord, vNormal, vPos);

    float lightIntensity = max(0.1, min(dot(tanLight, detailNormal) + 0.1, 1.0));

    vec4 lightColor = texture2D(samplerNight,vTexCoord);
    if (lightIntensity > 0.4) lightColor = vec4(0.0);
    else if (lightIntensity > 0.05) lightColor *= 1.0 - (lightIntensity-0.05)/(0.4-0.05);

    vec4 texColor = texture2D(sampler,vTexCoord);
    vec4 waterColor = texture2D(samplerWater,vTexCoord);

    vec4 color;
    if (waterColor.r < 0.5)
        color = min(light(texColor, vNormal, vCam, vLight), 0.7)*lightIntensity+lightColor;
    else color = texColor*lightIntensity+lightColor;

    gl_FragColor = vec4(color.xyz, 1.0);
    /*if(lightIntensity == 0.1){
        gl_FragColor = vec4(detailNormal, 1.0);
    }*/
}
/*// compute derivations of the texture coordinate
vec2 tc_dx = dFdx(tc_i);
vec2 tc_dy = dFdy(tc_i);
// compute initial tangent and bi-tangent
vec3 t = normalize( tc_dy.y * p_dx - tc_dx.y * p_dy );
vec3 b = normalize( tc_dy.x * p_dx - tc_dx.x * p_dy ); // sign inversion
// get new tangent from a given mesh normal
vec3 n = normalize(n_obj_i);
vec3 x = cross(n, t);
t = cross(x, n);
t = normalize(t);
// get updated bi-tangent
x = cross(b, n);
b = cross(n, x);
b = normalize(b);
mat3 tbn = mat3(t, b, n);
*/