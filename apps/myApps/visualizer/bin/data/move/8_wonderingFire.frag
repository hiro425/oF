#ifdef GL_ES
precision mediump float;
#endif

#define N 30
#define NF float(N)
#define PI 3.14
#define HPI PI/2.0

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform float vol;
void main( void ) {
    vec2 PixelCoord = (gl_FragCoord.xy/resolution.xy);
    vec3 PixelColor = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < N; i++)
    {
        vec2 ParticlePos = vec2(0.5, 0.5);
        ParticlePos.x += 0.25*sin(time+(vol*5.) + float(i) / NF);
        ParticlePos.y += 0.25*cos(time+(vol) + float(i) / NF);
        float ParticleSize = 0.0005 * float(i) * vol;
        PixelColor.r += ParticleSize * (1.0/length(ParticlePos-PixelCoord));
        PixelColor.g += ParticleSize * (1.0/length(ParticlePos-PixelCoord) * float(i)/NF);
        PixelColor.b += 0.5 * ParticleSize * (1.0/length(ParticlePos-PixelCoord) * float(i)/NF);
    }
    
    
    gl_FragColor = vec4(PixelColor, 1.0);
}
