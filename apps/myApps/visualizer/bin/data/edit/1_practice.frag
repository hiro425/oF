uniform vec2 resolution;
uniform float time;
uniform sampler2D backbuffer;

void main() {
    vec2 pos = (gl_FragCoord.xy * 2.0 - resolution)/resolution.y;

    float theta = time*3.0;
    vec2 ballPos = vec2(cos(theta), sin(theta))*0.5;
    vec2 texPos = vec2(gl_FragCoord.xy/resolution);
    vec2 texDelta = vec2(1.0/resolution);

    if(distance(pos, ballPos) < 0.3) {
        gl_FragColor = vec4(1.0);
    }
    else {
        gl_FragColor = 
        texture2D(backbuffer, texPos+vec2(0.0, -texDelta.y))*0.33+
        texture2D(backbuffer, texPos+vec2(texDelta.x, -texDelta.y))*0.33+
        texture2D(backbuffer, texPos+vec2(-texDelta.x, -texDelta.y))*0.33;;
    }
}
