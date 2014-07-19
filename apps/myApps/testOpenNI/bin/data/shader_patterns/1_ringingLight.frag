#ifdef GL_ES
precision mediump float;
#endif

uniform vec2  resolution;
uniform float time;
uniform float vol;

void main() {
    float x = resolution.x / 2.0;
    float y = resolution.y / 2.0;
    float size = 5.0;
    //float size = 3.0 - 2.0*sin(time * 2.0);
    vec2  pos = vec2(x, y);
    float dist = length(gl_FragCoord.xy - pos);
    //float color = size / dist;
    float color = pow(size/dist*80.0*vol, 2.0);
    gl_FragColor = vec4(vec3(color), 1.0);
}
