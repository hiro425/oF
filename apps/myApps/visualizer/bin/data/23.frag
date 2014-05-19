#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 resolution;

const vec2 gravity = vec2(0, -.3);

// thank you for this function, anonymous person on the interwebs
float
rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233)))*43758.5453);
}

float
rand(float from, float to, vec2 co)
{
    return from + rand(co)*(to - from);
}

float calculate_alpha(float particle_time, float threshold, float period) {
    float alpha = 0.0;
    
    if (particle_time > threshold)
        alpha = 1.0 - (particle_time - threshold)/(period - threshold);
    else
        alpha = 1.0;
    
    return alpha;
}

vec4 calculate_particle_color(float i, float index) {
    return vec4(rand(vec2(i*index, 2.0)), rand(vec2(i*index, 1.5)), rand(vec2(i*index, 1.2)), 1.0);
}

float counter = 0.0;
void main(void)
{
    float scale = 1.5/max(resolution.x, resolution.y);
    vec2 coord = gl_FragCoord.xy*scale;
    
    vec2 origin = vec2(.5*resolution.x*scale, 0.0);

    gl_FragColor = vec4(0., 0., 0., 0.);

    for (float i = 1.; i < 20.; i++) {
        float period = rand(1.5, 2.5, vec2(i, 0.));

        float t = time - period*rand(vec2(i, 1.));

        float particle_time = mod(t, period);
        float index = ceil(t/period);

        vec2 speed = vec2(rand(-.5, .5, vec2(index*i, 3.)), rand(.5, 1., vec2(index*i, 4.)));
        vec2 pos = origin + particle_time*speed + gravity*particle_time*particle_time;

        float threshold = .7*period;

        float alpha = calculate_alpha(particle_time, threshold, period);
        vec4 particle_color = calculate_particle_color(i, index);

        float angle_speed = rand(-4.0, 4.0, vec2(index*i, 5.0));
        float angle = atan(pos.y - coord.y, pos.x - coord.x) + angle_speed*time;

        float radius = rand(.09, .05, vec2(index*i, 2.));

        float dist_1 = sin(angle)*radius;
        float dist_2 = -2.0 * radius + sin(angle)*radius;
        float dist_3 = 0.002 + sin(angle)*radius;
        
        counter++;
        float psuedo_random = mod(counter, 3.0);
        
        if (psuedo_random == 0.0) {
            gl_FragColor += alpha * ((1.0 - smoothstep(dist_1, dist_1 + .25, distance(coord, pos))) + 
                                  (1.0 - smoothstep(0.01, 0.02, distance(coord, pos))) +
                                  (1.0 - smoothstep(radius * 0.1, radius * 0.9, distance(coord, pos)))) * particle_color;
        }
        
        else if (psuedo_random  == 1.0) {
            gl_FragColor += alpha * ((1.0 - smoothstep(dist_2, dist_2 + .05, distance(coord, pos))) + 
                                  (1.0 - smoothstep(0.01, 0.02, distance(coord, pos))) +
                                  (1.0 - smoothstep(radius * 0.1, radius * 0.11, distance(coord, pos)))) * particle_color;          
        }
        
        else if (psuedo_random == 2.0) {
            gl_FragColor += alpha * ((1.0 - smoothstep(dist_3, dist_3 + .105, distance(coord, pos))) + 
                                  (1.0 - smoothstep(0.01, 0.102, distance(coord, pos))) +
                                  (1.0 - smoothstep(radius * 0.1, radius * 0.9, distance(coord, pos)))) * particle_color;   
        }
        
        if (counter > 100.0) counter = 0.0;
    }
}

