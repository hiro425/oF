#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D backbuffer;
void main( void ) {

    vec4 tex = texture2D(backbuffer,( (gl_FragCoord.xy / resolution.xy)-0.01 ))*vec4(0.99,0.99,0.99,0.9)-0.01;
    vec2 position = ( gl_FragCoord.xy / resolution.x );
    float l = pow(1.0-length(position-vec2(cos(time*2.0)/4.0+0.5,sin(time*3.0)/8.0+0.25)),50.0);
    gl_FragColor = vec4( pow(l,0.75)*2.0,l,pow(l,2.0), 1.0 );
    gl_FragColor.rgb += tex.rgb;

