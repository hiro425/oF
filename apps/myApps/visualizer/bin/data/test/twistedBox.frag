#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

vec3 rotate_x(float theta, vec3 v) {
    float ct = cos(theta), st = sin(theta);
    return v * mat3(1.0, 0.0, 0.0, 0.0, ct,  -st, 0.0, st,  ct );
}

vec3 rotate_y(float theta, vec3 v) {
    float ct = cos(theta), st = sin(theta);
    return v * mat3(ct,  0.0, st,  0.0, 1.0, 0.0, -st, 0.0, ct );
}

vec3 rotate_z(float theta, vec3 v) {
    float ct = cos(theta), st = sin(theta);
    return v * mat3(ct,  -st, 0.0, st,  ct,  0.0, 0.0, 0.0, 1.0);
}

// Distance map.
float map(vec3 p) {
    float t = time + p.y * cos(time * 0.2) * 9.0;
    p = rotate_y(t, rotate_z(time, p));
    float d = length(max(abs(p)-vec3(0.2, 0.7, 0.3),0.0));
    
    return d;   
}

// Normal vector of point on map() surface near p.
vec3 surface_normal(in vec3 p) {
    vec2 e = vec2(0.05, 0.0);
    vec3 grad = vec3(map(p+e.xyy) - map(p-e.xyy),
             map(p+e.yxy) - map(p-e.yxy),
             map(p+e.yyx) - map(p-e.yyx));
    return normalize(grad);
}

void main(void) {
    vec2 position = (gl_FragCoord.xy / resolution.xy) * 2.0 - 1.0;
    position.y *= resolution.y / resolution.x;

    vec3 color = vec3(0.0);
    
    vec3 origin = vec3(0.0, 0.0, 2.0);
    vec3 direction = vec3(position, -1.0);
    
    float dstep = 1.0;
    float dist = 0.0, dmax = 20.0;
    
    // March from the origin.
    for (int i = 0; i < 100; i++) {
        dstep = map(origin + dist * direction);
        if (dstep < 0.001 || dist > dmax) break;
        dist += dstep * 0.3;
    }
    
    if (dist < dmax) {
        // We hit the object; cast light sources on it.
        vec3 pos = origin + dist * direction;
        vec3 norm = surface_normal(pos);
        vec3 light = normalize(vec3(mouse * 2.0 - 1.0, 1.0));
        
        color  = vec3(0.7, 0.8, 0.6) * clamp(dot(light, norm), 0.0, 1.0);
        color += vec3(0.0, 0.0, 0.2) * clamp(norm.y, 0.0, 1.0);
        color += vec3(0.1, 0.0, 0.0);
    }
    gl_FragColor = vec4( color, 1.0 );
}
