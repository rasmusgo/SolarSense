#version 130
uniform sampler2D EarthDay;
uniform sampler2D EarthNight;

varying vec2 vTexCoord0;
varying vec2 vTexCoord1;
varying float LightIntensity; 

void main() {
		vec3 lightColor = vec3(texture2D(EarthDay,vTexCoord0));
		//vec3 lightColor2 = vec3(texture2D(EarthNight,vTexCoord0.st));

		gl_FragColor = vec4(lightColor * LightIntensity, 1.0); 
}

