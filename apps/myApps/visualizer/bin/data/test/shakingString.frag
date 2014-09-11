#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform float vol;
uniform vec2 mouse;
uniform vec2 resolution;

void main( void ) {

    //this is me deconstructing http://glsl.heroku.com/e#19344.0 to try and figure out how it works
    
    vec2 position = ( gl_FragCoord.xy / resolution.xy ) - 0.5;
    
    float density = 0.2 * vol;
    float amplitude = 0.3;
    float frequency = 16.0 * vol + 0.5;
    float scroll = vol + 0.5;

    //if (mod(position.y, 3.0) == 0.0) {

    vec3 colour = vec3 (0.4, 0.2, 0.1) * 
         ((1.0 / abs((position.y + (amplitude * sin((position.x + time * scroll) *frequency)))) * density));
        //+ (1.0 / abs((position.y + (amplitude * sin((position.x + time * scroll) *frequency+2.0943951)))) * density)
        //+ (1.0 / abs((position.y + (amplitude * sin((position.x + time * scroll) *frequency-2.0943951)))) * density));
    
    gl_FragColor = vec4( colour, 1.0 );

    //}
}
