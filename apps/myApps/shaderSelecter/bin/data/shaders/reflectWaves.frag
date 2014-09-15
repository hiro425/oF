//precision mediump float;

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
varying vec2 surfacePosition;

#define MAX_ITER 2
void main( void ) {
    vec2 sp = surfacePosition;//vec2(.4, .7);
    vec2 p = sp*4.0- vec2(1);
    vec2 i = p;
    float c = 0.2;
    float inten = .04;

    for (int n = 0; n < MAX_ITER; n++) 
    {
        float t = time * (1.0 - (1.1 / float(n+1)));
        i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
        c += 1.0/length(vec2(p.x / (sin(i.x+t)/inten),p.y / (cos(i.y+t)/inten)));
    }
    c /= float(MAX_ITER);
    c = 1.5-sqrt(c);
    gl_FragColor = vec4(vec3(0.6*0.4*1.1*c), 9.0) + vec4(0.1, 0.1, 0.2, 1.0);

}
