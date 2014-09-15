#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D bb;

//keep the mouse near center, but as you go far enough left or right, you can do a barrel roll.
//uncomment to reverse vertical control, as in flight and not in arcade
//#define SIM

vec2 rotate(vec2 p, float a)
    {
    return vec2(p.x * cos(a) - p.y * sin(a), p.x * sin(a) + p.y * cos(a));
    }
float box(vec2 p, vec2 b, float r)
    {
    return length(max(abs(p) - b, 0.0)) - (r * 2.0);
    }

vec3 intersect(in vec3 o, in vec3 d, vec3 c, vec3 u, vec3 v)
    {
    vec3 q = o - c;
    return vec3(
    dot(cross(u, v), q),
    dot(cross(q, u), d),
    dot(cross(v, q), d)) / dot(cross(v, u), d);
    }

float rand11(float p)
    {
    return fract(sin(p * 591.32) * 43758.5357);
    }
float rand12(vec2 p)
    {
    return fract(sin(dot(p.xy, vec2(12.9898, 78.233))) * 43758.5357);
    }
vec2 rand21(float p)
    {
    return fract(vec2(sin(p * 591.32), cos(p * 391.32)));
    }

vec2 rand22(in vec2 p)
    {
    return fract(vec2(sin(p.x * 591.32 + p.y * 154.077), cos(p.x * 391.32 + p.y * 49.077)));
    }

float noise11(float p)
    {
    float fl = floor(p);
    return mix(rand11(fl), rand11(fl + 1.0), fract(p));//smoothstep(0.0, 1.0, fract(p)));
    }
float fbm11(float p)
    {
    return noise11(p) * 0.5 + noise11(p * 2.0) * 0.25 + noise11(p * 5.0) * 0.125;
    }
vec3 noise31(float p)
    {
    return vec3(noise11(p), noise11(p + 18.952), noise11(p - 11.372)) * 2.0 - 1.0;
    }

float sky(vec3 p)
    {
    float a = atan(p.x, p.z);
    float t = time * 0.1;
    float v = rand11(floor(a * 4.0 + t)) * 0.5 + rand11(floor(a * 8.0 - t)) * 0.25 + rand11(floor(a * 16.0 + t)) * 0.125;
    return v;
    }

vec3 voronoi(in vec2 x)
    {
    vec2 n = floor(x); // grid cell id
    vec2 f = fract(x); // grid internal position
    vec2 mg; // shortest distance...
    vec2 mr; // ..and second shortest distance
    float md = 5.0, md2 = 1.0;
    for(int j = -1; j <= 1; j ++)
        {
        for(int i = -1; i <= 1; i ++)
            {
            vec2 g = vec2(float(i), float(j)); // cell id
            vec2 o = rand22(n + g); // offset to edge point
            vec2 r = g + o - f;
            
            float d = max(abs(r.x), abs(r.y)); // distance to the edge
            
            if(d < md)
                {
                md2 = md; md = d; mr = r; mg = g;
                }
            else if(d < md2)
                {
                md2 = d;
                }
            }
        }
    return vec3(n + mg, md2 - md);
    }

#define A2V(a) vec2(sin((a) * 6.28318531 / 100.0), cos((a) * 6.28318531 / 100.0))

float circles(vec2 p)
    {
    float v, w, l, c;
    vec2 pp;
    l = length(p);
    
    
    pp = rotate(p, time * -3.0);
    c = max(dot(pp, normalize(vec2(-0.75, 0.5))), -dot(pp, normalize(vec2(0.2, 0.5))));
    c = min(c, max(dot(pp, normalize(vec2(0.5, -0.5))), -dot(pp, normalize(vec2(0.2, -0.5)))));
    c = min(c, max(dot(pp, normalize(vec2(0.3, 0.5))), -dot(pp, normalize(vec2(0.2, 0.5)))));
    
    // innerest stuff
    v = abs(l - 0.5) - 0.03;
    v = max(v, -c);
    v = min(v, abs(l - 0.35) - 0.0002);
    v = min(v, abs(l - 0.75) - 0.005);
    
    pp = rotate(p, time * 10.333);
    c = max(dot(pp, A2V(-5.0)), -dot(pp, A2V(5.0)));
    c = min(c, max(dot(pp, A2V(25.0 - 5.0)), -dot(pp, A2V(25.0 + 5.0))));
    c = min(c, max(dot(pp, A2V(50.0 - 5.0)), -dot(pp, A2V(50.0 + 5.0))));
    c = min(c, max(dot(pp, A2V(70.0 - 5.0)), -dot(pp, A2V(75.0 + 5.0))));
    
    w = abs(l - 0.83) - 0.09;
    v = min(v, max(w, c));
    
    return v;
    }

float shade1(float d)
    {
    float v = 1.0 - smoothstep(0.0, mix(0.012, 0.2, 0.0), d);
    float g = exp(d * -20.0);
    return v + g * 0.5;
    }


