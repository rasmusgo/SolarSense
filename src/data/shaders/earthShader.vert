#version 120
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

uniform vec3 lightPos;
uniform vec3 camPos;

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;

attribute vec3 vTangent;

varying vec2 vTexCoord;
varying vec3 vNormal;
varying vec3 vCam;
varying vec3 vLight;

varying mat3 localSurface2World;
//uniform m_3x3_inv_transp;
mat3 m3( mat4 m )
{
mat3 result;

result[0][0] = m[0][0]; 
result[0][1] = m[0][1]; 
result[0][2] = m[0][2]; 


result[1][0] = m[1][0]; 
result[1][1] = m[1][1]; 
result[1][2] = m[1][2]; 

result[2][0] = m[2][0]; 
result[2][1] = m[2][1]; 
result[2][2] = m[2][2]; 

return result;
}
void main () {
    gl_Position = modelViewProjectionMatrix * vec4(a_position,1.0);
    vTexCoord = a_texCoord;

    vec4 mpos = modelMatrix * vec4(a_position,1.0);
    vec4 mnormal = modelMatrix * vec4(a_position + normalize(a_position),1.0);
    vec3 pos = mpos.xyz;
    vec3 norm = mnormal.xyz;
    vec3 camForw = -vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]);
    vec3 tvNormal = -normalize(norm - pos);
    vec3 tvCam = normalize(camForw);
    vec3 tvLight = normalize(pos - lightPos);
    //vNormal = vec3(0,0,1.0);

      // the signs and whether tangent is in localSurface2View[1] or
  // localSurface2View[0] depends on the tangent attribute, texture
  // coordinates, and the encoding of the normal map
  mat3 m_3x3_inv_transp = m3(modelMatrix); //transpose(inverse());
  localSurface2World[0] = normalize(vec3(modelMatrix * vec4(vTangent, 0.0)));
  localSurface2World[2] = normalize(gl_NormalMatrix * tvNormal);
  localSurface2World[1] = normalize(cross(localSurface2World[2], localSurface2World[0]));
 
  //varyingNormal = normalize(m_3x3_inv_transp * v_normal);
 
  //texCoords = v_texcoords;

    vNormal = tvNormal;
    vCam = tvCam;
    vLight = tvLight;

    /*vec3 n = normalize(gl_NormalMatrix * gl_Normal);
    vec3 t = normalize(gl_NormalMatrix * vTangent);
    vec3 b = cross(n, t);

     mat3 rotMatrix = mat3(
        t.x, t.y, t.z,  
        b.x, b.y, b.z, 
        n.x, n.y, n.z 
    ); 

    vCam = normalize(rotMatrix *  tvCam);
    vLight = normalize(rotMatrix *  tvLight);
    vNormal = vec3(0,0,1.0); //rotMatrix * tvNormal;
    if(dot(vLight, vLight)<0.1){
        vNormal = vec3(1,0,0);
    }*/
    /*vLight.x = dot(tvLight, t);
    vLight.y = dot(tvLight, b);
    vLight.z = dot(tvLight, n);

    vCam.x = dot(tvCam, t);
    vCam.y = dot(tvCam, b);
    vCam.z = dot(tvCam, n);*/
    
    //vec3 vVertex = gl_Position.xyz;//vec3(gl_ModelViewMatrix * gl_Vertex);
    //vec3 tmpVec = lightPos - vVertex; //gl_LightSource[0].position.xyz

   /* vLight.x = dot(tmpVec, t);
    vLight.y = dot(tmpVec, b);
    vLight.z = dot(tmpVec, n);

    tmpVec = -vVertex;
    vCam.x = dot(tmpVec, t);
    vCam.y = dot(tmpVec, b);
    vCam.z = dot(tmpVec, n);*/
}
