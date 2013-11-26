uniform vec3 v3LightPos;
varying vec3 v3Direction;
varying vec4 c0;
varying vec4 c1;
uniform float g;
uniform float g2;

// Calculates the Mie phase function
float getMiePhase(float fCos, float fCos2, float g, float g2)
{
	return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos2) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
}

// Calculates the Rayleigh phase function
float getRayleighPhase(float fCos2)
{
	return 0.75 + 0.75*fCos2;
}
 
void main (void)
{
    float fCos = dot(v3LightPos, v3Direction) / length(v3Direction);   
	float fCos2 = fCos * fCos;

    float fRayleighPhase = getRayleighPhase(fCos2);
    float fMiePhase = getMiePhase(fCos, fCos2, g, g2);        

    gl_FragColor = fRayleighPhase * c0 + fMiePhase * c1;
    gl_FragColor.a = gl_FragColor.b;
}
