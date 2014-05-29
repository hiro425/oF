#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 resolution;

const vec2 gravity = vec2(0, -.3);

vec2 coord;

// thank you for this function, anonymous person on the interwebs
float
rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898,78.233)))*43758.5453);
    
    // https://gist.github.com/johansten/3633917
    //float a = fract(dot(co.xy, vec2(2.067390879775102, 12.451168662908249))) - 0.5;
    //float s = a * (6.182785114200511 + a*a * (-38.026512460676566 + a*a * 53.392573080032137));
    //return fract(s * 43758.5453);
}

float
rand(float from, float to, vec2 co)
{
    return from + rand(co)*(to - from);
}

void main(void)
{
    float scale = 1./max(resolution.x, resolution.y);
    coord = gl_FragCoord.xy*scale;
    
    vec2 origin = vec2(.5*resolution.x*scale, 0.);

    gl_FragColor = vec4(0., 0., 0., 0.);

    for (float i = 1.; i < 16.; i++) {
        float period = rand(1.5, 2.5, vec2(i, 0.));

        float t = time - period*rand(vec2(i, 1.));

        float particle_time = mod(t, period);
        float index = ceil(t/period);

        vec2 speed = vec2(rand(-.5, .5, vec2(index*i, 3.)), rand(.5, 1., vec2(index*i, 4.)));
        vec2 pos = origin + particle_time*speed + gravity*particle_time*particle_time;

        float threshold = .7*period;

        float alpha;
        if (particle_time > threshold)
            alpha = 1. - (particle_time - threshold)/(period - threshold);
        else
            alpha = 1.;

        vec4 particle_color = vec4(rand(vec2(i*index, 4.)), rand(vec2(i*index, 5.)), rand(vec2(i*index, 6.)), 1.);

        float angle_speed = rand(-4., 4., vec2(index*i, 5.));
        float angle = atan(pos.y - coord.y, pos.x - coord.x) + angle_speed*time;

        float radius = rand(.025, .075, vec2(index*i, 2.));

        float dist = radius + .6*sin(11.*angle)*radius;

        gl_FragColor += alpha * ((1.0 - smoothstep(dist, dist + .01, distance(coord, pos))) + 
                                  (1.0 - smoothstep(0.01, 0.2, distance(coord, pos)))/2. +
                                  (1.0 - smoothstep(radius * 0.2, radius * 0.4, distance(coord, pos)))) * particle_color;
    }
}