void main()
    {
    const float twopi = 6.2831853;
    vec4 old = texture2D(bb,vec2(0.0));
    vec2 oldpos = (time < 2.0) ? vec2(0.5) : old.rg;
    float oldroll = (time < 2.0) ? 0.5 : old.b;
    float tunnel = 14.0; //limits
    vec2 op = (oldpos - 0.5) * tunnel;
    vec2 ms = mouse-0.5;
#ifdef SIM
    ms.y = -ms.y;
#endif
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    uv = uv * 2.0 - 1.0;
    uv.x *= resolution.x / resolution.y;
    
    
    // using an iq styled camera this time :)
    // ray origin
    vec3 ro = 0.7 * vec3(0.0, op.y, op.x);
    // camera look at
    vec3 ta = vec3(1.0, ms.y, ms.x) * 48.;
    
    //also stateful
    float roll = twopi * (oldroll - 0.5) + ms.x * 0.7;

    // build camera matrix
    vec3 ww = normalize(ta - ro);
    vec3 uu = normalize(cross(ww, vec3(0.0, cos(roll), sin(roll))));
    vec3 vv = normalize(cross(uu, ww));
    // obtain ray direction
    vec3 rd = normalize(uv.x * uu + uv.y * vv + 1.0 * ww);
    
    // shaking and movement
    ro.x += time * 2.0;
    
    float blue = 0.0;
    float red = 0.0;
    float green = 0.0;
    
    // background
    float sd = dot(rd, vec3(0.0, 1.0, 0.0));
    blue = pow(1.0 - abs(sd), 16.0) + pow(sky(rd), 5.0) * step(0.0, rd.y) * 0.2;
    
    vec3 its;
    float v, g;
    
    // voronoi floor layers
    for(int i = 0; i < 16; i ++)
        {
        float layer = float(i);
        its = intersect(ro, rd, vec3(0.0, -5.0 - layer * 5.0, 0.0), vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0));
        if(its.x > 0.0)
            {
            vec3 vo = voronoi((its.yz) * 0.05 + 8.0 * rand21(float(i)));
            v = exp(-100.0 * (vo.z - 0.02));
            
            float fx = 0.0;
            
            // add some special fx to lowest layer
            if(i == 3)
                {
                float crd = 0.0;//fract(time * 0.2) * 50.0 - 25.0;
                float fxi = cos(vo.x * 0.2 + time * 1.5);//abs(crd - vo.x);
                fx = clamp(smoothstep(0.9, 1.0, fxi), 0.0, 0.9) * 1.0 * rand12(vo.xy);
                fx *= exp(-3.0 * vo.z) * 2.0;
                }
            blue += v * 0.1 + fx;
            }
        }
    
    // draw the gates
    float gatex = floor(ro.x / 8.0 + 0.5) * 8.0 - 4.0;
    const int ngates = 16;
    float gateSize = 10.;
    float go = 8.0;
    for(int i = 0; i < ngates; i ++)
        {
        float rx = cos(time*0.9878915919)*2. + cos(-time*1.1591)*3.;
        float ry = sin(time*0.87416981)*2. + sin(-time*0.998402)*3.;
        its = intersect(ro, rd, vec3(gatex + go, rx, ry), vec3(0.0, gateSize, 0.0), vec3(0.0, 0.0, gateSize));
        //if(dot(its.yz, its.yz) < 2.0 && its.x > 0.0)
            {
            v = circles(its.yz);
            red += shade1(v);
            }
        
        go += 8.0;
        }
    
    // draw the stream
    for(int j = 0; j < 5; j ++)
        {
        float id = float(j);
        
        vec3 bp = vec3(0.0, (rand11(id) * 2.0 - 1.0) * 0.25, 0.0);
        vec3 its = intersect(ro, rd, bp, vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0));
        
        if(its.x > 0.0)
            {
            vec2 pp = its.yz;
            float spd = (1.0 + rand11(id) * 3.0) * 2.5;
            pp.y += time * spd;
            pp += (rand21(id) * 2.0 - 1.0) * vec2(0.3, 1.0);
            float rep = rand11(id) + 1.5;
            pp.y = mod(pp.y, rep * 2.0) - rep;
            float d = box(pp, vec2(0.02, 0.3), 0.1);
            float foc = 0.0;
            float v = 1.0 - smoothstep(0.0, 0.03, abs(d) - 0.001);
            float g = min(exp(d * -20.0), 2.0);
            
            green += (v + g * 0.7) * 0.5;
            
            }
        }
    vec3 rgb = vec3(0.0);
#if 1
    float inten = 1.0; //0.4 + (sin(time*0.3) * 0.5 + 0.5) * 0.6;
    float desat = 0.2 * (cos(time*3.) * 0.5 + 0.5) + 0.07;
    rgb.b = inten * (blue + desat * (red+green));
    rgb.r = inten * (red + desat * (blue+green));
    rgb.g = inten * (green + desat * (red+blue));
#else
    rgb = vec3(red,green,blue);
#endif
    vec3 col = pow(rgb, vec3(1.0, 1.0, 1.25));
    float mn = 0.1; //maneuverability
    if (int(gl_FragCoord.x) == 0) {
    if (int(gl_FragCoord.y) == 0)
    {
        col.r = oldpos.x + (ms.x * mn);
        col.g = oldpos.y + (ms.y * mn);
        col.b = mix(0.5, fract(oldroll + (0.0008 * (pow(7., 5.0 * abs(ms.x))) * sign(ms.x))), abs(ms.x * 2.0)); //fixme: this is sloppy
    }
    }
    gl_FragColor = vec4(col, 1.0);

    }
