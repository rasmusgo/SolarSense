
#ifdef GL_ES
precision highp float;
#endif

varying vec3 color;
varying vec3 secondaryColor;

uniform vec3 v3LightPos;
uniform float g;
uniform float g2;

varying vec3 v3Direction;
// Calculates the Mie phase function
float getMiePhase(float fCos, float fCos2, float g, float g2)
{
   return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos2) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
}
 
// Calculates the Rayleigh phase function
float getRayleighPhase(float fCos2)
{
   return 0.75 * (2.0 + 0.5 * fCos2);
   //return 0.75 + 0.75*fCos2;
}
 
void main (void)
{
    float fCos = dot(v3LightPos, v3Direction) / length(v3Direction);   
	float fCos2 = fCos * fCos;

    float fRayleighPhase = getRayleighPhase(fCos2);
    float fMiePhase = getMiePhase(fCos, fCos2, g, g2);        
    //float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos*fCos) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);        
    // gl_FragColor.rgb = 1.0 - exp( -fExposure * (fRayleighPhase * color + fMiePhase * secondaryColor) );
    // gl_FragColor.a = 1.0; 
    gl_FragColor.rgb = fRayleighPhase * color + fMiePhase * secondaryColor;
    gl_FragColor.a = 0.5;
}
