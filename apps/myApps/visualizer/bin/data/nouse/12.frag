#ifdef GL_ES
precision highp float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

//messy code, hacky light, and a few experiments 
//sphinx

#define aspect resolution.x/resolution.y;
#define pi (4.*atan(1.))

#define oceanC      vec3(0.4, 0.6, .7)
#define coastalC    vec3(0.85, 0.85, .5)
#define plainsC     vec3(.55, .75, .35)
#define woodlandC   vec3(.35, .75, .25)
#define evergreenC  vec3(.25, .65, .4) 
#define highlandC   vec3(.47, .55, .45)
#define snowcapC    vec3(.65, .65, .69) 
#define wetlandC
#define tundraC
#define jungleC
#define desertC

vec2  toworld(vec2 uv);
float sinc(float x);
float sincn(float x);
float hash(float n);
float noise(in vec2 uv);
float fbm(in vec2 p);
vec4  pid(float p, float t, float e, float i, float d, vec3 k);
float map(vec2 p);
vec3  derivate(vec3 p, float o);
float shade(vec3 p, vec3 l, vec3 n);
vec3  harmonic(in vec4 n);
vec3  hsv(float h,float s,float v);

void main() 
{
    vec2 uv   = gl_FragCoord.xy/resolution;
    vec3 p    = vec3(0.);
    vec2 m    = (mouse-.5) * 2.;
    m.x       *= aspect;
    
    p.xz      = toworld(uv);
    bool zoom = length(p.xz-m) < .3 ;
    p.xz      = zoom ? (p.xz+m)*.5 : p.xz;
    p.xz     += 256.+mouse*64.;
    
    p.y       = map(p.xz);
    
    vec3 n      = normalize(derivate(p, zoom ? .2 : .6)+derivate(p, abs(.5-p.y+.25)*.15));//h4x
    
    float at    = time * .025 + p.y*.005;
    vec3 an     = derivate(p-p.y*.025, 32.);
    float ap    = max(0., fbm(p.y*.125-2.*an.y*.5+at+p.xz*1.5-at*-an.y*.01)-.5)*.25;
    float a     = fbm(ap*7.+an.y+abs(n.x-n.z)*3.-p.y*(cos(p.x)-sin(p.y))+p.xz*9.+at-(n.x+n.z));
    a = abs(ap*2.-a*6.);
    float t = 5.1;
    mat2 rmat = mat2(cos(t), sin(t), -sin(t), cos(t));
    
    vec3 hn   = n;
    vec3 h    = harmonic(vec4(hn, 1.));
    
    vec3 lp = vec3(-4096., -4096.,-4097.);
    lp.xz *= rmat;
    vec3 ld = normalize(p-lp);
    float l = shade(p, ld, n)*.75+.15;
        
    vec3 c = vec3(0.);
    c = p.y > -1.         ? oceanC     : c;
    c = p.y > .15         ? coastalC   : c;
    c = p.y > .28         ? plainsC    : c;
    c = p.y > .3          ? woodlandC  : c;
    c = p.y > .9          ? evergreenC : c;
    c = p.y > 1.47        ? highlandC  : c;
    c = p.y > 5.2         ? snowcapC   : c;
    
    c += .12*hsv(length(c+p.y*.6), 1., 1.)*.9-.3*clamp(sqrt(p.y)*.05, 0.1, 3.);
    c *= .75;
    
    //c = p.z < 0. || p.z > 1. ? vec3(1., 0., 0.) : c;
    
    c = c*(l+h);
    c += clamp(sqrt(abs(p.y+32.-a)), 0., ap)*abs(ap-a*1.5)*1.25-ap;    
    c = max(vec3(.05)*h, sqrt(c*.95)*c);
    gl_FragColor = vec4(c, 1.);
}//sphinx


#define usepid

