#ifdef GL_ES
precision mediump float;
#endif
uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

uniform sampler2D backbuffer;

//DRAW WITH MOUSE ON LEFT  SIDE OF SCREEN, ERASE ON RIGHT

//copied lighting from e#18403
//fixed my specular bug
#define THRESHOLD 0.001
#define MAX_STEPS 64
#define RANGE 4.
#define SHADOWTESTS 10
#define OUTSIDE 100.
//comment to hide green stuff
//#define SHOWDATA 
 //uncomment to gradually destroy the model.
//#define ERODE
//brush radius, 
#define BRUSH 0.17 
#define ERASER 0.25

// famebuffer dimensions must be higher than SQ*SQ*SQ or greater to work.
#define SQ 7 //increase for higher resolution voxels
const int size=SQ*SQ;
const float fsize = float(size);
const float sq=float(SQ); //square root of the number of voxels in each axis. data is sliced along z and arranged into sq*sq tiles.
const float isq=1./sq;

float sphere(vec3 p, float r)
{
    return length(p) - r;
}

float sdBox( vec3 p, vec3 b )
{
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) +
         length(max(d,0.0));
}
float sdCylinder( vec3 p, vec3 c )
{
  return length(p.xz-c.xy)-c.z;
}
vec3 rotY(vec3 p,float a){
    float s=sin(a), c=cos(a); 
    return vec3(p.z*s+p.x*c,p.y,p.z*c-p.x*s);
}
vec3 rotX(vec3 p,float a){
    float s=sin(a), c=cos(a); 
    return vec3(p.z*s+p.y*c,p.x,p.z*c-p.y*s);
}
float get( vec3 p )
{
    p.z*=isq;
    vec2 c = p.xy+0.5;
    c.y+=floor(p.z)*fsize;
    c.x+=fract(p.z)*sq*fsize;
        
    float d=  texture2D(backbuffer, c/resolution.xy ).a-0.5;
    d/=RANGE;
    return d;
}

float sdf(  vec3 v )
{
    const vec2 o=vec2(0.,1.);
        const float bound2 = (fsize*0.5-.5)/fsize;
        vec3 x=v+0.5;
        x*=fsize;
    x-=0.5;
        vec3 p = floor(x);
        vec3 f = fract(x);
    
    v = abs(v)-bound2;
    if((max(max(v.x,v.y),v.z))<0.0)
        return mix(mix(mix( get(p), get(p+o.yxx),f.x), mix( get(p+o.xyx), get(p+o.yyx),f.x),f.y),
                    mix(mix( get(p+o.xxy), get(p+o.yxy),f.x), mix( get(p+o.xyy), get(p+o.yyy),f.x),f.y),f.z);
    
    return length(max(v,THRESHOLD)); //just inside the edge of bounding box. 
}
float sdfshadow(  vec3 v ) //same as above but returns 100 outside of bounds.
{
    const vec2 o=vec2(0.,1.);
        const float bound2 = (fsize*0.5-.5)/fsize;
        vec3 x=v+0.5;
        x*=fsize;
    x-=0.5;
        vec3 p = floor(x);
        vec3 f = fract(x);
    
    v = abs(v)-bound2;
    if((max(max(v.x,v.y),v.z))<0.0)
        return mix(mix(mix( get(p), get(p+o.yxx),f.x), mix( get(p+o.xyx), get(p+o.yyx),f.x),f.y),
                    mix(mix( get(p+o.xxy), get(p+o.yxy),f.x), mix( get(p+o.xyy), get(p+o.yyy),f.x),f.y),f.z);
    
    return OUTSIDE;
}


vec3 normal(vec3 p) {
    vec3 e = vec3(0.0,0.5/fsize,0.0);
    
    return normalize(vec3(
            sdf(p+e.yxx)-sdf(p-e.yxx),
            sdf(p+e.xyx)-sdf(p-e.xyx),
            sdf(p+e.xxy)-sdf(p-e.xxy)
            )
        );  
}

const float rSpeed = 1.;
vec3 lightdir=rotX(-vec3(0.5,0.5,-0.5),time*rSpeed);

