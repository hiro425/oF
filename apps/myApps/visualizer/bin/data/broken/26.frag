fdef GL_ES
precision mediump float;
#endif
//Ported by Redexe@gmail.com 
uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D u_texture;

// rendering params
const float sphsize=1.0; // planet size
const float dist=1.27; // distance for glow and distortion
const float perturb=50.0; // distortion amount of the flow around the planet
const float displacement=1.0; // hot air effect
const float windspeed=0.95; // speed of wind flow
const float steps=15.3; // number of steps for the volumetric rendering
const float stepsize=.07; 
const float brightness=.9;
const vec3  planetcolor=vec3(0.0,0.0,0.0);
const float fade=.051; //fade by distance
const float glow=1.2; // glow amount, mainly on hit side


// fractal params
const int iterations=9; 
const float fractparam=0.9;
const vec3 offset=vec3(20.54,142.,-1.55);


float wind(vec3 p) {
    float d=max(0.,dist-max(0.,length(p)-sphsize)/sphsize)/dist; // for distortion and glow area
    float x=max(0.2,p.x*2.); // to increase glow on left side
    p.y*=1.+max(0.,-p.x-sphsize*.25)*1.5; // left side distortion (cheesy)
    p-=d*normalize(p)*perturb; // spheric distortion of flow
    p+=vec3(time*windspeed,0.,0.); // flow movement
    p=abs(fract((p+offset)*.1)-.5); // tile folding 
    for (int i=0; i<iterations; i++) {  
        p=abs(p)/dot(p,p)-fractparam; // the magic formula for the hot flow
    }
    return length(p)*(1.2+d*glow*x)+d*glow*x; // return the result with glow applied
}

void main(void)
{
    // get ray dir  
    // vec2 uv = gl_FragCoord.xy / resolution.xy-.5;
    vec2 uv = 0.33 * ( 2.0 * gl_FragCoord.xy - resolution.xy ) / (0.5 * (resolution.x + resolution.y));
    vec3 dir=vec3(uv,1.);
    dir.x*=resolution.x/resolution.y;
    vec3 from=vec3(0.,0.,-2.+texture2D(u_texture,uv*.5+time).x*stepsize); //from+dither
    // volumetric rendering
    float v=0., l=-0.0001, t=time*windspeed*.2;
    vec3 p;
    float tx;
    for (float r=10.;r<steps;r++) {
        p=from+r*dir*stepsize;
        tx=texture2D(u_texture,uv*.2+vec2(t,0.)).x*displacement; // hot air effect
        
        v+=min(50.,wind(p))*max(0.,1.-r*fade); 
    }
    
    v/=steps; v*=brightness; // average values and apply bright factor
    vec3 col=vec3(v,v*v,v*v*v); // set color
    col *= (0.75-length(sqrt(3.0 * uv*uv)));
    col *= length(col) * 50.0;
//  col.b += col.r * (0.4 - sin(col.r / 3.14159));
    gl_FragColor = vec4(col,1.0);
}
