uniform float bias; //bias value to reduce shadow-acne

void main(void)
{
	gl_FragColor.x = gl_FragColor.z + bias;
}