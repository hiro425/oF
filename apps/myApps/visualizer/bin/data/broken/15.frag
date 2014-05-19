#ifdef GL_ES
precision highp float;
#endif

float pi = 3.14159265;

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D backbuffer;

varying vec2 surfacePosition;


vec4 eaMain(vec2 p, float t);


void main( void ) {
    vec2 p = ( gl_FragCoord.xy / resolution.xy ) * 2.0 - 1.0;
    p = surfacePosition;
    
    gl_FragColor.r = eaMain(p, time+0.02*sin(time+length(p)*1000.)).r;
    gl_FragColor.g = eaMain(p, time+0.33*cos(time)).g;
    gl_FragColor.b = eaMain(p, time-0.33*sin(time*0.9)).b;
    
}

vec4 eaMain(vec2 p, float t){
    vec4 ret = vec4(0);
    
    
    float a = atan( p.y, p.x );
    float r = sqrt( dot( p, p ) );

    vec2 uv = vec2( 0, 0 );
    uv.x = mod( mouse.x * cos( a ) / r + t * 0.05, 1.0 );
    uv.y = mod( mouse.y * sin( a ) / r + t * 0.06, 1.0 );
    
    float amount = sin( t * 0.5 ) * 0.01;

    vec4 color0 = texture2D( backbuffer, uv );
    vec4 color1 = texture2D( backbuffer, uv + vec2( 0.0, - amount ) );
    vec4 color2 = texture2D( backbuffer, uv + vec2( 0.0, amount ) );
    vec4 color3 = texture2D( backbuffer, uv + vec2( amount, 0.0 ) );
    vec4 color4 = texture2D( backbuffer, uv + vec2( - amount, 0.0 ) );

    ret = ( ( color0 + color1 + color2 + color3 + color4 ) / 8.0 ) + pow( 1.0 - r, 3.0 );

    float border = 0.997;

    if ( gl_FragCoord.x < 1. || gl_FragCoord.x > resolution.x-1. || gl_FragCoord.y < 1. || gl_FragCoord.y > resolution.y-1. ) {
        ret = vec4( sin(atan(p.x, p.y)+time) );

    }
    
    
    return ret;
}
