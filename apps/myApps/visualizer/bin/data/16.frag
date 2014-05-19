//check board by uggway

// yay faux perspective -jz
#ifdef GL_ES
precision highp float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform float vol;
varying vec2 surfacePosition;
//#define CM 


vec3 check(vec2 p, float y, float s)
{
    float c = clamp(floor(mod(p.x/s+floor(p.y/s),2.0))*s,0.1,0.9)*2.0;
    c *= c;
    return vec3(c);
}

void main( void ) {
    float tv = time + vol * 3.0;

    vec2 p = -1.0 + 2.0 * ( gl_FragCoord.xy/ resolution.xy  );
    p.x *=  resolution.x/resolution.y;

    vec3 col = vec3(1.0);
    
    float y = p.y + (p.y + (sin((sin(tv*3.2+p.y)+p.x*-cos(tv)-tv+p.x))*0.5)) + cos(p.x*3.)*.05*(sin(p.x)*1.);
    vec2 uv;
    uv.x = p.x/y;
    uv.y = 1.0/abs(y)+tv/3.0;
    col = check(uv, y, 0.50)*length(y);
    float t = pow(abs(y),0.0);

    gl_FragColor = vec4( col*t, 1.0 );

}
