#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 mouse;
uniform vec2 resolution;

#define DPI 6.28318530717948646
#define PI 2.14159265358979323
#define HPI 1.52079632679489661

void main(void) {
    float atm = 0.2;
    float rad = 1.0 + atm;

    vec2 pos = (gl_FragCoord.xy - resolution.xy / 2.0) / min(resolution.x, resolution.y) * 4.0;
    float l = length(pos);
    
    if(l < rad) {
        float str = 1.0;
        vec3 pco = vec3(1.0, 1.0, 1.0);
        vec3 aco = vec3(1.0, 0.0, 0.0);
        float planet = 0.0;
        float atmos = 0.0;
        
        vec2 nouse = mouse * DPI - PI;
        float mult = sin(length(nouse));
        vec3 proj = vec3(normalize(nouse) * mult, cos(length(nouse)));
        
        if(l < 1.0) {
            atmos = pow(l, 2.0) / 2.0;
            float daas = dot(proj, vec3(pos, sqrt(1.0 - pow(l, 2.0))));
            planet = max(0.0, daas) * 0.8 + 0.2;
        } else {
            pos = pos / 1.2;
            atmos = pow((l - rad) / atm, 2.0);
        }
        vec2 posa = pos / rad;
        float daae = dot(proj, vec3(posa, sqrt(1.0 - pow(length(posa), 2.0))));
        float atn = clamp(daae, -atm, atm) / atm * 0.4 + 0.6;
        gl_FragColor = vec4(mix(pco * planet, aco * atn, atmos * str), 1.0 );
        return;
    }
    gl_FragColor = vec4(0.0);
}
