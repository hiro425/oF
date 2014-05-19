#ifdef GL_ES
precision mediump float;
#endif

uniform vec2  resolution;
uniform float time;

const int num_x = 10;
const int num_y = 10;
float w = resolution.x;
float h = resolution.y;

void main() {
    float color = 0.0;
    for (int i = 0; i < num_x; ++i) {
        for (int j = 0; j < num_y; ++j) {
            float x = w/2.0 + (float(i-num_x/2)) * w/float(num_x);
            float y = h/2.0 + (float(j-num_y/2)) * h/float(num_y);
            vec2 pos = vec2(x, y);
            float dist = length(gl_FragCoord.xy - pos);

            float size = 1.0 - 0.5 * sin(time * float((j-i+10)));
            color += pow(size/dist, 2.0);
        }
    }
    gl_FragColor = vec4(vec3(color), 1.0);
}