float softshadow( in vec3 ro, in vec3 rd, float mint, float k )
{
    float res = 1.0;
    float t = mint;

    for( int i=0; i<SHADOWTESTS; i++ ) 
    {
        float h = sdfshadow(ro + rd*t);
        if(h == OUTSIDE)
        return clamp(res,0.,1.);
    h = max( h, 0.0 );
        res = min( res, k*h/t );
        t += clamp( h, 0.1, 0.5 );
    }
    return clamp(res,0.0,1.0);
}


float light(in vec3 p, in vec3 dir) {
    vec3 ldir=normalize(lightdir);
    vec3 n=normal(p);
    float sh=softshadow(p,-ldir,.5/fsize,20.);
    float diff=max(0.,dot(ldir,-n));
    vec3 r = reflect(ldir,n);
    float spec=max(0.,dot(dir,-r));
    return diff*sh+pow(spec,30.)*.5*sh+.15*max(0.,dot(normalize(dir),-n));  
        }
float rand(vec2 co){
    return fract(sin(dot(co.xy, vec2(12.9898,78.233)))*43758.5453);
}


vec3 trace(vec3 ro, vec3 rd, out bool hit)
{
    hit = false;
    vec3 pos = ro+rd;
    pos.z-=2.;
    
    pos=rotY(pos,time*rSpeed);
    rd =rotY(rd,time*rSpeed);
    
    for( int i=0;i<MAX_STEPS;i++)
    {
        float d = sdf(pos);
        if(d<THRESHOLD ) {
            hit = true;
            return pos;
        }
        pos += d*rd;
    }
    return pos;
}


void main( void ) {

    vec2 position = ( (gl_FragCoord.xy/resolution.xy)*2. -vec2(1.0)); 
    
    vec2 pxl = gl_FragCoord.xy; 
    float a = texture2D(backbuffer, pxl/resolution.xy ).a;
    const float bound= (fsize*0.5-1.)/fsize; //volume between all datapoints is fsize-1, clip surface to fsize-2 so that normals will still work.
    a-=0.5;
    a/=RANGE;
    #ifdef ERODE
        a+=0.0007;
    #endif
    int page = int(pxl.y)/size;
    int frame = int(pxl.x)/size;
    
    if(page<SQ && frame<SQ)
    {
        //calc coords of this pixel's voxel
        vec3 c   = vec3( mod(floor(pxl),float(size)) ,float(page)*sq + float(frame) );
        c+=0.5;
        c/=fsize;
        c-=0.5;
        
        vec2 t = vec2(time*11.11,-time*0.311);
        vec3 k=vec3(1.-mouse.x,1.-mouse.y,0.5);
        //vec3 k =vec3(rand(t.xy),fract(time),fract(time*.1));
        k-=0.5;
        k*=1.0;
        
        k=rotY(k,time*rSpeed);
        if(mouse.x<0.55)
            a= min( a, max(sdBox((rotX(c+k,3.14159*time)),vec3(BRUSH,BRUSH*0.15,BRUSH*0.15)),sdBox(c,vec3(bound)))); 
        else
            a= max( a, -sphere(c+k,ERASER));
    }
    
    a*=RANGE;
    a+=0.5;
    
    if( texture2D(backbuffer, vec2(0.) ).a ==0.)
        a=1.;
    if(floor(pxl.y)==0.)
        a=1.;
    
    float foc = 1.;
    
    vec3 rgb = vec3(0.0);
    vec3 rf = vec3(position.x*foc,position.y*foc, foc*3.0);

    vec3 ro=vec3(0.);
    vec3 rd=normalize(rf-ro);
    bool hit;
    vec3 pos = trace(ro,rd,hit);
        
    float f = 0.1/length(pos);

    if (hit)
    {
        rgb += light(pos,rotY(rd,time*rSpeed));
        rgb += .01+pos;
        
    }
    else
        rgb+= vec3(0.5);
#ifdef SHOWDATA
    gl_FragColor = vec4(rgb.r,rgb.g+1.-a,rgb.b,a);
#else
    gl_FragColor = vec4(rgb,a);
#endif
    
    
}
