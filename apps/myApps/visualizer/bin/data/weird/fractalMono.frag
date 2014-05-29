#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform float vol;

/*
 * inspired by http://www.fractalforums.com/new-theories-and-research/very-simple-formula-for-fractal-patterns/
 * a slight(?) different 
 * public domain
 */

#define N 60
void main( void ) {
    vec2 v = (gl_FragCoord.xy - resolution/2.0) / min(resolution.y,resolution.x) * 20.0;
    
    float rsum = 0.0;
    float pi2 = 3.1415926535 * 2.0;
    //float a = (.5-mouse.x)*pi2;
    float a = (.5)*pi2;
    float C = cos(a);
    float S = sin(a);
    vec2 xaxis=vec2(C, -S);
    vec2 yaxis=vec2(S, C);
    #define MAGIC 0.618
    vec2 shift = vec2( 0, 1.0+MAGIC);
    //float zoom = 1.0 + mouse.y*8.0;
    float zoom = abs(150.0 * 8.0 - vol*10.0 * 150.0*2.0);
    
    for ( int i = 0; i < N; i++ ){
        float rr = dot(v,v);
        if ( rr > 1.0 )
        {
            rr = (1.0)/rr ;
            v.x = v.x * rr;
            v.y = v.y * rr;
        }
        rsum *= .99;
        rsum += rr;
        
        v = vec2( dot(v, xaxis), dot(v, yaxis)) * zoom + shift;
    }
    float col1 = fract(rsum);
    col1 = 2.0 * min(col1, 1.0-col1);
    gl_FragColor = vec4(col1, col1, col1, 1.0); 
}
