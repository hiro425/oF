precision mediump float;

uniform float time;
varying vec2 surfacePosition;

void main(void) {
    float intensity = 2.2; // Bright effect
    vec2 offset = vec2(0 , 0); // x / y offset
    vec3 light_color = vec3(1, 0.8, 0.6); // RGB, proportional values, higher increases intensity
    float master_scale = 0.2 + 0.02*sin(time); // Change the size of the effect
    float c = pow(master_scale/(length(surfacePosition+offset)), -0.6*sin(time) + intensity);
    
    
    gl_FragColor = vec4(vec3(c) * light_color, 65.0);

