#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform float vol;

void main( void ) {
    float SimulationTime = time * 1.0;
    float AspectRatio = resolution.x / resolution.y;
    vec2 PixelPosition = AspectRatio * (gl_FragCoord.xy / resolution)-0.5;
    PixelPosition.x *= AspectRatio;
    float PixelColor = 0.0;
    for (int i = 0; i < 100; i++)
    {
        // Particle index is i. Let's calculate particle position
        vec2 ParticlePosition = vec2(0.5*AspectRatio, 0.4);
        //ParticlePosition.x += sin(time) + cos(SimulationTime) * 0.1 * float(i)/15.0;
        ParticlePosition.x += sin(time+vol*2.0) + cos(SimulationTime+vol*2.0) * 0.18 * float(i)/15.0;
        ParticlePosition.y += sin(SimulationTime) * 0.1;
        //ParticlePosition.y += sin(ParticlePosition.x*cos(time*2.0))/2.0;
        ParticlePosition.y += sin(ParticlePosition.x*cos(time*2.0))/2.0*vol*5.0;
        
        PixelColor += float(i)/100.0*0.01/length(ParticlePosition - PixelPosition) * vol;    
    }
    //gl_FragColor = vec4(PixelColor, PixelColor * (1.0+sin(time)/2.0), PixelColor * (1.0+cos(time)/1.0), 1.0);
    gl_FragColor = vec4(PixelColor, PixelColor, PixelColor, 1.0);
}
