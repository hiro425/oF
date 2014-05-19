#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

vec4 mod289(vec4 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}
 
vec4 permute(vec4 x)
{
    return mod289(((x*34.0)+1.0)*x);
}
 
vec4 taylorInvSqrt(vec4 r)
{
    return 1.79284291400159 - 0.85373472095314 * r;
}
 
vec2 fade(vec2 t) {
    return t*t*t*(t*(t*6.0-15.0)+10.0);
}
 
// Classic Perlin noise
float cnoise(vec2 P)
{
    vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
    vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
    Pi = mod289(Pi); // To avoid truncation effects in permutation
    vec4 ix = Pi.xzxz;
    vec4 iy = Pi.yyww;
    vec4 fx = Pf.xzxz;
    vec4 fy = Pf.yyww;
     
    vec4 i = permute(permute(ix) + iy);
     
    vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;
    vec4 gy = abs(gx) - 0.5 ;
    vec4 tx = floor(gx + 0.5);
    gx = gx - tx;
     
    vec2 g00 = vec2(gx.x,gy.x);
    vec2 g10 = vec2(gx.y,gy.y);
    vec2 g01 = vec2(gx.z,gy.z);
    vec2 g11 = vec2(gx.w,gy.w);
     
    vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
    g00 *= norm.x; 
    g01 *= norm.y; 
    g10 *= norm.z; 
    g11 *= norm.w; 
     
    float n00 = dot(g00, vec2(fx.x, fy.x));
    float n10 = dot(g10, vec2(fx.y, fy.y));
    float n01 = dot(g01, vec2(fx.z, fy.z));
    float n11 = dot(g11, vec2(fx.w, fy.w));
     
    vec2 fade_xy = fade(Pf.xy);
    vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
    float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
    return 2.3 * n_xy;
}

float fbm(vec2 P, float lacunarity, float gain)
{
    float sum = 0.0;
    float amp = 1.0;
    vec2 pp = P;
     
    for(int i = 0; i < 2; i+=1)
    {
        amp *= gain;
        sum += amp * cnoise(pp);
        pp *= lacunarity;
    }
    return sum;
 
}

float pattern(in vec2 p) {
    float l = 2.5;
    float g = 0.4;
     
    vec2 q = vec2( fbm( p + vec2(0.0,0.0),l,g),fbm( p + vec2(5.2,1.3),l,g));
    vec2 r = vec2( fbm( p + 4.0*q + vec2(1.7,9.2),l,g ), fbm( p + 4.0*q + vec2(8.3,2.8) ,l,g));
    return fbm( p + 4.0*r ,l,g);   
}

float sphere(vec3 p, vec3 pos, float r) {
    p -= pos;
    return length(p) - r;
}

float displacement(vec3 p) {
    return pattern(vec2(p.xy*1.5));
}

float scene(vec3 p) {
    return sphere(p, vec3(0,0,-2), 1.) + .1*displacement(p);
}

vec3 get_n(vec3 p) {
    float eps = 1e-3;
    vec3 n = vec3(
        scene(vec3(p.x+eps,p.y,p.z)) - scene(vec3(p.x-eps,p.y,p.z)),
        scene(vec3(p.x,p.y+eps,p.z)) - scene(vec3(p.x,p.y-eps,p.z)), 
        scene(vec3(p.x,p.y,p.z+eps)) - scene(vec3(p.x,p.y,p.z-eps))
    );
    return normalize(n);
}

vec4 circle(float r,vec2 p0,vec2 p1,vec3 color){
    return vec4(color,step(distance(p0,p1),r));
}

vec4 cutcircle1(float r,vec2 p0,vec2 p1,vec3 color,float cut){
    vec4 c=circle(r,p0,p1,color);
    return vec4(c.xyz,c.w*step(cut,((p1.y-p0.y)+r)/(2.0*r)));
}

vec4 cutcircle2(float r,vec2 p0,vec2 p1,vec3 color,float cut){
    vec4 c=circle(r,p0,p1,color);
    return vec4(c.xyz,c.w*step(cut,1.0-((p1.y-p0.y)+r)/(2.0*r)));
}

vec3 smiley(vec2 p){
    p=fract(p);
    vec3 c=vec3(0);
    //yellow
    vec4 temp=circle(0.45,vec2(0.5),p,vec3(0.9,0.8,0.2));
    c=mix(c,temp.xyz,temp.w);
    //mouth border
    temp=cutcircle2(0.34,vec2(0.47,0.42),p,vec3(0),0.45);
    c=mix(c,temp.xyz,temp.w);
    //mouth inside
    temp=cutcircle2(0.3,vec2(0.47,0.4),p,vec3(0.5,0.0,0.2),0.45);
    c=mix(c,temp.xyz,temp.w);
    //eye1 border
    temp=cutcircle1(0.15,vec2(0.25,0.62),p,vec3(0),0.2);
    c=mix(c,temp.xyz,temp.w);
    //eye1 inside
    temp=cutcircle1(0.12,vec2(0.25,0.63),p,vec3(1),0.2);
    c=mix(c,temp.xyz,temp.w);
    //eye2 border
    temp=cutcircle1(0.15,vec2(0.63,0.62),p,vec3(0),0.2);
    c=mix(c,temp.xyz,temp.w);
    //eye2 inside
    temp=cutcircle1(0.12,vec2(0.63,0.63),p,vec3(1),0.2);
    c=mix(c,temp.xyz,temp.w);
    //eye1 pupil
    temp=circle(0.05,vec2(0.69,0.69),p,vec3(0));
    c=mix(c,temp.xyz,temp.w);
    //eye2 pupil
    temp=circle(0.05,vec2(0.31,0.69),p,vec3(0));
    c=mix(c,temp.xyz,temp.w);
    return c;
}
void main(void) {
    vec2 uv = gl_FragCoord.xy/min(resolution.x,resolution.y);
    float ar = resolution.x/resolution.y;
    uv -= vec2(ar/2.,.5);
    
    vec3 color = vec3(-uv.y+.5);
    
    float foc = 1e-6;
    vec3 ro = vec3(uv,0);
    vec3 rd = normalize(vec3(uv*foc, -foc));
    vec3 ld = normalize(vec3(sin(time), cos(time),-1));
    //vec3 ld = normalize(vec3(1,1,-1));
    
    
    float d = 0.;
    const float md = 5.;
    for (int i=0; i<128; i++) {
        vec3 rp = ro + d*rd;
        vec3 normal = get_n(rp);
        float td = scene(rp);
        if (abs(td) < 1e-3) {
            color = 0.5*dot(normal, normalize(ld-rp))+0.5*smiley(1.5*rp.xy);
            break;
        }
        d += td;
        if (d > md) {
            break;
        }
    }
    
    gl_FragColor = vec4(color, 1);
}
