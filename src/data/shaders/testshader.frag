uniform sampler2D sampler;

varying vec2 vTexCoord;
uniform vec3 eyePosition;
uniform vec3 sunPosition;

// @author Michael de Hoog

void main(void)
{
	vec4 tile = texture2D(sampler, vTexCoord);

	vec4 specularColor = vec4(1.0, 1.0, 0.6, 1.0);
	vec3 norm = normalize(a_normal);
	
	float nxDir = max(0.0, dot(norm, sunPosition));
	nxDir = min(1.0, nxDir * 2.0);
	
	vec4 specular = vec4(0.0);
	
	if(nxDir != 0.0)
	{
		vec3 cameraVector = normalize(eyePosition - a_position.xyz);
		vec3 halfVector = normalize(sunPosition + cameraVector);
		float nxHalf = max(0.0, dot(norm, halfVector));
		
		float shininess = 80.0;
		float specularPower = pow(nxHalf, shininess);
		specular = specularColor * specularPower;
		
		float amount = tile.b;
		//amount = clamp(amount * 5.0 - 2.0, 0.0, 1.0);
		specular *= vec4(amount);
	}
	
	float red = tile.r;
	gl_FragColor = mix(vec4(red, red * 0.9, red * 0.8, 0.8), vec4(0.0), nxDir) + specular;
}


uniform sampler2D sampler;

varying vec2 vTexCoord;

void main() {
        gl_FragColor = texture2D(sampler,vTexCoord);
}
