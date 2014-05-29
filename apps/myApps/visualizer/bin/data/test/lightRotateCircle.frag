#ifdef GL_ES
precision mediump float;
#endif

uniform vec2  resolution;
uniform float time;
uniform float vol;

const int num_x = 10;
const int num_y = 10;
float w = resolution.x;
float h = resolution.y;

void main() {
    float t = time;
    float color = 0.0;
    for (int i = 0; i < num_x; ++i) {
        for (int j = 0; j < num_y; ++j) {
            float x = w/2.0 * (1.0 + cos(1.2 * t + float(3*i+4*j)));
            float y = h/2.0 * (1.0 + sin(1.7 * t + float(3*i+4*j)));
            float size = 2.0 - 1.0 * sin(t);
            vec2 pos = vec2(x, y);
            float dist = length(gl_FragCoord.xy - pos);
            //color += pow(size/dist, 2.0);
            color += pow(size/dist*20.0*vol, 2.0);
        }
    }
    gl_FragColor = vec4(vec3(color), 1.0);
}
