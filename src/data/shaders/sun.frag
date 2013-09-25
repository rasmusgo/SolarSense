uniform sampler2D sampler;
uniform float time;

varying vec2 vTexCoord;

float turbulence(float x, float y, int size)
{
    float value = 0.0, initialSize = float(size);

    for (int i = size; i > 1; i/=2)
    {
        value += fract(sin(dot(vec2(x,y) ,vec2(12.9898,78.233))) * 43758.5453);
    }

    return(128.0 * value / initialSize);
}

float light() {
    float xPeriod = 5.0; //defines repetition of marble lines in x direction
    float yPeriod = 10.0; //defines repetition of marble lines in y direction
        //turbPower = 0 ==> it becomes a normal sine pattern
    float turbPower = 10.0; //makes twists
    int turbSize = 32;
    //return fract(sin(dot(vTexCoord.xy ,vec2(12.9898*sin(time),78.233*cos(time)))) * 43758.5453);

    float xyValue = vTexCoord.x * xPeriod*sin(time/7.0) / 1.0 + vTexCoord.y * yPeriod*cos(time/15.0) / 1.0 + turbPower * turbulence(vTexCoord.x, vTexCoord.y, turbSize) / 256.0;
    float sineValue = abs(sin(xyValue * 3.14159));
    return min(sineValue + 0.6, 1.0);
}

void main() {
            gl_FragColor = vec4(texture2D(sampler,vTexCoord).xyz*light(), 1.0f);
}
