#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

float noise(vec3 p) //Thx to Las^Mercury
{
    vec3 i = floor(p);
    vec4 a = dot(i, vec3(1., 57., 21.)) + vec4(0., 57., 21., 78.);
    vec3 f = cos((p-i)*acos(-1.))*(-.5)+.5;
    a = mix(sin(cos(a)*a),sin(cos(1.+a)*(1.+a)), f.x);
    a.xy = mix(a.xz, a.yw, f.y);
    return mix(a.x, a.y, f.z);
}

float sphere(vec3 p, vec4 spr)
{
    return length(spr.xyz-p) - spr.w;
}

mat4 rotate_x(float theta)
{
    return mat4(
        vec4(1.0,         0.0,         0.0, 0.0),
        vec4(0.0,   cos(theta),   sin(theta), 0.0),
        vec4(0.0,  -sin(theta),   cos(theta), 0.0),
        vec4(0.0,         0.0,         0.0, 1.0)
    );
}

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

float rotatingcube(vec3 p) {
    p.y += 2.0;
    float a2 = 0.1*time;
    mat4 m = rotationMatrix(vec3(1,1,1), a2);
    p = (vec4(p,1)*m).xyz;
    p = abs(p);
    return max(p.x, max(p.y, p.z))-1.;
}

float flame(vec3 p)
{
    float d = rotatingcube(p);
    //return d + (noise(p+vec3(.0,time*.2,.0)) + noise(p*3.)*.5)*.25*(p.y) ;
    vec3 p2 = 3.0*p+vec3(0,1,0)*time;
    return d + noise(p2*3.+vec3(0,1,0)*time)*.12*min(0.0,(p.y+1.1));
}

float scene(vec3 p)
{
    return min(100.-length(p) , abs(flame(p)) );
}

vec4 raymarch(vec3 org, vec3 dir)
{
    float d = 0.0, glow = 0.0, eps = 0.02;
    vec3  p = org;
    bool glowed = false;
    
    for(int i=0; i<64; i++)
    {
        d = scene(p) + eps;
        p += d * dir;
        if( d>eps )
        {
            if(flame(p) < .0)
                glowed=true;
            if(glowed)
                glow = float(i)/64.;
        }
    }
    return vec4(p,glow);
}

void main()
{
    vec2 v = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;
    v.x *= resolution.x/resolution.y;
    
    vec3 org = vec3(0., -2., 4.);
    vec3 dir = normalize(vec3(v.x, -v.y, -1.5));
    
    vec4 p = raymarch(org, dir);
    float glow = p.w;
    
    vec4 col = mix(vec4(1.,.5,.1,1.), vec4(0.1,.5,1.,1.), p.y*.02+.4);
    
    gl_FragColor = mix(vec4(0.), col, pow(glow*1.7,4.));
    //gl_FragColor = mix(vec4(1.), mix(vec4(1.,.5,.1,1.),vec4(0.1,.5,1.,1.),p.y*.02+.4), pow(glow*2.,4.));

}
