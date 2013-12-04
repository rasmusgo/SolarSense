uniform sampler2D sampler;
/*uniform sampler2D EarthCloudGloss;
uniform sampler2D EarthNight;
uniform sampler2D EarthSpecularMap;
*/
varying vec2 vTexCoord;
varying float LightIntensity; 

varying float Diffuse;
varying vec3 Specular;

// vec4 light(vec4 texColor, vec3 N, vec3 V, vec3 L)
// {
//         N=normalize(N); V=normalize(V); L=normalize(L);
//         vec3 R = normalize( 2.0*dot(N,L)*N-L );
//         float NdotL = max( 0.0, dot( N,L ) );
//         float RdotV = max( 0.0, dot( R,V ) );
//         float Idiff = NdotL;
//         float Ispec = pow( RdotV, shininess );

//         return
//                 vec4(emission,1.0) +
//                 texColor * vec4(lightAmbient,1.0) + //ambient
//                 texColor * vec4(lightDiffuse,1.0) * Idiff+ //difuse
//                 vec4(specular,1.0) * vec4(lightSpecular,1.0) * Ispec; //specular
// }

void main() {
     // Monochrome cloud cover value will be in clouds.r
     // Gloss value will be in clouds.g
     // clouds.b will be unused
    //vec2 clouds = vec3(texture2D(EarthCloudGloss,vTexCoord)).rg;
 //   vec3 daytime = (texture2D(sampler, vTexCoord).rgb) * Diffuse + 
   //                 Specular * clouds.g) * (1.0 - clouds.r) +
    //                clouds.r * Diffuse;
    //vec3 nighttime = texture2D(EarthNight, vTexCoord).rgb * 
    //                 (1.0 - clouds.r) * 2.0;

    //vec3 specular_map = texture2D(EarthSpecularMap, vTexCoord).rgb;
    //vec3 color = daytime;
    vec4 sample =  texture2D(sampler, vTexCoord);
    vec3 color = sample.rgb * max((Diffuse+ Specular), 0.0);

    //if (Diffuse < 0.1)
    //    color = mix(nighttime, daytime, (Diffuse + 0.15) * 5.0);

    gl_FragColor =  vec4(color, sample.a); //vec4(clamp(daytime, 0, 255), 1.0);
}

