#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
varying vec2 surfacePosition;

float julia(vec2 pos, vec2 a)
{
    const int iter = 100;
    float ret;
    vec2 ps;
    vec2 nps;
    
    float time = time + pos.y*12. + length(pos)*3.;
    float pp = 0.01;
    
    ps = (surfacePosition-.5)*0.5;
    ret = 0.0;
    
    for (int i = 0; i < iter; i++) {
        nps.x = ps.x * ps.x - ps.y * ps.y;
        nps.y = ps.x * ps.y + ps.x * ps.y;
        nps += a;
        ps = nps;
        if (sqrt(ps.x * ps.x + ps.y * ps.y) < 0.2 + (1e-1)*cos(time)) {
            ret += pp;
        }
    }
    return ret;
}

void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / (2.0 * resolution.y);
    vec4 gs = vec4(vec3(fract(10.*julia(pos + vec2(-0.15, 0.5), vec2(0.38, 0.3+0.0001*cos(time*surfacePosition.x*surfacePosition.y))))), 1.0);
    gl_FragColor = gs;
}
