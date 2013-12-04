#version 120

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalMatrix;

uniform vec3 lightPos;
uniform vec3 camPos;

uniform vec3 v3CameraPos;                // The direction vector to the light source 
uniform vec3 v3LightPos;                // The direction vector to the light source 
uniform vec3 v3InvWavelength;        // 1 / pow(wavelength, 4) for the red, green, and blue channels 
uniform float fCameraHeight2;        // fCameraHeight^2 
uniform float fOuterRadius;                // The outer (atmosphere) radius 
uniform float fOuterRadius2;        // fOuterRadius^2 
uniform float fInnerRadius;                // The inner (planetary) radius 
uniform float fInnerRadius2;        // fInnerRadius^2 
uniform float fKrESun;                        // Kr * ESun 
uniform float fKmESun;                        // Km * ESun 
uniform float fKr4PI;                        // Kr * 4 * PI 
uniform float fKm4PI;                        // Km * 4 * PI 
uniform float fScale;                        // 1 / (fOuterRadius - fInnerRadius) 
uniform float fScaleDepth;                // The scale depth (i.e. the altitude at which the atmosphere's average density is found) 
uniform float fScaleOverScaleDepth;        // fScale / fScaleDepth 
const int iSamples = 8; 
const float fInvSamples = 0.125; 


attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;


varying vec3 c0;
varying vec3 c1;

varying vec2 vTexCoord;
varying vec3 vNormal;
varying vec3 vCam;
varying vec3 vLight;
varying vec3 vPos;




float scale(float fCos)        
{        
        float x = 1.0 - fCos;        
        return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));        
}    

void main () {
     
    // Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)        
    vec3 v3Pos = (vec4(a_position,1.0) ).xyz;        
    vec3 v3Ray = v3Pos-v3CameraPos;        
    float fFar = length(v3Ray);        
    v3Ray /= fFar;        
    
    // Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)        
    float B = -2.0 * dot(v3CameraPos, v3Ray);        
    float C = fCameraHeight2 - fOuterRadius2;        
    float fDet = max(0.0, B*B - 4.0 * C);        
    float fNear = 0.5 * (-B - sqrt(fDet));        
    
    // Calculate the ray's starting position, then calculate its scattering offset        
    vec3 v3Start = v3CameraPos + v3Ray * fNear;        
    fFar -= fNear;        
    float fDepth = exp((fInnerRadius - fOuterRadius) / fScaleDepth);        
    float fCameraAngle = dot(-v3Ray, v3Pos) / length(v3Pos);        
    float fLightAngle = dot(v3LightPos, v3Pos) / length(v3Pos);        
    float fCameraScale = scale(fCameraAngle);        
    float fLightScale = scale(fLightAngle);        
    float fCameraOffset = fDepth*fCameraScale;        
    float fTemp = (fLightScale + fCameraScale);        
    
    // Initialize the scattering loop variables        
    float fSampleLength = fFar * fInvSamples;        
    float fScaledLength = fSampleLength * fScale;        
    vec3 v3SampleRay = v3Ray * fSampleLength;        
    vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;        
    
    // Now loop through the sample rays        
    vec3 v3FrontColor = vec3(0.0, 0.0, 0.0);        
    vec3 v3Attenuate;        
    for(int i=0; i<iSamples; i++)        
    {        
            float fHeight = length(v3SamplePoint);        
            float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));        
            float fScatter = fDepth*fTemp - fCameraOffset;        
            v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));        
            v3FrontColor += v3Attenuate * (fDepth * fScaledLength);        
            v3SamplePoint += v3SampleRay;        
    }        
    
    c0 = v3FrontColor * (v3InvWavelength * fKrESun + fKmESun);        
    
    // Calculate the attenuation factor for the ground        
    c1 = v3Attenuate; 

    vec4 mpos = modelMatrix * vec4(a_position,1.0);

    //vec4 mnormal = modelMatrix * vec4(a_position + normalize(a_position),1.0);
    vec3 pos = mpos.xyz;
    vPos = pos;
    //vec3 norm = mnormal.xyz;
    vec3 camForw = -vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]);
    vec3 tvNormal = -normalize(vec3(normalMatrix  * vec4(a_position, 0)) ); //-normalize(norm - pos);
    vec3 tvCam = normalize(camForw);
    vec3 tvLight = normalize(pos - lightPos);
    //vNormal = vec3(0,0,1.0);


    vNormal = tvNormal;
    vCam = tvCam;
    vLight = tvLight;

    gl_Position = modelViewProjectionMatrix * vec4(a_position,1.0);
    vTexCoord = a_texCoord;

  
}
