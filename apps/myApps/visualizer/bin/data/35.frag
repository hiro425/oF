#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

vec4 tex( vec2 position ) {

    float color = 0.1;
    
    if ( mod(position.x, 30.) < 5.)
    {
        color += 0.4;
    }
    
    if ( mod(position.y, 30.) < 5.)
    {
        color += 0.4;
    }
    
    return vec4(color) * vec4(1,2,3,4) * 0.3;

}

float map(vec3 p) { return 1.5 - dot(abs(p), vec3(0,1,0)); }
void main( void ) {
    vec2 uv  = -1.0 + 2.0 * (gl_FragCoord.xy / resolution.xy);
    vec3 pos = vec3(time * 2., 0, time * 0.1);
    vec3 dir = normalize(vec3(uv, 1));
    float t = 0.0;
    for(int i = 0 ; i < 30; i++)
        t += map(dir * t + pos);
    vec3 inter = dir * t + pos;
    gl_FragColor = mix(tex(inter.xz * 50.0), vec4(3,2,1,1) * 0.1, t * 0.02) + t * 0.03;
}
