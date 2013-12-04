uniform sampler2D sampler;
uniform float globaltime;

varying vec2 vTexCoord;

float snoise(vec3 uv, float res)    // by trisomie21
{
    const vec3 s = vec3(1.0, 100, 10000);
    
    uv *= res;
    
    vec3 uv0 = floor(mod(uv, res))*s;
    vec3 uv1 = floor(mod(uv+vec3(1.), res))*s;
    
    vec3 f = fract(uv); f = f*f*(3.0-2.0*f);
    
    vec4 v = vec4(uv0.x+uv0.y+uv0.z, uv1.x+uv0.y+uv0.z,
                  uv0.x+uv1.y+uv0.z, uv1.x+uv1.y+uv0.z);
    
    vec4 r = fract(sin(v*0.001)*100000.0);
    float r0 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);
    
    r = fract(sin((v + uv1.z - uv0.z)*0.001)*100000.0);
    float r1 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);
    
    return mix(r0, r1, f.z)*2.-1.;
}


void main(void)
{
    //gl_FragColor = vec4(texture2D(sampler,vTexCoord).xyz, 1.0f);//*light(), 1.0f);


    float brightness    = 0.02;
    float radius        = 0.14 + brightness * 0.01;
    float invRadius     = 1.0/radius;
    
    vec3 orange         = 0.55*vec3( 0.8, 0.65, 0.3 );
    vec3 orangeRed      = 1.75*vec3( 0.8, 0.35, 0.1 );
    float time      = globaltime*0.05;
    float aspect    = 1.0;
    vec2 uv         = vTexCoord.xy;
    vec2 p          = -0.5 + uv;
    p.x *= aspect;

    float fade      = 1.25*pow( length( 2.0 * p ), 0.65 );
    float fVal1     = 1.0 - fade;
    float fVal2     = 1.0 - fade;
    
    float angle     = atan( p.x, p.y )/6.2832;
    float dist      = length(p)/1.0f;
    vec3 coord      = vec3( angle, dist, time * 0.1 );
    
    float newTime1  = abs( snoise( coord + vec3( 0.0, -time * ( 0.35 + brightness * 0.001 ), time * 0.015 ), 15.0 ) );
    float newTime2  = abs( snoise( coord + vec3( 0.0, -time * ( 0.15 + brightness * 0.001 ), time * 0.015 ), 45.0 ) );  
    for( int i=1; i<=3; i++ ){
        float power = pow( 2.0, float(i + 1) );
        fVal1 += ( 0.5 / power ) * snoise( coord + vec3( 0.0, -time, time * 0.2 ), ( power * ( 10.0 ) * ( newTime1 + 1.0 ) ) );
        fVal2 += ( 0.5 / power ) * snoise( coord + vec3( 0.0, -time, time * 0.2 ), ( power * ( 25.0 ) * ( newTime2 + 1.0 ) ) );
    }
    
    float corona        = pow( fVal1 * max( 1.1 - fade, 0.0 ), 2.0 ) * 150.0;
    corona              += pow( fVal2 * max( 1.1 - fade, 0.0 ), 2.0 ) * 150.0;
    corona              *= 1.5 - newTime1;
    vec3 starSphere     = vec3( 0.0 );
    
    vec2 sp = -1.0 + 2.0 * uv;
    sp.x *= aspect;
    sp *= ( 2.0 - brightness );
    float r = dot(sp,sp);
    float f = (1.0-sqrt(abs(1.0-r)))/(r) + brightness * 0.5;
    if( dist < radius ){
        corona          *= pow( dist * invRadius * 2.0, 24.0 );
        vec2 newUv;
        newUv.x = sp.x*f;
        newUv.y = sp.y*f;
        newUv += vec2( time, 0.0 );
        
        vec3 texSample  = texture2D( sampler, newUv ).rgb;
        float uOff      = ( texSample.g * brightness * 4.5 + time );
        vec2 starUV     = newUv + vec2( uOff, 0.0 );
        starSphere      = texture2D( sampler, starUV ).rgb;
    }
    
    float starGlow  = min( max( 1.0 - dist*1.9 , 0.0 ), 1.0 );
    gl_FragColor.rgb    = vec3( f * ( 0.75 + brightness * 0.3 ) * orange ) + starSphere*0.001  + corona * orange + starGlow * orangeRed;
    gl_FragColor.a      = gl_FragColor.r+gl_FragColor.g+gl_FragColor.b;
}