float map(vec2 p){
    const float vk = 0.7071067;
    float v = fbm(vec2(vk*-p.x-p.y, vk*p.y-p.x)*.5+time*.01);
    v = abs(2.5*v-abs(fract(p.y-v*3.)-.5));
    
    
    float n   = .01 + v * .75;
    float a   = 1.;
    float f   = 1.;
    
    
    float t   = .1;
    t = t-abs(t * 1.-t);
    
    vec3 k    = vec3(1.125,1.6, .5925);
    vec4 c    = vec4(1.);
    
    for(int i = 0; i < 8; i++)
    {   
        float ft = float(i+1);
        n     += noise((p) * (f-c.x*.00125*-n)) * (a * c.w * .75);
        p     = -p.yx;
        c     =  pid(n, t, c.x, c.y, c.z, k);
        
        k.x   -= c.x - abs(v-n) * 4.;
        k.z   += c.w * (n-1.) * .25;  
        k.y   -= ft * 2.5 - n - c.z + v;

        f     *= 2. + .05 * 8./ft;
        a     *= .57 - v * .05;
        t     = n;
    }
    
    n = pow(n, 4.-v+n) * abs(.5-v);
    n += .02*(n-1.)*hash(c.y);
    n = n < .1 ? (abs(noise(p*64.+v*32.+time*.8)*.5)+noise(p*256.+v*32.+time*2.5))*.05 : n;
    
    return n;
}

vec3 derivate(vec3 p, float o)
{
    vec3 e = vec3(o/resolution.xy, .0);
    vec3 n = vec3(0., 0., .1);
    n.x = map(p.xz+e.xz)-map(p.xz-e.xz);
    n.z = map(p.xz+e.zy)-map(p.xz-e.zy);
    n.y = .25-(n.x+n.y)*.5;
    n = normalize(n);
    return n;
}

vec2 toworld(vec2 uv)
{
    uv = uv * 2. - 1.;
    uv.x *= aspect;
    return uv;
}

float hash(float n)
{
    return fract(fract(sin(n)*1234.5678)*1234.5678);
}

float noise(in vec2 uv)
{
    vec2 p = floor(uv);
    vec2 f = fract(uv);
    f = f*f*(3.0-2.0*f);
    float n = p.x + p.y*57.0;
    p.x = mix(hash(n+ 0.), hash(n+  1.), f.x);
    p.y = mix(hash(n+57.), hash(n+ 58.), f.x);
    return mix(p.x, p.y, f.y);;
}

vec4 pid(float p, float t, float e, float i, float d, vec3 k){
    float error     = t - p;
    float integral  = i + error/2.;
    float delta     = error - e;
    
    float position  = k.x*error + k.y*integral + k.z*delta;
    return vec4(position, error, integral, delta);
}

float sinc(float x) {
    return sin(x)/(x);
}

float sincn(float x) {
    return sin(pi*x)/(pi*x);
}
   
float fbm(vec2 uv)
{
    float n = 0.;
    uv += 32.;
    n += noise(uv)*.5;
    n += noise(uv*1.5) *.25;
    n += noise(uv*3.)  *.15;
    n += noise(uv*6.)  *.075;
    n += noise(uv*12.) *.0325;
    n += noise(uv*24.) *.015125;
    n = clamp(n, 0., 1.);
    return n;
}

vec3 harmonic(in vec4 n){   
  
    vec3 l1, l2, l3;
    
    vec4 c[7];
    c[0] = vec4(-.2,  .0, -.2,  .35);
    c[1] = vec4(-.1,  .0, -.2,  .35);
    c[2] = vec4(-.1, .0,  -.2,  .3);
    c[3] = vec4(0.,  .0,  0.,  .0);
    c[4] = vec4(0.,  .0,  0.,  .0);
    c[5] = vec4(0.,  .0,  0.,  .0);
    c[6] = vec4(0.,  0.,  0.,  0.);
    
    l1.r = dot(c[0], n);
    l1.g = dot(c[1], n);
    l1.b = dot(c[2], n);
    
    vec4 m2 = n.xyzz * n.yzzx;
    l2.r = dot(c[3], m2);
    l2.g = dot(c[4], m2);
    l2.b = dot(c[5], m2);
    
    float m3 = n.x*n.x - n.y*n.y;
    l3 = c[6].xyz * m3;
        
    vec3 sh = vec3(l1 + l2 + l3);
    
    return clamp(sh, 0., 1.);
}


float shade(vec3 p, vec3 l, vec3 n){
   
    float ndl = dot(n, l);
    ndl = clamp(ndl, .05, .95);

    return ndl;
}

vec3 hsv(float h,float s,float v)
{
    return mix(vec3(1.),clamp((abs(fract(h+vec3(3.,2.,1.)/3.)*6.-3.)-1.),0.,1.),s)*v;
}
