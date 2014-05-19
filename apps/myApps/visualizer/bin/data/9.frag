// rakesh@picovico.com

#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 resolution;

void main(void) {
    //our final desired color
    vec4 vColor = vec4(0.3, 0.3, 0.3, 1.0);
    
    vec2 c = vec2(resolution.x/2.0, resolution.y/2.0);
    
    vec2 p = gl_FragCoord.xy;

    p.x += 150.0 + sin(0.01*time);
    p.x *= cos(p.x*p.y)+0.9*sin(p.y*p.x);
    
    //for simplicity, assume we're dealing with square sizes
    float dist = resolution.y - distance(p, c);
    
    vec4 color = vec4(dist, dist, dist, 1.0);
    
    //clamp to rgb range so we can multiply with vertex color
    color = clamp(color, 0.0, 1.0);
        
    gl_FragColor = color * vColor;
}
